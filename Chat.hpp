#pragma once

#include "Clients.hpp"

class Chat
{
private:
	int						m_fds;
	std::vector<Clients>	m_clients;
	string					m_password;
	fd_set					m_rfd;
	int 					m_max_fd;

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
	void 	checkPassword(string input, Clients &src);
	void 	putNickname(string input, Clients &src);
	int 	getMesage(Clients &src);
};
