#pragma once

#include "../Clients/Clients.hpp"
#include "../Channel/Channel.hpp"


class Chat
{
private:
	struct 					s_command
	{
		std::string			s_commandName;
		void 				(Chat::*f)(Clients &src, std::vector<string> &cmd);
	};
	int						m_fds;
	vector<Clients *>		m_clients;
	vector<Channel *>		m_channels;
	string					m_password;
	fd_set					m_rfd;
	int 					m_max_fd;
	s_command				m_commands[10];

public:
	Chat();
	Chat(Chat const &copy);
	~Chat();

	Chat	&operator=(Chat const &copy);

	void 	socketPreparation();
	void 	runChat();
	void 	putFdSpace();
	void 	newClientHandler();
	void 	checkClientsFd();
	void 	checkPassword(Clients &src);
	void 	putNickname(Clients &src);
	void 	putUsername(Clients &src);
	void 	sendMessage(Clients &src);
	int 	getMessage(Clients &src);
	bool 	checkCommand(Clients &src);

	void 	whois(Clients &src, vector<string> &cmd);
	void 	who(Clients &src, vector<string> &cmd);
	void 	list(Clients &src, vector<string> &cmd);
	void 	leave(Clients &src, vector<string> &cmd);
	void	kick(Clients &src, vector<string> &cmd);
	void 	printHelp(Clients &src, vector<string> &cmd);
	void 	createChannel(Clients &src, vector<string> &cmd);
	void	quitClient(Clients &src, vector<string> &cmd);
	void    changeNickname(Clients &src, vector<string> &cmd);
	void    sendPrivateMessage(Clients &src, vector<string> &cmd);
};

bool 			checkNicknameAlreadyUsed(std::vector<Clients *> &clients,
										  string input);
int				checkEmptyMessage(Clients &src);
void			sendMessageToClient(Clients &src, string output);
vector<string>	ft_split(std::string str, const std::string &del);

# define	NO_COLOR	"\033[0m"

# define	BLACK		"\033[0;30m"
# define	RED			"\033[0;31m"
# define	GREEN		"\033[0;32m"
# define	YELLOW		"\033[0;33m"
# define	BLUE		"\033[0;34m"
# define	PURPLE		"\033[0;35m"
# define	CYAN		"\033[0;36m"

# define	B_GRAY		"\033[1;30m"
# define	B_RED		"\033[1;31m"
# define	B_GREEN		"\033[1;32m"
# define	B_YELLOW	"\033[1;33m"
# define	B_BLUE		"\033[1;34m"
# define	B_PURPLE	"\033[1;35m"
# define	B_CYAN		"\033[1;36m"
# define	B_WHITE		"\033[1;37m"