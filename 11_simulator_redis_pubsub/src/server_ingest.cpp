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
    std::cout << "[server_ingest] Starting up..." << std::endl;
    std::cout.flush();

    signal(SIGINT, signal_handler);

    cpp_redis::client pub;

    // Connect publisher (retry until successful)
    while (running) {
        try {
            pub.connect("127.0.0.1", 6379);
            std::cout << "[server_ingest] Publisher connected to Redis." << std::endl;
            std::cout.flush();
            break;
        } catch (const cpp_redis::redis_error &e) {
            std::cerr << "[server_ingest] Publisher connection failed: " << e.what() << ", retrying..." << std::endl;
            std::cerr.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // Main loop: reconnect + resubscribe if subscriber disconnects
    while (running) {
        try {
            cpp_redis::subscriber sub;
            sub.connect("127.0.0.1", 6379);

            std::cout << "[server_ingest] Subscriber connected. Subscribing to locations_raw..." << std::endl;
            std::cout.flush();

            sub.subscribe("locations_raw", [&](const std::string&, const std::string& msg){
                std::cout << "[server_ingest] Received: " << msg << std::endl;
                std::cout.flush();

                try {
                    pub.publish("locations_mid", msg);
                    pub.commit();
                    std::cout << "[server_ingest] Forwarded to locations_mid: " << msg << std::endl;
                    std::cout.flush();
                } catch (const cpp_redis::redis_error &e) {
                    std::cerr << "[server_ingest] Publish failed: " << e.what() << std::endl;
                    std::cerr.flush();
                }
            });

            sub.commit();
            std::cout << "[server_ingest] Ready, waiting for messages..." << std::endl;
            std::cout.flush();

            // Keep running until disconnected or Ctrl-C
            while (running && sub.is_connected()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            if (!running) break;

            std::cerr << "[server_ingest] Subscriber disconnected. Reconnecting in 1s..." << std::endl;
            std::cerr.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));

        } catch (const cpp_redis::redis_error &e) {
            std::cerr << "[server_ingest] Redis error: " << e.what() << ", retrying in 1s..." << std::endl;
            std::cerr.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    pub.disconnect();
    std::cout << "[server_ingest] Shutting down." << std::endl;
    std::cout.flush();
    return 0;
}
