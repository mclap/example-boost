#include "smtpd_reply.h"

#include <boost/lexical_cast.hpp>

namespace smtp
{

namespace misc
{

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };
const char reply_sep_inter[] = { '-' };
const char reply_sep_final[] = { ' ' };

const std::string ok = "OK";
const std::string general_error = "General error";
const std::string unidentified_failure = "Unidentified failure";

boost::asio::const_buffer to_buffer(const reply::status_type type)
{
	switch(type)
	{
	case reply::ok: return boost::asio::buffer(ok);
	case reply::general_error: return boost::asio::buffer(general_error);
	}

	return boost::asio::buffer(unidentified_failure);
};

} // namespace smtp::misc

std::vector<boost::asio::const_buffer> reply::to_buffers()
{
	std::vector<boost::asio::const_buffer> buffers;
	status_str = boost::lexical_cast<std::string>(status);

	buffers.push_back(boost::asio::buffer(status_str));
	buffers.push_back(boost::asio::buffer(lines.empty()
		? misc::reply_sep_final
		: misc::reply_sep_inter
		));
	buffers.push_back(misc::to_buffer(status));
	buffers.push_back(boost::asio::buffer(misc::crlf));
	for (std::size_t i = 0; i < lines.size(); ++i)
	{
		buffers.push_back(boost::asio::buffer(status_str));

		buffers.push_back(boost::asio::buffer( (i == lines.size()-1)
			? misc::reply_sep_final
			: misc::reply_sep_inter
			));

		buffers.push_back(boost::asio::buffer(lines[i]));
		buffers.push_back(boost::asio::buffer(misc::crlf));
	}

	return buffers;
}

reply reply::stock_reply(status_type status)
{
	reply r;
	r.status = status;
	return r;
}

} // namespace smtp
