/**
* @file   : FileAppender.h
* @brief  : �ļ���־
* @author : pengxiaolei
* @date   : 2021/06/19
*/


#ifndef __FILE_APPENDER_H__
#define __FILE_APPENDER_H__

#include "../LogTypeDef.h"
#include <fstream>
#include <set>
#include "../LogCategory.h"

class CFileAppdender : public IFileAppender
{
private:
	std::string m_file_name;	//�ļ�����
	int			m_flags;		//��־λ
	AppointMemoryLevel	m_aml;	//ָ���洢�ȼ�
	int			m_priority;		//��־�Ƚϵȼ�
	//FILE*		m_logfp;
	std::fstream m_logf;
	tm			m_log_create_tm;	//��ǰ��־�ļ�����ʱ��
	time_t		m_log_create_time;	//��ǰ��־�ļ�����ʱ��

	time_t		m_last_time_write;	//�ϴμ�¼ʱ���ʱ��
	char		m_time_str_cache[32];//ʱ���ַ�������
	time_t		m_last_time_flush;  //�ϴ�flush��־��ʱ��
	std::string m_prefix;

public:
	CFileAppdender(const char* file_name)
		:m_file_name(file_name)
		,m_flags(0)
		,m_aml(AML_All)
		,m_priority(ILogInterface::PRIO_DEBUG)
		,m_last_time_write(0)
		,m_last_time_flush(0)
		,m_log_create_time(0)
	{
	}
	virtual ~CFileAppdender()
	{
		//CloseFile();
	}

	virtual void SetAttrib(unsigned int flags, AppointMemoryLevel level, int priority)
	{
		m_flags = flags;
		m_aml = level;
		m_priority = priority;
	}

	virtual void Append(int priority, int64 user_param, const char* msg);

	virtual void Release()
	{
		delete this;
	}

protected:
	bool CheckPriority(int priority)
	{
		switch(m_aml)
		{
		case AML_One:
			return (m_priority == priority);
		case AML_NoMoreThan:
			return (m_priority <= priority);
		case AML_NoLessThan:
			return (m_priority >= priority && priority!=0);
		default:
			return true;
		}
	}
	void CreateNewFile();
	void CloseFile()
	{
	}
};

class CLogFilter : public ILogFilter
{
	ILogAppender* m_appender;

	typedef std::set<LogType> Types;
	Types m_types;

	typedef std::set<int64> Players;
	Players m_players;
public:
	CLogFilter() : m_appender(0)
	{
	}
	virtual void SetLowAppender(ILogAppender* apd)
	{
		m_appender = apd;
	}
	virtual void SetFocusOn(LogType type_id)
	{
		m_types.insert(type_id);
	}
	virtual void SetFocusOn(int64 player_id)
	{
		m_players.insert(player_id);
	}
	virtual void RemoveFocusOn(LogType type_id)
	{
		m_types.erase(type_id);
	}
	virtual void RemoveFocusOn(int64 player_id)
	{
		m_players.erase(player_id);
	}
	virtual void Clear()
	{
		m_types.clear();
		m_players.clear();
	}

	virtual void Append(int priority, int64 user_param, const char* msg)
	{
		
	}
	virtual void Release()
	{
		delete this;
	}
};

#endif
