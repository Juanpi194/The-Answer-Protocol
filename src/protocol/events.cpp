#include "protocol/events.hpp"

std::string	evt_presence_enter(const std::string& name)
{
	return ("EVT ROOM PRESENCE ENTER " + name + "\n");
}

std::string	evt_presence_leave(const std::string& name)
{
	return ("EVT ROOM PRESENCE LEAVE " + name + "\n");
}

std::string	evt_room_chat(const std::string& name, const std::string& msg)
{
	return ("EVT ROOM CHAT " + name + " " + msg + "\n");
}

std::string	evt_global_chat(const std::string& name, const std::string& msg)
{
	return ("EVT GLOBAL CHAT " + name + " " + msg + "\n");
}

std::string	evt_group_invite(const std::string& inviter)
{
	return ("EVT GROUP INVITE " + inviter + "\n");
}

std::string	evt_group_join(const std::string& name)
{
	return ("EVT GROUP JOIN " + name + "\n");
}

std::string	evt_group_leave(const std::string& name)
{
	return ("EVT GROUP LEAVE " + name + "\n");
}

std::string	evt_group_chat(const std::string& name, const std::string& msg)
{
	return ("EVT GROUP CHAT " + name + " " + msg + "\n");
}

std::string	evt_stats(size_t count)
{
	return ("EVT STATS players=" + std::to_string(count) + "\n");
}
