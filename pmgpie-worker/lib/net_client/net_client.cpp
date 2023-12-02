/**
 * Network client for PMGPIe Worker
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 02/12/2023
 */

#include "net_client.hpp"

#define CONNECT_RETRY_INTERVAL 10

#include <iostream>

net::NetworkClient::NetworkClient(std::string coordinator_host)
{
    // Save config
    this->coordinator_host = coordinator_host;
    this->port = "3141";

    // Construct ASIO stuff
    this->io_service = std::make_shared<asio ::io_service>();
    this->socket = std::make_shared<asio ::ip::tcp::socket>(*this->io_service);

    this->connected = false;

    net::Message test;
    test.header.type = net::MessageType::DispatchWorkUnit;

    net::DispatchWorkUnitBody body;
    body.n_digits = 1000;
    body.start = 10000;
    body.thread = 5;
    test << body;

    std::cout << test << std::endl;
}

void net::NetworkClient::run()
{

    net::Message in_message;

    // Main loop
    while (true)
    {
        if (!this->connected)
            do_connect(); // Connect if not connected

        // Read next message
        try
        {
            in_message = read_message();
            std::cout << "Msg: " << in_message << std::endl;
        }
        catch (std::system_error &e)
        {
            std::cout << e.what() << std::endl;

            // We are no longer connected to the server
            this->connected = false;
            continue;
        }

        // Handle received message
        handle_message(in_message);
    }
};

/**
 * Handle connecting to the server
 */
void net::NetworkClient::do_connect()
{
    // Resolve endpoint for provided host
    asio::ip::tcp::resolver resolver(*this->io_service);
    asio::ip::tcp::resolver::query query(this->coordinator_host, this->port);

    while (!this->connected)
    {
        asio::ip::basic_resolver_results<asio::ip::tcp> endpoints;
        try
        {
            endpoints = resolver.resolve(query);
        }
        catch (std::exception e)
        {
            std::cout << "[NETWORK] Unable to resolve hostname: " << this->coordinator_host << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(CONNECT_RETRY_INTERVAL));
            continue;
        }

        try
        {
            // Connect to socket
            asio::connect(*this->socket, endpoints);
        }
        catch (std::exception e)
        {
            std::cout << "[NETWORK] Connection error: " << e.what() << ", retrying in 10 seconds... " << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(CONNECT_RETRY_INTERVAL));
            continue;
        }

        // Connection was succesful!
        this->connected = true;

        std::cout << "[NETWORK] Connected to " << this->coordinator_host << "!" << std::endl;
    }
}

/**
 * Handle Reading messages from the server
 */
net::Message net::NetworkClient::read_message()
{
    net::Message message;

    // Read messasge header
    message.header = read_header();

    // Read body as well
    if (message.header.length > 0)
    {
        read_body(message.header.length, message.body);
    }

    return message;
}

net::MessageHeader net::NetworkClient::read_header()
{
    net::MessageHeader header;

    // Read header
    asio::read(*this->socket, asio::buffer(&header, sizeof(net::MessageHeader)));

    return header;
}

void net::NetworkClient::read_body(size_t length, std::vector<uint8_t> &body)
{
    body.resize(length);

    // Read body
    asio::read(*this->socket, asio::buffer(body.data(), length));
}

void net::NetworkClient::handle_message(net::Message message)
{
    std::cout << message;
    // Check type of message
    switch (message.header.type)
    {
    case net::MessageType::DispatchWorkUnit:
        break;
    }
}

void net::NetworkClient::handle_dispatch_work_unit_message(net::Message message)
{

    // Extract payload from message
    net::DispatchWorkUnitBody body;
    message >> body;

    // Extract work unit from payload
    worker_thread::WorkUnit work_unit;
    work_unit.start = body.start;
    work_unit.n = body.n_digits;

    // Do dispatch
    dispatch_work_unit(work_unit, body.thread);
}

void net::NetworkClient::set_dispatch_work_unit_callback(std::function<void(worker_thread::WorkUnit, int)> callback)
{
    this->dispatch_work_unit_callback = callback;
}

void net::NetworkClient::dispatch_work_unit(worker_thread::WorkUnit work_unit, int thread)
{
    if (this->dispatch_work_unit_callback.has_value())
    {
        this->dispatch_work_unit_callback.value()(work_unit, thread);
    }

    // TODO add exception for when there is no callback
}