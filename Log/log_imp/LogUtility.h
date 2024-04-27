#pragma once
#ifndef __PXLPLATFORM_H__jkafuopklsadf98234__
#define __PXLPLATFORM_H__jkafuopklsadf98234__



#include <stdio.h>
#include <time.h>
#include <string>
#include <string.h>
#include <process.h>
#include <Windows.h> 
#include "../SafeHelper.h"
class CLock
{
private:
	CRITICAL_SECTION m_sect;
public:
	CLock()
	{
		InitializeCriticalSection(&m_sect);
	}
	~CLock()
	{
		DeleteCriticalSection(&m_sect);
	}
	void Lock()
	{
		EnterCriticalSection(&m_sect);
	}
	void Unlock()
	{
		LeaveCriticalSection(&m_sect);
	}
};

typedef CLock CRecursiveLock;


class CFakeLock
{
public:
	void Lock() {}
	void Unlock() {}
};


template<class T>
class CScopedLock
{
public:
	CScopedLock(T* l) :m_lock(l) { m_lock->Lock(); }
	~CScopedLock() { m_lock->Unlock(); }
private:
	T* m_lock;
};


template<class ListT>
class ThreadSafeList
{
	typedef CLock LockType;
	mutable LockType m_lock;

	ListT m_list;
	typedef typename ListT::value_type ElemT;
	size_t m_size_cache;
public:
	ThreadSafeList()
		: m_size_cache(0)
	{

	}
	void lock()
	{
		m_lock.Lock();
	}
	void unlock()
	{
		m_lock.Unlock();
	}
	void pushBack(const ElemT& elem)
	{
		CScopedLock<LockType> sl(&m_lock);
		m_list.push_back(elem);
		++m_size_cache;
	}
	void pushFront(const ElemT& elem)
	{
		CScopedLock<LockType> sl(&m_lock);
		m_list.push_front(elem);
		++m_size_cache;
	}
	void spliceFrom(ListT& from)
	{
		CScopedLock<LockType> sl(&m_lock);
		m_list.splice(m_list.end(), from);
		m_size_cache = m_list.size();
	}
	void spliceTo(ListT& to)
	{
		CScopedLock<LockType> sl(&m_lock);
		to.splice(to.end(), m_list);
		m_size_cache = 0;
	}
	ElemT front()const
	{
		CScopedLock<LockType> sl(&m_lock);
		return m_list.front();
	}
	ElemT back()const
	{
		CScopedLock<LockType> sl(&m_lock);
		return m_list.back();
	}
	void popFront()
	{
		CScopedLock<LockType> sl(&m_lock);
		m_list.pop_front();
		--m_size_cache;
	}
	void popBack()
	{
		CScopedLock<LockType> sl(&m_lock);
		m_list.pop_back();
		--m_size_cache;
	}
	size_t size()
	{
		CScopedLock<LockType> sl(&m_lock);
		return m_list.size();
	}
	size_t sizeCache()
	{
		CScopedLock<LockType> sl(&m_lock);
		return m_size_cache;
	}
	bool empty()
	{
		CScopedLock<LockType> sl(&m_lock);
		return m_list.empty();
	}
	void clear()
	{
		CScopedLock<LockType> sl(&m_lock);
		m_list.clear();
		m_size_cache = 0;
	}
};


///将int转换成char型
inline std::string ItoA(int i)
{
	char buff[32];
	safe_sprintf(buff, "%d", i);
	return buff;
}

inline std::string ItoA(int64 i)
{
	char buf[64] = {};
	safe_sprintf(buf, "%lld", i);
	return buf;
}

inline uint64 get_file_time(const char* name)
{
	uint64 res=0;
	HANDLE hFile;
	
	hFile = CreateFileA(name,               // open MYFILE.TXT 
				GENERIC_READ,              // open for reading 
				FILE_SHARE_READ,           // share for reading 
				NULL,                      // no security 
				OPEN_EXISTING,             // existing file only 
				FILE_ATTRIBUTE_NORMAL,     // normal file 
				NULL);                     // no attr. template 
	if (hFile != INVALID_HANDLE_VALUE) 
	{ 
		FILETIME ftCreate, ftAccess, ftWrite;

		// Retrieve the file times for the file.
		if(GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		{
			res=ftWrite.dwHighDateTime;
			res<<=32;
			res|=ftWrite.dwLowDateTime;
			res=(res-116444736000000000)/10000000;
		}
		CloseHandle(hFile);
	}
	return res;
}

inline std::string time_str_2(time_t t)
{
	tm local = safe_localtime(t);

	char ti[1024];

	safe_sprintf(ti, "%d-%d-%d.%d_%d_%d",
		local.tm_year + 1900, local.tm_mon + 1, local.tm_mday,
		local.tm_hour, local.tm_min, local.tm_sec);

	return ti;
}

inline std::string time_str_2()
{
	return time_str_2(time(0));
}

//为了编译而定义
#ifdef _DEBUG
#define lightAssert(exp)		(int)( (exp) || (PAF_ASSERT(exp),0))
#else
#define lightAssert(exp)		((int)1)
#endif

#endif
