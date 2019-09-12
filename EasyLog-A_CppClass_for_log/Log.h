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

#endif


// 用法

// #include "Log.h"  
  
// #define EASYLOG 1  
  
// int main(){  
// #if EASYLOG  
//     EasyLog::Inst()->Log("Run...");  
// #endif  
// }  