


#include "LogImp.h"
#include <list>
#include <algorithm>
#include "../UMConverter.h"
#include <filesystem>
namespace fs = std::filesystem;

#if defined(_MSC_VER)
//#define VSNPRINTF _vsnprintf_s
#define VSNPRINTF vsnprintf
#else
    #define VSNPRINTF vsnprintf
#endif

//#define NO_LOG_AT_ALL 1
CLogInterface::CLogInterface()
	:m_root(LOGCAT_ROOT, PRIO_DEFAULT), m_add_console(true), m_add_file(true)
{
	m_is_init = false;
}
CLogInterface::~CLogInterface()
{
	for(CategoryMap::iterator ci=m_category_map.begin(); ci!=m_category_map.end(); ++ci)
	{
		delete ci->second;
	}

	m_console_appender->Release();
	m_fapd_all->Release();
}
void CLogInterface::WriteLog(LogCatg category, int priority, int64 user_param, const char* msg)
{

	CScopedLock<LockType> sl(&m_lock);

	if(m_b_use_thread)
	{
		LogParamInfo info;
		info.category = category;
		info.priority = priority;
		info.user_param = user_param;
		info.msg = msg;
		m_log_list.push_back(info);
	}
	else
	{
		WriteLogImp(category,priority,user_param,msg);
	}
}
void CLogInterface::WriteLogImp(LogCatg category, int priority, int64 user_param, const char* msg)
{
	CScopedLock<LockType> sl(&m_lock);

	CCategory* cat = GetCategory(category);
	int prio = cat->GetPriority();
	if(prio==PRIO_DEFAULT)
	{
		prio = m_root.GetPriority();
	}
	if(prio==PRIO_DEFAULT || prio>=priority)
	{
		cat->Write(priority, user_param, msg);
	}
}
void CLogInterface::WriteLogF(LogCatg category, int priority, int64 user_param, const char* format, ...)
{

	va_list argptr;

	va_start (argptr, format);
	CLogInterface::WriteLogV(category, priority, user_param, format, argptr);
	va_end (argptr);
}

void CLogInterface::WriteLogV(LogCatg category, int priority, int64 user_param, const char* format, va_list argptr)
{
	//std::string temp_str(format);
	//if (temp_str.find("%") == std::string::npos)
	
	int n = VSNPRINTF(G_BUFF_CHR, G_LOG_BUFLEN, format, argptr);
	G_BUFF_CHR[n] = '\n';
	G_BUFF_CHR[n + 1] = 0;
	WriteLog(category, priority, user_param, G_BUFF_CHR);
	
}

bool CLogInterface::IsOpen(LogCatg category, int priority) 
{
//return false;
	int prio = GetCategoryPriority(category);
	return prio==PRIO_DEFAULT || prio>=priority;
}

int CLogInterface::GetCategoryPriority(LogCatg category)
{
	CCategory* cat = GetCategory(category);
	int prio = cat->GetPriority();
	if(prio==PRIO_DEFAULT)
		prio = m_root.GetPriority();

	return prio;
}
///////////////////////////////////////////////////////////////

void CLogInterface::RemoveLogs(int max_days, int max_files)
{

}

void CLogInterface::WriteLog(void* msg, std::size_t size, int acc)
{
#ifdef NO_LOG_AT_ALL
	return;
#endif

	//static CMMapBinLog loger;
	//loger.Log(msg, size, acc);
}

bool CLogInterface::init(const std::string& logDir, const std::string& logFileName)
{
	if (m_is_init)
	{
		LogWarningEx(u8"日志文件重复初始化, logDir ", logDir, " file name : ", logFileName);
		return false;
	}
	m_is_init = true;

	std::string baseDir = std::string(logDir) + "LogFile\\";
	fs::path fspath = fs::u8path(baseDir);
	fs::create_directories(fspath);
	time_t now = time(NULL);
	std::string tmp =   time_str_2(now) + "_" +  logFileName;

	std::string logFilePath = baseDir + tmp;

	m_b_use_thread = false;

	CLogInterface* logintf = this;

	m_console_appender = logintf->CreateConsoleAppender();

	int flags;
	std::string create_time = logFilePath ;
	m_fapd_all = logintf->CreateFileAppender(create_time.c_str());
	flags = IFileAppender::ADD_PREFIX | IFileAppender::ADD_SUFFIX | IFileAppender::AUTO_CHANGE_DAILY;
	m_fapd_all->SetAttrib(flags);

	errno_t err;
	FILE* fp;
	if ((err = safe_fopen(&fp, "no_console.ini", "r")) == 0)
	{
		fclose(fp);
		m_add_console = false;
		printf("no_console.ini found, so no ConsoleAppender\n");
	}
#ifdef NO_CONSOLE_IN_BASEUTIL
	m_add_console = false;
#endif

	//BeginThread(CLogInterface::LogThread, (void*)this);
	printf("LogInterface created\n");
	return true;
}

void CLogInterface::StartLogThread()
{
	m_b_use_thread = true;
}
void CLogInterface::EndLogThread()
{
	m_b_use_thread = false;
}

void CLogInterface::logStandard(const char* format, ...)
{
	CScopedLock<LockType> sl(&m_lock);
	va_list argptr;

	va_start(argptr, format);
	WriteLogV(LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_INFO, GetThreadID, format, argptr);
	va_end(argptr);
}

void CLogInterface::logError(const char* format, ...)
{
	CScopedLock<LockType> sl(&m_lock);
	va_list argptr;

	va_start(argptr, format);
	WriteLogV(LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_ERROR, GetThreadID, format, argptr);

	va_end(argptr);
}

void CLogInterface::logWarning(const char* format, ...)
{
	CScopedLock<LockType> sl(&m_lock);
	va_list argptr;

	va_start(argptr, format);
	WriteLogV(LOGCAT_SYSTEM_LOG, ILogInterface::PRIO_WARNING, GetThreadID, format, argptr);

	va_end(argptr);
}

