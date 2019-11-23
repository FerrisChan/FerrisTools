// SIMAuth.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "CurlUtil.hpp"

int main(int argc, char* argv[]){
	std::string strSendMsg = "hello world";
	CurlUtil::CurlRespMsg res;
	printf("send data is [%s]\n", strSendMsg.c_str());
	res = CurlUtil::CurlSend("localhost:8888", "", strSendMsg);
	// printf("send result is [%s]\n", res.errMsg.c_str());
	printf("recive data is [%s]\n", res.strResp.c_str());
}
