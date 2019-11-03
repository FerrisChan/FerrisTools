// Hash.cpp: implementation of the Hash class.
//
//////////////////////////////////////////////////////////////////////
#include "stdinc.h"
#include "hash.h"
using std::logic_error;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define INVALID_STATE throw logic_error("Invalid state")
Hash::~Hash(){
	delete[]m_hash;
}
void Hash::reset()
{
	status=INIT;
	doReset();
	delete[] m_hash;
	m_hash=NULL;
}
void Hash::add(const BYTE* byte,size_t size)throw(logic_error)
{
	if(status==END)
		INVALID_STATE;
	status=HASHING;
	doAdd(byte,size);
}
const BYTE* Hash::getHash()const throw(logic_error)
{
	if(status==INIT)
		INVALID_STATE;
	status=Hash::END;
	if(m_hash)
		return m_hash;
	else
	{
		m_hash=new BYTE[m_hashsize];
		return doGetHash();
	}
}

Hash::Hash(const std::string& name,size_t hashsize):m_hashsize(hashsize),m_name(name),m_hash(NULL),status(INIT)
{
}
