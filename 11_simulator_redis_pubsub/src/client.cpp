////
//// Friends sending their latest status
////

#include <cpp_redis/cpp_redis>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <client_id>" << std::endl;
        return 1;
    }

    int id = std::stoi(argv[1]);
    std::cout << "[client" << id << "] Starting up..." << std::endl;
    std::cout.flush();

    signal(SIGINT, signal_handler);

    cpp_redis::client redis_client;

    // Retry connecting to Redis until successful
    while (running) {
        try {
            redis_client.connect("127.0.0.1", 6379);
            std::cout << "[client" << id << "] Connected to Redis." << std::endl;
            std::cout.flush();
            break;
        } catch (const cpp_redis::redis_error &e) {
            std::cerr << "[client" << id << "] Redis not ready, retrying in 1s..." << std::endl;
            std::cerr.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    float x = 10.0f + id, y = 20.0f + id;

    while (running) {
        std::ostringstream msg;
        msg << id << "," << x << "," << y;

        try {
            ///
            /// Send the location update
            ///
            redis_client.publish("locations_raw", msg.str());
            redis_client.commit();
            std::cout << "[client" << id << "] Published: " << msg.str() << std::endl;
            std::cout.flush();
        } catch (const cpp_redis::redis_error &e) {
            std::cerr << "[client" << id << "] Publish failed: " << e.what() << std::endl;
            std::cerr.flush();
        }

        // simulate the person moving
        x += 0.1f;
        y += 0.1f;
        // Wait a few seconds before sending another update
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    redis_client.disconnect();
    std::cout << "[client" << id << "] Shutting down." << std::endl;
    std::cout.flush();
    return 0;
}
