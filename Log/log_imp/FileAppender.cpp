

#include "FileAppender.h"
#include "ConsoleAppender.h"
#include "DateTime.h"

void CFileAppdender::Append(int priority, int64 user_param, const char* msg)
{
	if (!CheckPriority(priority))
	{
		//该等级未通过检验，不写入文件
		return;
	}

	time_t curr_time = time(NULL);

	if (m_last_time_write != curr_time && m_logf.is_open())
	{
		//如果自动换文件，则要比对日期
		tm now = safe_localtime(curr_time);

		if(m_flags&AUTO_CHANGE_DAILY)
		{
			if (  m_log_create_tm.tm_year != now.tm_year
				||m_log_create_tm.tm_mon  != now.tm_mon
				||m_log_create_tm.tm_mday != now.tm_mday)
			{
				m_logf.close();
			}
		}
		else if(m_flags & AUTO_CHANGE_HOURLY)
		{
			if(m_log_create_tm.tm_hour != now.tm_hour)
			{
				m_logf.close();
			}
		}
	}

	//if ((m_flags&ADD_PREFIX) /*&& GlobalHasString("module_name") && (m_prefix!=GlobalGetString("module_name"))*/)
	//{
	//	//如果前缀改了，则重新开一个文件
	//	if ( m_logf.is_open() )
	//		m_logf.close();
	//}

	if(!m_logf.is_open())
	{
		CreateNewFile();
	}
	if(m_logf.is_open())
	{
		//time_t t = time(0);
		//if(m_last_time_write!=t)
		//{
		//	fprintf(m_logfp, "\n%s\n", time_str());
		//	m_last_time_write=t;
		//}
		//if(m_flags&HIDE_CATEGORY)
		//{
		//	fprintf(m_logfp, "%s", msg);
		//}
		//else
		//{
		//	fprintf(m_logfp, "%s %s%s", category, GetLevelName(priority).c_str(), msg);
		//}

		//fflush(m_logfp);

		//if(m_last_time_write!=t)
		//{
		//	m_logf << "\n" << time_str() << "\n";
		//	m_last_time_write=t;
		//}
		if (m_last_time_write != curr_time)
		{
			tm local = safe_localtime(curr_time);
			safe_sprintf(m_time_str_cache, "%04d-%02d-%02d %02d:%02d:%02d",
				local.tm_year+1900,local.tm_mon+1,local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
			m_last_time_write = curr_time;
		}
		if(m_flags&HIDE_CATEGORY)
		{
			m_logf << m_time_str_cache << " " << GetLevelName(priority).c_str() << " [ThreadID: " << user_param << "] "<< msg;
		}
		else
		{			 //-V523
//			m_logf << TimeUtil::TimeToTimeStr(curr_time) << " " << category << " " << GetLevelName(priority).c_str() << msg;
			m_logf << m_time_str_cache << " " << GetLevelName(priority).c_str() << " [ThreadID: " << user_param << "] "<< msg;
		}

		if ((m_flags&FLUSH_PER_SECOND) == 0
			|| m_last_time_flush != curr_time)
		{
			m_last_time_flush = curr_time;
			m_logf.flush();
		}		
	}
}

void CFileAppdender::CreateNewFile()
{
	std::string file_name = m_file_name;
	std::string category;


	if ((m_flags&ADD_PREFIX) )
	{
		category = m_file_name;
	}
	else
	{
		category = m_file_name;
	}


	time_t now = time(NULL);

	if(m_flags&OVER_WRITE)
	{
		m_log_create_time = now;

		m_logf.open(file_name.c_str(), std::ios_base::out|std::ios_base::trunc);
	}
	else
	{
#if _PLATFORM_WIN32_

#if NO_CONSOLE_IN_BASEUTIL && FOR_QQGAME_HALL
		file_name = /*std::string("./logs/") +*/ category;
#elif NO_CONSOLE_IN_BASEUTIL
		file_name = /*std::string("./launcher_logs/") +*/ category;
#else//NO_CONSOLE_IN_BASEUTIL
		file_name = /*std::string("../logs/") +*/ category;
#endif//NO_CONSOLE_IN_BASEUTIL
		m_log_create_time = now;
		file_name += ".";
		file_name += time_str_2(m_log_create_time);
		file_name += ".log";
#else//_PLATFORM_WIN32_

//#if NO_CONSOLE_IN_BASEUTIL && FOR_QQGAME_HALL
//		file_name = std::string("./logs/") + category + ".log";
//#elif NO_CONSOLE_IN_BASEUTIL
//		file_name = std::string("./launcher_logs/") + category + ".log";
//#else//NO_CONSOLE_IN_BASEUTIL
//		file_name = std::string("../logs/") + category + ".log";
//#endif//NO_CONSOLE_IN_BASEUTIL
		if(m_log_create_time == 0)
		{
			m_log_create_time = get_file_time(file_name.c_str());
		}
		if(m_log_create_time && m_flags&AUTO_CHANGE_DAILY && !TimeUtil::IsSameDay(m_log_create_time, now))
		{
#if NO_CONSOLE_IN_BASEUTIL && FOR_QQGAME_HALL
			std::string tmp_name = /*std::string("./logs/") +*/ category + "." + TimeUtil::TimeToDateStr(m_log_create_time) + ".log";
#elif NO_CONSOLE_IN_BASEUTIL
			std::string tmp_name = /*std::string("./launcher_logs/") +*/ category + "." + TimeUtil::TimeToDateStr(m_log_create_time) + ".log";
#else//NO_CONSOLE_IN_BASEUTIL
			std::string tmp_name = /*std::string("../logs/") + */category + "." + TimeUtil::TimeToDateStr(m_log_create_time) + ".log";
#endif//NO_CONSOLE_IN_BASEUTIL			
			rename(file_name.c_str(), tmp_name.c_str());
			m_log_create_time = now;
		}
		if(m_log_create_time && m_flags&AUTO_CHANGE_HOURLY && !TimeUtil::IsSameHour(m_log_create_time, now))
		{
#if NO_CONSOLE_IN_BASEUTIL && FOR_QQGAME_HALL
			std::string tmp_name = /*std::string("./logs/") + */category + "." + TimeUtil::TimeToDateHourStr(m_log_create_time) + ".log";
#elif NO_CONSOLE_IN_BASEUTIL
			std::string tmp_name = /*std::string("./launcher_logs/") +*/ category + "." + TimeUtil::TimeToDateHourStr(m_log_create_time) + ".log";
#else//NO_CONSOLE_IN_BASEUTIL
			std::string tmp_name = /*std::string("../logs/") + */category + "." + TimeUtil::TimeToDateHourStr(m_log_create_time) + ".log";
#endif//NO_CONSOLE_IN_BASEUTIL			
			rename(file_name.c_str(), tmp_name.c_str());
			m_log_create_time = now;
		}
#endif//_PLATFORM_WIN32_
		m_logf.open(file_name.c_str(), std::ios_base::out|std::ios_base::app);
	}
	m_log_create_tm = safe_localtime(m_log_create_time);
}
