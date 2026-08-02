#include "ui/CLI.hpp"

#include <sstream>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

static const int DEFAULT_PORT = 8080; // matches Server::DEFAULT_PORT

CLI::CLI(void):
    socketFd_(-1),
    running_(false)
{
}

CLI::~CLI(void)
{
    disconnect();
}

// --- Connecting --------------------------------------------------------

bool CLI::connect(const std::string& host, int port)
{
    disconnect();

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return false;
#endif

    socketFd_ = static_cast<int>(socket(AF_INET, SOCK_STREAM, 0));
    if (socketFd_ < 0)
        return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<unsigned short>(port));
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
    {
        closeSocket();
        return false;
    }

    if (::connect(socketFd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        closeSocket();
        return false;
    }

    running_ = true;
    recvThread_ = std::thread(&CLI::recvLoop, this);
    return true;
}

// MODIFIED: parser del "nc host port" que el jugador escribe como primera
// linea (en vez de un formulario de host/puerto). Formas aceptadas:
//   "nc"                      -> 127.0.0.1:8080
//   "nc <host>"                -> <host>:8080
//   "nc <host> <port>"         -> <host>:<port>
//   "nc <host>:<port>"         -> <host>:<port> (todo junto)
bool CLI::parseNcCommand(const std::string& line, std::string& host, int& port)
{
    std::istringstream iss(line);
    std::string keyword;

    iss >> keyword;
    if (keyword != "nc" && keyword != "NC")
        return false;

    host = "127.0.0.1";
    port = DEFAULT_PORT;

    std::string hostToken;
    if (iss >> hostToken)
    {
        size_t colonPos = hostToken.find(':');
        if (colonPos != std::string::npos)
        {
            host = hostToken.substr(0, colonPos);
            try
            {
                port = std::stoi(hostToken.substr(colonPos + 1));
            }
            catch (const std::exception&)
            {
                return false; // "nc host:notanumber"
            }
        }
        else
        {
            host = hostToken;
            std::string portToken;
            if (iss >> portToken)
            {
                try
                {
                    port = std::stoi(portToken);
                }
                catch (const std::exception&)
                {
                    return false; // "nc host notanumber"
                }
            }
        }
    }

    if (host == "localhost")
        host = "127.0.0.1";

    return true;
}

void CLI::disconnect(void)
{
    running_ = false;
    closeSocket();
    if (recvThread_.joinable())
        recvThread_.join();
#ifdef _WIN32
    WSACleanup();
#endif
}

// --- Talking to the game -------------------------------------------------

void CLI::sendCommand(const std::string& text)
{
    if (socketFd_ >= 0)
        send(socketFd_, text.c_str(), text.size(), 0);
}

std::vector<std::string> CLI::pollMessages(void)
{
    std::vector<std::string> result;
    std::lock_guard<std::mutex> lock(incomingMtx_);

    result.assign(incoming_.begin(), incoming_.end());
    incoming_.clear();
    return result;
}

// --- State -----------------------------------------------------------------

bool CLI::isConnected(void) const noexcept
{
    return socketFd_ >= 0;
}

// --- Private -----------------------------------------------------------

void CLI::recvLoop(void)
{
    char buf[1024];

    while (running_)
    {
        int bytes = static_cast<int>(recv(socketFd_, buf, sizeof(buf) - 1, 0));
        if (bytes <= 0)
            break; // connection closed or error
        buf[bytes] = '\0';
        {
            std::lock_guard<std::mutex> lock(incomingMtx_);
            incoming_.emplace_back(buf, bytes);
        }
    }
}

void CLI::closeSocket(void)
{
    if (socketFd_ >= 0)
    {
#ifdef _WIN32
        closesocket(socketFd_);
#else
        close(socketFd_);
#endif
        socketFd_ = -1;
    }
}