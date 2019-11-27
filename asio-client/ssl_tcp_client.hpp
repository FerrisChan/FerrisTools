#ifndef SSL_TCP_CLIENT_HPP_
#define SSL_TCP_CLIENT_HPP_
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
class ssl_tcp_client
{
public:
	ssl_tcp_client()
	{
		io_service_=NULL;
		context_=NULL;
		deadline_=NULL;
		socket_=NULL;
	}
	virtual ~ssl_tcp_client()
	{
		try
		{
			boost::system::error_code err;
			if (socket_)
			{
				socket_->lowest_layer().close(err);
				delete socket_;
				socket_ = NULL;
			}
			if (deadline_)
			{
				deadline_->cancel();
				delete deadline_;
				deadline_ = NULL;
			}
			if (io_service_)
			{
				io_service_->stop();
				delete io_service_;
				io_service_ = NULL;
			}
			if (context_)
			{
				delete context_;
				context_ = NULL;
			}
		}
		catch (...)
		{ }
	}
	

	int connect(const char* host, const char* service, int timeout=0)
	{
		if (init()!=0)
		{
			return -1;
		}
		// Resolve the host name and service to a list of endpoints.
		boost::asio::ip::tcp::resolver::query query(host, service);
		boost::system::error_code err;
		boost::asio::ip::tcp::resolver::iterator iter =boost::asio::ip::tcp::resolver(*io_service_).resolve(query,err);
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
		deadline_->expires_from_now(td);
		boost::system::error_code ec = boost::asio::error::would_block;
		boost::asio::async_connect(socket_->lowest_layer(), iter, boost::bind(&ssl_tcp_client::handle_connect,boost::asio::placeholders::error, &ec));
		do
		{
			io_service_->run_one(err);
			if(err)  break;
		}while (ec == boost::asio::error::would_block);

		if (ec || !socket_->lowest_layer().is_open())
		{
			return ec.value();
		}
		boost::asio::ip::tcp::endpoint et = socket_->lowest_layer().local_endpoint(err);
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
		return connect(host,service.c_str(),timeout);
	}

	int hand_shake(int timeout = 0)
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
		deadline_->expires_from_now(td);
		boost::system::error_code ec = boost::asio::error::would_block;
		socket_->async_handshake(boost::asio::ssl::stream_base::client,
			boost::bind(&ssl_tcp_client::handle_handshake,
			boost::asio::placeholders::error,&ec));
		boost::system::error_code err;
		do
		{
			io_service_->run_one(err); 
			if(err)  break;
		}while (ec == boost::asio::error::would_block);

		if (ec || !socket_->lowest_layer().is_open())
		{
			return -1;
		}
		return 0;
	}

	int abort()
	{
		return deadline_->expires_from_now(boost::posix_time::seconds(0));
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
		deadline_->expires_from_now(td);
		boost::system::error_code ec = boost::asio::error::would_block;
		size_t length = 0;
		boost::asio::async_read(*socket_, boost::asio::buffer(data,len),
			boost::bind(&ssl_tcp_client::handle_read,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			&ec,
			&length));

		// Block until the asynchronous operation has completed.
		boost::system::error_code err;
		do
		{
			io_service_->run_one(err); 
			if(err)  break;
		}
		while (ec == boost::asio::error::would_block);
		if (ec&&length==0)
		{
			return ~ec.value();
		}
		return length;
	}

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
		deadline_->expires_from_now(td);
		boost::system::error_code ec = boost::asio::error::would_block;
		size_t length = 0;
		boost::asio::async_write(*socket_, boost::asio::buffer(data,len), 
			boost::bind(&ssl_tcp_client::handle_write,boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			&ec,
			&length));
		boost::system::error_code err;
		do
		{
			io_service_->run_one(err);
			if(err)  break;
		}while (ec == boost::asio::error::would_block);
		if (ec)
		{
			return ~ec.value();
		}
		return (int)length;
	}

private:
	int	init()
	{
		int ret=0;
		if (socket_)  return 0;
		try
		{
			context_=new boost::asio::ssl::context(boost::asio::ssl::context::sslv23);
			io_service_=new boost::asio::io_service();
			deadline_=new boost::asio::deadline_timer(*io_service_);
			socket_=new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(*io_service_, *context_);
			socket_->set_verify_mode(boost::asio::ssl::verify_none);
			socket_->set_verify_callback(
				boost::bind(&ssl_tcp_client::verify_certificate, this, _1, _2));
			deadline_->expires_at(boost::posix_time::pos_infin);
			check_deadline();
		}
		catch (...)
		{
			ret=5;
		}
		return ret;
	}
	void check_deadline()
	{
		if (deadline_->expires_at() <= boost::asio::deadline_timer::traits_type::now())
		{
			boost::system::error_code ignored_ec;
			socket_->lowest_layer().close(ignored_ec);
			deadline_->expires_at(boost::posix_time::pos_infin);
		}
		deadline_->async_wait(boost::bind(&ssl_tcp_client::check_deadline, this));
	}

	bool verify_certificate(bool preverified,
		boost::asio::ssl::verify_context& ctx)
	{
		char subject_name[256];
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

		//std::cout << "Verifying " << subject_name << "\n";
		return preverified;
	}

	static void handle_connect(const boost::system::error_code& ec, boost::system::error_code* out_ec)
	{
		//if(ec)
		//{
		//	std::cout << "connect failed: " << ec.message() << "\n";
		//}
		*out_ec = ec;
	}
	static void handle_handshake(const boost::system::error_code& ec, boost::system::error_code* out_ec)
	{
		//if(ec)
		//{
		//	//cout << "Handshake failed: " << ec.message() << "\n";
		//}
		//else
		//{
		//	//cout<<"handshake succeed"<<endl;
		//}
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
	ssl_tcp_client(const ssl_tcp_client&);
	void operator=(const ssl_tcp_client&);

private:
	boost::asio::io_service* io_service_;
	boost::asio::ssl::context* context_;
	boost::asio::deadline_timer* deadline_;
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* socket_;
	std::string local_address_;
};

#endif

