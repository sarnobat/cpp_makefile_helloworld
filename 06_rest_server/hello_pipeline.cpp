// hello_pipeline.cpp
// Requires: https://github.com/yhirose/cpp-httplib (drop httplib.h next to this file)

#include <string>
#include <unordered_map>
#include <functional>
#include <utility>
#include <iostream>

#include "httplib.h"   // header-only

// --- Simple pipeline operator ---
template <class T, class F>
decltype(auto) operator|(T&& x, F&& f) {
    return std::forward<F>(f)(std::forward<T>(x));
}

// --- Minimal request/response context for our pipeline ---
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
};

// --- Pipeline stages ---
auto hello_json = [](Ctx c) {
    c.out = R"({"message":"Hello, World!"})";
    c.out_headers["Content-Type"] = "application/json; charset=utf-8";
    return c;
};

auto add_server_header = [](std::string name, std::string value) {
    return [=](Ctx c) {
        c.out_headers[name] = value;
        return c;
    };
};

auto ensure_get_only = [](Ctx c) {
    if (c.method != "GET") {
        c.status = 405;
        c.out = R"({"error":"Method Not Allowed"})";
        c.out_headers["Content-Type"] = "application/json; charset=utf-8";
        c.out_headers["Allow"] = "GET";
    }
    return c;
};

auto route_root = [](Ctx c) {
    if (c.path == "/") {
        return c | hello_json;
    }
    c.status = 404;
    c.out = R"({"error":"Not Found"})";
    c.out_headers["Content-Type"] = "application/json; charset=utf-8";
    return c;
};

// Optional stage to pretty-print to stdout when we serve
auto log_ctx = [](Ctx c) {
    std::cerr << c.method << " " << c.path << " -> " << c.status << "\n";
    return c;
};

int main() {
    httplib::Server srv;

    // Generic handler: build Ctx, run through pipeline, write response
    srv.Get(".*", [&](const httplib::Request& req, httplib::Response& res) {
        Ctx ctx;
        ctx.method = "GET";
        ctx.path   = req.path;
        for (const auto& [k,v] : req.headers) ctx.headers[k] = v;

        // The pipeline!
        ctx = ctx
            | ensure_get_only
            | route_root
            | add_server_header("Server", "cpp-httplib + pipes")
            | log_ctx;

        res.status = ctx.status;
        for (auto& [k,v] : ctx.out_headers) res.set_header(k.c_str(), v.c_str());
        res.set_content(ctx.out, ctx.out_headers.count("Content-Type")
                                   ? ctx.out_headers["Content-Type"].c_str()
                                   : "text/plain; charset=utf-8");
    });

    // You can add more routes with the same pipeline style:
    srv.Get("/health", [&](const httplib::Request& req, httplib::Response& res) {
        Ctx ctx;
        ctx.method = "GET";
        ctx.path   = req.path;
        ctx = ctx
            | ensure_get_only
            | [](Ctx c){ c.out = "OK\n"; c.out_headers["Content-Type"]="text/plain; charset=utf-8"; return c; }
            | add_server_header("Server", "cpp-httplib + pipes")
            | log_ctx;

        res.status = ctx.status;
        for (auto& [k,v] : ctx.out_headers) res.set_header(k.c_str(), v.c_str());
        res.set_content(ctx.out, ctx.out_headers["Content-Type"].c_str());
    });

    std::cout << "Server on http://localhost:8080\n";
    srv.listen("0.0.0.0", 8080);
    return 0;
}
