#include "c.h"

void Test::testlog()
{
    	EasyLog::Inst()->Log("[%s],test Log works in other file",__FUNCTION__);
}

Test::Test(){

}
   
Test::~Test(){

}
