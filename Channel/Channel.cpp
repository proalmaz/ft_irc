#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(string name, Clients &admin) : m_name(name), m_admin(&admin)
{
	m_users.push_back(&admin);
}

Channel::~Channel()
{
//	for (int i = 0; i < m_users.size(); ++i)
//		sendMessageToClient(*m_users[i],"Channel " + m_name +
//		" was destroy.\n");
}

Channel::Channel(Channel const &copy) { *this = copy; }

Channel		&Channel::operator=(Channel const &copy)
{
	if (this == &copy)
		return *this;
	m_name = copy.m_name;
	m_users = copy.m_users;
	m_admin = copy.m_admin;
	return *this;
}

Clients		*Channel::getAdmin() { return m_admin; }

std::string Channel::getName() { return m_name; }

void 		Channel::addUser(Clients &client)
{
	m_users.push_back(&client);
}