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

        /**
         * @brief   Local session: builds a World and a PlayerConnection in
         *          this same process (client_fd=-1, server=nullptr), same
         *          as debug_mode() in tap.cpp. Always succeeds unless the
         *          name fails Player's own validation (throws in that case,
         *          same as the real classes do -- not caught here).
         */
        void connectLocal(const std::string& name);

        /**
         * @brief   Remote session (skeleton): opens a real TCP socket to
         *          host:port and sends `name` as the first raw line. The
         *          server does not parse this yet (client_thread hardcodes
         *          "Juanpi" server-side) -- this exists so the wiring is in
         *          place for when it does.
         * @returns `true` if the socket connected, `false` otherwise.
         */
        bool connectRemote(const std::string& host, int port, const std::string& name);

        void disconnect(void);

        // --- Talking to the game ------------------------------------------

        /**
         * @brief   Sends a raw command line, exactly as a real client would
         *          type it (e.g. "MOVE NORTH"). Routed to
         *          CommandHandler::handle() in LOCAL mode, or written to the
         *          socket in REMOTE mode. No-op if not connected.
         */
        void sendCommand(const std::string& text);

        /**
         * @brief   Returns and clears whatever new lines arrived since the
         *          last call -- Player's outbox in LOCAL mode, or whatever
         *          text came over the socket in REMOTE mode. Call this once
         *          per frame.
         */
        std::vector<std::string> pollMessages(void);

        // --- State ---------------------------------------------------------

        SessionMode mode(void) const noexcept;
        bool        isConnected(void) const noexcept;

        /**
         * @brief   The real Player object, ONLY in LOCAL mode. Returns
         *          nullptr in REMOTE mode (there is no local Player there --
         *          see the class comment above) and when not connected.
         *          The UI must read everything it shows through this.
         * @note    Non-const because Fighter::get_inventory() (used to draw
         *          the Inventory tab) is itself non-const in the real class,
         *          which we are not touching. The UI is still expected to
         *          treat this as read-only by convention -- there is no
         *          compiler enforcement of that here.
         */
        Player* getPlayer(void) const noexcept;

    private:
        SessionMode mode_;

        // --- LOCAL mode state ---
        std::unique_ptr<World>             world_;
        std::unique_ptr<PlayerConnection>  connection_;

        // --- REMOTE mode state (skeleton) ---
        int                         socketFd_;
        std::thread                 recvThread_;
        std::atomic<bool>           remoteRunning_;
        std::mutex                  incomingMtx_;
        std::deque<std::string>     incoming_;

        void remoteRecvLoop(void);
        void closeSocket(void);
};
