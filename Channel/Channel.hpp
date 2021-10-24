#pragma once

#include "../Chat/Chat.hpp"

class Channel
{
private:
	string					m_name;
	vector<Clients *>		m_users;
	Clients					*m_admin;

public:
	Channel();
	Channel(string name, Clients &admin);
	Channel(Channel const &copy);

	~Channel();

	Channel					&operator=(Channel const &copy);
	string		 			getName();
	Clients					*getAdmin();
	vector<Clients *>		getUsers();

	void 					addUser(Clients &client);
};
