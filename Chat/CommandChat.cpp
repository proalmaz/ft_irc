#include "Chat.hpp"

void Chat::createChannel(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 2 || cmd[1].front() == '\n' || cmd[1].front() != '#')
		return sendMessageToClient(src, B_RED "Correct format:\nJOIN "
											  "#<channel name>\n" NO_COLOR);
	string name = ft_strtrim(cmd[1], "\n");
	for (int i = 0; i < m_channels.size(); ++i)
	{
		if (m_channels[i]->getName() == name)
		{
			m_channels[i]->addUser(src);
			if (src.getChannel() != nullptr)
				src.getChannel()->removeUser(src);
			src.setChannel(m_channels[i]);
			sendMessageToClient(src, B_GREEN "You are joined to channel " +
			m_channels[i]->getName() + "\n" NO_COLOR);
			return;
		}
	}
	m_channels.push_back( new Channel(name, src));
	if (src.getChannel() != nullptr)
		src.getChannel()->removeUser(src);
	src.setChannel(m_channels.at(m_channels.size() - 1));
	sendMessageToClient(src, B_GREEN "You are created channel " +
	m_channels.back()->getName() + "\n" NO_COLOR);
}

void 	Chat::leave(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 1 || cmd[1].front() == '\n')
	{
		return sendMessageToClient(src, B_RED "Correct format:\nLEAVE\n"
		NO_COLOR);
	}
	if (src.getChannel() == nullptr)
		return sendMessageToClient(src, B_RED "You are not a member of any "
										"channel.\n" NO_COLOR);
	for (int i = 0; i < m_channels.size(); ++i)
	{
		if (m_channels[i] == src.getChannel())
		{
			m_channels[i]->removeUser(src);
			src.setChannel(nullptr);
			return sendMessageToClient(src, B_GREEN "You have left the channel "
			+ m_channels[i]->getName() + NO_COLOR "\n");
		}
	}
}

void    Chat::sendPrivateMessage(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 3 || cmd[2].front() == '\n')
		sendMessageToClient(src, B_RED "Correct format:\nPRIVMSG <nickname> <message>\n" NO_COLOR);
	else
	{
		if (cmd[1] == src.getNickname())
			return;
		for (int i = 0; i < m_clients.size(); ++i)
		{
			if (m_clients[i]->getNickname() == cmd[1])
				return sendMessageToClient(*m_clients[i], B_PURPLE +
				src.getNickname() + " whispered you: " + cmd[2] + NO_COLOR);
		}
		sendMessageToClient(src, B_RED + cmd[1] + " does not exist.\n" NO_COLOR);
	}
}

void Chat::kick(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 2 || cmd[1].front() == '\n')
		sendMessageToClient(src, B_RED "Correct format:\nKICK <nickname> \n" NO_COLOR);
	else
	{
		vector<Clients *> tmp = src.getChannel()->getUsers();
		if (&src == src.getChannel()->getAdmin())
		{
			string name = ft_strtrim(cmd[1], "\n");
			for (int i = 0; i < tmp.size(); ++i)
			{
				if (tmp[i]->getNickname() == name)
				{
					if (tmp[i] == src.getChannel()->getAdmin())
					{
						sendMessageToClient(src, B_RED "You can't kick "
						"yourself.\n" NO_COLOR);
						return;
					}
					sendMessageToClient(*tmp[i], B_RED "You was kicked by " + src
					.getNickname() + "\n" NO_COLOR);
					tmp[i]->setChannel(nullptr);
					src.getChannel()->removeUser(*tmp[i]);
					return;
				}
			}
			sendMessageToClient(src, B_RED "Client " + name + " not found in "
			"this channel.\n" NO_COLOR);
		}
		else
		{
			sendMessageToClient(src, B_RED "Only admin can kick users.\n"
			NO_COLOR);
		}
	}
}

void Chat::printHelp(Clients &src, std::vector<string> &cmd)
{
	std::string output = "HELP 	- show a list of available commands.\n"
						 "KICK 	- kick the client off the channel.\n"
						 "JOIN 	- create or enter an existing channel.\n"
						 "NICK 	- change nickname.\n"
						 "PRIVMSG	- send private message somebody.\n"
						 "QUIT	- leave a chat.\n"
						 "LEAVE	- leave a channel.\n";
	sendMessageToClient(src, output);
}

void    Chat::changeNickname(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 2)
		sendMessageToClient(src, B_RED "Correct format:\nNICK <nickname>\n" NO_COLOR);
	else if (cmd[1].front() == '\n')
		sendMessageToClient(src, B_RED "I can't identify you if you put empty nickname."
									   " Try again!\n" NO_COLOR);
	else if (checkNicknameAlreadyUsed(m_clients, cmd[1]))
		sendMessageToClient(src, B_RED "Such a nickname already exists. "
									   "Please choose another option.\n" NO_COLOR);
	else
	{
		src.setNickname(ft_strtrim(cmd[1], "\n"));
		sendMessageToClient(src, B_GREEN "Nickname was change to " + src.getNickname() + "\n" NO_COLOR );
	}
}

void Chat::quitClient(Clients &src, vector<string> &cmd)
{
	for (int i = 0; i < m_clients.size(); ++i)
	{
		if (m_clients[i]->getFd() == src.getFd())
		{
			close(src.getFd());
			m_clients.erase(m_clients.begin() + i);
		}
	}
}