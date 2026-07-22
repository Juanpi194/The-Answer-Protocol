#define SDL_MAIN_HANDLED // avoid depending on SDL2main / WinMain on Windows
#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <cstdio>
#include "GameClient.hpp"


static void logMsg(std::vector<std::string>& log, const std::string& msg)
{
    log.push_back(msg);
    if (log.size() > 200)
        log.erase(log.begin());
}

// Para no tener que estar usando Direction:: cada vez que quier aponer una direccion
static const char* directionLabel(Direction dir)
{
    switch (dir)
    {
        case Direction::NORTH: return "North";
        case Direction::SOUTH: return "South";
        case Direction::EAST:  return "East";
        case Direction::WEST:  return "West";
    }
    return "?";
}

static ImVec2 directionOffset(Direction dir)
{
    switch (dir)
    {
        case Direction::NORTH: return ImVec2(0, -1);
        case Direction::SOUTH: return ImVec2(0, 1);
        case Direction::EAST:  return ImVec2(1, 0);
        case Direction::WEST:  return ImVec2(-1, 0);
    }
    return ImVec2(0, 0);
}

// Itero una BFS sobre la room en la que estoy para chequear a que direcciones tengo acceso.
static void drawMapWindow(const GameClient& client)
{
    ImGui::Begin("Discovered Map");

    std::map<std::string, ImVec2> coords;
    std::set<std::string>         visited;
    std::queue<std::string>       queue;

    std::string startId = client.getCurrentRoom().id;
    coords[startId] = ImVec2(0, 0);
    queue.push(startId);
    visited.insert(startId);

    const auto& discovered = client.getDiscoveredRoomIds();
    auto isDiscovered = [&](const std::string& id) {
        return std::find(discovered.begin(), discovered.end(), id) != discovered.end();
    };

    while (!queue.empty())
    {
        std::string roomId = queue.front();
        queue.pop();
        const ClientRoom& room = client.getRoom(roomId);
        ImVec2 base = coords[roomId];

        for (const auto& [dir, targetId] : room.exits)
        {
            if (!isDiscovered(targetId) || visited.count(targetId))
                continue;

            ImVec2 off = directionOffset(dir);
            coords[targetId] = ImVec2(base.x + off.x, base.y + off.y);
            visited.insert(targetId);
            queue.push(targetId);
        }
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();
    const float cell = 90.0f;

    for (const std::string& roomId : discovered)
    {
        const ClientRoom& room = client.getRoom(roomId);
        ImVec2 grid = coords.count(roomId) ? coords[roomId] : ImVec2(0, 0);
        ImVec2 pos(origin.x + 100 + grid.x * cell, origin.y + 100 + grid.y * cell);

        bool isCurrent = (roomId == client.getCurrentRoom().id);
        ImU32 fill = isCurrent ? IM_COL32(120, 200, 120, 255) : IM_COL32(200, 200, 200, 255);

        drawList->AddRectFilled(pos, ImVec2(pos.x + cell - 10, pos.y + cell - 10), fill);
        drawList->AddRect(pos, ImVec2(pos.x + cell - 10, pos.y + cell - 10), IM_COL32(0, 0, 0, 255));
        drawList->AddText(ImVec2(pos.x + 5, pos.y + 5), IM_COL32(0, 0, 0, 255), room.name.c_str());
    }

    ImGui::Dummy(ImVec2(400, 400));
    ImGui::End();
}


static void drawRoomWindow(GameClient& client, std::vector<std::string>& log)
{
    ImGui::Begin("Current Room");

    const ClientRoom& room = client.getCurrentRoom();
    ImGui::TextColored(ImVec4(1, 1, 0.6f, 1), "%s", room.name.c_str());
    ImGui::TextWrapped("%s", room.description.c_str());

    ImGui::Separator();
    ImGui::Text("Items in this room:");
    if (room.items.empty())
    {
        ImGui::TextDisabled("(none)");
    }
    else
    {
        std::vector<std::string> itemIds;
        for (const ClientItem& item : room.items)
            itemIds.push_back(item.id);

        for (const std::string& itemId : itemIds)
        {
            const ClientRoom& freshRoom = client.getCurrentRoom();
            auto found = std::find_if(freshRoom.items.begin(), freshRoom.items.end(),
                                       [&](const ClientItem& it) { return it.id == itemId; });
            if (found == freshRoom.items.end())
                continue;

            ImGui::BulletText("%s", found->name.c_str());
            ImGui::SameLine();
            ImGui::PushID(itemId.c_str());
            if (ImGui::SmallButton("Take"))
            {
                std::string itemName = found->name;
                if (client.takeItem(itemId))
                    logMsg(log, "Took: " + itemName);
            }
            ImGui::PopID();
        }
    }

    ImGui::Separator();
    ImGui::Text("NPC in this room:");
    if (room.npc)
        ImGui::BulletText("%s%s", room.npc->name.c_str(), room.npc->isEnemy ? " (hostile)" : "");
    else
        ImGui::TextDisabled("(none)");

    ImGui::End();
}


static void drawStatsWindow(const GameClient& client)
{
    ImGui::Begin("Stats");

    const t_stats& stats = client.getStats();
    float hpFraction = (float)stats.hp / (float)stats.max_health;
    std::string hpLabel = std::to_string(stats.hp) + " / " + std::to_string(stats.max_health);
    ImGui::ProgressBar(hpFraction, ImVec2(-1, 0), hpLabel.c_str());
    ImGui::Text("Strength: %u", stats.strength);
    ImGui::Text("Gold: %u", client.getGold());

    ImGui::End();
}


// Bottom menu with the 4 tabs: Actions / Inventory / Stats / Settings
static void drawMenuWindow(GameClient& client, std::vector<std::string>& log)
{

    static int selectedIndex = -1;

    ImGui::Begin("Menu");

    if (ImGui::BeginTabBar("MenuTabs"))
    {
        if (ImGui::BeginTabItem("Actions"))
        {
            ImGui::Text("Move:");
            for (Direction dir : {Direction::NORTH, Direction::SOUTH, Direction::EAST, Direction::WEST})
            {
                ImGui::PushID((int)dir);
                if (ImGui::Button(directionLabel(dir)))
                {
                    if (client.move(dir))
                        logMsg(log, std::string("You move ") + directionLabel(dir));
                    else
                        logMsg(log, std::string("No exit to the ") + directionLabel(dir));
                }
                ImGui::PopID();
                ImGui::SameLine();
            }
            ImGui::NewLine();

            ImGui::Separator();
            if (ImGui::Button("Talk (non-hostile NPC)"))
            {
                std::string line = client.talkToNpc();
                if (!line.empty())
                    logMsg(log, "NPC: " + line);
                else
                    logMsg(log, "There is no one to talk to here.");
            }
            if (ImGui::Button("Attack (hostile NPC)"))
            {
                const ClientRoom& room = client.getCurrentRoom();
                if (room.npc && room.npc->isEnemy)
                {
                    client.attackNpc();
                    logMsg(log, "You attack " + room.npc->name);
                }
                else
                {
                    logMsg(log, "There is no enemy here.");
                }
            }

            ImGui::Separator();
            ImGui::Text("Log:");
            ImGui::BeginChild("LogScroll", ImVec2(0, 150), true);
            for (const std::string& line : log)
                ImGui::TextUnformatted(line.c_str());
            ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Inventory"))
        {
                        const auto& inventory = client.getInventory();
            int i = 0;
            for (const auto& item : inventory)
            {
                bool isSelected = (selectedIndex == i);
                if (ImGui::Selectable(item.name.c_str(), isSelected))
                    selectedIndex = i;
                ++i;
            }
            ImGui::Separator();
            if (ImGui::Button("Drop selected item"))
            {
                if (selectedIndex >= 0 && selectedIndex < (int)inventory.size())
                {
                    auto it = inventory.begin();
                    std::advance(it, selectedIndex);
                    std::string name = it->name;
                    std::string id = it->id;
                    if (client.dropItem(id))
                        logMsg(log, "Dropped: " + name);
                    selectedIndex = -1;
                }
                else
                {
                    logMsg(log, "Select an item from the inventory first.");
                }
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Stats"))
        {
            const t_stats& stats = client.getStats();
            ImGui::Text("HP: %u", stats.hp);
            ImGui::Text("Strength: %u", stats.strength);
            ImGui::Text("Gold: %u", client.getGold());
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Settings"))
        {
            // TODO: Poner setings validas, puertos de conexion, informacion del juego (gasto de memoria, ms de respuesta...)
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
        "TAP - GUI client (proxy)",
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

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion);

    GameClient client;
    std::vector<std::string> log;
    bool running = true;

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

        drawMapWindow(client);
        drawRoomWindow(client, log);
        drawStatsWindow(client);
        drawMenuWindow(client, log);

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