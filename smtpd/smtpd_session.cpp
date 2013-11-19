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

	reply_ = reply::stock_reply(reply::banner);

	async_write_buffers(reply_.to_buffers());
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
		<< "READ [" << socket_.remote_endpoint(t) << ']'
		<< " received bytes: " << bytes_transferred
		<< ", error_code: " << e
		<< std::endl;

	if (!e)
	{

		boost::tribool result;
		boost::tie(result, boost::tuples::ignore) =
			parser_.parse(
			request_, buffer_.data(),
			buffer_.data() + bytes_transferred);

		if (result)
		{
			std::cout
				<< "command:["
				<<request_.command
				<< "], argument:["
				<< request_.argument
				<< "]" << std::endl;
			/*
			request_handler_.handle_request(request_, reply_);
			*/

			reply_ = reply::stock_reply(reply::ok);

			async_write_buffers(reply_.to_buffers());
			request_.clear();
		}
		else if (!result)
		{
			std::cout
				<< "ERROR in input"
				<< std::endl;
		}
		else
		{
			start_read();
		}
	}
	else
	{
		stop();
	}
}

void session::handle_write(const boost::system::error_code& e)
{
	boost::system::error_code t;
	std::cout
		<< "WRITE [" << socket_.remote_endpoint(t) << ']'
		<< ", error_code: " << e
		<< std::endl;

	if (!e)
	{
		start_read();
	}
}

void session::stop()
{
	std::cout << "STOP" << std::endl;
	socket_.close();
}

void session::async_write_buffers(const std::vector<boost::asio::const_buffer>& buffers)
{ 
	boost::asio::async_write(socket_, buffers,
		boost::bind(&session::handle_write,
			shared_from_this(),
			boost::asio::placeholders::error));
}

} // namespace smtp
