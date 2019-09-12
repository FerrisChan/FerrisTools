#include "stdio.h"
#include "Log.h"
#include <string>
// #include "test.hpp"
// #include "c.h"
//  g++ test.hpp main.cpp Log.cpp c.cpp -o main
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
	// testLog();
	// Test c;
	// c.testlog();	
}
