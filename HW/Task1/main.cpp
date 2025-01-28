#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <map>
#define DISTANCE 100


class Swimmer
{
public:
    std::string name;
    int time = 0;
    int distance = 0;
    int speed;
    bool finished = false;

    Swimmer(std::string name, int speed) {
        this->name = name;
        this->speed = speed;
    }

};

std::mutex mtx;


void swim(Swimmer& swimmer) {
    while(swimmer.distance < DISTANCE) {
        swimmer.distance += swimmer.speed;
        std::cout << swimmer.name << " reached " << swimmer.distance << "m" << std::endl;
        swimmer.time += 1;
        mtx.lock();
        std::cout << "Swimmer " << swimmer.name << "reached " << swimmer.distance << "m" << std::endl;
        mtx.unlock();

    }
    swimmer.finished = true;
}

bool comparator(const Swimmer& a, const Swimmer& b) {
    return a.time < b.time;
}

int main() {
    std::vector<Swimmer> swimmers;
    std::string name;
    int speed;

    for (int i = 0; i < 5; i++) {
        std::cout << "Enter swimmer's name and speed in order:" << std::endl;
        std::cin >> name >> speed;
        swimmers.emplace_back(name, speed); //Clion предложил поменять push_back на emplace_back, очень удобная штука
    }

    std::vector<std::thread> threads;

    for (Swimmer& swimmer : swimmers) {
        threads.emplace_back(swim, std::ref(swimmer));
    }

    for (std::thread& thread : threads) {
        thread.join();
    }

    std::sort(swimmers.begin(), swimmers.end(), comparator);


    std::cout << "\nResults:\n";
    for (Swimmer& swimmer : swimmers) {
        std::cout << swimmer.name << ": " << swimmer.time << " seconds" << std::endl;
    }

    return 0;
}