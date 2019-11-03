// Hash.h: interface for the Hash class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "stdinc.h"
class Hash  
{
public:
	virtual ~Hash()=0;
	void reset();
	void add(const BYTE* byte,size_t size)throw(std::logic_error);
	const BYTE* getHash()const throw(std::logic_error);
	const std::string& getName()const{return m_name;}
	bool operator==(const Hash& other)const
	{
		return (m_name==other.m_name);
	}
	bool operator!=(const Hash& other)const
	{
		return !(*this==other);
	}
	size_t getHashSize()const
	{
		return m_hashsize;
	}
protected:
	Hash(const std::string& name,size_t hashsize);
	virtual void doAdd(const BYTE* byte,size_t size)throw(std::logic_error)=0;
	virtual const BYTE* doGetHash()const throw(std::logic_error)=0;
	virtual void doReset()=0;
	mutable BYTE* m_hash;
	size_t m_hashsize;
	std::string m_name;
	mutable enum Status{INIT,HASHING,END} status;
};


