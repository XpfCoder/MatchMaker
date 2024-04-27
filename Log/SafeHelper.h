#ifndef __SAFE_HELPER_H_6885el81ksaao__
#define __SAFE_HELPER_H_6885el81ksaao__

#include "LogTypeDef.h"

//#if !defined(_TRUNCATE)
//#define _TRUNCATE ((size_t)-1)
//#endif

#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef WIN32
#include <wtypes.h>
#else
#include <stdarg.h>
#endif

#if _MSC_VER <= 1310 //VS.Net 2003
typedef int errno_t;
#endif

//---------------------------------------------------------------
inline char* safe_strcpy(char* dest, size_t L, const char* src, size_t len=-1)
{
	if(src==0)
		src="";

#if !defined(_MSC_VER) || _MSC_VER <= 1310 //VS.Net 2003 和 linux
	strncpy_s(dest, src, L);
	dest[L-1] = 0;//防止这个字符串没有终结
#else
	strncpy_s(dest, L, src, len);
#endif
	return dest;
}

template<int L>
inline char* safe_strcpy(char (&dest)[L], const char* src, size_t len=-1)
{
	return safe_strcpy(dest, L, src, len);
}

//---------------------------------------------------------------
inline char* safe_strcat(char* dest, size_t L, const char* src)
{
	if(src==0)
		src="";
#if !defined(_MSC_VER) || _MSC_VER <= 1310 //VS.Net 2003 和 linux
	strcat(dest, src);
#else
	strcat_s(dest, L, src);
#endif
	dest[L-1] = 0;//防止这个字符串没有终结
	return dest;
}

template<int L>
inline char* safe_strcat(char (&dest)[L], const char* src)
{
	return safe_strcat(dest, L, src);
}

//---------------------------------------------------------------
inline int safe_sprintf(char *buffer, size_t count, const char *format, ...)
{
	if (count == 0)
	{
		return -1;
	}
	va_list argptr;
	va_start (argptr, format);

#if defined(_MSC_VER)
#if _MSC_VER <= 1310 //VS.Net 2003
	int res = _vsnprintf(buffer, count, format, argptr);
	if(res>=count)
		res=-1;
#else
	int res =_vsnprintf_s(buffer, count, _TRUNCATE, format, argptr);
#endif
#else  // linux
	int res = vsnprintf(buffer, count, format, argptr);
	if(res>=count)
		res=-1;
#endif

	va_end (argptr);
	return res;
}

template<int L>
inline int safe_sprintf( char (&buffer)[L], const char *format, ...)
{
	if (L == 0)
	{
		return -1;
	}
	va_list argptr;
	va_start (argptr, format);

#if defined(_MSC_VER)
#if _MSC_VER <= 1310 //VS.Net 2003
	int res = _vsnprintf(buffer, L, format, argptr);
	if(res>=L)
		res=-1;
#else
	int res =_vsnprintf_s(buffer, L, _TRUNCATE, format, argptr);
#endif
#else  // linux
	int res = vsnprintf(buffer, L, format, argptr);
	if(res>=L)
		res=-1;
#endif

	va_end (argptr);
	return res;
}

inline int safe_fopen(FILE** file, const char* file_name, const char* mode)
{
#if _MSC_VER <= 1310 //VS.Net 2003 和 linux
	*file = fopen(file_name, mode);
	if(*file==0)
		return -1; //fail
	else
		return 0; //success
#else
	return fopen_s(file, file_name, mode);
#endif
}

inline tm safe_localtime(time_t t)
{
	static time_t last_time = 0;
	static tm last_tm;

	if(t == last_time)
		return last_tm;
	
	last_time = t;

#if defined(_MSC_VER)
#if _MSC_VER <= 1310 //VS.Net 2003
	tm* local = localtime(&t);

	last_tm = *local;
	return *local;
#else
	tm local;
	localtime_s(&local, &t);

	last_tm = local;
	return local;
#endif
#else // linux
	tm local;
	localtime_r(&t, &local);

	last_tm = local;
	return local;
#endif
}


#endif
