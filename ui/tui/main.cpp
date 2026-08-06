#include <atomic>
#include <cctype>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "ui/CLI.hpp"


// TUI (Terminal User Interface): 

static void printMessages(CLI& client)
{
    for (const std::string& msg : client.pollMessages())
        std::cout << msg;
}

int main(void)
{
    CLI client;

    std::cout << "TAP - CLI client" << std::endl;
    std::cout << "Type a connection command to start, e.g. \"nc 127.0.0.1 8080\":" << std::endl;

    std::string line;
    std::string host;
    int         port = 0;

    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            return 1;

        if (!CLI::parseNcCommand(line, host, port))
        {
            std::cout << "Not a valid connection command. Try: nc 127.0.0.1 8080" << std::endl;
            continue;
        }
        if (client.connect(host, port))
            break;
        std::cout << "Could not connect to " << host << ":" << port << std::endl;
    }

    std::cout << "Connected to " << host << ":" << port << std::endl;
    std::cout << "Type commands (e.g. \"CONNECT alice\", \"LOOK\", \"MOVE NORTH\"), "
                 "or \"QUIT\" to exit." << std::endl;

    std::atomic<bool> running{true};
    std::thread printerThread([&client, &running]()
    {
        while (running)
        {
            printMessages(client);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // MODIFIED: Ctrl+D (EOF) and "QUIT" same functionality
    while (client.isConnected())
    {
        if (!std::getline(std::cin, line))
        {
            // Ctrl+D
            client.sendCommand("QUIT");
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            break;
        }

        if (line.empty())
            continue;

        client.sendCommand(line);

        std::string upperLine = line;
        for (char& c : upperLine)
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if (upperLine == "QUIT")
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            break;
        }
    }

    running = false;
    printerThread.join();

    client.disconnect();
    return 0;
}