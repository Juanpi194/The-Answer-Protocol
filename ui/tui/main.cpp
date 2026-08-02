#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "ui/CLI.hpp"

// ---------------------------------------------------------------------------
// TUI (Terminal User Interface): el cliente CLI que pide el subject aparte
// del server y de la GUI. Comparte CLI al 100% con ui/gui/main.cpp -- la
// única diferencia es cómo se muestra lo que llega (aquí, texto plano).
//
// MODIFIED: sin modo Local -- solo conecta por socket real. La primera
// instrucción que escribe el jugador es la propia conexión, estilo
// `nc <host> <puerto>` (ver CLI::parseNcCommand) -- no hay formulario de
// host/puerto separado. El nombre tampoco se pide aquí: se manda como un
// comando más ("CONNECT alice") una vez conectado, igual que cualquier otro.
// ---------------------------------------------------------------------------

static void printMessages(CLI& client)
{
    for (const std::string& msg : client.pollMessages())
        std::cout << msg << std::endl;
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

    // MODIFIED: hilo impresor en segundo plano -- imprime lo que llegue por
    // el socket en cuanto llega, sin esperar a que el usuario escriba la
    // siguiente línea. El hilo principal se queda libre para hacer lo que
    // siempre ha hecho una terminal: bloquear en std::getline() esperando
    // la siguiente línea.
    std::atomic<bool> running{true};
    std::thread printerThread([&client, &running]()
    {
        while (running)
        {
            printMessages(client);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    while (client.isConnected() && std::getline(std::cin, line))
    {
        if (line.empty())
            continue;

        client.sendCommand(line);
    }

    running = false;
    printerThread.join();

    client.disconnect();
    return 0;
}