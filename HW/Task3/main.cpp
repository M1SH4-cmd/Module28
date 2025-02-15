#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>

std::mutex orderMutex;
std::mutex readyMutex;
std::vector<std::string> order_queue;
std::vector<std::string> ready_orders;
int delivered = 0;

std::string getRandomDish() {
    const std::vector<std::string> dishes = {"Pizza", "Soup", "Steak", "Salad", "Sushi"};
    return dishes[rand() % dishes.size()];
}

void orderThread() {
    while (delivered < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(5 + rand() % 6));
        std::string dish = getRandomDish();

        orderMutex.lock();
        order_queue.push_back(dish);
        std::cout << "New order: " << dish << std::endl;
        orderMutex.unlock();
    }
}

void kitchenThread() {
    while (delivered < 10) {
        orderMutex.lock();
        if (!order_queue.empty()) {
            std::string dish = order_queue.front();
            order_queue.erase(order_queue.begin());
            orderMutex.unlock();

            std::cout << "Cooking: " << dish << "..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5 + rand() % 11));

            readyMutex.lock();
            ready_orders.push_back(dish);
            std::cout << dish << " is ready!" << std::endl;
            readyMutex.unlock();
        } else {
            orderMutex.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Небольшая задержка для проверки очереди
    }
}

// Поток для доставки
void courierThread() {
    while (delivered < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));

        readyMutex.lock();
        if (!ready_orders.empty()) {
            std::string dish = ready_orders.front();
            ready_orders.erase(ready_orders.begin());
            delivered++;
            std::cout << "Courier delivered: " << dish << ". Total deliveries: " << delivered << std::endl;
        }
        readyMutex.unlock();
    }
}

int main() {
    srand(time(0));

    std::thread order(orderThread);
    std::thread kitchen(kitchenThread);
    std::thread courier(courierThread);

    order.join();
    kitchen.join();
    courier.join();

    std::cout << "Program finished after 10 deliveries." << std::endl;
    return 0;
}
