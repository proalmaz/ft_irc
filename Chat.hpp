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
	void 	checkPassword(Clients &src);
	void 	putNickname(Clients &src);
	void 	sendMessage(Clients &src);
	int 	getMessage(Clients &src);

};
