#include "smtpd_session.h"

#include <boost/bind.hpp>

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

void session::start()
{
	boost::system::error_code t;
	std::cout
		<< "new connection: "
		<< socket_.remote_endpoint(t)
		<< std::endl;

	start_read();
}

void session::start_read()
{
	socket_.async_read_some(boost::asio::buffer(buffer_),
		boost::bind(&session::handle_read, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	boost::system::error_code t;
	std::cout
		<< '[' << socket_.remote_endpoint(t) << ']'
		<< " received bytes: " << bytes_transferred
		<< ", error_code: " << e
		<< std::endl;

	start_read();
}

} // namespace smtp
