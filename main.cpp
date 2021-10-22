#include "Clients/Clients.hpp"
#include "Chat/Chat.hpp"

int main(int argc, char **argv)
{
	Chat	chat;
	chat.socketPreparation();
	chat.runChat();
	return 0;
}
