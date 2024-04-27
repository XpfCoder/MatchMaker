#ifndef __LOGCATEGORY_H__iojjkka__
#define __LOGCATEGORY_H__iojjkka__

#include <stdio.h>
#include <iostream>
#include "SafeHelper.h"
#include "LogFrameWork.h"
#include "log_imp/LogUtility.h"
#include <Windows.h>
//#include "DeepWorldCore/DeepWorldApi/LogManagerInterface.h"
//typedef const char* LogCatg;



enum CONSOLE_COLOR
{
	DEFAULT,
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PURPLE,
	YELLOW,
	GRAY,
	HI_BLUE,
	HI_GREEN,
	HI_CYAN,
	HI_RED,
	PINK,
	HI_YELLOW,
	WHITE,
};

enum LogType
{
	Log_T_None = -1,
	Log_T_Any = 0,

	Log_T_Login = 1,
	

	Log_T_OSS_End = 2000,
};




class ILogAppender
{
public:
	virtual void Release() = 0;
	virtual void Append(int priority, int64 user_param, const char* msg) = 0;
};

class IConsoleAppender : public ILogAppender
{
public:
	virtual void SetColor(int priority, CONSOLE_COLOR color) = 0;
};

class IFileAppender : public ILogAppender
{
public:
	enum AppointMemoryLevel
	{
		AML_All,		//全部
		AML_One,		//指定一个
		AML_NoMoreThan,	//不高于
		AML_NoLessThan,	//不低于
	};
	enum
	{
		ADD_PREFIX = 0x01,		//加前缀，前缀名为“GlobalGetString("module_name")”定义
		ADD_SUFFIX = 0x02,		//加后缀，后缀为时间
		AUTO_CHANGE_DAILY = 0x04,	//每日0:00的时候自动换一个文件
		OVER_WRITE = 0x08,		//覆盖原文件
		HIDE_CATEGORY = 0x10,	//不打印category
		FLUSH_PER_SECOND = 0x20, //每秒flush一次
		AUTO_CHANGE_HOURLY = 0x40,	//每小时自动换一个文件
	};
	virtual void SetAttrib(unsigned int flags, AppointMemoryLevel level= AML_NoMoreThan, int priority=0) = 0;
};


class ILogFilter : public ILogAppender
{
public:
	virtual void SetLowAppender(ILogAppender* apd) = 0;
	virtual void SetFocusOn(LogType type_id) = 0;
	virtual void SetFocusOn(int64 player_id) = 0;
	virtual void RemoveFocusOn(LogType type_id) = 0;
	virtual void RemoveFocusOn(int64 player_id) = 0;
	virtual void Clear() = 0;
};

class ILogInterface
{
public:
	enum PRIORITY
	{
		//数值越大，表示信息越繁琐

		PRIO_DEFAULT  = -1,		//自动推断，会根据Category默认值。如果这个也是，那就跟据系统默认
		PRIO_ALWAYS   = 0,		//总是打印
		PRIO_EMERG    = 100,	//紧急
		PRIO_FATAL    = 200,	//致命
		PRIO_CRITICAL = 300,	//关键
		PRIO_ERROR    = 400,	//错误
		PRIO_WARNING  = 500,	//警告
		PRIO_NOTICE   = 600,	//注意，用于轻微异常，尤其是预期会经常出现的异常
		PRIO_INFO     = 700,	//信息
		PRIO_DEBUG    = 800,	//调试1
		PRIO_DEBUG_2  = 900,	//调试2
		PRIO_DEBUG_3  = 1000,	//调试3
		PRIO_DEBUG_4  = 1100,	//调试4
		//PRIO_RESOURCE = 1200,	//客户端资源
	};

	virtual bool init(const std::string& logDir, const std::string& logFileName) = 0;

	//增加Category级的Appender
	virtual void AddCategoryAppender(LogCatg category, ILogAppender* appender) = 0;
	//屏蔽Category级的Appender
	virtual void RemoveCategoryAppender(LogCatg category, ILogAppender* appender) = 0;
	//屏蔽默认的console或者file
	virtual void RemoveCategoryAppender(LogCatg category, bool console, bool file) = 0;

	//设置系统级别的参数
	virtual void SetSystemPriority(int priority) = 0;
	//设置category级别的参数，默认为DEFAULT
	virtual	void SetCategoryPriority(LogCatg category, int priority) = 0;
	virtual	int GetCategoryPriority(LogCatg category) = 0;

	//写LOG。
	//priority如果是default(-1)，则使用默认级别
	virtual void WriteLog(LogCatg category, int priority, int64 user_param, const char* msg) = 0;
	virtual void WriteLogF(LogCatg category, int priority, int64 user_param, const char* format, ...) = 0;
	virtual void WriteLogV(LogCatg category, int priority, int64 user_param, const char* format, va_list argptr) = 0;

	//查询这个级别下的priority是否需要输出
	virtual bool IsOpen(LogCatg category, int priority) = 0;

	virtual IConsoleAppender* CreateConsoleAppender() = 0;
	virtual IFileAppender* CreateFileAppender(const char* file_name) = 0;

	
	// 删除Log文件，max_days以外的文件将被删除，同时最多保存max_files个文件(按修改日期排序，保留最近的)。
	virtual void RemoveLogs(int max_days, int max_files)=0;

	//写binlog
	virtual void WriteLog(void* msg, std::size_t size, int acc)=0;
	virtual void StartLogThread()=0;
	virtual void EndLogThread()=0;

	virtual void logStandard(const char* format, ...) = 0;
	virtual void logError(const char* format, ...) = 0;
	virtual void logWarning(const char* format, ...) = 0;

};

inline int Log_ToStr(bool val, char* buff, int buff_size)
{
	if(buff_size<4)
		return 0;
	if(val)
	{
		return safe_sprintf(buff, buff_size, "true");
	}
	else
	{
		return safe_sprintf(buff, buff_size, "false");
	}
}

inline int Log_ToStr(char val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%d", val);
}
inline int Log_ToStr(unsigned char val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%u", val);
}

inline int Log_ToStr(short val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%d", val);
}
inline int Log_ToStr(unsigned short val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%u", val);
}

inline int Log_ToStr(int val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%d", val);
}
inline int Log_ToStr(unsigned int val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%u", val);
}

//inline int Log_ToStr(std::size_t val, char* buff, int buff_size){
//	return safe_sprintf(buff, buff_size, "%u", val);
//}

inline int Log_ToStr(long val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%d", val);
}
inline int Log_ToStr(unsigned long val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%u", val);
}

inline int Log_ToStr(int64 val, char* buff, int buff_size){
//#if _IS_WIN32_
	return safe_sprintf(buff, buff_size, "%I64d", val);
//#else
//	return safe_sprintf(buff, buff_size, "%lld", val);
//#endif
}
inline int Log_ToStr(uint64 val, char* buff, int buff_size){
//#if _IS_WIN32_
	return safe_sprintf(buff, buff_size, "%I64u", val);
//#else
//	return safe_sprintf(buff, buff_size, "%llu", val);
//#endif
}

inline int Log_ToStr(float val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%g", val);
}
inline int Log_ToStr(double val, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%g", val);
}

inline int Log_ToStr(const char* str, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%s", str);
}
inline int Log_ToStr(char* str, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%s", str);
}

inline int Log_ToStr(const std::string& str, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%s", str.c_str());
}

inline int Log_ToStr(void* ptr, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%p", ptr);
}

inline int Log_ToStr(const void* ptr, char* buff, int buff_size){
	return safe_sprintf(buff, buff_size, "%p", ptr);
}

inline ILogInterface* GetLogInterface()
{
	/*ILogInterface* log = Resafety::DeepWorld::GetLogManager();
	return log;*/
	return GetLogFrameWork()->GetLogInterface();
}

static const int G_LOG_BUFLEN = 1024*1024*10; 
static char G_BUFF_CHR[G_LOG_BUFLEN];
static CRecursiveLock G_BUFF_CHR_LOCK;
#define _LOG_FUN_BEGIN G_BUFF_CHR_LOCK.Lock();\
					   if(GetLogInterface()->IsOpen(category, priority)){ \
                           int pos=0; \
                           int res; 

#define _LOG_FUN_END       size_t _n = strlen(seperator);\
						   G_BUFF_CHR[pos - _n]='\n';G_BUFF_CHR[pos - _n+1] = 0;\
                           GetLogInterface()->WriteLog(category, priority, user_param, G_BUFF_CHR); \
						   G_BUFF_CHR_LOCK.Unlock();\
						   return pos;\
                       } \
                       else { \
						   G_BUFF_CHR_LOCK.Unlock();\
						   return 0; \
					   }

#define _LOG_ITEM_(item) res = Log_ToStr(item, G_BUFF_CHR+pos, G_LOG_BUFLEN-1-pos);if(res<0)pos=G_LOG_BUFLEN-2;else pos+=res;

#define _LOG_ITEM(item) \
	_LOG_ITEM_(item) \
	_LOG_ITEM_(seperator)

template<typename T1>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
	_LOG_FUN_END
}

template<typename T1, typename T2>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
	_LOG_FUN_END
}
template<typename T1, typename T2, typename T3>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
	_LOG_FUN_END
}
template<typename T1, typename T2, typename T3, typename T4>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
	_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
	_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
			 const T6& t6)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
	_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
			 const T6& t6, const T7& t7)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
	_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7, typename T8>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
			 const T6& t6, const T7& t7, const T8& t8 )
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
	_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7, typename T8, typename T9>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
			 const T6& t6, const T7& t7, const T8& t8, const T9& t9)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
	_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7, typename T8, typename T9, typename T10>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
			 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
	_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
			 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
	_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15
, typename T16>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
typename T34>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
					 , const T34& t34)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
typename T34, typename T35>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
					 , const T34& t34, const T35& t35)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
typename T34, typename T35, typename T36>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
					 , const T34& t34, const T35& t35, const T36& t36)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
typename T34, typename T35, typename T36, typename T37>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
					 , const T34& t34, const T35& t35, const T36& t36, const T37& t37)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
typename T34, typename T35, typename T36, typename T37, typename T38>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
					 , const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
typename T34, typename T35, typename T36, typename T37, typename T38, typename T39>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
					 , const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
					 , const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41>
inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
					 const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
					 , const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
					 , const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
					 , const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
    typename T43>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46, typename T47>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46, const T47& t47)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_ITEM(t47)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46, typename T47, typename T48>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46, const T47& t47, const T48& t48)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_ITEM(t47)
		_LOG_ITEM(t48)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46, const T47& t47, const T48& t48, const T49& t49)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_ITEM(t47)
		_LOG_ITEM(t48)
		_LOG_ITEM(t49)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46, const T47& t47, const T48& t48, const T49& t49, const T50& t50)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_ITEM(t47)
		_LOG_ITEM(t48)
		_LOG_ITEM(t49)
		_LOG_ITEM(t50)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46, const T47& t47, const T48& t48, const T49& t49, const T50& t50, const T51& t51)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_ITEM(t47)
		_LOG_ITEM(t48)
		_LOG_ITEM(t49)
		_LOG_ITEM(t50)
		_LOG_ITEM(t51)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51,
    typename T52>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46, const T47& t47, const T48& t48, const T49& t49, const T50& t50, const T51& t51
	, const T52& t52)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_ITEM(t47)
		_LOG_ITEM(t48)
		_LOG_ITEM(t49)
		_LOG_ITEM(t50)
		_LOG_ITEM(t51)
		_LOG_ITEM(t52)
		_LOG_FUN_END
}

template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51,
	typename T52, typename T53>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46, const T47& t47, const T48& t48, const T49& t49, const T50& t50, const T51& t51
	, const T52& t52, const T53& t53)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_ITEM(t47)
		_LOG_ITEM(t48)
		_LOG_ITEM(t49)
		_LOG_ITEM(t50)
		_LOG_ITEM(t51)
		_LOG_ITEM(t52)
		_LOG_ITEM(t53)
		_LOG_FUN_END
}


template<typename T1, typename T2, typename T3, typename T4, typename T5,
	typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
	typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24,
	typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33,
	typename T34, typename T35, typename T36, typename T37, typename T38, typename T39, typename T40, typename T41, typename T42,
	typename T43, typename T44, typename T45, typename T46, typename T47, typename T48, typename T49, typename T50, typename T51,
	typename T52, typename T53, typename T54>
	inline int _WriteLog(const char* seperator, LogCatg category, int priority, int64 user_param, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5,
	const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15
	, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24
	, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31, const T32& t32, const T33& t33
	, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40, const T41& t41, const T42& t42
	, const T43& t43, const T44& t44, const T45& t45, const T46& t46, const T47& t47, const T48& t48, const T49& t49, const T50& t50, const T51& t51
	, const T52& t52, const T53& t53, const T54& t54)
{
	_LOG_FUN_BEGIN
		_LOG_ITEM(t1)
		_LOG_ITEM(t2)
		_LOG_ITEM(t3)
		_LOG_ITEM(t4)
		_LOG_ITEM(t5)
		_LOG_ITEM(t6)
		_LOG_ITEM(t7)
		_LOG_ITEM(t8)
		_LOG_ITEM(t9)
		_LOG_ITEM(t10)
		_LOG_ITEM(t11)
		_LOG_ITEM(t12)
		_LOG_ITEM(t13)
		_LOG_ITEM(t14)
		_LOG_ITEM(t15)
		_LOG_ITEM(t16)
		_LOG_ITEM(t17)
		_LOG_ITEM(t18)
		_LOG_ITEM(t19)
		_LOG_ITEM(t20)
		_LOG_ITEM(t21)
		_LOG_ITEM(t22)
		_LOG_ITEM(t23)
		_LOG_ITEM(t24)
		_LOG_ITEM(t25)
		_LOG_ITEM(t26)
		_LOG_ITEM(t27)
		_LOG_ITEM(t28)
		_LOG_ITEM(t29)
		_LOG_ITEM(t30)
		_LOG_ITEM(t31)
		_LOG_ITEM(t32)
		_LOG_ITEM(t33)
		_LOG_ITEM(t34)
		_LOG_ITEM(t35)
		_LOG_ITEM(t36)
		_LOG_ITEM(t37)
		_LOG_ITEM(t38)
		_LOG_ITEM(t39)
		_LOG_ITEM(t40)
		_LOG_ITEM(t41)
		_LOG_ITEM(t42)
		_LOG_ITEM(t43)
		_LOG_ITEM(t44)
		_LOG_ITEM(t45)
		_LOG_ITEM(t46)
		_LOG_ITEM(t47)
		_LOG_ITEM(t48)
		_LOG_ITEM(t49)
		_LOG_ITEM(t50)
		_LOG_ITEM(t51)
		_LOG_ITEM(t52)
		_LOG_ITEM(t53)
		_LOG_ITEM(t54)
		_LOG_FUN_END
}



  #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

	#define GetThreadID			GetCurrentThreadId()

	#define _DEFINE_LOG_WRAPPER(fun_name, category, priority, seperator) \
	template<typename T1> \
	inline int fun_name(const T1& t1) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1);} \
	\
	template<typename T1, typename T2> \
	inline int fun_name(const T1& t1, const T2& t2) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2);} \
	\
	template<typename T1, typename T2, typename T3> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3);} \
	\
	template<typename T1, typename T2, typename T3, typename T4> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36);} \
    \
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37) \
    {return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46,const T47& t47) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46,t47);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46,const T47& t47,const T48& t48) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46,t47,t48);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46,const T47& t47,const T48& t48,const T49& t49) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46,t47,t48,t49);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46,const T47& t47,const T48& t48,const T49& t49,const T50& t50) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46,t47,t48,t49,t50);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, typename T51> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46,const T47& t47,const T48& t48,const T49& t49,const T50& t50,const T51& t51) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46,t47,t48,t49,t50,t51);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, typename T51,typename T52> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46,const T47& t47,const T48& t48,const T49& t49,const T50& t50,const T51& t51,const T52& t52) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46,t47,t48,t49,t50,t51,t52);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, typename T51,typename T52,typename T53> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46,const T47& t47,const T48& t48,const T49& t49,const T50& t50,const T51& t51,const T52& t52,const T53& t53) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46,t47,t48,t49,t50,t51,t52,t53);} \
	\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25, typename T26, typename T27, typename T28, typename T29, typename T30, typename T31, typename T32, typename T33, typename T34, typename T35, typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,typename T48,typename T49,typename T50, typename T51,typename T52,typename T53,typename T54> \
	inline int fun_name(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9, const T10& t10, const T11& t11, const T12& t12, const T13& t13, const T14& t14, const T15& t15, const T16& t16, const T17& t17, const T18& t18, const T19& t19, const T20& t20, const T21& t21, const T22& t22, const T23& t23, const T24& t24, const T25& t25, const T26& t26, const T27& t27, const T28& t28, const T29& t29, const T30& t30, const T31& t31,const T32& t32, const T33& t33, const T34& t34, const T35& t35, const T36& t36, const T37& t37, const T38& t38, const T39& t39, const T40& t40,const T41& t41,const T42& t42,const T43& t43,const T44& t44,const T45& t45,const T46& t46,const T47& t47,const T48& t48,const T49& t49,const T50& t50,const T51& t51,const T52& t52,const T53& t53,const T54& t54) \
	{return _WriteLog(seperator, category, priority, GetThreadID, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36, t37, t38,t39,t40,t41,t42,t43,t44,t45,t46,t47,t48,t49,t50,t51,t52,t53,t54);} \


  #else

	#define _DEFINE_LOG_WRAPPER(fun_name, category, priority, seperator) \
	template<typename... Args> \
	inline int fun_name(const Args&... args) \
	{return _WriteLog(seperator, category, priority, 0, args...); } \

  #endif

//#endif

enum LOG_CATEGORY
{
	LOGCAT_DEFAULT = 0,
	LOGCAT_ALL = 1,
	LOGCAT_ROOT = 2,
	LOGCAT_SYSTEM_LOG = 10,
	
};

#define TLOG_SEPERATOR "|"

#define DEFINE_LOG_WRAPPER(name,cate,pri) _DEFINE_LOG_WRAPPER(name,cate,pri,"")

#pragma warning(disable:4002)

// #ifndef _DEBUG
//   #define LogDebugEx(t1) 0
//  // #pragma warning(disable:4002)
// #else
_DEFINE_LOG_WRAPPER(LogDebugEx,    LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_DEBUG, "")
//#endif
_DEFINE_LOG_WRAPPER(LogInfoEx,     LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_INFO, "")
_DEFINE_LOG_WRAPPER(LogNoticeEx,   LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_NOTICE, "")
_DEFINE_LOG_WRAPPER(LogWarningEx,  LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_WARNING, "")
_DEFINE_LOG_WRAPPER(LogErrorEx,    LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_ERROR, "")

//总是打印
_DEFINE_LOG_WRAPPER(LogAlwaysEx, LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_ALWAYS, "")



//---------------------------------------------------

#   define strcmp_no_case _strcmpi

inline int ParsePriorityString(const char* str)
{
	int prio = ILogInterface::PRIO_DEFAULT;

	if(strcmp_no_case(str, "default")==0)
		prio = ILogInterface::PRIO_DEFAULT;
	else if(strcmp_no_case(str, "emerg")==0)
		prio = ILogInterface::PRIO_EMERG;
	else if(strcmp_no_case(str, "fatal")==0)
		prio = ILogInterface::PRIO_FATAL;
	else if(strcmp_no_case(str, "critical")==0)
		prio = ILogInterface::PRIO_CRITICAL;
	else if(strcmp_no_case(str, "error")==0)
		prio = ILogInterface::PRIO_ERROR;
	else if(strcmp_no_case(str, "warning")==0)
		prio = ILogInterface::PRIO_WARNING;
	else if(strcmp_no_case(str, "notice")==0)
		prio = ILogInterface::PRIO_NOTICE;
	else if(strcmp_no_case(str, "info")==0)
		prio = ILogInterface::PRIO_INFO;
	else if(strcmp_no_case(str, "debug")==0)
		prio = ILogInterface::PRIO_DEBUG;
	else if(strcmp_no_case(str, "debug2")==0)
		prio = ILogInterface::PRIO_DEBUG_2;
	else if(strcmp_no_case(str, "debug3")==0)
		prio = ILogInterface::PRIO_DEBUG_3;
	else if(strcmp_no_case(str, "debug4")==0)
		prio = ILogInterface::PRIO_DEBUG_4;
	else
		prio = atoi(str);

	return prio;
}
inline void SetLogPriorityString(const char* str, LogCatg category=0)
{
	int level = ParsePriorityString(str);
	if(category)
		GetLogInterface()->SetCategoryPriority(category, level);
	else
		GetLogInterface()->SetSystemPriority(level);
}



#endif
