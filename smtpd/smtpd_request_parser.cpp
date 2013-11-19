#include "smtpd_request_parser.h"
#include "smtpd_request.h"

namespace smtp
{

request_parser::request_parser()
	: state_(command_start)
{
}

request_parser::~request_parser()
{
}


boost::tribool request_parser::consume(request& req, const char input)
{
	switch(state_)
	{
	case command_start:
		if (!is_char(input) || is_ctl(input) || is_tspecial(input))
		{
			return false;
		}
		else
		{
			state_ = command;
			req.command.push_back(input);
			return boost::indeterminate;
		}
	case command:
		if (':' == input)
		{
			state_ = argument;
			return boost::indeterminate;
		}
		else if ('\r' == input)
		{
			state_ = newline_1;
			return boost::indeterminate;
		}
		else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
		{
			return false;
		}
		else
		{
			req.command.push_back(input);
			return boost::indeterminate;
		}
	case argument:
		if ('\r' == input)
		{
			state_ = newline_1;
			return boost::indeterminate;
		}
		else if (!is_char(input))
		{
			return false;
		}
		else
		{
			req.argument.push_back(input);
			return boost::indeterminate;
		}
	case newline_1:
		if ('\n' == input)
		{
			state_ = command_start;
			return true;
		}
		else
		{
			return false;
		}
	}
}

void request_parser::reset()
{
	state_ = command_start;
}

bool request_parser::is_char(int c)
{
	return c >= 0 && c <= 127;
}

bool request_parser::is_ctl(int c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool request_parser::is_tspecial(int c)
{
	switch (c)
	{
	case '(': case ')': case '<': case '>': case '@':
	case ',': case ';': case ':': case '\\': case '"':
	case '/': case '[': case ']': case '?': case '=':
	case '{': case '}': case '\t':
		return true;
	default:
		return false;
	}
}

bool request_parser::is_digit(int c)
{
	return c >= '0' && c <= '9';
}

} // namespace smtp
