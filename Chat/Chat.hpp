#pragma once

#include "../Clients/Clients.hpp"
#include "../Channel/Channel.hpp"

class Chat
{
private:
	struct 					s_command
	{
		std::string			s_commandName;
		void 				(Chat::*f)(Clients &src, std::vector<string> &cmd);
	};
	int						m_fds;
	std::vector<Clients>	m_clients;
//	std::vector<Channel>	m_channels;
	string					m_password;
	fd_set					m_rfd;
	int 					m_max_fd;
	s_command				m_commands[7];

public:
	Chat();
	Chat(Chat const &copy);
	~Chat();

	Chat	&operator=(Chat const &copy);

	void 	socketPreparation();
	void 	runChat();
	void 	putFdSpace();
	void 	newClientHandler();
	void 	checkClientsFd();
	void 	checkPassword(Clients &src);
	void 	putNickname(Clients &src);
	void 	sendMessage(Clients &src);
	void 	createChannel(Clients &src);
	int 	getMessage(Clients &src);
	bool 	checkCommand(Clients &src);

	void 	printHelp(Clients &src, std::vector<string> &cmd);
	void	quitClient(Clients &src, vector<string> &cmd);
};

int			checkNicknameAlreadyUsed(std::vector<Clients> &clients, Clients &src);
int			checkEmptyMessage(Clients &src);
void		sendMessageToClient(Clients &src, string output);
std::vector<std::string>	ft_split(std::string str, const std::string &del);
