#pragma once
#include <boost/asio.hpp>

#include "smtpd_session.h"
#include "smtpd_request_handler.h"

namespace smtp
{

class server : private boost::noncopyable
{
public:
	enum
	{
		DefaultPort = 34567
	};
	server(int port = DefaultPort);
	~server();

	void run();

private:
	boost::asio::io_service io_;
	boost::asio::ip::tcp::acceptor acceptor_;
	session_ptr new_session_;
	request_handler handler_;

	void start_accept();
	void handle_accept(const boost::system::error_code& e);
};

} // namespace smtp
