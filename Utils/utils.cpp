#include "../Clients/Clients.hpp"

std::string		ft_strtrim(const std::string &str, const std::string &set)
{
	size_t		begin = 0;
	size_t		end = str.length() - 1;

	while (str[begin] && set.find(str[begin]) != std::string::npos)
		++begin;
	while (end && set.find(str[end]) != std::string::npos)
		--end;
	return str.substr(begin, end - begin + 1);
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

bool checkNicknameAlreadyUsed(std::vector<Clients> &clients, string input)
{
	input = ft_strtrim(input, "\n");
	for (int i = 0; i < clients.size(); ++i)
	{
		if (clients[i].getNickname() == input)
			return true;
	}
	return false;
}

void	sendMessageToClient(Clients &src, string output)
{
	send(src.getFd(), output.c_str(), output.length(), 0);
}

std::vector<std::string>	ft_split(std::string str, const std::string &del)
{
	std::vector<std::string>	splitStr;
	size_t						pos;

	if (!str.empty())
	{
		if (del.empty())
			splitStr.push_back(str);
		else
		{
			str = ft_strtrim(str, del);
			if (!str.empty())
			{
				for ( pos = str.find(del) ; pos != std::string::npos ; pos = str.find(del) )
				{
					if (str.substr(0, pos) != "")
						splitStr.push_back(str.substr(0, pos));
					str = str.substr(pos + del.length(), str.length() - pos);
				}
				splitStr.push_back(str.substr(0, pos));
			}
		}
	}
	return splitStr;
}