#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <iostream>

using boost::asio::deadline_timer;
using boost::asio::ip::udp;


class asio_udp_client
{
public:
	asio_udp_client(const udp::endpoint& listen_endpoint)
		: socket_(io_service_, udp::endpoint(udp::v4(), 0)),
		deadline_(io_service_)
	{
		
		deadline_.expires_at(boost::posix_time::pos_infin);

		check_deadline();
	}
	
	void send_to(std::string strNotice, boost::asio::ip::udp::endpoint end_point,
		boost::system::error_code& ec)
	{
		boost::shared_array<char> spData(new char[12+strNotice.length()+1]);
		memset(spData.get(),0,12+strNotice.length()+1);
		memcpy(spData.get(),"WBCC",4);
		*((unsigned int*)(spData.get()+4)) = 0;
		*((unsigned int*)(spData.get()+8)) = strNotice.length();
		memcpy(spData.get()+12,strNotice.c_str(),strNotice.length());
		socket_.send_to(boost::asio::buffer(spData.get(),12+strNotice.length()+1), end_point,0,ec);
	}

	void send_to(const char* szbuf, size_t len, boost::asio::ip::udp::endpoint end_point,
		boost::system::error_code& ec)
	{
		socket_.send_to(boost::asio::buffer(szbuf, len), end_point,0,ec);
	}
	
	std::size_t receive(const boost::asio::mutable_buffer& buffer,
		boost::posix_time::time_duration timeout, boost::system::error_code& ec)
	{
		deadline_.expires_from_now(timeout);

		ec = boost::asio::error::would_block;
		std::size_t length = 0;

		socket_.async_receive(boost::asio::buffer(buffer),
			boost::bind(&asio_udp_client::handle_receive, _1, _2, &ec, &length));

		do io_service_.run_one(); while (ec == boost::asio::error::would_block);

		return length;
	}

private:
	void check_deadline()
	{
		
		if (deadline_.expires_at() <= deadline_timer::traits_type::now())
		{

			socket_.cancel();

			deadline_.expires_at(boost::posix_time::pos_infin);
		}

		// Put the actor back to sleep.
		deadline_.async_wait(boost::bind(&asio_udp_client::check_deadline, this));
	}

	static void handle_receive(
		const boost::system::error_code& ec, std::size_t length,
		boost::system::error_code* out_ec, std::size_t* out_length)
	{
		*out_ec = ec;
		*out_length = length;
	}

private:
	boost::asio::io_service io_service_;
	udp::socket socket_;
	deadline_timer deadline_;
};