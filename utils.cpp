#include "Clients.hpp"

std::string		ft_strtrim(const std::string &str, const std::string &set)
{
	size_t		begin = 0;
	size_t		end = str.length() - 1;

	while (str[begin] && set.find(str[begin]) != std::string::npos)
		++begin;
	while (end && set.find(str[end]) != std::string::npos)
		--end;
	return str.substr(begin, end - begin + 1);
<<<<<<< HEAD
}

int	checkEmptyMessage(Clients &src)
{
	for (int i = 0; i < src.getMessage().length(); ++i)
	{
		if (src.getMessage()[i] != '\n')
			return 1;
	}
	src.setMessage("");
	return 0;
}

int checkNicknameAlreadyUsed(std::vector<Clients> &clients, Clients &src)
{
	std::string input = src.getMessage();
	input = ft_strtrim(input, "\n");
	for (int i = 0; i < clients.size(); ++i)
	{
		if (clients[i].getNickname() == input)
			return 1;
	}
	return 0;
=======
>>>>>>> 9d2e5857ad47d7058adb45ec5d8872c20c779b14
}