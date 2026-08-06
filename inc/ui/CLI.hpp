#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <deque>

// ---------------------------------------------------------------------------
// MODIFIED: added parseNcCommand(). Both UIs now bootstrap the connection
// by having the player type a line like "nc 127.0.0.1 8080" -- same idea
// as running `nc host port` in a real terminal -- instead of filling in a
// host/port form. This lives here (not duplicated in each UI's main.cpp)
// so GUI and TUI parse it identically.
// ---------------------------------------------------------------------------

class CLI
{
    public:
        CLI(void);
        ~CLI(void);

        CLI(const CLI&) = delete;
        CLI& operator=(const CLI&) = delete;

        // --- Connecting --------------------------------------------------

        /**
         * @brief   Opens a real TCP socket to host:port. Nothing is sent
         *          automatically -- the first command (e.g. "CONNECT alice")
         *          is just typed like any other, same as `nc host port`
         *          followed by typing the protocol yourself.
         * @returns `true` if the socket connected, `false` otherwise.
         */
        bool connect(const std::string& host, int port);

        /**
         * @brief   Parses the bootstrap line the player types to open the
         *          connection, e.g. "nc 127.0.0.1 8080", "nc localhost",
         *          "nc localhost:8080", or just "nc" (defaults to
         *          127.0.0.1:8080, matching Server::DEFAULT_PORT).
         *          This is UI-side parsing (there is no server to ask
         *          before a socket exists) -- unrelated to the server's
         *          own CommandParser, which only sees lines after connect()
         *          has already succeeded.
         * @param   line    The raw line the player typed.
         * @param   host    Out: the parsed host.
         * @param   port    Out: the parsed port.
         * @returns `true` if `line` matched the expected "nc ..." form,
         *          `false` otherwise (not an nc command at all).
         */
        static bool parseNcCommand(const std::string& line, std::string& host, int& port);

        void disconnect(void);

        // --- Talking to the game -------------------------------------------

        /**
         * @brief   Sends a raw line over the socket, exactly as typed
         *          (e.g. "MOVE NORTH", "CONNECT alice"). No-op if not
         *          connected.
         */
        void sendCommand(const std::string& text);

        /**
         * @brief   Returns and clears whatever new lines arrived over the
         *          socket since the last call. Call this once per frame
         *          (GUI) or after sending a command (TUI).
         */
        std::vector<std::string> pollMessages(void);

        // --- State ---------------------------------------------------------

        bool isConnected(void) const noexcept;

    private:
        int                         socketFd_;
        std::thread                 recvThread_;
        std::atomic<bool>           running_;
        std::mutex                  incomingMtx_;
        std::deque<std::string>     incoming_;

        void recvLoop(void);
        void closeSocket(void);
};