#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

std::mutex kitchenMutex;
bool isKitchenFree = true;
int completedDeliveries = 0;

std::string getRandomDish() {
    const std::vector<std::string> dishes = {"Pizza", "Soup", "Steak", "Salad", "Sushi"};
    return dishes[rand() % dishes.size()];
}

void kitchen() {
    while (completedDeliveries < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(5 + rand() % 6));
        std::string dish = getRandomDish();

        kitchenMutex.lock();
        while (!isKitchenFree) {
            kitchenMutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            kitchenMutex.lock();
        }

        isKitchenFree = false;
        kitchenMutex.unlock();

        std::cout << "Kitchen is preparing " << dish << "..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5 + rand() % 11));

        kitchenMutex.lock();
        isKitchenFree = true;
        std::cout << dish << " is ready!" << std::endl;
        kitchenMutex.unlock();
    }
}

void courier() {
    while (completedDeliveries < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        std::cout << "Courier is picking up the orders..." << std::endl;

        kitchenMutex.lock();
        if (isKitchenFree) {
            completedDeliveries++;
            std::cout << "Courier has delivered an order. Total deliveries: " << completedDeliveries << std::endl;
        }
        kitchenMutex.unlock();
    }
}

int main() {
    srand(time(0));

    std::thread kitchenThread(kitchen);
    std::thread courierThread(courier);

    kitchenThread.detach();
    courierThread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(300)); // Дал программе время на выполнение
    return 0;
}