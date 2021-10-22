#include "Chat.hpp"

Chat::Chat() : m_fds(-1), m_password("123")
{}

Chat::Chat(const Chat &copy) { *this = copy; }

Chat::~Chat() {}

Chat	&Chat::operator=(const Chat &copy)
{
	if (this == &copy)
		return *this;
	m_rfd = copy.m_rfd;
	m_max_fd = copy.m_max_fd;
	m_fds = copy.m_fds;
	m_clients = copy.m_clients;
	m_password = copy.m_password;
//	m_channels = copy.m_channels;
	return *this;
}

void Chat::socketPreparation()
{
	struct sockaddr_in addr;
	// создание фд сокета
	m_fds = socket(AF_INET, SOCK_STREAM, 0);
	if (m_fds == -1)
		exit(1);
	int reuseOpt = 1;
	setsockopt(m_fds, SOL_SOCKET, SO_REUSEADDR, &reuseOpt, sizeof(reuseOpt));
	// мы говорим, что будем использовать ТСP протокол
	addr.sin_family = AF_INET;
	// порт любой, кроме (1 - 1023)
	addr.sin_port = htons(8080);
	// 127.0.0.1
//	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (!inet_aton("127.0.0.1", &(addr.sin_addr)))
	{
		cout << "not valid ip address\n";
		exit(1);
	}
	// связываем фд сокета со структурой адреса
	if (bind(m_fds, (const struct sockaddr *)&addr, sizeof(addr)) == -1) //
		exit(111);
	// переводим сокет в режим прослушки
	if (listen(m_fds, 128) != 0)
		exit(1);
}

void Chat::putFdSpace()
{
	m_max_fd = m_fds;
	// очищаем множество фдшников для чтения
	FD_ZERO(&m_rfd);
	// добавляем фд сокета в множество фдшников для чтения
	FD_SET(m_fds, &m_rfd);
	for (int i = 0; i < m_clients.size(); ++i)
	{
		// добавляем фд клиентов в это множество
		FD_SET(m_clients[i].getFd(), &m_rfd);
		if (m_clients[i].getFd() > m_max_fd)
			// ищем и сохраняем макс фдшник
			m_max_fd = m_clients[i].getFd();
	}
}

void Chat::newClientHandler()
{
	// создаем нового клиента
	Clients client;
	// принимаем новый фд, попутно заполняя его объект адрессом
	int fdnew = accept(m_fds, (sockaddr *)(client.getAddr()),
					   client.getLen());
	if (fdnew < 0)
		exit(1);
	else
		// сохраняю фд клиента в класс
		client.setFd(fdnew);
	client.setStatus(NEW_CLIENT);
	client.authorization();
	// добавляю клиента в вектор
	m_clients.push_back(client);
}

void Chat::checkClientsFd()
{
	for (int i = 0; i < m_clients.size(); ++i)
	{
		if (FD_ISSET(m_clients[i].getFd(), &m_rfd))
		{
			if (getMessage(m_clients[i]) == CLIENT_OFF)
			{
				close(m_clients[i].getFd());
				m_clients.erase(m_clients.begin() + i);
			}
		}
	}
}

void Chat::checkPassword(Clients &src)
{
	std::string input = ft_strtrim(src.getMessage(), "\n");
	if (input == m_password)
	{

		sendMessageToClient(src, "Authorization success!\n"
								 "Please enter you nickname: ");
		src.setStatus(AUTHORIZED_PASSWORD);
	}
	else
		sendMessageToClient(src, "Authorization failed!\n"
								 "Please enter correct password: ");
}

void Chat::putNickname(Clients &src)
{
	std::string input = ft_strtrim(src.getMessage(), "\n");
	if (input.empty())
		sendMessageToClient(src, "I can't identify you if you put empty nickname."
								 " Try again!\nPlease enter you nickname: ");
	else if (checkNicknameAlreadyUsed(m_clients, src))
		sendMessageToClient(src, "Such a nickname already exists. "
								 "Please choose another option.\nPlease enter you nickname: ");
	else
	{
		sendMessageToClient(src, "Welcome in our chat " + input + "!\n");
		src.setNickname(input);
		src.setStatus(AUTHORIZED_NICK);
	}
}

void Chat::sendMessage(Clients &src)
{
	std::string input = src.getMessage();
	for (int i = 0; i < m_clients.size(); ++i)
	{
		if (m_clients[i].getFd() != src.getFd()
		&& m_clients[i].getStatus()	== AUTHORIZED_NICK)
			sendMessageToClient(m_clients[i], src.getNickname() + ": " + input);
	}
}

void Chat::createChannel(Clients &src)
{

}

int Chat::getMessage(Clients &src)
{
	char buff[10000];
	int res = recv(src.getFd(), buff, 10000, 0);
	if (res <= 0)
		return CLIENT_OFF;
	buff[res] = '\0';
	src.appendMessage(buff);
	if (src.getMessage().back() != '\n')
		return CLIENT_ALL_RIGHT;
	if (src.getStatus() == NEW_CLIENT)
		checkPassword(src);
	else if (src.getStatus() == AUTHORIZED_PASSWORD)
		putNickname(src);
	else if (src.getStatus() == AUTHORIZED_NICK
			&& src.getMessage().front() != '\n')
		sendMessage(src);
	src.setMessage("");
//	else if (src.getStatus() == AUTHORIZED_NICK && src.getMessage() ==
//	"create channel")
//		createChannel(src);
	return CLIENT_ALL_RIGHT;
}

void Chat::runChat()
{
	while (1)
	{
		putFdSpace();
		// отлавливаем фдшник на котором произошло событие
		if (select(m_max_fd + 1, &m_rfd, 0, 0, 0) < 1)
			exit(1);
		// проверяем есть ли фд сокета в нашем множестве
		if (FD_ISSET(m_fds, &m_rfd))
			newClientHandler();
		else
			checkClientsFd();
	}
}