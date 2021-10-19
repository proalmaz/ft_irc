#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>

class m_clients
{
private:
	int m_fd;
	sockaddr_in m_addr;
	socklen_t m_len;

public:
	m_clients() { bzero(&m_addr, sizeof(m_addr));
		m_len = sizeof (m_addr); }
	m_clients(m_clients const &copy) { *this = copy; }
	m_clients &operator=(m_clients const &copy)
	{
		if (this == &copy)
			return *this;
		m_addr = copy.m_addr;
		m_len = copy.m_len;
		m_fd = copy.m_fd;
		return *this;
	}
	~m_clients() {}
	int 			getFd() { return m_fd; }
	sockaddr_in 	*getAddr() { return &m_addr; }
	socklen_t 		*getLen() {return &m_len; }
	void 			setFd(int fd) {m_fd = fd;}
};

int main()
{
	int fds;
	struct sockaddr_in addr; // структура для хранения семьи, порта и адреса
	fds = socket(AF_INET, SOCK_STREAM, 0); // создание фд сокета
	if (fds == -1)
		return (1);
	addr.sin_family = AF_INET;  // мы говорим, что будем использовать ТСP
	// протокол
	addr.sin_port = htons(8080); // порт любой, кроме (1 - 1023)
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // 127.0.0.1
	if (bind(fds, (const struct sockaddr *)&addr, sizeof(addr)) == -1) //
		// связываем фд сокета со структурой адреса
		return (1);
	if (listen(fds, 128) != 0) // переводим сокет в режим прослушки
		return (1);
	std::vector<m_clients> clients;
	while (1)
	{
		fd_set rfd, allfd;
		int max_fd = fds;
		// очищаем множество фдшников для чтения
		FD_ZERO(&rfd);
		// добавляем фд сокета в множество фдшников для чтения
		FD_SET(fds, &rfd);
		for (int i = 0; i < clients.size(); ++i)
		{
			// добавляем фд клиентов в это множество
			FD_SET(clients[i].getFd(), &rfd);
			if (clients[i].getFd() > max_fd)
				// ищем и сохраняем макс фдшник
				max_fd = clients[i].getFd();
		}
		// отлавливаем фдшник на котором произошло событие
		int res = select(max_fd + 1, &rfd, 0, 0, 0);
		if (res < 1)
			return (-1);
		// проверяем есть ли фд сокета в нашем множестве
		if (FD_ISSET(fds, &rfd))
		{
			// создаем нового клиента
			m_clients client;
			// принимаем новый фд, попутно заполняя его объект адрессом
			int fdnew = accept(fds, (sockaddr *)(client.getAddr()),
							   client.getLen());
			if (fdnew < 0)
				return (1);
			else
				// сохраняю фд клиента в класс
				client.setFd(fdnew);
			// добавляю клиента в вектор
			clients.push_back(client);
		}
	}
	return 0;
}
