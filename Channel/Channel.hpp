#pragma once

#include "../Chat/Chat.hpp"

class Channel
{
private:
	string					m_name;
	std::vector<Clients *>	m_users;
	Clients					*m_admin;

public:
	Channel();
	Channel(string name, Clients &admin);
	Channel(Channel const &copy);
	~Channel();

	Channel					&operator=(Channel const &copy);

	std::string 			getName();
	Clients					*getAdmin();
	void 					addUser(Clients &client);
};
