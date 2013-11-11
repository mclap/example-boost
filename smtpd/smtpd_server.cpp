#include "smtpd_server.h"

#include <boost/bind.hpp>

namespace smtp
{

server::server(int port)
	: io_()
	, acceptor_(io_, boost::asio::ip::tcp::endpoint(
		boost::asio::ip::tcp::v4(), port))
{
	start_accept();
}

server::~server()
{
}


void server::start_accept()
{
	new_session_.reset(new session(
		io_, handler_));
	acceptor_.async_accept(new_session_->socket(),
		boost::bind(&server::handle_accept, this,
		boost::asio::placeholders::error));
}

void server::handle_accept(const boost::system::error_code& e)
{
	boost::system::error_code t;
	std::cout
		<< "new connection: "
		<< new_session_->socket().remote_endpoint(t)
		<< std::endl;
	start_accept();
}

void server::run()
{
	io_.run();
}

} // namespace smtp
