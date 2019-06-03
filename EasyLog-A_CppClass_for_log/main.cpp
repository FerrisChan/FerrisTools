#include "stdio.h"
#include "Log.hpp"
#include <string>
int main(int argc, char* argv[])
{
	EasyLog::Inst()->Log("Run...");
	EasyLog::Inst()->Log("this %d", 7);
	EasyLog::Inst()->Log("string %s", "hello");
	std::string s = "中文";
	EasyLog::Inst()->Log("Run...");
	EasyLog::Inst()->Log("this %d", 7);
	EasyLog::Inst()->Log("string %s", "hello");
	EasyLog::Inst()->Log("3333");
	EasyLog::Inst()->Log("this %d", 7);
	EasyLog::Inst()->Log("string %s", s.c_str());
	EasyLog::Inst()->Log("Run...");
	EasyLog::Inst()->Log("this %d", 7);
	EasyLog::Inst()->Log("string %s %d %s", "hello",7,"我房子可以嗎");	
}
