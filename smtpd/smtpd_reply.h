#pragma once

#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace smtp
{

struct reply
{
	enum status_type
	{
		banner = 220,
		ok = 250,
		general_error = 550
	} status;

	std::string status_str;

	std::vector<std::string> lines;

	std::vector<boost::asio::const_buffer> to_buffers();

	/// Get a stock reply.
	static reply stock_reply(status_type status);
};

} // namespace smtp
