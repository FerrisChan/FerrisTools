#include "stdafx.h"
#include <iostream>
#include <string>
#include "asio_tcp_client.hpp"

int main(int argc, char* argv[]){
	asio_tcp_client tcp_client;
	int iErr = tcp_client.connect("127.0.0.1",8888, 5);
	if (iErr !=0)
	{
		printf("connect 127.0.0.1 Failed!Err=%d\n", iErr);
		return -1;
	}

	std::string xml_response;
	xml_response = "test asio_tcp_client ";

	int sslret = tcp_client.write(xml_response.c_str(), xml_response.length,5);   // timeout = 5s
	if (sslret < 0)
	{
		printf("send restart request to 127.0.0.1 Failed!Err=%d\n", iErr);
		return -1;
	}
	printf(" asio_tcp_client send success");
	return 0;
}
