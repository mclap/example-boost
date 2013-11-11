#include "smtpd_session.h"

namespace smtp
{

session::session(boost::asio::io_service& io, request_handler& handler)
	: io_(io)
	, parser_()
	, handler_(handler)
	, buffer_()
	, socket_(io_)
{
}

boost::asio::ip::tcp::socket& session::socket()
{
	return socket_;
}

} // namespace smtp
