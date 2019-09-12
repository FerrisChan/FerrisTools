#include "Log.h"

 
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

std::auto_ptr<EasyLog> EasyLog::_instance;  