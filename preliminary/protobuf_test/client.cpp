#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>

#include "test.pb.h"
#include "client_connection.hpp"

int main()
{
    std::cout << "Starting client..." << std::endl;

    ClientConnection connection(
        "192.168.1.122", 2301,

        [](std::string msg)
        { std::cout << "Client received: " << msg << std::endl; },

        []()
        {
            std::cout << "Client connected!" << std::endl;
        });

    int i = 0;
    while (true)
    {
        std::string message = "Test " + std::to_string(i);

        std::cout << "Seding: " << message << std::endl;

        connection.send_message(message);

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        i++;
    }
}