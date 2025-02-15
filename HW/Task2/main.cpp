#include <iostream>
#include <thread>
#include <mutex>
#include <string>

std::mutex stationMutex;

void travel(char id, int time) {
    std::cout << "Train " << id << " is traveling for " << time << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(time));

    std::cout << "Train " << id << " is waiting for the station to be free." << std::endl;
    stationMutex.lock();

    std::cout << "Train " << id << " has arrived at the station." << std::endl;

    std::string command;
    while (true) {
        std::cout << "Type 'depart' to allow train " << id << " to leave the station: ";
        std::cin >> command;
        if (command == "depart") {
            break;
        } else {
            std::cout << "Invalid command. Please type 'depart'." << std::endl;
        }
    }

    std::cout << "Train " << id << " is departing." << std::endl;
    stationMutex.unlock();
}

int main() {
    int timeA, timeB, timeC;

    std::cout << "Enter traveling time for train A: ";
    std::cin >> timeA;
    std::cout << "Enter traveling time for train B: ";
    std::cin >> timeB;
    std::cout << "Enter traveling time for train C: ";
    std::cin >> timeC;

    std::thread trainA(travel, 'A', timeA);
    std::thread trainB(travel, 'B', timeB);
    std::thread trainC(travel, 'C', timeC);

    trainA.join();
    trainB.join();
    trainC.join();

    std::cout << "All trains have visited the station." << std::endl;
    return 0;
}
