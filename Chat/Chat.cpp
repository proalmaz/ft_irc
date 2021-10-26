#include "Chat.hpp"

Chat::Chat() : m_fds(-1), m_password("123")
{
	m_commands[0].s_commandName = "HELP";
	m_commands[0].f = &Chat::printHelp;
	m_commands[1].s_commandName = "KICK";
	m_commands[1].f = &Chat::kick;
	m_commands[2].s_commandName = "JOIN";
	m_commands[2].f = &Chat::createChannel;
	m_commands[3].s_commandName = "PRIVMSG";
	m_commands[3].f = &Chat::sendPrivateMessage;
	m_commands[4].s_commandName = "QUIT";
	m_commands[4].f = &Chat::quitClient;
	m_commands[5].s_commandName = "LEAVE";
	m_commands[5].f = &Chat::leave;
	m_commands[6].s_commandName = "NICK";
	m_commands[6].f = &Chat::changeNickname;
	m_commands[7].s_commandName = "LIST";
	m_commands[7].f = &Chat::list;
	m_commands[8].s_commandName = "WHO";
	m_commands[8].f = &Chat::who;
	m_commands[9].s_commandName = "WHOIS";
	m_commands[9].f = &Chat::whois;
	m_commands[10].s_commandName = "WHOIN";
	m_commands[10].f = &Chat::whoin;
}

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
		throw string("Error: socket\n");
	int reuseOpt = 1;
	if (setsockopt(m_fds, SOL_SOCKET, SO_REUSEADDR, &reuseOpt, sizeof
		(reuseOpt)) == -1)
		throw string("Error: setsockopt\n");
	// мы говорим, что будем использовать ТСP протокол
	addr.sin_family = AF_INET;
	// порт любой, кроме (1 - 1023)
	addr.sin_port = htons(8081);
	// 127.0.0.1
//	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (!inet_aton("127.0.0.1", &(addr.sin_addr)))
		throw string("not valid ip address\n");
	// связываем фд сокета со структурой адреса
	if (bind(m_fds, (const struct sockaddr *)&addr, sizeof(addr)) == -1) //
		throw string("Error: bind\n");
	// переводим сокет в режим прослушки
	if (listen(m_fds, 128) != 0)
		throw string("Error: listen\n");
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
		FD_SET(m_clients[i]->getFd(), &m_rfd);
		if (m_clients[i]->getFd() > m_max_fd)
			// ищем и сохраняем макс фдшник
			m_max_fd = m_clients[i]->getFd();
	}
}

void Chat::newClientHandler()
{
	// создаем нового клиента
	Clients *client = new Clients();
	// принимаем новый фд, попутно заполняя его объект адрессом
	int fdnew = accept(m_fds, (sockaddr *)(client->getAddr()),
					   client->getLen());
	if (fdnew < 0)
		exit(1);
	else
		// сохраняю фд клиента в класс
		client->setFd(fdnew);
	client->setStatus(NEW_CLIENT);
//	client.authorization();
    sendMessageToClient(*client, "Enter password: ");
	// добавляю клиента в вектор
	m_clients.push_back(client);
}

void Chat::checkClientsFd()
{
	for (int i = 0; i < m_clients.size(); ++i)
	{
		if (FD_ISSET(m_clients[i]->getFd(), &m_rfd))
		{
			if (getMessage(*m_clients[i]) == CLIENT_OFF)
			{
				close(m_clients[i]->getFd());
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

		sendMessageToClient(src, B_GREEN "Authorization success!\n"
								 "Please enter your nickname: " NO_COLOR);
		src.setStatus(AUTHORIZED_PASSWORD);
	}
	else
		sendMessageToClient(src,  B_RED "Authorization failed!\n"
								 "Please enter correct password: " NO_COLOR);
}

void Chat::putNickname(Clients &src)
{
	std::string input = ft_strtrim(src.getMessage(), "\n");
	if (input.empty())
		sendMessageToClient(src, B_RED "I can't identify you if you put empty nickname."
								 " Try again!\nPlease enter your nickname: " NO_COLOR);
	else if (checkNicknameAlreadyUsed(m_clients, input))
		sendMessageToClient(src, B_RED "Such a nickname already exists. "
								 "Please choose another option.\nPlease enter"
								 " your nickname: " NO_COLOR );
	else
	{
		sendMessageToClient(src, B_GREEN "Please enter your username: "
		NO_COLOR);
		src.setNickname(input);
		src.setStatus(AUTHORIZED_NICK);
	}
}

void Chat::putUsername(Clients &src)
{
	std::string input = ft_strtrim(src.getMessage(), "\n");
	if (input.empty())
		sendMessageToClient(src, B_RED "I can't identify you if you put empty"
		" name. Try again!\nPlease enter your name: " NO_COLOR);
	else
	{
		sendMessageToClient(src, B_GREEN "Welcome to our chat " + input + "!\n"
		"Enter HELP for show a list of available commands\n" NO_COLOR);
		src.setName(input);
		src.setStatus(AUTHORIZED_USERNAME);
	}
}

void Chat::sendMessage(Clients &src)
{
	string input = src.getMessage();
	if (src.getChannel() == nullptr)
		sendMessageToClient(src, B_RED "Create or join channel.\nEnter HELP "
		"for more info.\n" NO_COLOR);
	else
	{
		vector<Clients *> tmp = src.getChannel()->getUsers();
		for (int i = 0; i < tmp.size(); ++i)
		{
			if (tmp[i]->getFd() != src.getFd())
				sendMessageToClient(*tmp[i], B_YELLOW + src.getChannel()
				->getName() + NO_COLOR ": " + src.getNickname() + ": " +
				input);
		}
	}
}

bool Chat::checkCommand(Clients &src)
{
	string input = src.getMessage();
	vector<string> cmd = ft_split(input, " ");
	string command = ft_strtrim(cmd[0], "\n");
	for (int i = 0; i < 11; ++i)
	{
		if (command == m_commands[i].s_commandName)
		{
			(this->*m_commands[i].f)(src, cmd);
	        src.setMessage("");
			return true;
		}
	}
	return false;
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
	else if (src.getStatus() == AUTHORIZED_NICK)
		putUsername(src);
	else if (checkCommand(src))
		return CLIENT_ALL_RIGHT;
	else if (src.getStatus() == AUTHORIZED_USERNAME
			&& src.getMessage().front() != '\n')
		sendMessage(src);
	src.setMessage("");
	return CLIENT_ALL_RIGHT;
}

void Chat::runChat()
{
	while (1)
	{
		putFdSpace();
		// отлавливаем фдшник на котором произошло событие
		if (select(m_max_fd + 1, &m_rfd, 0, 0, 0) < 0)
			exit(1);
		// проверяем есть ли фд сокета в нашем множестве
		if (FD_ISSET(m_fds, &m_rfd))
			newClientHandler();
		else
			checkClientsFd();
	}
}