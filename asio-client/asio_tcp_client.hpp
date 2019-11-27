#ifndef ASIO_TCP_CLIENT_HPP_
#define ASIO_TCP_CLIENT_HPP_
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
class asio_tcp_client
{
public:
	asio_tcp_client()
		: socket_(io_service_),
		deadline_(io_service_)
	{
		deadline_.expires_at(boost::posix_time::pos_infin);
		check_deadline();
	}
	virtual ~asio_tcp_client()
	{
		boost::system::error_code err;
		socket_.close(err);
		deadline_.cancel();
		io_service_.stop();
	}


	int connect(const std::string& host, const std::string& service, int timeout=0)
	{
		// Resolve the host name and service to a list of endpoints.
		boost::asio::ip::tcp::resolver::query query(host, service);
		boost::system::error_code err;
		boost::asio::ip::tcp::resolver::iterator iter =boost::asio::ip::tcp::resolver(io_service_).resolve(query,err);
		if (err)
		{
			return err.value();
		}
		boost::posix_time::time_duration td;
		if (timeout==0)
		{
			td = boost::date_time::pos_infin;
		}
		else
		{
			td = boost::posix_time::seconds(timeout);
		}
		deadline_.expires_from_now(td);
		boost::system::error_code ec = boost::asio::error::would_block;
		boost::asio::async_connect(socket_, iter, boost::bind(&asio_tcp_client::handle_connect,boost::asio::placeholders::error, &ec));
		do
		{
			io_service_.run_one(err);
		}while (ec == boost::asio::error::would_block);

		if (ec || !socket_.is_open())
		{
			return ec.value();
		}
		boost::asio::ip::tcp::endpoint et = socket_.local_endpoint(err);
		if (!err)
		{
			local_address_ = et.address().to_string(err);
		}
		return 0;
	}

	int connect(const char* host, int port, int timeout=0)
	{
		std::string service=boost::lexical_cast<std::string>(port);
		boost::algorithm::erase_all(service,",");
		return connect(host,service,timeout);
	}

	

	int abort()
	{
		return deadline_.expires_from_now(boost::posix_time::seconds(0));
	}

	std::string local_address()
	{
		return local_address_;
	}
	int read(char* data, const int len, int timeout=0)
	{
		boost::posix_time::time_duration td;
		if (timeout==0)
		{
			td = boost::date_time::pos_infin;
		}
		else
		{
			td = boost::posix_time::seconds(timeout);
		}
		deadline_.expires_from_now(td);
		boost::system::error_code ec = boost::asio::error::would_block;
		size_t length = 0;
		boost::asio::async_read(socket_, boost::asio::buffer(data,len),
			boost::bind(&asio_tcp_client::handle_write,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			&ec,
			&length));

		// Block until the asynchronous operation has completed.
		boost::system::error_code err;
		do
		{
			io_service_.run_one(err); 
		}
		while (ec == boost::asio::error::would_block);
		if (ec)
		{
			return ~ec.value();
		}
		return length;
	}
#ifdef USE_ASIO_PROTOCOL
	int read(asio_message &msg, int time_out=0)
	{
		int ret = read(msg.data(), asio_message::header_length, time_out);
		if (ret!=asio_message::header_length)
		{
			return 0;
		}
		msg.decode_header();
		if (msg.body_length()>0)
		{
			ret = read(msg.body(), msg.body_length(), 5);
			if (ret <= 0)
			{
				return 0;
			}
		}
		return ret;
	}
#endif
	
	int write(const char* data, const int len, int timeout=0)
	{
		boost::posix_time::time_duration td;
		if (timeout==0)
		{
			td = boost::date_time::pos_infin;
		}
		else
		{
			td = boost::posix_time::seconds(timeout);
		}
		deadline_.expires_from_now(td);
		boost::system::error_code ec = boost::asio::error::would_block;
		size_t length = 0;
		boost::asio::async_write(socket_, boost::asio::buffer(data,len), 
			boost::bind(&asio_tcp_client::handle_write,boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			&ec,
			&length));
		boost::system::error_code err;
		do io_service_.run_one(err); while (ec == boost::asio::error::would_block);
		if (ec)
		{
			return ~ec.value();
		}
		return length;
	}
#ifdef USE_ASIO_PROTOCOL
	int write(const asio_message& msg, int timeout=0)
	{
		return write(msg.data(), msg.length(), timeout);
	}
#endif

private:
	void check_deadline()
	{
		if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
		{
			boost::system::error_code ignored_ec;
			socket_.close(ignored_ec);
			deadline_.expires_at(boost::posix_time::pos_infin);
		}
		deadline_.async_wait(boost::bind(&asio_tcp_client::check_deadline, this));
	}

	static void handle_connect(const boost::system::error_code& ec, boost::system::error_code* out_ec)
	{
		if(ec)
		{
			std::cout << "connect failed: " << ec.message() << "\n";
		}
		*out_ec = ec;
	}

	static void handle_write(const boost::system::error_code& ec, std::size_t length,
		boost::system::error_code* out_ec, std::size_t* out_length)
	{
		*out_ec = ec;
		*out_length = length;
	}

	static void handle_read(	const boost::system::error_code& ec, std::size_t length,
		boost::system::error_code* out_ec, std::size_t* out_length)
	{
		*out_ec = ec;
		*out_length = length;
	}
private:
	asio_tcp_client(const asio_tcp_client&);
	void operator=(const asio_tcp_client&);

private:
	boost::asio::io_service io_service_;
	boost::asio::deadline_timer deadline_;
	boost::asio::ip::tcp::socket socket_;
	std::string local_address_;
};

#endif

