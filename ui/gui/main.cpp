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
#include <cctype>
#include <chrono>
#include <thread>
#include <atomic>

#include "ui/CLI.hpp"
#include "libs/json.hpp"

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

static std::string toUpperCopy(const std::string& text)
{
    std::string result = text;
    for (char& c : result)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return result;
}

// WorldCache: Copy in local of the information from world just to display in imgui
struct RoomInfo
{
    std::string                        id;
    std::string                        name;
    std::string                        description;
    std::map<std::string, std::string> exits;
    std::vector<std::string>           items;
    std::string                        npc;
    std::vector<std::string>           players;
};

// MODIFIED: status information in local.
struct StatsInfo
{
    bool         hasData = false;
    unsigned int current_hp = 0;
    unsigned int hp = 0;
    unsigned int current_strength = 0;
    unsigned int strength = 0;
    unsigned int current_defense = 0;
    unsigned int defense = 0;
    unsigned int current_speed = 0;
    unsigned int speed = 0;
    std::string  status;
};

// MODIFIED: quest recieved
struct QuestInfo
{
    std::string  quest_id;
    std::string  description;
    unsigned int gold_reward = 0;
    std::string  item_reward;
    std::string  status;
};

// MODIFIED: combat state
struct CombatInfo
{
    bool      hasData = false;
    StatsInfo blue; // tú
    StatsInfo red;  // el enemigo
};

class WorldCache
{
    public:
        // MODIFIED: Added receptions from server, as status, inventory, etc.
        void ingest(const std::string& line)
        {
            size_t jsonStart = line.find_first_of("{[");
            if (jsonStart == std::string::npos)
                return;

            nlohmann::json data;
            try
            {
                data = nlohmann::json::parse(line.substr(jsonStart));
            }
            catch (const std::exception&)
            {
                return;
            }

            if (data.is_object() && data.contains("room"))
                ingestRoom(data);
            else if (data.is_object() && data.contains("blue") && data.contains("red"))
                ingestCombat(data);
            else if (data.is_object() && data.contains("current_hp"))
                ingestStats(data);
            else if (data.is_array())
                ingestArray(data);
        }

        const RoomInfo* getRoom(const std::string& id) const
        {
            auto it = rooms_.find(id);
            return (it != rooms_.end()) ? &it->second : nullptr;
        }

        const RoomInfo* getCurrentRoom() const { return getRoom(currentRoomId_); }
        const std::map<std::string, RoomInfo>& getRooms() const { return rooms_; }
        const StatsInfo& getStats() const { return stats_; }
        const CombatInfo& getCombat() const { return combat_; }
        const std::vector<std::string>& getInventory() const { return inventory_; }
        const std::vector<QuestInfo>& getQuests() const { return quests_; }

    private:
        std::map<std::string, RoomInfo> rooms_;
        std::string                     currentRoomId_;
        StatsInfo                       stats_;
        CombatInfo                      combat_;
        std::vector<std::string>        inventory_;
        std::vector<QuestInfo>          quests_;

        void ingestRoom(const nlohmann::json& data)
        {
            if (!data["room"].is_object())
                return;

            const nlohmann::json& roomJson = data["room"];
            RoomInfo info;
            info.id = roomJson.value("id", "");
            info.name = roomJson.value("name", "");
            info.description = roomJson.value("description", "");

            if (roomJson.contains("exits") && roomJson["exits"].is_object())
                for (auto it = roomJson["exits"].begin(); it != roomJson["exits"].end(); ++it)
                    if (it.value().is_string())
                        info.exits[it.key()] = it.value().get<std::string>();

            // Server can send a None call to this. Needs to check the type before use
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

        // MODIFIED: Stats parser for display
        static StatsInfo parseStatsObject(const nlohmann::json& data)
        {
            StatsInfo s;
            s.hasData = true;
            s.current_hp = data.value("current_hp", 0u);
            s.hp = data.value("hp", 0u);
            s.current_strength = data.value("current_strength", 0u);
            s.strength = data.value("strength", 0u);
            s.current_defense = data.value("current_defense", 0u);
            s.defense = data.value("defense", 0u);
            s.current_speed = data.value("current_speed", 0u);
            s.speed = data.value("speed", 0u);
            s.status = data.value("status", "");
            return s;
        }

        void ingestStats(const nlohmann::json& data)
        {
            stats_ = parseStatsObject(data);
        }

        // MODIFIED: Combat state
        void ingestCombat(const nlohmann::json& data)
        {
            if (!data["blue"].is_object() || !data["red"].is_object())
                return;

            CombatInfo c;
            c.hasData = true;
            c.blue = parseStatsObject(data["blue"]);
            c.red = parseStatsObject(data["red"]);
            combat_ = c;
        }

        void ingestArray(const nlohmann::json& data)
        {
            if (data.empty())
                return;

            // INVENTORY
            if (data[0].is_string())
            {
                std::vector<std::string> items;
                for (const auto& item : data)
                    if (item.is_string())
                        items.push_back(item.get<std::string>());
                inventory_ = items;
                return;
            }

            // QUESTS
            if (data[0].is_object() && data[0].contains("quest_id"))
            {
                std::vector<QuestInfo> quests;
                for (const auto& q : data)
                {
                    QuestInfo info;
                    info.quest_id = q.value("quest_id", "");
                    info.description = q.value("description", "");
                    info.gold_reward = q.value("gold_reward", 0u);
                    if (q.contains("item_reward") && q["item_reward"].is_string())
                        info.item_reward = q["item_reward"].get<std::string>();
                    info.status = q.value("status", "");
                    quests.push_back(info);
                }
                quests_ = quests;
            }
        }
};

// MODIFIED: imgui colors
static void applyThemeHue(float hue)
{
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    float r, g, b;
    ImGui::ColorConvertHSVtoRGB(hue, 0.55f, 0.55f, r, g, b);
    ImVec4 base(r, g, b, 1.0f);
    ImGui::ColorConvertHSVtoRGB(hue, 0.65f, 0.70f, r, g, b);
    ImVec4 hovered(r, g, b, 1.0f);
    ImGui::ColorConvertHSVtoRGB(hue, 0.75f, 0.85f, r, g, b);
    ImVec4 active(r, g, b, 1.0f);

    style.Colors[ImGuiCol_Header]         = base;
    style.Colors[ImGuiCol_HeaderHovered]  = hovered;
    style.Colors[ImGuiCol_HeaderActive]   = active;
    style.Colors[ImGuiCol_Button]         = base;
    style.Colors[ImGuiCol_ButtonHovered]  = hovered;
    style.Colors[ImGuiCol_ButtonActive]   = active;
    style.Colors[ImGuiCol_FrameBgActive]  = active;
    style.Colors[ImGuiCol_FrameBgHovered] = hovered;
    style.Colors[ImGuiCol_TitleBgActive]  = base;
    style.Colors[ImGuiCol_CheckMark]      = active;
    style.Colors[ImGuiCol_SliderGrab]     = base;
    style.Colors[ImGuiCol_SliderGrabActive] = active;
    style.Colors[ImGuiCol_Tab]            = base;
    style.Colors[ImGuiCol_TabHovered]     = hovered;
    style.Colors[ImGuiCol_TabActive]      = active;
    style.Colors[ImGuiCol_ResizeGrip]     = base;
    style.Colors[ImGuiCol_ResizeGripHovered] = hovered;
    style.Colors[ImGuiCol_ResizeGripActive]  = active;
}

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
    ImGui::SetNextWindowSize(ImVec2(420, 150), ImGuiCond_FirstUseEver);
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

static ImVec2 directionOffset(const std::string& dir)
{
    if (dir == "NORTH") return ImVec2(0, -1);
    if (dir == "SOUTH") return ImVec2(0, 1);
    if (dir == "EAST")  return ImVec2(1, 0);
    if (dir == "WEST")  return ImVec2(-1, 0);
    return ImVec2(0, 0);
}

// MODIFIED: Adaptative position and size of imgui windows
static void drawMapWindow(const WorldCache& world, const ImVec2& pos, const ImVec2& size)
{
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin("Discovered Map", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

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

    for (const auto& [roomId, room] : world.getRooms())
    {
        if (!coords.count(roomId))
            continue;
        ImVec2 grid = coords[roomId];
        ImVec2 cellPos(origin.x + size.x * 0.5f + grid.x * cell, origin.y + size.y * 0.4f + grid.y * cell);

        bool isCurrent = (roomId == current->id);
        ImU32 fill = isCurrent ? IM_COL32(120, 200, 120, 255) : IM_COL32(200, 200, 200, 255);

        drawList->AddRectFilled(cellPos, ImVec2(cellPos.x + cell - 10, cellPos.y + cell - 10), fill);
        drawList->AddRect(cellPos, ImVec2(cellPos.x + cell - 10, cellPos.y + cell - 10), IM_COL32(0, 0, 0, 255));
        drawList->AddText(ImVec2(cellPos.x + 5, cellPos.y + 5), IM_COL32(0, 0, 0, 255), room.name.c_str());
    }

    ImGui::End();
}


// MODIFIED: Recosntruct the real name of the item by his ID to use the buttons wothout
// need of write the object name
static std::string idToItemName(const std::string& itemId)
{
    std::string slug = itemId;

    const std::string itemPrefix = "item.";
    if (slug.rfind(itemPrefix, 0) == 0)
        slug = slug.substr(itemPrefix.size());

    size_t lastDot = slug.find_last_of('.');
    if (lastDot != std::string::npos)
    {
        bool allDigits = true;
        for (size_t i = lastDot + 1; i < slug.size(); ++i)
            if (!std::isdigit(static_cast<unsigned char>(slug[i])))
                allDigits = false;
        if (allDigits)
            slug = slug.substr(0, lastDot);
    }

    std::string name;
    bool capitalizeNext = true;
    for (char c : slug)
    {
        if (c == '_')
        {
            name += ' ';
            capitalizeNext = true;
        }
        else if (capitalizeNext)
        {
            name += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            capitalizeNext = false;
        }
        else
        {
            name += c;
        }
    }
    return name;
}

static void drawRoomWindow(CLI& client, WorldCache& world, std::vector<std::string>& log,
                            const ImVec2& pos, const ImVec2& size)
{
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin("Current Room", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

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
    {
        ImGui::TextDisabled("(none)");
    }
    else
    {
        for (const std::string& itemId : room->items)
        {
            std::string itemName = idToItemName(itemId);
            ImGui::PushID(itemId.c_str());
            ImGui::BulletText("%s", itemName.c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton("Take"))
            {
                std::string cmd = "TAKE " + itemName;
                client.sendCommand(cmd);
                logMsg(log, "> " + cmd);
            }
            ImGui::PopID();
        }
    }

    // MODIFIED: NPC name cant be reconstructed from his ID, so the call needs to be
    // by writing his name
    ImGui::Separator();
    ImGui::Text("NPC in this room:");
    if (room->npc.empty())
    {
        ImGui::TextDisabled("(none)");
    }
    else
    {
        ImGui::BulletText("%s", room->npc.c_str());

        if (ImGui::SmallButton("Fight"))
        {
            client.sendCommand("FIGHT");
            logMsg(log, "> FIGHT");
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Attack"))
        {
            client.sendCommand("ATTACK");
            logMsg(log, "> ATTACK");
        }

        static char roomTalkBuf[128] = "";
        ImGui::PushItemWidth(140);
        ImGui::InputTextWithHint("##RoomTalkBuf", "NPC name", roomTalkBuf, IM_ARRAYSIZE(roomTalkBuf));
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::SmallButton("Talk##Room") && roomTalkBuf[0] != '\0')
        {
            std::string cmd = std::string("TALK ") + roomTalkBuf;
            client.sendCommand(cmd);
            logMsg(log, "> " + cmd);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Quest##Room") && roomTalkBuf[0] != '\0')
        {
            std::string cmd = std::string("QUEST ") + roomTalkBuf;
            client.sendCommand(cmd);
            logMsg(log, "> " + cmd);
        }
    }

    ImGui::Separator();
    ImGui::Text("Players here:");
    if (room->players.empty())
        ImGui::TextDisabled("(none)");
    else
        for (const std::string& p : room->players)
            ImGui::BulletText("%s", p.c_str());

    ImGui::End();
}


// Main Menu: Actions / Inventory / Stats / Quests / Settings.
static void drawMenuWindow(CLI& client, std::vector<std::string>& log, WorldCache& world,
                            const ImVec2& pos, const ImVec2& size, bool& quitRequested)
{
    static int selectedIndex = -1;

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

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
                }
                ImGui::SameLine();
            }
            if (ImGui::Button("Look"))
            {
                client.sendCommand("LOOK");
                logMsg(log, "> LOOK");
            }
            ImGui::SameLine();
            if (ImGui::Button("Open"))
            {
                client.sendCommand("OPEN");
                logMsg(log, "> OPEN");
            }
            ImGui::NewLine();

            ImGui::Separator();
            static char talkBuf[128] = "";
            ImGui::PushItemWidth(200);
            ImGui::InputTextWithHint("##TalkBuf", "NPC name", talkBuf, IM_ARRAYSIZE(talkBuf));
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Talk") && talkBuf[0] != '\0')
            {
                std::string cmd = std::string("TALK ") + talkBuf;
                client.sendCommand(cmd);
                logMsg(log, "> " + cmd);
            }

            static char takeBuf[128] = "";
            ImGui::PushItemWidth(200);
            ImGui::InputTextWithHint("##TakeBuf", "Item name", takeBuf, IM_ARRAYSIZE(takeBuf));
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Take") && takeBuf[0] != '\0')
            {
                std::string cmd = std::string("TAKE ") + takeBuf;
                client.sendCommand(cmd);
                logMsg(log, "> " + cmd);
            }

            ImGui::Separator();
            ImGui::Text("Log:");
            ImGui::BeginChild("LogScroll", ImVec2(0, ImGui::GetContentRegionAvail().y - 40), true);
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

                // MODIFIED: escribir QUIT en el chat se comporta igual que
                // el botón Exit de Settings -- ver más abajo.
                if (toUpperCopy(commandBuf) == "QUIT")
                    quitRequested = true;

                commandBuf[0] = '\0';
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Inventory"))
        {
            if (ImGui::Button("Refresh"))
            {
                client.sendCommand("INVENTORY");
                logMsg(log, "> INVENTORY");
            }

            ImGui::Separator();
            const auto& inventory = world.getInventory();
            if (inventory.empty())
            {
                ImGui::TextDisabled("(empty, or not refreshed yet)");
            }
            else
            {
                int i = 0;
                for (const std::string& itemId : inventory)
                {
                    bool isSelected = (selectedIndex == i);
                    if (ImGui::Selectable(itemId.c_str(), isSelected))
                        selectedIndex = i;
                    ++i;
                }
            }

            ImGui::Separator();
            static char dropBuf[128] = "";
            ImGui::PushItemWidth(200);
            ImGui::InputTextWithHint("##DropBuf", "Item name", dropBuf, IM_ARRAYSIZE(dropBuf));
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Drop") && dropBuf[0] != '\0')
            {
                std::string cmd = std::string("DROP ") + dropBuf;
                client.sendCommand(cmd);
                logMsg(log, "> " + cmd);
                dropBuf[0] = '\0';
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Stats"))
        {
            if (ImGui::Button("Refresh"))
            {
                client.sendCommand("STATUS");
                logMsg(log, "> STATUS");
            }

            ImGui::Separator();
            const StatsInfo& s = world.getStats();
            if (!s.hasData)
            {
                ImGui::TextDisabled("Not refreshed yet -- press Refresh.");
            }
            else
            {
                float hpFraction = s.hp > 0 ? (float)s.current_hp / (float)s.hp : 0.0f;
                std::string hpLabel = std::to_string(s.current_hp) + " / " + std::to_string(s.hp);
                ImGui::Text("HP:");
                ImGui::ProgressBar(hpFraction, ImVec2(-1, 0), hpLabel.c_str());
                ImGui::Text("Strength: %u / %u", s.current_strength, s.strength);
                ImGui::Text("Defense:  %u / %u", s.current_defense, s.defense);
                ImGui::Text("Speed:    %u / %u", s.current_speed, s.speed);
                ImGui::Text("Status:   %s", s.status.c_str());
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Quests"))
        {
            if (ImGui::Button("Refresh"))
            {
                client.sendCommand("QUESTS");
                logMsg(log, "> QUESTS");
            }

            ImGui::Separator();
            static char questNpcBuf[128] = "";
            ImGui::PushItemWidth(200);
            ImGui::InputTextWithHint("##QuestNpcBuf", "NPC name", questNpcBuf, IM_ARRAYSIZE(questNpcBuf));
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Request quest") && questNpcBuf[0] != '\0')
            {
                std::string cmd = std::string("QUEST ") + questNpcBuf;
                client.sendCommand(cmd);
                logMsg(log, "> " + cmd);
            }

            ImGui::Separator();
            const auto& quests = world.getQuests();
            if (quests.empty())
            {
                ImGui::TextDisabled("(none, or not refreshed yet)");
            }
            else
            {
                for (const QuestInfo& q : quests)
                {
                    ImGui::TextColored(ImVec4(1, 1, 0.6f, 1), "%s", q.quest_id.c_str());
                    ImGui::TextWrapped("%s", q.description.c_str());
                    ImGui::Text("Reward: %u gold", q.gold_reward);
                    if (!q.item_reward.empty() && q.item_reward != "None")
                        ImGui::SameLine(), ImGui::Text("+ %s", q.item_reward.c_str());
                    ImGui::TextDisabled("Status: %s", q.status.c_str());
                    ImGui::Separator();
                }
            }

            ImGui::EndTabItem();
        }

        // MODIFIED: combat window
        if (ImGui::BeginTabItem("Combat"))
        {
            const CombatInfo& c = world.getCombat();
            if (!c.hasData)
            {
                ImGui::TextDisabled("Not in a fight yet -- use \"Fight\"/\"Attack\" "
                                     "next to an NPC in Current Room to start one.");
            }
            else
            {
                ImGui::Text("You:");
                float yourFraction = c.blue.hp > 0 ? (float)c.blue.current_hp / (float)c.blue.hp : 0.0f;
                std::string yourLabel = std::to_string(c.blue.current_hp) + " / " + std::to_string(c.blue.hp);
                ImGui::ProgressBar(yourFraction, ImVec2(-1, 0), yourLabel.c_str());

                ImGui::Spacing();
                ImGui::Text("Enemy:");
                float enemyFraction = c.red.hp > 0 ? (float)c.red.current_hp / (float)c.red.hp : 0.0f;
                std::string enemyLabel = std::to_string(c.red.current_hp) + " / " + std::to_string(c.red.hp);
                ImGui::ProgressBar(enemyFraction, ImVec2(-1, 0), enemyLabel.c_str());
            }

            ImGui::Separator();
            if (ImGui::Button("Attack"))
            {
                client.sendCommand("ATTACK");
                logMsg(log, "> ATTACK");
            }
            ImGui::SameLine();
            if (ImGui::Button("Defend"))
            {
                client.sendCommand("DEFEND");
                logMsg(log, "> DEFEND");
            }
            ImGui::SameLine();
            if (ImGui::Button("Flee"))
            {
                client.sendCommand("FLEE");
                logMsg(log, "> FLEE");
            }

            static char consumeBuf[128] = "";
            ImGui::PushItemWidth(200);
            ImGui::InputTextWithHint("##ConsumeBuf", "Item name", consumeBuf, IM_ARRAYSIZE(consumeBuf));
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Consume") && consumeBuf[0] != '\0')
            {
                std::string cmd = std::string("CONSUME ") + consumeBuf;
                client.sendCommand(cmd);
                logMsg(log, "> " + cmd);
                consumeBuf[0] = '\0';
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Settings"))
        {
            ImGui::Text("Color theme:");
            static float themeHue = 0.58f;
            if (ImGui::SliderFloat("Hue", &themeHue, 0.0f, 1.0f, ""))
                applyThemeHue(themeHue);

            ImGui::Separator();
            if (ImGui::Button("Exit"))
            {
                client.sendCommand("QUIT");
                logMsg(log, "> QUIT");
                quitRequested = true;
            }

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
        1280, 800, windowFlags);
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
    applyThemeHue(0.58f); // MODIFIED: main theme
    ImGui::GetIO().FontGlobalScale = 1.3f;
    ImGui::GetStyle().ScaleAllSizes(1.3f);

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion);

    CLI                         client;
    AppState                    state = AppState::CONNECT;
    std::string                 connectError;
    std::vector<std::string>    log;
    WorldCache                  world;
    bool                        running = true;

    // MODIFIED: Close window now wait 300ms to the server to respond
    bool                                  quitRequested = false;
    std::chrono::steady_clock::time_point quitRequestedAt{};

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            bool closeRequested = (event.type == SDL_QUIT)
                || (event.type == SDL_WINDOWEVENT
                    && event.window.event == SDL_WINDOWEVENT_CLOSE
                    && event.window.windowID == SDL_GetWindowID(window));

            if (closeRequested)
            {
                if (state == AppState::GAME && client.isConnected() && !quitRequested)
                {
                    client.sendCommand("QUIT");
                    quitRequested = true;
                    quitRequestedAt = std::chrono::steady_clock::now();
                }
                else
                {
                    running = false;
                }
            }
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
                world.ingest(msg);
            }

            ImGuiIO& io = ImGui::GetIO();
            float leftW  = io.DisplaySize.x * 0.38f;
            float rightW = io.DisplaySize.x - leftW;
            float topH   = io.DisplaySize.y * 0.55f;
            float menuH  = io.DisplaySize.y - topH;

            bool wasQuitRequested = quitRequested;
            drawMapWindow(world, ImVec2(0, 0), ImVec2(leftW, topH));
            drawRoomWindow(client, world, log, ImVec2(leftW, 0), ImVec2(rightW, topH));
            drawMenuWindow(client, log, world, ImVec2(0, topH), ImVec2(io.DisplaySize.x, menuH), quitRequested);

            if (quitRequested && !wasQuitRequested)
                quitRequestedAt = std::chrono::steady_clock::now();
            if (quitRequested
                && std::chrono::steady_clock::now() - quitRequestedAt > std::chrono::milliseconds(300))
                running = false;
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

    // MODIFIED: Explicit close
    client.disconnect();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}