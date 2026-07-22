#include "GameClient.hpp"
#include <algorithm>

GameClient::GameClient()
{
    // Builder para el cliente de pruebas. Esto se seteara con el cliente real cuando toque
    gold_ = 15;
    buildWorld_();
    currentRoomId_ = "start";
    discoveredRoomIds_.push_back(currentRoomId_);

    stats_.hp = 10;
    stats_.max_health = 100;
    stats_.strength = 10;

    inventory_.push_back(ClientItem{"item.torch", "Torch", "Modelo de IA que me tiene loco"});
    inventory_.push_back(ClientItem{"item.map", "Old Map", "Para ver si me encuentro a mi mismo"});
}

void GameClient::buildWorld_()
{

    //proxy world para pruebas
    ClientRoom start;
    start.id = "start";
    start.name = "Village Square";
    start.description = "Lo llaman villaverde, pero es mas marron que otra cosa";
    start.exits[Direction::NORTH] = "tavern";
    start.exits[Direction::EAST] = "shop";
    start.npc = ClientNpc{"npc.guard", "Village Guard",
                           "Hum?", false};
    world_["start"] = start;

    ClientRoom tavern;
    tavern.id = "tavern";
    tavern.name = "El pony pisador";
    tavern.description = "Huele a cerveza y morcilla";
    tavern.exits[Direction::SOUTH] = "start";
    tavern.items.push_back(ClientItem{"item.ale", "Ambrosia", "Tambien llamado morcilla"});
    tavern.items.push_back(ClientItem{"item.potion", "Health Potion", "Tambien llamado cerveza"});
    world_["tavern"] = tavern;

    ClientRoom shop;
    shop.id = "shop";
    shop.name = "Parquesur";
    shop.description = "LLeno de canis y gitanos";
    shop.exits[Direction::WEST] = "start";
    shop.items.push_back(ClientItem{"item.dagger", "Rusty Dagger", "Aun tiene un poco de sangre"});
    shop.npc = ClientNpc{"npc.rat", "Giant Rat", "Creo que me ha llamdo 'hermano'?", 100, true};
    world_["shop"] = shop;
}

const ClientRoom& GameClient::getCurrentRoom() const
{
    return world_.at(currentRoomId_);
}

const ClientRoom& GameClient::getRoom(const std::string& roomId) const
{
    return world_.at(roomId);
}

const t_stats& GameClient::getStats() const
{
    return stats_;
}

unsigned int GameClient::getGold() const
{
    return gold_;
}

const std::list<ClientItem>& GameClient::getInventory() const
{
    return inventory_;
}

const std::list<std::string>& GameClient::getDiscoveredRoomIds() const
{
    return discoveredRoomIds_;
}

bool GameClient::move(Direction direction)
{
    const ClientRoom& room = world_.at(currentRoomId_);
    auto it = room.exits.find(direction);
    if (it == room.exits.end())
        return false;

    currentRoomId_ = it->second;
    if (std::find(discoveredRoomIds_.begin(), discoveredRoomIds_.end(), currentRoomId_) == discoveredRoomIds_.end())
        discoveredRoomIds_.push_back(currentRoomId_);
    return true;
}

bool GameClient::takeItem(const std::string& itemId)
{
    ClientRoom& room = world_.at(currentRoomId_);
    auto found = std::find_if(room.items.begin(), room.items.end(),
                               [&](const ClientItem& it) { return it.id == itemId; });
    if (found == room.items.end())
        return false;

    inventory_.push_back(*found);
    room.items.erase(found);
    return true;
}

bool GameClient::dropItem(const std::string& itemId)
{
    auto found = std::find_if(inventory_.begin(), inventory_.end(),
                               [&](const ClientItem& it) { return it.id == itemId; });
    if (found == inventory_.end())
        return false;

    world_.at(currentRoomId_).items.push_back(*found);
    inventory_.erase(found);
    return true;
}

std::string GameClient::talkToNpc()
{
    const ClientRoom& room = world_.at(currentRoomId_);
    if (room.npc && !room.npc->isEnemy)
        return room.npc->description;
    return "";
}

void GameClient::attackNpc()
{
    ClientRoom& room = world_.at(currentRoomId_);
    if (room.npc && room.npc->isEnemy)
    {
        if (stats_.hp > 1)
            stats_.hp -= 1;
    }
}