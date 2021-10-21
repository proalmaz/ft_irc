#include "Clients.hpp"
#include "Chat.hpp"

int main(int argc, char **argv)
{
	Chat	chat;
	chat.socketPreparation();
	chat.runChat();
	return 0;
}
