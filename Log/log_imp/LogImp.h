
/**
* @file   : LogImp.h
* @brief  : 日志系统
* @author : pengxiaolei
* @date   : 2021/06/19
*/


#ifndef __LOGIMP_SYSTEM_H__
#define __LOGIMP_SYSTEM_H__


#include "LogUtility.h"
#include "../LogCategory.h"
#include "ConsoleAppender.h"
#include "FileAppender.h"
#include <list>
#include <map>


class CLogInterface : public ILogInterface
{
public:
	CLogInterface(  );
	virtual ~CLogInterface();
	class CCategory
	{
	protected:
		LogCatg m_category;
		int m_priority;
		typedef std::list<ILogAppender*> AppenderList;
		AppenderList m_appenders;
	public:
		CCategory(LogCatg categ, int priority)
			:m_category(categ), m_priority(priority)
		{
		}
		void Write(int priority, int64 user_param, const char* msg)
		{
			for(AppenderList::iterator ai=m_appenders.begin(); ai!=m_appenders.end(); ++ai)
			{
				if (!(*ai))
				{
					//PAF_ASSERT(0);
					continue;//异常
				}
				(*ai)->Append(priority, user_param, msg);
			}
		}
		void SetPriority(int priority)
		{
			m_priority = priority;
		}
		int GetPriority()
		{
			return m_priority;
		}
		void AddAppender(ILogAppender* appender)
		{
			m_appenders.push_back(appender);
		}
		void RemoveAppender(ILogAppender* appender)
		{
			m_appenders.remove(appender);
		}
	};
	struct LogParamInfo
	{
		LogCatg category;
		int priority;
		int64 user_param;
		std::string msg;
		LogParamInfo()
		{
			category=0;
			priority = 0;
			user_param = 0;
		}
	};
protected:
	CCategory m_root;

	typedef std::map<LogCatg, CCategory*> CategoryMap;
	CategoryMap m_category_map;

	typedef CRecursiveLock LockType;
	LockType m_lock;

	IConsoleAppender* m_console_appender;
	IFileAppender* m_fapd_all;
	bool m_add_console = true;
	bool m_add_file ;
	bool m_b_use_thread = false;
	std::list<LogParamInfo> m_log_list;
	bool m_is_init;
public:
	virtual bool init(const std::string& logDir, const std::string& logFileName);

	
	virtual void StartLogThread();
	virtual void EndLogThread();
	//设置系统级别的参数
	virtual void SetSystemPriority(int priority)
	{
		CScopedLock<LockType> sl(&m_lock);
		m_root.SetPriority(priority);
	}
	//设置category级别的参数，默认为DEFAULT
	virtual	void SetCategoryPriority(LogCatg category, int priority)
	{
		CScopedLock<LockType> sl(&m_lock);
		GetCategory(category)->SetPriority(priority);
	}
	virtual	int GetCategoryPriority(LogCatg category);
	//增加Category级的Appender
	virtual void AddCategoryAppender(LogCatg category, ILogAppender* appender)
	{
		CScopedLock<LockType> sl(&m_lock);
		GetCategory(category)->AddAppender(appender);
	}
	//屏蔽Category级的Appender
	virtual void RemoveCategoryAppender(LogCatg category, ILogAppender* appender)
	{
		CScopedLock<LockType> sl(&m_lock);
		if(category==LOGCAT_ALL)
		{
			for(CategoryMap::iterator ci=m_category_map.begin(); ci!=m_category_map.end(); ++ci)
			{
				ci->second->RemoveAppender(appender);
			}
			if(appender==m_console_appender)
				m_add_console = false;
			if(appender==m_fapd_all)
				m_add_file = false;
		}
		else
		{
			GetCategory(category)->RemoveAppender(appender);
		}
	}
	virtual void RemoveCategoryAppender(LogCatg category, bool console, bool file)
	{
		if(console)
			RemoveCategoryAppender(category, m_console_appender);
		if(file)
			RemoveCategoryAppender(category, m_fapd_all);
	}

	//写LOG。
	//priority如果是default(-1)，则使用默认级别
	virtual void WriteLog(LogCatg category, int priority, int64 user_param, const char* msg);
	virtual void WriteLogF(LogCatg category, int priority, int64 user_param, const char* format, ...);
	virtual void WriteLogV(LogCatg category, int priority, int64 user_param, const char* format, va_list argptr);
	virtual void WriteLogImp(LogCatg category, int priority, int64 user_param, const char* msg);

	virtual bool IsOpen(LogCatg category, int priority);

	virtual IConsoleAppender* CreateConsoleAppender()
	{
		return new CConsoleAppdender();
	}
	virtual IFileAppender* CreateFileAppender(const char* file_name)
	{
		return new CFileAppdender(file_name);
	}

	virtual ILogFilter* CreateLogFilter()
	{
		return new CLogFilter();
	}

	virtual void RemoveLogs(int max_days, int max_files);

	virtual void WriteLog(void* msg, std::size_t size, int acc);

	//static void LogThread( void * param );

	virtual void logStandard(const char* format, ...);
	virtual void logError(const char* format, ...) ;
	virtual void logWarning(const char* format, ...);
protected:
	CCategory* GetCategory(LogCatg category)
	{
		CScopedLock<LockType> sl(&m_lock);
		CategoryMap::iterator ci=m_category_map.find(category);
		if(ci==m_category_map.end())
		{
			CCategory* cat = new CCategory(category, PRIO_DEFAULT);
			m_category_map[category] = cat;

			if(m_add_console)
				cat->AddAppender(m_console_appender);
			if(m_add_file)
				cat->AddAppender(m_fapd_all);
			return cat;
		}
		else
			return ci->second;
	}
};

#endif
