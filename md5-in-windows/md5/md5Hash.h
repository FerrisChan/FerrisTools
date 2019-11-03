// md5Hash.h: interface for the md5Hash class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "hash.h"
#include "md5.h"
class md5Hash : public Hash  
{
public:
	md5Hash();
	virtual ~md5Hash();
protected:
	virtual void doAdd(const BYTE* byte,size_t size)throw(std::logic_error);
	virtual const BYTE* doGetHash()const throw(std::logic_error);
	virtual void doReset();
private:
	MD5Context m_context;
};


