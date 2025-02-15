#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

#define DISTANCE 100

class Swimmer {
public:
    std::string name;
    int speed;
    int time = 0;
    bool finished = false;

    Swimmer(std::string name, int speed) : name(std::move(name)), speed(speed) {}
};

std::mutex mtx;
std::vector<Swimmer*> results;

void swim(Swimmer* swimmer) {
    while (swimmer->time * swimmer->speed < DISTANCE) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        swimmer->time += 1;
        std::cout << swimmer->name << " has swum " << swimmer->time * swimmer->speed << " meters\n";
    }

    mtx.lock();
    results.push_back(swimmer);
    mtx.unlock();
}

int main() {
    std::vector<Swimmer> swimmers;
    std::string name;
    int speed;

    for (int i = 0; i < 6; ++i) {
        std::cout << "Enter swimmer's name and speed (in meters per second): ";
        std::cin >> name >> speed;
        swimmers.push_back(Swimmer(name, speed));
    }

    std::vector<std::thread> threads;

    for (std::vector<Swimmer>::iterator it = swimmers.begin(); it != swimmers.end(); ++it) {
        threads.push_back(std::thread(swim, &(*it)));
    }

    for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it) {
        it->join();
    }

    std::cout << "\nFinal Results:\n";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << i + 1 << ". " << results[i]->name << " - " << results[i]->time << " seconds\n";
    }

    return 0;
}
