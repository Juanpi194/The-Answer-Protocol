#include "group/Group.hpp"

#include "server/PlayerConnection.hpp"

unsigned int		Group::available_id = 1;
const std::string	Group::PREFIX = "group_";

// Constructors ---------------------------------------------------------------

Group::Group(PlayerConnection& leader):
	id(PREFIX + std::to_string(available_id++)),
	leader(&leader)
{
	members.push_back(&leader);
}

// Operators ------------------------------------------------------------------

bool	Group::operator==(const Group& other) const noexcept
{
	if (id == other.id)
		return (true);
	return (false);
}

// Getters and setters --------------------------------------------------------

std::string							Group::get_id(void) const noexcept
{
	return (id);
}

PlayerConnection					*Group::get_leader(void) const noexcept
{
	return (leader);
}

const std::list<PlayerConnection*>&	Group::get_members(void) const noexcept
{
	return (members);	
}

const std::list<PlayerConnection*>&	Group::get_invited(void) const noexcept
{
	return (invited);
}

void	Group::set_leader(PlayerConnection& leader) noexcept
{
	this->leader = &leader;
}

// Utils ----------------------------------------------------------------------

bool	Group::invite_member(PlayerConnection& member)
{
	if (member.get_group())
		return (false);
	if (is_member_invited(member))
		return (false);
	invited.push_back(&member);
	return (true);
}

bool	Group::accept_member(PlayerConnection& member)
{
	if (!is_member_invited(member))
		return (false);
	invited.remove(&member);
	members.push_back(&member);
	member.set_group(this);
	return (true);
}

bool	Group::remove_member(PlayerConnection& member) noexcept
{
	if (!is_member_in_group(member))
		return (false);
	members.remove(&member);
	member.set_group(nullptr);
	if (&member == leader && members.size() > 0)
		leader = members.front();
	return (true);
}

bool	Group::is_member_in_group(PlayerConnection& member) const noexcept
{
	if (!member.get_group())
		return (false);
	if (member.get_group() != this)
		return (false);
	for (PlayerConnection *group_member: members)
		if (group_member == &member)
			return (true);
	return (false);
}

bool	Group::is_member_invited(PlayerConnection& member) const noexcept
{
	if (member.get_group())
		return (false);
	for (PlayerConnection *invited_member: invited)
		if (invited_member == &member)
			return (true);
	return (false);
}
