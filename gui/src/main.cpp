#define SDL_MAIN_HANDLED // avoid depending on SDL2main / WinMain on Windows
#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <algorithm>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <cstdio>
#include "gui/GuiClient.hpp"
#include "world/Room.hpp"
#include "items/Item.hpp"
#include "characters/NPC.hpp"
#include "characters/Fighter.hpp"
#include "utils/types.hpp"

enum class AppState
{
    LOGIN,
    GAME
};

static void logMsg(std::vector<std::string>& log, const std::string& msg)
{
    log.push_back(msg);
    if (log.size() > 200)
        log.erase(log.begin());
}

static void drawLoginScreen(GuiClient& client, AppState& state, std::string& errorMsg)
{
    static char username[64] = "";
    static char password[64] = "";
    static char host[64] = "127.0.0.1";
    static int  port = 8080;
    static int  modeIndex = 0;

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                             ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(420, 260), ImGuiCond_FirstUseEver);
    ImGui::Begin("TAP - Login", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
    ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

    ImGui::Separator();
    ImGui::RadioButton("Local (single process)", &modeIndex, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Remote (TCP socket)", &modeIndex, 1);

    if (modeIndex == 1)
    {
        ImGui::InputText("Host", host, IM_ARRAYSIZE(host));
        ImGui::InputInt("Port", &port);
    }

    ImGui::Separator();
    if (ImGui::Button("Play", ImVec2(120, 0)))
    {
        errorMsg.clear();
        if (username[0] == '\0')
        {
            errorMsg = "Username cannot be empty.";
        }
        else if (modeIndex == 0)
        {
            try
            {
                client.connectLocal(username);
                state = AppState::GAME;
            }
            catch (const std::exception& e)
            {
                errorMsg = std::string("Could not start local session: ") + e.what();
            }
        }
        else
        {
            // TODO: SERVER DEBE RECOGER USERNAME Y PASSWORD
            if (client.connectRemote(host, port, username))
                state = AppState::GAME;
            else
                errorMsg = "Could not connect to " + std::string(host) + ":" + std::to_string(port);
        }
    }

    if (!errorMsg.empty())
    {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", errorMsg.c_str());
    }

    ImGui::End();
}

static void drawMapWindow(Player* player, std::set<std::string>& discovered)
{
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Discovered Map");

    if (!player)
    {
        ImGui::TextDisabled("Not available in Remote mode yet.");
        ImGui::End();
        return;
    }

    Room* currentRoom = player->get_current_room();
    if (!currentRoom)
    {
        ImGui::TextDisabled("Player has no current room.");
        ImGui::End();
        return;
    }
    discovered.insert(currentRoom->get_id());

    std::map<std::string, ImVec2> coords;
    std::set<std::string>         visited;
    std::queue<Room*>             queue;

    coords[currentRoom->get_id()] = ImVec2(0, 0);
    queue.push(currentRoom);
    visited.insert(currentRoom->get_id());

    while (!queue.empty())
    {
        Room* room = queue.front();
        queue.pop();
        ImVec2 base = coords[room->get_id()];

        for (const auto& [dir, neighbor] : room->get_adjacent_rooms())
        {
            if (!neighbor || !discovered.count(neighbor->get_id()) || visited.count(neighbor->get_id()))
                continue;

            ImVec2 offset = base;
            switch (dir)
            {
                case Direction::NORTH:   offset.y -= 1; break;
                case Direction::SOUTH:   offset.y += 1; break;
                case Direction::EAST:    offset.x += 1; break;
                case Direction::WEST:    offset.x -= 1; break;
                case Direction::INVALID: break;
            }
            coords[neighbor->get_id()] = offset;
            visited.insert(neighbor->get_id());
            queue.push(neighbor);
        }
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();
    const float cell = 90.0f;

    for (const auto& [roomId, pos] : coords)
    {
        if (!discovered.count(roomId))
            continue;
        ImVec2 screenPos(origin.x + 100 + pos.x * cell, origin.y + 100 + pos.y * cell);
        bool isCurrent = (roomId == currentRoom->get_id());
        ImU32 fill = isCurrent ? IM_COL32(120, 200, 120, 255) : IM_COL32(200, 200, 200, 255);

        drawList->AddRectFilled(screenPos, ImVec2(screenPos.x + cell - 10, screenPos.y + cell - 10), fill);
        drawList->AddRect(screenPos, ImVec2(screenPos.x + cell - 10, screenPos.y + cell - 10), IM_COL32(0, 0, 0, 255));
        drawList->AddText(ImVec2(screenPos.x + 5, screenPos.y + 5), IM_COL32(0, 0, 0, 255), roomId.c_str());
    }

    ImGui::Dummy(ImVec2(400, 400));
    ImGui::End();
}

static void drawRoomWindow(Player* player)
{
    ImGui::SetNextWindowPos(ImVec2(440, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Current Room");

    if (!player || !player->get_current_room())
    {
        ImGui::TextDisabled("Not available in Remote mode yet.");
        ImGui::End();
        return;
    }

    Room* room = player->get_current_room();
    ImGui::TextColored(ImVec4(1, 1, 0.6f, 1), "%s", room->get_name().c_str());
    ImGui::TextWrapped("%s", room->get_description().c_str());

    ImGui::Separator();
    ImGui::Text("Items in this room:");
    if (room->get_items().empty())
        ImGui::TextDisabled("(none)");
    else
    {
        for (Item* item : room->get_items())
            ImGui::BulletText("%s", item->get_name().c_str());
    }

    ImGui::Separator();
    ImGui::Text("NPC in this room:");
    NPC* npc = room->get_NPC();
    if (npc)
        ImGui::BulletText("%s", npc->get_name().c_str());
    else
        ImGui::TextDisabled("(none)");

    ImGui::End();
}

static void drawStatsWindow(Player* player)
{
    ImGui::SetNextWindowPos(ImVec2(860, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(320, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Stats");

    if (!player)
    {
        ImGui::TextDisabled("Not available in Remote mode yet.");
        ImGui::End();
        return;
    }

    const t_stats& stats = player->get_stats();
    float hpFraction = stats.hp > 0 ? (float)stats.current_hp / (float)stats.hp : 0.0f;
    std::string hpLabel = std::to_string(stats.current_hp) + " / " + std::to_string(stats.hp);
    ImGui::Text("HP:");
    ImGui::ProgressBar(hpFraction, ImVec2(-1, 0), hpLabel.c_str());

    ImGui::Text("Level: %u", stats.level);
    ImGui::Text("Strength: %u / %u", stats.current_strength, stats.strength);
    ImGui::Text("Defense: %u / %u", stats.current_defense, stats.defense);
    ImGui::Text("Speed: %u / %u", stats.current_speed, stats.speed);
    ImGui::Text("Gold: %u", player->get_gold());
    ImGui::Text("Quests: %zu", player->get_quest_list().size());

    ImGui::End();
}

static void drawMenuWindow(GuiClient& client, std::vector<std::string>& log)
{
    static int selectedIndex = -1;

    ImGui::SetNextWindowPos(ImVec2(20, 440), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1160, 240), ImGuiCond_FirstUseEver);
    ImGui::Begin("Menu");

    Player* player = client.getPlayer();

    if (ImGui::BeginTabBar("MenuTabs"))
    {
        if (ImGui::BeginTabItem("Actions"))
        {
            ImGui::Text("Move:");
            for (Direction dir : {Direction::NORTH, Direction::SOUTH, Direction::EAST, Direction::WEST})
            {
                ImGui::PushID((int)dir);
                if (ImGui::Button(direction_to_string(dir).c_str()))
                {
                    std::string cmd = "MOVE " + direction_to_string(dir);
                    client.sendCommand(cmd);
                    logMsg(log, "> " + cmd);
                }
                ImGui::PopID();
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
            ImGui::BeginChild("LogScroll", ImVec2(0, 130), true);
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
            if (!player)
            {
                ImGui::TextDisabled("Not available in Remote mode yet.");
            }
            else
            {
                const auto& items = player->get_inventory().get_items();
                int i = 0;
                for (Item* item : items)
                {
                    bool isSelected = (selectedIndex == i);
                    if (ImGui::Selectable(item->get_name().c_str(), isSelected))
                        selectedIndex = i;
                    ++i;
                }
                if (items.empty())
                    ImGui::TextDisabled("(empty)");
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Stats"))
        {
            if (!player)
            {
                ImGui::TextDisabled("Not available in Remote mode yet.");
            }
            else
            {
                const t_stats& stats = player->get_stats();
                ImGui::Text("HP: %u / %u", stats.current_hp, stats.hp);
                ImGui::Text("Strength: %u / %u", stats.current_strength, stats.strength);
                ImGui::Text("Defense: %u / %u", stats.current_defense, stats.defense);
                ImGui::Text("Speed: %u / %u", stats.current_speed, stats.speed);
                ImGui::Text("Gold: %u", player->get_gold());
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Settings"))
        {
            // TODO
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

    GuiClient                  client;
    AppState                   state = AppState::LOGIN;
    std::string                loginError;
    std::vector<std::string>   log;
    std::set<std::string>      discoveredRooms;
    bool                       running = true;

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

        if (state == AppState::LOGIN)
        {
            drawLoginScreen(client, state, loginError);
        }
        else
        {
            for (const std::string& msg : client.pollMessages())
                logMsg(log, msg);

            Player* player = client.getPlayer();
            drawMapWindow(player, discoveredRooms);
            drawRoomWindow(player);
            drawStatsWindow(player);
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