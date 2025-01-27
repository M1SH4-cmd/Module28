#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

void wait_for_call()
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "Ring ding dong..." << std::endl;
}


int main()
{
    std::thread call(wait_for_call);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Oowww vy is Anglii?" << std::endl;
    call.join();
    return 0;
}
