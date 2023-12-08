#include <iostream>

#include "server_class.hpp"

int main()
{
    Server server(
        2301,
        [](std::string msg, std::shared_ptr<ServerConnection> connection)
        {
            std::string reply = "Server replies to: ";
            reply.append(msg);

            std::cout << "Message received: " << msg << std::endl;

            connection->send_message(reply);
        });

    while (true)
    {
        server.send_all("Hello there!");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}