#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <mutex>


std::vector<std::string> history;
std::mutex history_access;


void wait_for_call(int time, std::string name)
{
    std::this_thread::sleep_for(std::chrono::seconds(time));
    std::cout << std::this_thread::get_id() << std::endl;
    std::cout << "Ring ding dong... From " << name << std::endl;
    history_access.lock();
    history.push_back(name);
    history_access.unlock();
}



int main()
{
    int time;
    std::cin >> time;
    std::string name;
    std::cin >> name;

    std::cout << std::this_thread::get_id() << std::endl;
    std::thread call(wait_for_call, time, name);
    std::thread call2(wait_for_call, 4, "Skillbox");
    call.detach();
    call2.detach();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    if(call.joinable()) call.join();
    if(call2.joinable()) call2.join();

    history_access.lock();
    std::cout << "Missed calls:" << std::endl;
    for (int i = 0; i < history.size(); ++i) {
        std::cout << history[i] << std::endl;
    }
    history_access.unlock();


    return 0;
}