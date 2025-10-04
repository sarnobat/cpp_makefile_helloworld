// hello_pipeline.cpp
// Requirements:
//   1) cpp-httplib single header "httplib.h" in the same directory.
//      curl -L -o httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
//   2) POSIX platform (uses flock for single-instance locking)

#include <string>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <unistd.h>      // getpid, write
#include <sys/file.h>    // flock
#include <sys/stat.h>    // umask
#include <fcntl.h>       // open, O_*
#include "httplib.h"

// ---------- Single-instance lock (PID file + flock) ----------
class PidFileLock {
    int fd_ = -1;
    std::string path_;
public:
    explicit PidFileLock(const std::string& path) : path_(path) {
        // Create/open pidfile
        umask(022);
        fd_ = ::open(path_.c_str(), O_CREAT | O_RDWR, 0644);
        if (fd_ == -1) {
            throw std::runtime_error(std::string("open pidfile failed: ") + std::strerror(errno));
        }
        // Try to take an exclusive, non-blocking lock
        if (::flock(fd_, LOCK_EX | LOCK_NB) != 0) {
            ::close(fd_);
            fd_ = -1;
            throw std::runtime_error("another instance is running (flock failed)");
        }
        // Truncate & write our PID
        if (ftruncate(fd_, 0) != 0) {
            throw std::runtime_error(std::string("ftruncate pidfile failed: ") + std::strerror(errno));
        }
        std::string pid = std::to_string(::getpid()) + "\n";
        if (::write(fd_, pid.data(), pid.size()) < 0) {
            throw std::runtime_error(std::string("write pidfile failed: ") + std::strerror(errno));
        }
        // Keep fd_ open; lock is held by this process
    }
    ~PidFileLock() {
        if (fd_ != -1) {
            // Best-effort: unlock and close; leave file present (harmless).
            ::flock(fd_, LOCK_UN);
            ::close(fd_);
        }
    }
    PidFileLock(const PidFileLock&) = delete;
    PidFileLock& operator=(const PidFileLock&) = delete;
};

// ---------- Pipeline plumbing ----------
template <class T, class F>
decltype(auto) operator|(T&& x, F&& f) {
    return std::forward<F>(f)(std::forward<T>(x));
}

struct Ctx {
    // Input
    std::string method;
    std::string path;

    // Output
    int status = 200;
    std::string out;
    std::unordered_map<std::string, std::string> out_headers;

    // Control
    bool handled = false;
};

// Stages
auto ensure_get_only = [](Ctx c) {
    if (c.method != "GET") {
        c.status = 405;
        c.out = R"({"error":"Method Not Allowed"})";
        c.out_headers["Content-Type"] = "application/json; charset=utf-8";
        c.out_headers["Allow"] = "GET";
        c.handled = true;
    }
    return c;
};

auto h_root = [](Ctx c) {
    c.out = R"({"message":"Hello, World!"})";
    c.out_headers["Content-Type"] = "application/json; charset=utf-8";
    return c;
};

auto h_health = [](Ctx c) {
    c.out = "OK\n";
    c.out_headers["Content-Type"] = "text/plain; charset=utf-8";
    return c;
};

template <class Handler>
auto route_exact(std::string path, Handler handler) {
    return [path = std::move(path), handler = std::move(handler)](Ctx c) mutable {
        if (!c.handled && c.path == path) {
            c = handler(std::move(c));
            c.handled = true;
        }
        return c;
    };
}

auto not_found_if_unhandled = [](Ctx c) {
    if (!c.handled) {
        c.status = 404;
        c.out = R"({"error":"Not Found"})";
        c.out_headers["Content-Type"] = "application/json; charset=utf-8";
        c.handled = true;
    }
    return c;
};

auto add_header = [](std::string k, std::string v) {
    return [=](Ctx c) {
        c.out_headers[k] = v;
        return c;
    };
};

auto log_ctx = [](Ctx c) {
    std::cerr << c.method << " " << c.path << " -> " << c.status << (c.handled ? " [handled]\n" : " [unhandled]\n");
    return c;
};

// ---------- Main ----------
int main() {
    // 1) Enforce single instance
    try {
        static PidFileLock _lock{"/tmp/hello_pipeline.pid"};
    } catch (const std::exception& e) {
        std::cerr << "Startup aborted: " << e.what() << "\n";
        return 2;
    }

    // 2) Start HTTP server
    httplib::Server srv;

    // One GET handler; routing is done purely via the pipeline
    srv.Get(".*", [&](const httplib::Request& req, httplib::Response& res) {
        Ctx ctx;
        ctx.method = "GET";
        ctx.path   = req.path;

        ctx = ctx
            | ensure_get_only
            | route_exact("/",       h_root)
            | route_exact("/health", h_health)
            | not_found_if_unhandled
            | add_header("Server", "cpp-httplib + pipes")
            | log_ctx;

        res.status = ctx.status;
        for (auto& [k, v] : ctx.out_headers) res.set_header(k.c_str(), v.c_str());
        const char* ctype = "text/plain; charset=utf-8";
        if (auto it = ctx.out_headers.find("Content-Type"); it != ctx.out_headers.end())
            ctype = it->second.c_str();
        res.set_content(ctx.out, ctype);
    });

    std::cout << "Server on http://localhost:8080\n";

    // 3) Fail fast if port is busy
    if (!srv.listen("0.0.0.0", 8080)) {
        std::cerr << "Port 8080 is in use (or bind failed). Exiting.\n";
        return 1;
    }
    return 0;
}
