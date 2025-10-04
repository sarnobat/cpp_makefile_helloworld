// hello_pipeline.cpp
// Requires: cpp-httplib single header "httplib.h" next to this file
//   https://github.com/yhirose/cpp-httplib (raw: httplib.h)

#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <functional>

#include "httplib.h"  // header-only

// --- Minimal pipeline operator: value pipes through callables (Ctx -> Ctx) ---
template <class T, class F>
decltype(auto) operator|(T&& x, F&& f) {
    return std::forward<F>(f)(std::forward<T>(x));
}

// --- Request/response context that flows through the pipeline ---
struct Ctx {
    // Input
    std::string method;
    std::string path;
    std::string body;
    std::unordered_map<std::string,std::string> headers;

    // Output
    int         status  = 200;
    std::string out;
    std::unordered_map<std::string,std::string> out_headers;

    // Routing / control
    bool handled = false;  // set true by a route when it produced a response
};

// --- Tiny helpers/stages (all are Ctx -> Ctx) ---

// Respond with JSON hello
auto h_root = [](Ctx c) {
    c.out = R"({"message":"Hello, World!"})";
    c.out_headers["Content-Type"] = "application/json; charset=utf-8";
    return c;
};

// Respond with plain-text health
auto h_health = [](Ctx c) {
    c.out = "OK\n";
    c.out_headers["Content-Type"] = "text/plain; charset=utf-8";
    return c;
};

// Only allow GET; if not GET, set 405 and mark handled to stop further routing.
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

// Add/override a header
auto add_server_header = [](std::string name, std::string value) {
    return [=](Ctx c) {
        c.out_headers[name] = value;
        return c;
    };
};

// Log the result
auto log_ctx = [](Ctx c) {
    std::cerr << c.method << " " << c.path << " -> " << c.status << (c.handled ? " (handled)\n" : " (unhandled)\n");
    return c;
};

// Route combinator: if path matches exactly AND not already handled, run handler and mark handled
template <class Handler>
auto route_exact(std::string path, Handler handler) {
    return [path=std::move(path), handler=std::move(handler)](Ctx c) mutable {
        if (!c.handled && c.path == path) {
            c = handler(std::move(c));
            c.handled = true;
        }
        return c;
    };
}

// After all routes, if still unhandled (and not an earlier error), set 404 JSON.
auto not_found_if_unhandled = [](Ctx c) {
    if (!c.handled) {
        c.status = 404;
        c.out = R"({"error":"Not Found"})";
        c.out_headers["Content-Type"] = "application/json; charset=utf-8";
        c.handled = true;
    }
    return c;
};

int main() {
    httplib::Server srv;

    // One GET handler for everything; pure pipeline routing:
    srv.Get(".*", [&](const httplib::Request& req, httplib::Response& res) {
        Ctx ctx;
        ctx.method = "GET";
        ctx.path   = req.path;
        ctx.body   = req.body;
        for (const auto& [k,v] : req.headers) ctx.headers[k] = v;

        ctx = ctx
            | ensure_get_only
            | route_exact("/",      h_root)
            | route_exact("/health",h_health)
            | not_found_if_unhandled
            | add_server_header("Server", "cpp-httplib + pipes")
            | log_ctx;

        res.status = ctx.status;
        for (auto& [k,v] : ctx.out_headers) res.set_header(k.c_str(), v.c_str());
        const char* ctype = "text/plain; charset=utf-8";
        if (auto it = ctx.out_headers.find("Content-Type"); it != ctx.out_headers.end())
            ctype = it->second.c_str();
        res.set_content(ctx.out, ctype);
    });

    std::cout << "Server on http://localhost:8080\n";
    srv.listen("0.0.0.0", 8080);
    return 0;
}
