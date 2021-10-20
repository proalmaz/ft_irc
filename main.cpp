#include "Clients.hpp"

int main()
{
	int fds;
	// структура для хранения семьи, порта и адреса
	struct sockaddr_in addr;
	// создание фд сокета
	fds = socket(AF_INET, SOCK_STREAM, 0);
	if (fds == -1)
		return (1);
	// мы говорим, что будем использовать ТСP протокол
	addr.sin_family = AF_INET;
	// порт любой, кроме (1 - 1023)
	addr.sin_port = htons(8080);
	// 127.0.0.1
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// связываем фд сокета со структурой адреса
	if (bind(fds, (const struct sockaddr *)&addr, sizeof(addr)) == -1) //
		return (1);
	// переводим сокет в режим прослушки
	if (listen(fds, 128) != 0)
		return (1);
	std::vector<Clients> clients;
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
			Clients client;
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
