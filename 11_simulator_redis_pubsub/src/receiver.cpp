////
//// Me receiving location updates from my friends from a server that aggregates them
////


// receiver.cpp
#include <cpp_redis/cpp_redis>
#include <iostream>
#include <vector>
#include <mutex>

int main() {
    cpp_redis::subscriber redis_subscriber;
    redis_subscriber.connect();

    // known[0] -> client0's latest message
    // known[1] -> client1's latest message
    // known[2] -> client2's latest message
    // known[3] -> client3's latest message
    // known[4] -> client4's latest message
    std::vector<std::string> known(5, "");
    std::mutex mtx;

    redis_subscriber.subscribe("locations_out",
        //
        // --- START OF CALLBACK FUNCTION ---
        //
        // This is a lambda function that acts as the callback.
        // It captures variables by reference [&] so it can access `mtx` and `known`.
        [&](const std::string& chan, const std::string& msg) {

            //
            // --- Parse the incoming message ---
            //

            // parse "id,lat,lon"
            auto pos = msg.find(',');
            if (pos == std::string::npos) return;
            int id = std::stoi(msg.substr(0, pos));

            //
            // --- Update shared state safely ---
            //
            {
                std::lock_guard<std::mutex> lk(mtx);
                if (id >= 0 && id < 5) known[id] = msg;
            }

            //
            // --- Print the latest known positions ---
            //
            std::cout << "\n[receiver] latest positions:\n";
            for (int i = 0; i < 5; ++i) {
                std::lock_guard<std::mutex> lk(mtx);
                if (!known[i].empty()) std::cout << "  " << known[i] << "\n";
                else std::cout << "  " << i << " -> (no data yet)\n";
            }
        });

    redis_subscriber.commit();
    // Shut down if we are idle for 60 seconds
    while (true) std::this_thread::sleep_for(std::chrono::seconds(60));
    return 0;
}
