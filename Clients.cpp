#include "Clients.hpp"

Clients::Clients()
{
	bzero(&m_addr, sizeof(m_addr));
	m_len = sizeof (m_addr);
}

Clients::Clients(const Clients &copy) { *this = copy; }

Clients			Clients::operator=(const Clients &copy)
{
	if (this == &copy)
		return *this;
	m_addr = copy.m_addr;
	m_len = copy.m_len;
	m_fd = copy.m_fd;
	return *this;
}

Clients::~Clients() {}

int				Clients::getFd() { return m_fd; }

sockaddr_in		*Clients::getAddr() { return &m_addr; }

socklen_t 		*Clients::getLen() {return &m_len; }

void 			Clients::setFd(int fd) {m_fd = fd;}