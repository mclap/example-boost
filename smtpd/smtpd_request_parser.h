#pragma once

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/noncopyable.hpp>

/*

buffer -> command [+ argument]
	| message body chunk

*/

namespace smtp
{

struct request;

class request_parser : private boost::noncopyable
{
public:
	request_parser();
	~request_parser();

	/// Parse some data. The tribool return value is true when a complete
	/// request has been parsed, false if the data is invalid,
	/// indeterminate when more data is required. The InputIterator
	/// return value indicates how much of the input has been consumed.
	template <typename InputIterator>
	boost::tuple<boost::tribool, InputIterator> parse(request& req,
		InputIterator begin, InputIterator end)
	{
		while (begin != end)
		{
			boost::tribool result = consume(req, *begin++);
			if (result || !result)
				return boost::make_tuple(result, begin);
		}
		boost::tribool result = boost::indeterminate;
		return boost::make_tuple(result, begin);
	}

	void reset();

private:
	enum state {
		  command_start
		, command
		, argument
		, newline_1
	};

	state state_;

	boost::tribool consume(request& req, const char input);

	static bool is_char(int c);

	static bool is_ctl(int c);

	static bool is_tspecial(int c);

	static bool is_digit(int c);
};

} // namespace smtp

