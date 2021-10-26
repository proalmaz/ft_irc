#include "Chat/Chat.hpp"

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		exit(1);
	}

	int port = atoi(argv[1]);

	if (port < 1024 || port > 49151)
	{
		std::cout << "Wrong port!" << std::endl;
		exit(1);
	}
	Chat	chat(port, argv[2]);
	try
	{
		chat.socketPreparation();
	}
	catch (string &error)
	{
		std::cerr << error << endl;
		return 1;
	}
	chat.runChat();
	return 0;
}
