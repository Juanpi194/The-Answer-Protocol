#pragma once
#include <string>
#include <list>
#include <map>
#include <optional>

enum class Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct t_stats
{
    unsigned int hp;
    unsigned int max_health;
    unsigned int strength;
};

struct ClientItem
{
    std::string id;
    std::string name;
    std::string description;
};

struct ClientNpc
{
    std::string id;
    std::string name;
    std::string description;
    int         max_health;
    bool        isEnemy = false;
};

struct ClientRoom
{
    std::string                        id;
    std::string                        name;
    std::string                        description;
    std::list<ClientItem>            items;
    std::optional<ClientNpc>           npc;
    std::map<Direction, std::string>   exits;
};

class GameClient
{
private:
    std::map<std::string, ClientRoom>   world_;
    std::string                         currentRoomId_;
    std::list<std::string>            discoveredRoomIds_;
    t_stats                             stats_;
    unsigned int                        gold_;
    int                                 max_health;
    std::list<ClientItem>             inventory_;

    // Mundo de prueba local extremadamente simple (tres salass, sus items, su npc opcional, sus salidas).
    void buildWorld_();

public:
    GameClient();

    const ClientRoom&                getCurrentRoom() const;
    const ClientRoom&                getRoom(const std::string& roomId) const;
    const t_stats&                   getStats() const;
    unsigned int                     getGold() const;
    const std::list<ClientItem>&     getInventory() const;
    const std::list<std::string>&    getDiscoveredRoomIds() const;

    bool        move(Direction direction);
    bool        takeItem(const std::string& itemId);
    bool        dropItem(const std::string& itemId);
    std::string talkToNpc();
    void        attackNpc();

    //TODO: QUESTS
};