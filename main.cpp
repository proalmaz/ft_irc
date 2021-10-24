#include "Chat/Chat.hpp"

int main(int argc, char **argv)
{
	Chat	chat;
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
