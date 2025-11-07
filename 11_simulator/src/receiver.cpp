////
//// Me receiving location updates from my friends from a server that aggregates them
////


// receiver.cpp
#include <cpp_redis/cpp_redis>
#include <iostream>
#include <vector>
#include <mutex>

int main() {
    cpp_redis::subscriber sub;
    sub.connect();

    std::vector<std::string> known(5, "");
    std::mutex mtx;

    sub.subscribe("locations_out",
        [&](const std::string& chan, const std::string& msg) {
            // parse "id,lat,lon"
            auto pos = msg.find(',');
            if (pos == std::string::npos) return;
            int id = std::stoi(msg.substr(0, pos));
            {
                std::lock_guard<std::mutex> lk(mtx);
                if (id >= 0 && id < 5) known[id] = msg;
            }

            std::cout << "\n[receiver] latest positions:\n";
            for (int i = 0; i < 5; ++i) {
                std::lock_guard<std::mutex> lk(mtx);
                if (!known[i].empty()) std::cout << "  " << known[i] << "\n";
                else std::cout << "  " << i << " -> (no data yet)\n";
            }
        });

    sub.commit();
    while (true) std::this_thread::sleep_for(std::chrono::seconds(60));
    return 0;
}
