#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>

class Clients
{
private:
	int			m_fd;
	sockaddr_in	m_addr;
	socklen_t	m_len;

public:
	Clients();
	Clients(Clients const &copy);
	~Clients();

	Clients			&operator=(Clients const &copy);

	int 			getFd();
	sockaddr_in 	*getAddr();
	socklen_t 		*getLen();
	void 			setFd(int fd);
};
