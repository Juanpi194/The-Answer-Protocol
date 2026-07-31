#pragma once
#include <string>
#include <list>

class PlayerConnection;

class Group
{
	private:
		const std::string				id;
		PlayerConnection				*leader;
		std::list<PlayerConnection*>	members;
		std::list<PlayerConnection*>	invited;

		static unsigned int			available_id;	// Defined in Group.cpp
		static const std::string	PREFIX;	// Defined in Group.cpp
	public:
		// Constructors -------------------------------------------------------

		Group(PlayerConnection& leader);
		Group(const Group& group) = delete;
		~Group(void) = default;

		// Operators ----------------------------------------------------------

		Group&	operator=(const Group& other) = delete;
		bool	operator==(const Group& other) const noexcept;

		// Getters and setters ------------------------------------------------

		std::string							get_id(void) const noexcept;
		PlayerConnection					*get_leader(void) const noexcept;
		const std::list<PlayerConnection*>&	get_members(void) const noexcept;
		const std::list<PlayerConnection*>&	get_invited(void) const noexcept;

		void	set_leader(PlayerConnection& leader) noexcept;

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Adds the member to the invite list.
		 * @param	member	The member to add.
		 * @returns	`true` if the member was successfully added to the invited
		 * 			list. `false` if the member is in a group, or if it is in
		 * 			the invited list already.
		 */
		bool	invite_member(PlayerConnection& member);

		/**
		 * @brief	Adds a member to the members list, and removes it from
		 * 			the invited list.
		 * @param	member	The member to accept.
		 * @returns	`true` if the member was successfully accepted. `false`
		 * 			otherwise (If the member is not invited).
		 */
		bool	accept_member(PlayerConnection& member);

		/**
		 * @brief	Removes the specified member from the group.
		 * @param	member	The member to remove from the group.
		 * @returns	`true` if the member was successfully removed,
		 * 			`false` otherwise.
		 * @note	Being in the group garantees leaving it.
		 */
		bool	remove_member(PlayerConnection& member) noexcept;

		/**
		 * @brief	Checks if a specific member is in this group.
		 * @param	member	The member to evaluate.
		 * @returns	`true` if the member is in the group. `false` otherwise.
		 */
		bool	is_member_in_group(PlayerConnection& member) const noexcept;

		/**
		 * @brief	Checks if a specific member is in the invited list of
		 * 			this group.
		 * @param	member	The member to evaluate.
		 * @returns	`true` if the member is in the invited list of the group.
		 * 			`false` otherwise.
		 */
		bool	is_member_invited(PlayerConnection& member) const noexcept;
};
