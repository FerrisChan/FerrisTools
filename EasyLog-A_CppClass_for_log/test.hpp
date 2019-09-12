#include "log.h"

int testLog(){
    	EasyLog::Inst()->Log("[%s],test Log works in other file",__FUNCTION__);
}