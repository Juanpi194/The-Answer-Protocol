// TODO: Change this file's name to main.cpp
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <cstring>
#include <cerrno>

#include "utils/utils.hpp"
#include "server/PlayerConnection.hpp"

const std::string	DEFAULT_CLIENT = "Alberto";
constexpr size_t	MAX_MSG_LENGTH = 1024;

static void	client_thread(PlayerConnection& client)
{
	struct timeval			timeout;
	char					msg[MAX_MSG_LENGTH];
	ssize_t					bytes;
	std::list<std::string>	outbox_msgs;
	const int				client_fd = client.get_client_fd();

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;
	setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	while (client.is_connected())
	{
		memset(msg, 0, MAX_MSG_LENGTH);
		bytes = recv(client_fd, msg, MAX_MSG_LENGTH, 0);
		if (bytes == 0)
		{
			client.disconnect();
			break ;
		}
		else if (bytes > 0)
		{
			// Send to server...
		}
		else
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				client.disconnect();
				break ;
			}
		}
		outbox_msgs = client.get_player().drain_outbox();
		for (const std::string& outbox_msg: outbox_msgs)
		{
			bytes = send(client_fd, outbox_msg.c_str(), outbox_msg.size(), MSG_NOSIGNAL);
			if (bytes == -1)
			{
				client.disconnect();
				break;
			}
		}
	}
}

static void	debug_mode(void)
{
	PlayerConnection	client(DEFAULT_CLIENT, -1, nullptr);

	// TODO
}

static void	normal_mode(void)
{
	// TODO
}

int	main(void)
{
	if (current_level == LogLevel::DEBUG)
		debug_mode();
	else
		normal_mode();
	return (0);
}