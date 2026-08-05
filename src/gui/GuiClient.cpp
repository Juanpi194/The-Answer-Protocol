#include "gui/GuiClient.hpp"
#include "commands/CommandHandler.hpp"
#include "commands/commandparser.hpp"
#include "utils/utils.hpp"

// Movidas windows
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

static const std::string DEFAULT_WORLD_NAME = "The Amazing World Of Gumball";

GuiClient::GuiClient(void):
    mode_(SessionMode::NONE),
    socketFd_(-1),
    remoteRunning_(false)
{
}

GuiClient::~GuiClient(void)
{
    disconnect();
}

// --- Connecting --------------------------------------------------------

void GuiClient::connectLocal(const std::string& name)
{
    disconnect();

    // Mirrors debug_mode() in tap.cpp exactly: World's default constructor
    // (no json_path -> no parser involved, see the World.cpp temp room),
    // a PlayerConnection with no socket/server, and the player placed in
    // the spawn room.
    world_ = std::make_unique<World>(DEFAULT_WORLD_NAME);
    connection_ = std::make_unique<PlayerConnection>(name, -1, nullptr);
    world_->get_spawn_room()->add_player(&connection_->get_player());
    // MODIFIED: connect() ya no existe en PlayerConnection -- el
    // constructor deja `connected` en true directamente.

    mode_ = SessionMode::LOCAL;
}

bool GuiClient::connectRemote(const std::string& host, int port, const std::string& name)
{
    disconnect();

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

    if (connect(socketFd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        closeSocket();
        return false;
    }

    mode_ = SessionMode::REMOTE;
    remoteRunning_ = true;
    recvThread_ = std::thread(&GuiClient::remoteRecvLoop, this);

    // Skeleton only: the server does not read/validate this yet
    // (Server::client_thread hardcodes the player name to "Juanpi").
    // Sent anyway so the wiring is in place once it does.
    sendCommand(name);
    return true;
}

void GuiClient::disconnect(void)
{
    if (mode_ == SessionMode::REMOTE)
    {
        remoteRunning_ = false;
        closeSocket();
        if (recvThread_.joinable())
            recvThread_.join();
#ifdef _WIN32
        WSACleanup();
#endif
    }
    world_.reset();
    connection_.reset();
    mode_ = SessionMode::NONE;
}

// --- Talking to the game -------------------------------------------------

void GuiClient::sendCommand(const std::string& text)
{
    if (mode_ == SessionMode::LOCAL)
    {
        try
        {
            Command cmd = CommandParser::parse(text);
            CommandHandler::handle(cmd, *connection_, *world_);
        }
        catch (const CommandParseError& e)
        {
            // Same feedback channel as any other command result, so it
            // shows up in the log like everything else.
            connection_->get_player().send_to_outbox(std::string("Parse error: ") + e.what());
        }
    }
    else if (mode_ == SessionMode::REMOTE && socketFd_ >= 0)
    {
        send(socketFd_, text.c_str(), text.size(), 0);
    }
}

std::vector<std::string> GuiClient::pollMessages(void)
{
    std::vector<std::string> result;

    if (mode_ == SessionMode::LOCAL)
    {
        std::list<std::string> outbox = connection_->get_player().drain_outbox();
        result.assign(outbox.begin(), outbox.end());
    }
    else if (mode_ == SessionMode::REMOTE)
    {
        std::lock_guard<std::mutex> lock(incomingMtx_);
        result.assign(incoming_.begin(), incoming_.end());
        incoming_.clear();
    }
    return result;
}

// --- State -----------------------------------------------------------------

SessionMode GuiClient::mode(void) const noexcept
{
    return mode_;
}

bool GuiClient::isConnected(void) const noexcept
{
    return mode_ != SessionMode::NONE;
}

Player* GuiClient::getPlayer(void) const noexcept
{
    if (mode_ == SessionMode::LOCAL && connection_)
        return &connection_->get_player();
    return nullptr;
}

// --- Private -----------------------------------------------------------

void GuiClient::remoteRecvLoop(void)
{
    char buf[1024];

    while (remoteRunning_)
    {
        int bytes = static_cast<int>(recv(socketFd_, buf, sizeof(buf) - 1, 0));
        if (bytes <= 0)
            break; // Error Exit
        buf[bytes] = '\0';
        {
            std::lock_guard<std::mutex> lock(incomingMtx_);
            incoming_.emplace_back(buf, bytes);
        }
    }
}

void GuiClient::closeSocket(void)
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