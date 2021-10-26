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
	if (cmd.size() != 1 || cmd[0].front() == '\n')
		return sendMessageToClient(src, B_RED "Correct format:\nLEAVE\n"
		NO_COLOR);
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
						 "LEAVE	- leave a channel.\n"
						 "LIST	- channel list.\n"
						 "WHO	- list of members in this channel.\n"
						 "WHOIS	- information about the member.\n"
						 "WHOIN	- information about the members in the channel.\n";
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

void Chat::who(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 1 || cmd[0].front() == '\n')
		return sendMessageToClient(src, B_RED "Correct format:\nWHO\n"
		NO_COLOR);
	if (src.getChannel() == nullptr)
		return sendMessageToClient(src, B_RED "You are not a member of any "
											  "channel.\n" NO_COLOR);
	vector<Clients *> tmp = src.getChannel()->getUsers();
	for (int i = 0; i < tmp.size(); ++i)
	{
		if (tmp[i] == src.getChannel()->getAdmin())
		{
			sendMessageToClient(src, B_YELLOW + std::to_string(i + 1)
			+ ". "+ tmp[i]->getNickname() + " (admin)" + NO_COLOR "\n");
			continue;
		}
		sendMessageToClient(src, B_YELLOW + std::to_string(i + 1)
		+ ". "+ tmp[i]->getNickname() + NO_COLOR "\n");
	}
}

void Chat::list(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 1 || cmd[0].front() == '\n')
		return sendMessageToClient(src, B_RED "Correct format:\nLIST\n"
		NO_COLOR);
	if (m_channels.size() == 0)
		return sendMessageToClient(src, B_RED "No channel.\n"
		NO_COLOR);
	for (int i = 0; i < m_channels.size(); ++i)
	{
		sendMessageToClient(src, B_YELLOW + std::to_string(i + 1)
		+ ". "+ m_channels[i]->getName() + " members(" +
		std::to_string(m_channels[i]->getUsers().size()) + ")" NO_COLOR "\n");
	}
}

void Chat::whois(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 2 || cmd[1].front() == '\n')
		sendMessageToClient(src, B_RED "Correct format:\nWHOIS <nickname> \n"
		NO_COLOR);
	string nick = ft_strtrim(cmd[1], "\n");
	for (int i = 0; i < m_clients.size(); ++i)
	{
		if (nick == m_clients[i]->getNickname())
			return sendMessageToClient(src, B_YELLOW "Nickname: " +
			m_clients[i]->getNickname() + "\nName: " + m_clients[i]->getName() + NO_COLOR
			"\n");;
	}
	sendMessageToClient(src, B_RED "Client not found.\n"
	NO_COLOR);
}

void Chat::whoin(Clients &src, vector<string> &cmd)
{
	if (cmd.size() != 2 || cmd[1].front() == '\n')
		return sendMessageToClient(src, B_RED "Correct format:\nWHOIN "
		"<channelname> \n" NO_COLOR);
	string channel = ft_strtrim(cmd[1], "\n");
	for (int i = 0; i < m_channels.size(); ++i)
	{
		if (m_channels[i]->getName() == channel)
		{
			vector<Clients *> tmp = m_channels[i]->getUsers();
			for (int j = 0; j < tmp.size(); ++j)
			{
				sendMessageToClient(src, B_YELLOW + std::to_string(j + 1)
				+ ". "+ tmp[j]->getNickname() + NO_COLOR "\n");
			}
			return;
		}
	}
	sendMessageToClient(src, B_RED "Channel not found.\n"NO_COLOR);
}