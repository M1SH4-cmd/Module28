#include <iostream>
#include <thread>
#include <mutex>
#include <string>

std::mutex mtx;
bool isFree = true;

void travel(char id, int time) {
    std::cout << "Train " << id << " is traveling for " << time << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(time));

    mtx.lock();
    while (!isFree) {
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mtx.lock();
    }

    isFree = false;
    std::cout << "Train " << id << " has arrived at the station." << std::endl;

    std::string command;
    while (true) {
        std::cout << "Type 'depart' to free the train station for the next train: ";
        std::cin >> command;
        if (command == "depart") {
            break;
        } else {
            std::cout << "Invalid command. Please type 'depart'." << std::endl;
        }
    }

    std::cout << "Train " << id << " is departing." << std::endl;
    isFree = true;
    mtx.unlock();
}

int main() {
    int time;

    std::cout << "Enter traveling time for train A: ";
    std::cin >> time;
    std::thread trainA(travel, 'A', time);

    std::cout << "Enter traveling time for train B: ";
    std::cin >> time;
    std::thread trainB(travel, 'B', time);

    std::cout << "Enter traveling time for train C: ";
    std::cin >> time;
    std::thread trainC(travel, 'C', time);

    trainA.join();
    trainB.join();
    trainC.join();

    return 0;
}