#pragma once
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <deque>
#include "world/World.hpp"
#include "server/PlayerConnection.hpp"
#include "characters/Player.hpp"

enum class SessionMode
{
    NONE,
    LOCAL,
    REMOTE
};

class GuiClient
{
    public:
        GuiClient(void);
        ~GuiClient(void);

        GuiClient(const GuiClient&) = delete;
        GuiClient& operator=(const GuiClient&) = delete;

        // --- Connecting --------------------------------------------------
        void connectLocal(const std::string& name);
        bool connectRemote(const std::string& host, int port, const std::string& name);

        void disconnect(void);

        // --- Talking to the game ------------------------------------------
        void sendCommand(const std::string& text);

        std::vector<std::string> pollMessages(void);

        // --- State ---------------------------------------------------------

        SessionMode mode(void) const noexcept;
        bool        isConnected(void) const noexcept;

        Player* getPlayer(void) const noexcept;

    private:
        SessionMode mode_;

        // --- LOCAL mode state ---
        std::unique_ptr<World>             world_;
        std::unique_ptr<PlayerConnection>  connection_;

        // --- TODO: REMOTE mode state ---
        int                         socketFd_;
        std::thread                 recvThread_;
        std::atomic<bool>           remoteRunning_;
        std::mutex                  incomingMtx_;
        std::deque<std::string>     incoming_;

        void remoteRecvLoop(void);
        void closeSocket(void);
};
