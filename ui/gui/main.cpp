#define SDL_MAIN_HANDLED // avoid depending on SDL2main / WinMain on Windows
#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <cstdio>
#include <thread>
#include <atomic>

#include "ui/CLI.hpp"
#include "libs/json.hpp" // MODIFIED: mismo json.hpp que usa el resto del proyecto (parser/, etc.)

enum class AppState
{
    CONNECT,
    GAME
};

static void logMsg(std::vector<std::string>& log, const std::string& msg)
{
    log.push_back(msg);
    if (log.size() > 200)
        log.erase(log.begin());
}

// ---------------------------------------------------------------------------
// MODIFIED: WorldCache -- la copia local del mapa que comentamos. No existe
// ningun Player local (no hay modo Local): todo lo que sabemos del mundo es
// lo que el servidor nos ha mandado alguna vez en una respuesta a LOOK. Esta
// clase interpreta esas respuestas (JSON) y se queda con lo visto hasta
// ahora -- exactamente igual de espiritu que el "discoveredRooms" que
// teniamos en la version local, solo que alimentado por texto real del
// socket en vez de un World en memoria.
//
// Deliberadamente NO vive en CLI (que sigue siendo solo transporte, sin
// saber nada de JSON ni de salas) ni se comparte con la TUI -- es puramente
// una conveniencia de presentacion para la GUI, tal y como se acordo.
// ---------------------------------------------------------------------------
struct RoomInfo
{
    std::string                        id;
    std::string                        name;
    std::string                        description;
    std::map<std::string, std::string> exits;   // "NORTH" -> id de la sala destino
    std::vector<std::string>           items;   // ids tal cual los manda el server (p.ej. "item.apple.0")
    std::string                        npc;     // id del npc, vacio si no hay
    std::vector<std::string>           players;
};

class WorldCache
{
    public:
        // Intenta interpretar `line` como una respuesta de LOOK. Si no lo
        // es (cualquier otra cosa: "OK room=...", errores, chat...), no
        // hace nada -- esa linea se sigue viendo en el log de siempre, solo
        // que no actualiza el mapa.
        void ingest(const std::string& line)
        {
            size_t braceStart = line.find('{');
            if (braceStart == std::string::npos)
                return;

            nlohmann::json data;
            try
            {
                data = nlohmann::json::parse(line.substr(braceStart));
            }
            catch (const std::exception&)
            {
                return; // no era JSON valido -- no es una respuesta de LOOK
            }

            if (!data.contains("room") || !data["room"].is_object())
                return;

            const nlohmann::json& roomJson = data["room"];
            RoomInfo info;
            info.id = roomJson.value("id", "");
            info.name = roomJson.value("name", "");
            info.description = roomJson.value("description", "");

            if (roomJson.contains("exits") && roomJson["exits"].is_object())
            {
                for (auto it = roomJson["exits"].begin(); it != roomJson["exits"].end(); ++it)
                    if (it.value().is_string())
                        info.exits[it.key()] = it.value().get<std::string>();
            }

            // MODIFIED: el server manda "items"/"npc" como el string "None"
            // cuando estan vacios, y como array/string de verdad si no --
            // hay que comprobar el tipo antes de leer.
            if (data.contains("items") && data["items"].is_array())
                for (const auto& item : data["items"])
                    if (item.is_string())
                        info.items.push_back(item.get<std::string>());

            if (data.contains("npc") && data["npc"].is_string())
            {
                std::string npc = data["npc"].get<std::string>();
                if (npc != "None")
                    info.npc = npc;
            }

            if (data.contains("players") && data["players"].is_array())
                for (const auto& p : data["players"])
                    if (p.is_string())
                        info.players.push_back(p.get<std::string>());

            if (info.id.empty())
                return;

            rooms_[info.id] = info;
            currentRoomId_ = info.id;
        }

        const RoomInfo* getRoom(const std::string& id) const
        {
            auto it = rooms_.find(id);
            return (it != rooms_.end()) ? &it->second : nullptr;
        }

        const RoomInfo* getCurrentRoom() const
        {
            return getRoom(currentRoomId_);
        }

        const std::map<std::string, RoomInfo>& getRooms() const
        {
            return rooms_;
        }

    private:
        std::map<std::string, RoomInfo> rooms_;
        std::string                     currentRoomId_;
};

// ---------------------------------------------------------------------------
// MODIFIED: pantalla de conexion -- sin cambios de fondo respecto a la
// version anterior (hilo aparte para no bloquear el render, comando estilo
// "nc host puerto").
// ---------------------------------------------------------------------------
static void drawConnectScreen(CLI& client, AppState& state, std::string& errorMsg)
{
    static char              inputBuf[128] = "";
    static std::thread       connectThread;
    static std::atomic<bool> connecting{false};
    static std::atomic<bool> connectSucceeded{false};
    static std::string       connectHost;
    static int               connectPort = 0;

    if (connectThread.joinable() && !connecting)
    {
        connectThread.join();
        if (connectSucceeded)
            state = AppState::GAME;
        else
            errorMsg = "Could not connect to " + connectHost + ":" + std::to_string(connectPort);
    }

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                             ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(420, 140), ImGuiCond_FirstUseEver);
    ImGui::Begin("TAP", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped("Type a connection command to start, e.g.:");
    ImGui::TextColored(ImVec4(0.6f, 0.9f, 1.0f, 1.0f), "  nc 127.0.0.1 8080");

    ImGui::Separator();
    ImGui::BeginDisabled(connecting);
    bool submitted = ImGui::InputText("##ConnectInput", inputBuf, IM_ARRAYSIZE(inputBuf),
                                       ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    bool pressed = ImGui::Button("Go");
    ImGui::EndDisabled();

    if (connecting)
    {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.9f, 0.8f, 0.3f, 1.0f), "Connecting...");
    }

    if (!connecting && (submitted || pressed))
    {
        errorMsg.clear();
        std::string host;
        int port = 0;

        if (!CLI::parseNcCommand(inputBuf, host, port))
        {
            errorMsg = "Not a valid connection command. Try: nc 127.0.0.1 8080";
        }
        else
        {
            connectHost = host;
            connectPort = port;
            connecting = true;
            connectThread = std::thread([&client, host, port]()
            {
                bool ok = client.connect(host, port);
                connectSucceeded = ok;
                connecting = false;
            });
        }
    }

    if (!errorMsg.empty())
    {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", errorMsg.c_str());
    }

    ImGui::End();
}

// ---------------------------------------------------------------------------
// Mapa: mismo BFS por direcciones que teniamos en la version local, pero
// ahora sobre el WorldCache (ids/direcciones como strings, tal y como
// llegan del servidor) en vez de sobre un World/Room de verdad.
// ---------------------------------------------------------------------------
static ImVec2 directionOffset(const std::string& dir)
{
    if (dir == "NORTH") return ImVec2(0, -1);
    if (dir == "SOUTH") return ImVec2(0, 1);
    if (dir == "EAST")  return ImVec2(1, 0);
    if (dir == "WEST")  return ImVec2(-1, 0);
    return ImVec2(0, 0);
}

static void drawMapWindow(const WorldCache& world)
{
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Discovered Map");

    const RoomInfo* current = world.getCurrentRoom();
    if (!current)
    {
        ImGui::TextDisabled("Nothing seen yet -- try \"LOOK\".");
        ImGui::End();
        return;
    }

    std::map<std::string, ImVec2> coords;
    std::set<std::string>         visited;
    std::queue<std::string>       queue;

    coords[current->id] = ImVec2(0, 0);
    queue.push(current->id);
    visited.insert(current->id);

    while (!queue.empty())
    {
        std::string roomId = queue.front();
        queue.pop();
        const RoomInfo* room = world.getRoom(roomId);
        if (!room)
            continue;
        ImVec2 base = coords[roomId];

        for (const auto& [dir, targetId] : room->exits)
        {
            if (!world.getRoom(targetId) || visited.count(targetId))
                continue; // solo dibujamos salas que ya hemos visto de verdad (con LOOK)

            ImVec2 off = directionOffset(dir);
            coords[targetId] = ImVec2(base.x + off.x, base.y + off.y);
            visited.insert(targetId);
            queue.push(targetId);
        }
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();
    const float cell = 90.0f;

    for (const auto& [roomId, room] : world.getRooms())
    {
        if (!coords.count(roomId))
            continue; // sala conocida pero no alcanzable desde la actual por salas ya vistas
        ImVec2 grid = coords[roomId];
        ImVec2 pos(origin.x + 150 + grid.x * cell, origin.y + 150 + grid.y * cell);

        bool isCurrent = (roomId == current->id);
        ImU32 fill = isCurrent ? IM_COL32(120, 200, 120, 255) : IM_COL32(200, 200, 200, 255);

        drawList->AddRectFilled(pos, ImVec2(pos.x + cell - 10, pos.y + cell - 10), fill);
        drawList->AddRect(pos, ImVec2(pos.x + cell - 10, pos.y + cell - 10), IM_COL32(0, 0, 0, 255));
        drawList->AddText(ImVec2(pos.x + 5, pos.y + 5), IM_COL32(0, 0, 0, 255), room.name.c_str());
    }

    ImGui::Dummy(ImVec2(400, 400));
    ImGui::End();
}

static void drawRoomWindow(const WorldCache& world)
{
    ImGui::SetNextWindowPos(ImVec2(440, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Current Room");

    const RoomInfo* room = world.getCurrentRoom();
    if (!room)
    {
        ImGui::TextDisabled("Nothing seen yet -- try \"LOOK\".");
        ImGui::End();
        return;
    }

    ImGui::TextColored(ImVec4(1, 1, 0.6f, 1), "%s", room->name.c_str());
    ImGui::TextWrapped("%s", room->description.c_str());

    ImGui::Separator();
    ImGui::Text("Items in this room:");
    if (room->items.empty())
        ImGui::TextDisabled("(none)");
    else
        for (const std::string& item : room->items)
            ImGui::BulletText("%s", item.c_str());

    ImGui::Separator();
    ImGui::Text("NPC in this room:");
    if (room->npc.empty())
        ImGui::TextDisabled("(none)");
    else
        ImGui::BulletText("%s", room->npc.c_str());

    ImGui::Separator();
    ImGui::Text("Players here:");
    if (room->players.empty())
        ImGui::TextDisabled("(none)");
    else
        for (const std::string& p : room->players)
            ImGui::BulletText("%s", p.c_str());

    ImGui::End();
}

// ---------------------------------------------------------------------------
// Menu: Actions / Inventory / Stats / Settings.
// MODIFIED: los botones de mover mandan MOVE y, encadenado, un LOOK -- el
// servidor solo responde "OK room=<id>" a un MOVE (sin detalles), asi que
// sin este LOOK automatico el mapa/sala no se refrescarian solos.
// Inventory/Stats se quedan como estaban: el servidor no tiene INVENTORY ni
// STATUS implementados todavia, asi que no hay nada estructurado que
// mostrar ahi -- ver el chat para la respuesta en crudo.
// ---------------------------------------------------------------------------
static void drawMenuWindow(CLI& client, std::vector<std::string>& log)
{
    static int selectedIndex = -1;
    (void)selectedIndex;

    ImGui::SetNextWindowPos(ImVec2(20, 440), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(820, 240), ImGuiCond_FirstUseEver);
    ImGui::Begin("Menu");

    if (ImGui::BeginTabBar("MenuTabs"))
    {
        if (ImGui::BeginTabItem("Actions"))
        {
            ImGui::Text("Move:");
            const char* dirs[] = { "NORTH", "SOUTH", "EAST", "WEST" };
            for (const char* dir : dirs)
            {
                if (ImGui::Button(dir))
                {
                    std::string moveCmd = std::string("MOVE ") + dir;
                    client.sendCommand(moveCmd);
                    logMsg(log, "> " + moveCmd);
                    client.sendCommand("LOOK"); // MODIFIED: refresca mapa/sala automaticamente
                    logMsg(log, "> LOOK");
                }
                ImGui::SameLine();
            }
            ImGui::NewLine();

            ImGui::Separator();
            if (ImGui::Button("Look"))
            {
                client.sendCommand("LOOK");
                logMsg(log, "> LOOK");
            }
            ImGui::NewLine();

            ImGui::Separator();
            ImGui::Text("Log:");
            ImGui::BeginChild("LogScroll", ImVec2(0, 90), true);
            for (const std::string& line : log)
                ImGui::TextUnformatted(line.c_str());
            ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();

            static char commandBuf[256] = "";
            ImGui::PushItemWidth(-70);
            bool submitted = ImGui::InputText("##CommandInput", commandBuf, IM_ARRAYSIZE(commandBuf),
                                               ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            bool sendPressed = ImGui::Button("Send");
            if ((submitted || sendPressed) && commandBuf[0] != '\0')
            {
                client.sendCommand(commandBuf);
                logMsg(log, std::string("> ") + commandBuf);
                commandBuf[0] = '\0';
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Inventory"))
        {
            ImGui::TextDisabled("Not available yet -- the server does not send "
                                 "structured inventory state. Try \"INVENTORY\" "
                                 "in the chat and watch the log.");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Stats"))
        {
            ImGui::TextDisabled("Not available yet -- the server does not send "
                                 "structured stats state. Try \"STATUS\" in the "
                                 "chat and watch the log.");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Settings"))
        {
            // Intentionally empty for now.
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

int main(int, char**)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::fprintf(stderr, "Error initializing SDL2: %s\n", SDL_GetError());
        return 1;
    }

    const char* glslVersion = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(
        "TAP - GUI client",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 700, windowFlags);
    if (!window)
    {
        std::fprintf(stderr, "Error creating the SDL2 window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().FontGlobalScale = 1.5f;
    ImGui::GetStyle().ScaleAllSizes(1.5f);

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion);

    CLI                         client;
    AppState                    state = AppState::CONNECT;
    std::string                 connectError;
    std::vector<std::string>    log;
    WorldCache                  world; // MODIFIED: copia local del mapa, alimentada por LOOK
    bool                        running = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_WINDOWEVENT
                && event.window.event == SDL_WINDOWEVENT_CLOSE
                && event.window.windowID == SDL_GetWindowID(window))
                running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (state == AppState::CONNECT)
        {
            drawConnectScreen(client, state, connectError);
        }
        else
        {
            for (const std::string& msg : client.pollMessages())
            {
                logMsg(log, msg);
                world.ingest(msg); // MODIFIED: cada mensaje puede ser una respuesta de LOOK
            }

            drawMapWindow(world);
            drawRoomWindow(world);
            drawMenuWindow(client, log);
        }

        ImGui::Render();
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}