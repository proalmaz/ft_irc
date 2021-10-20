#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <arpa/inet.h>

#define CLIENT_OFF 1
#define CLIENT_ALL_RIGHT 0

using std::cin;
using std::string;
using std::cout;
using std::endl;

enum STATUSFLAG
{
	NEW_CLIENT,
	AUTHORIZED_PASSWORD,
	AUTHORIZED_NICK,
};

class Clients
{
private:
	string		m_nickname;
	STATUSFLAG	m_status;
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
	STATUSFLAG		getStatus();
	std::string		getNickname();
	void 			setNickname(string &nickname);
	void 			setStatus(STATUSFLAG statusflag);
	void 			setFd(int fd);
	void 			authorization();
};

int					getMesage(Clients &src);
std::string			ft_strtrim(const std::string &str, const std::string &set);