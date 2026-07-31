#pragma once
#include <string>

std::string	evt_presence_enter(const std::string& name);

std::string	evt_presence_leave(const std::string& name);

std::string	evt_room_chat(const std::string& name, const std::string& msg);

std::string	evt_global_chat(const std::string& name, const std::string& msg);

std::string	evt_group_invite(const std::string& inviter);

std::string	evt_group_join(const std::string& name);

std::string	evt_group_leave(const std::string& name);

std::string	evt_group_chat(const std::string& name, const std::string& msg);

std::string	evt_stats(size_t count);

