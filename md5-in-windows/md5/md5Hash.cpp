// md5Hash.cpp: implementation of the md5Hash class.
//
//////////////////////////////////////////////////////////////////////
#include "stdinc.h"
#include "hash.h"
#include "md5.h"
#include "md5Hash.h"
using std::logic_error;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

md5Hash::md5Hash():Hash("MD5",16)
{
	doReset();
}

md5Hash::~md5Hash()
{
}

void md5Hash::doAdd(const BYTE* byte,size_t size)throw(logic_error)
{
	MD5Update(&m_context,byte,size);
}
const BYTE* md5Hash::doGetHash()const throw(logic_error)
{
	MD5Final(m_hash,&m_context);
	return m_hash;
}
void md5Hash::doReset()
{
	MD5Init(&m_context);
}

