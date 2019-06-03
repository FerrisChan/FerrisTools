/* 
说明:(EasyLog.h) 
1, 简单的单件实现（自动垃圾回收） 
2, 使用方法：EasyLog::Inst()->Log("Run..."); 
*/  
#ifndef EASY_LOG_H_8080  
#define EASY_LOG_H_8080  
#include <memory>  
#include <ctime>  
#include <iostream>  
#include <fstream>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
class EasyLog  
{  
public:  
    static EasyLog * Inst(){  
        if (0 == _instance.get()){  
            _instance.reset(new EasyLog);  
        }  
        return _instance.get();  
    }  
  
	void Log(char* loginfo, ...); // 写日志的方法  
private:  
    EasyLog(void){}  
    virtual ~EasyLog(void){}  
    friend class std::auto_ptr<EasyLog>;  
    static std::auto_ptr<EasyLog> _instance;  
};  
  
std::auto_ptr<EasyLog> EasyLog::_instance;  
  
void EasyLog::Log(char* loginfo, ...) {
	char msgLog[1024];
	va_list args;
	va_start(args, loginfo);
	vsnprintf(msgLog, 1024, loginfo, args);
	va_end(args);
    std::ofstream ofs;  
    time_t t = time(0);  
    char strTime[64];  
	strftime(strTime, sizeof(strTime), "[%Y.%m.%d %X] ", localtime(&t));
    ofs.open("EasyLog.log", std::ofstream::app);  
	ofs << strTime << msgLog <<'\n';
	// ofs.write(msgLog, strlen(msgLog));
    
    ofs.close();  
}  
#endif


// 用法

// #include "EasyLog.h"  
  
// #define EASYLOG 1  
  
// int main(){  
// #if EASYLOG  
//     EasyLog::Inst()->Log("Run...");  
// #endif  
// }  