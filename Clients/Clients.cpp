#include "Clients.hpp"

Clients::Clients() : m_channel(nullptr)
{
	m_message = "";
	bzero(&m_addr, sizeof(m_addr));
	m_len = sizeof (m_addr);
}

Clients::Clients(const Clients &copy) { *this = copy; }

Clients			&Clients::operator=(const Clients &copy)
{
	if (this == &copy)
		return *this;
	m_nickname = copy.m_nickname;
	m_username = copy.m_username;
	m_message = copy.m_message;
	m_status = copy.m_status;
	m_addr = copy.m_addr;
	m_len = copy.m_len;
	m_fd = copy.m_fd;
	if (m_channel != nullptr)
		free(m_channel);
	m_channel = copy.m_channel;
	return *this;
}

Clients::~Clients() {}

int				Clients::getFd() { return m_fd; }

sockaddr_in		*Clients::getAddr() { return &m_addr; }

socklen_t 		*Clients::getLen() {return &m_len; }

void 			Clients::setFd(int fd) {m_fd = fd;}

void 			Clients::setStatus(STATUSFLAG statusflag)
{
	m_status = statusflag;
}

STATUSFLAG		Clients::getStatus() {return m_status;}

string		Clients::getNickname() { return m_nickname; }

string		Clients::getName() { return m_username; }

void 		Clients::setChannel(Channel *channel)
{
	m_channel = channel;
}

Channel		*Clients::getChannel()
{
	return m_channel;
}

void 		Clients::setNickname(const string &nickname)
{
	m_nickname = nickname;
}

void 		Clients::setName(string name)
{
	m_username = name;
}

std::string Clients::getMessage() { return m_message; }

void 	Clients::setMessage(string src)
{
	m_message = src;
}

void 	Clients::appendMessage(string src)
{
	m_message += src;
}