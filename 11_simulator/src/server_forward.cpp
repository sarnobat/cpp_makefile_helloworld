#include <cpp_redis/cpp_redis>
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

int main() {
    std::cout << "[server_forward] Starting up..." << std::endl;
    std::cout.flush();

    signal(SIGINT, signal_handler);

    cpp_redis::subscriber sub;
    cpp_redis::client pub;

    // Connect pub and sub, retry if Redis is not ready
    while (running) {
        try {
            pub.connect("127.0.0.1", 6379);
            std::cout << "[server_forward] Publisher connected to Redis." << std::endl;
            std::cout.flush();
            break;
        } catch(const cpp_redis::redis_error &e) {
            std::cerr << "[server_forward] Publisher connection failed, retrying..." << std::endl;
            std::cerr.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    while (running) {
        try {
            sub.connect("127.0.0.1", 6379);
            std::cout << "[server_forward] Subscriber connected to Redis." << std::endl;
            std::cout.flush();
            break;
        } catch(const cpp_redis::redis_error &e) {
            std::cerr << "[server_forward] Subscriber connection failed, retrying..." << std::endl;
            std::cerr.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    sub.subscribe("locations_mid", [&](const std::string& chan, const std::string& msg){
        std::cout << "[server_forward] Received: " << msg << std::endl;
        std::cout.flush();
        try {
            pub.publish("locations_out", msg);
            pub.commit();
            std::cout << "[server_forward] Forwarded to locations_out: " << msg << std::endl;
            std::cout.flush();
        } catch(const cpp_redis::redis_error &e) {
            std::cerr << "[server_forward] Publish failed: " << e.what() << std::endl;
            std::cerr.flush();
        }
    });

    sub.commit();

    std::cout << "[server_forward] Ready, waiting for messages..." << std::endl;
    std::cout.flush();

    while (running) std::this_thread::sleep_for(std::chrono::milliseconds(100));

    sub.unsubscribe("locations_mid");
    sub.disconnect();
    pub.disconnect();
    std::cout << "[server_forward] Shutting down." << std::endl;
    std::cout.flush();
    return 0;
}
