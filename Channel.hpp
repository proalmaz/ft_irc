#pragma once

#include "Chat.hpp"

class Channel
{
private:
	string					m_name;
	std::vector<Clients *>	m_users;
	Clients					*m_admin;

	Channel();
public:
	Channel(string name, Clients *admin);
	Channel(Channel &copy);
	~Channel();

	Channel					&operator=(Channel &copy);

	std::string 			getName();
	Clients					*getAdmin();
	void 					addUser(Clients &client);
};
