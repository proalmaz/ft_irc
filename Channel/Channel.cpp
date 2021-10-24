#include "Channel.hpp"

Channel::Channel() : m_users(0),  m_name(""), m_admin(0) {}

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
	if (!m_users.empty())
		for (int i = 0; i < m_users.size(); ++i)
			free(m_users[i]);
	for (int i = 0; i < copy.m_users.size(); ++i)
		m_users[i] = copy.m_users[i];
	if (m_admin)
		free(m_admin);
	m_admin = copy.m_admin;
	return *this;
}

Clients		*Channel::getAdmin() { return m_admin; }

std::string Channel::getName() { return m_name; }

void 		Channel::removeUser(Clients &user)
{
	for (int i = 0; i < m_users.size(); ++i)
	{
		if (&user == m_users[i])
		{
			m_users.erase(m_users.begin() + i);
		}
	}
}

void 		Channel::addUser(Clients &client)
{
	m_users.push_back(&client);
}

vector<Clients *> Channel::getUsers() { return m_users; }