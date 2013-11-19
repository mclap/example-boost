#pragma once

#include <string>

namespace smtp
{

struct request
{
	std::string command;
	std::string argument;

	void clear()
	{
		command.clear();
		argument.clear();
	}
};

} // namespace smtp
