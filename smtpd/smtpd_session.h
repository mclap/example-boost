#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "smtpd_request_parser.h"
#include "smtpd_request_handler.h"
#include "smtpd_request.h"
#include "smtpd_reply.h"

namespace smtp
{

class session : public boost::enable_shared_from_this<session>, private boost::noncopyable
{
public:
	session(boost::asio::io_service& io, request_handler& handler);

	/// Get the socket associated with the connection.
	boost::asio::ip::tcp::socket& socket();

	/// Begin I/O operations
	void start();

	/// Stop I/O operations
	void stop();

private:
	/// Basic I/O object
	boost::asio::io_service& io_;

	/// SMTP Request parser
	request_parser parser_;

	/// SMTP Request handler
	request_handler& handler_;

	/// Buffer for incoming data.
	boost::array<char, 8192> buffer_;

	/// SMTP request itself
	request request_;

	reply reply_;

	/// Socket for the connection.
	boost::asio::ip::tcp::socket socket_;

	void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);
	void handle_write(const boost::system::error_code& e);

	void start_read();
};

typedef boost::shared_ptr<session> session_ptr;

} // namespace smtp
