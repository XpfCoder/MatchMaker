#ifndef __CONSOLE_APPENDER_H__
#define __CONSOLE_APPENDER_H__

#include "../LogCategory.h"
#include "../SafeHelper.h"
#include "LogUtility.h"
#include <map>
#include "ConsoleWin32.h"
#include "DateTime.h"
#include "../UMConverter.h"


inline std::string GetLevelName(int priority)
{
	if(priority==ILogInterface::PRIO_ALWAYS)
		return "";
	else if(priority==ILogInterface::PRIO_EMERG)
		return "Emergent: ";
	else if(priority==ILogInterface::PRIO_FATAL)
		return "Fatal: ";
	else if(priority==ILogInterface::PRIO_CRITICAL)
		return "Critical: ";
	else if(priority==ILogInterface::PRIO_ERROR)
		return "Error: ";
	else if(priority==ILogInterface::PRIO_WARNING)
		return "Warning: ";
	else if(priority==ILogInterface::PRIO_NOTICE)
		return "Notice: ";
	else if(priority==ILogInterface::PRIO_INFO)
		return "Info: ";
	else if(priority==ILogInterface::PRIO_DEBUG)
		return "Debug: ";
	else if(priority==ILogInterface::PRIO_DEBUG_2)
		return "Debug2: ";
	else if(priority==ILogInterface::PRIO_DEBUG_3)
		return "Debug3: ";
	else if(priority==ILogInterface::PRIO_DEBUG_4)
		return "Debug4: ";
	/*else if(priority==ILogInterface::PRIO_RESOURCE)
		return "";*/
	else
		return ItoA(priority)+": ";
}

class CConsoleAppdender : public IConsoleAppender
{
	typedef std::map<int, std::string> ColorMap;
	ColorMap m_colormap;

	ColorConsole m_console;
public:
	CConsoleAppdender()
	{
		SetColor(ILogInterface::PRIO_DEBUG,    WHITE);
		SetColor(ILogInterface::PRIO_DEBUG_2,  WHITE);
		SetColor(ILogInterface::PRIO_DEBUG_3,  WHITE);
		SetColor(ILogInterface::PRIO_DEBUG_4,  WHITE);
	//	SetColor(ILogInterface::PRIO_RESOURCE,  WHITE);
		SetColor(ILogInterface::PRIO_INFO, HI_GREEN);
		SetColor(ILogInterface::PRIO_NOTICE,   YELLOW);
		SetColor(ILogInterface::PRIO_WARNING,  HI_YELLOW);
		SetColor(ILogInterface::PRIO_ERROR,    HI_RED);
		SetColor(ILogInterface::PRIO_CRITICAL, RED);
		SetColor(ILogInterface::PRIO_FATAL, RED);

		SetColor(ILogInterface::PRIO_ALWAYS, PINK);
	}

	virtual void SetColor(int priority, CONSOLE_COLOR color)
	{
		m_colormap[priority] = GetColorString(color);
	}

	virtual void Append(int priority, int64 user_param, const char* msg)
	{
		ColorMap::iterator ci=m_colormap.find(priority);
		m_console.Print(TimeUtil::TimeToTimeStr(time(NULL)));
		m_console.Print(" ");
		if(ci!=m_colormap.end())
		{
			m_console.Print("$PUSH$");
			m_console.Print(ci->second.c_str());
		}

		m_console.Print(GetLevelName(priority).c_str());


		//MUtf8RealConverter con(msg);
		 m_console.Print(" [ThreadID: ");
		 m_console.Print( ItoA(user_param).c_str());
		 m_console.Print("] ");

		m_console.Print(msg);

		if(ci!=m_colormap.end())
		{
			m_console.Print("$POP$");
		}
	}
	virtual void Release()
	{
		delete this;
	}

protected:
	const char* GetColorString(CONSOLE_COLOR color)
	{
		switch(color)
		{
		case BLACK:
			return "$BLACK$";
		case BLUE:
			return "$BLUE$";
		case GREEN:
			return "$GREEN$";
		case CYAN:
			return "$CYAN$";
		case RED:
			return "$RED$";
		case PURPLE:
			return "$PURPLE$";
		case YELLOW:
			return "$YELLOW$";
		case GRAY:
			return "$GRAY$";
		case HI_BLUE:
			return "$HI_BLUE$";
		case HI_GREEN:
			return "$HI_GREEN$";
		case HI_CYAN:
			return "$HI_CYAN$";
		case HI_RED:
			return "$HI_RED$";
		case PINK:
			return "$PINK$";
		case HI_YELLOW:
			return "$HI_YELLOW$";
		case WHITE:
			return "$WHITE$";
		default:
			return "";
		}
	}
};

#endif
