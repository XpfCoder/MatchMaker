//#include "stdafx.h"

#include "../SafeHelper.h"

#include "ConsoleWin32.h"

ColorConsole::ColorConsole()
{
	CBPrintStr = 0;
#if _IS_WIN32_
	m_mode = 0;
	hStdout = 0; 

	m_color_map["BLACK"]		= FOREGROUND_BLACK;
	m_color_map["BLUE"]		= FOREGROUND_BLUE;
	m_color_map["GREEN"]		= FOREGROUND_GREEN;
	m_color_map["CYAN"]		= FOREGROUND_LESS_BLUE;
	m_color_map["RED"]		= FOREGROUND_RED;
	m_color_map["PURPLE"]		= FOREGROUND_PINK;
	m_color_map["YELLOW"]		= FOREGROUND_YELLOW;
	m_color_map["GRAY"]		= FOREGROUND_WHITE;
	m_color_map["HI_BLUE"]	= FOREGROUND_HI_BLUE;
	m_color_map["HI_GREEN"]	= FOREGROUND_HI_GREEN;
	m_color_map["HI_CYAN"]	= FOREGROUND_HI_LESS_BLUE;
	m_color_map["HI_RED"]		= FOREGROUND_HI_RED;
	m_color_map["PINK"]		= FOREGROUND_HI_PINK;
	m_color_map["HI_YELLOW"]	= FOREGROUND_HI_YELLOW;
	m_color_map["WHITE"]		= FOREGROUND_HI_WHITE;


	m_trans_map["BLACK"]		= "<C 000000>";

	m_trans_map["RED"]		= "<C 800000>";
	m_trans_map["GREEN"]		= "<C 008000>";
	m_trans_map["YELLOW"]		= "<C 808000>";
	m_trans_map["BLUE"]		= "<C 000080>";
	m_trans_map["PURPLE"]		= "<C 800080>";
	m_trans_map["CYAN"]		= "<C 008080>";
	m_trans_map["GRAY"]		= "<C 808080>";

	m_trans_map["HI_RED"]		= "<C ff0000>";
	m_trans_map["HI_GREEN"]	= "<C 00ff00>";
	m_trans_map["HI_YELLOW"]	= "<C ffff00>";
	m_trans_map["HI_BLUE"]	= "<C 0000ff>";
	m_trans_map["PINK"]		= "<C ff00ff>";
	m_trans_map["HI_CYAN"]	= "<C 00ffff>";
	m_trans_map["WHITE"]		= "<C ffffff>";

	SetMode(0);
#else
	m_mode = 1;
	m_trans_map["BLACK"]		= "\033[0;30m";

	m_trans_map["RED"]		= "\033[0;31m";
	m_trans_map["GREEN"]		= "\033[0;32m";
	m_trans_map["YELLOW"]		= "\033[0;33m";
	m_trans_map["BLUE"]		= "\033[0;34m";
	m_trans_map["PURPLE"]		= "\033[0;35m";
	m_trans_map["CYAN"]		= "\033[0;36m";
	m_trans_map["GRAY"]		= "\033[0;37m";

	m_trans_map["HI_RED"]		= "\033[1;31m";
	m_trans_map["HI_GREEN"]	= "\033[1;32m";
	m_trans_map["HI_YELLOW"]	= "\033[1;33m";
	m_trans_map["HI_BLUE"]	= "\033[1;34m";
	m_trans_map["PINK"]		= "\033[1;35m";
	m_trans_map["HI_CYAN"]	= "\033[1;36m";
	m_trans_map["WHITE"]		= "\033[1;37m";

	SetMode(1);
#endif

	m_current_color = "GRAY";
}

void ColorConsole::Literal(const char* str, int len, std::string& trans)
{
	//printf("\x1b[31mtest\n");
	if(len>200)
		return;
	char buff[256];
	safe_strcpy(buff, str, len);

	if(strcmp(buff,"PUSH")==0)
	{
		m_colorstack.push_back(m_current_color);
	}
	else if(strcmp(buff,"POP")==0)
	{
		if(!m_colorstack.empty())
		{
			m_current_color = m_colorstack.back();
			m_colorstack.pop_back();
			Literal(m_current_color.c_str(), (int)m_current_color.length(), trans);
		}
	}
	else
	{
		if(m_mode==0)
		{
#if _IS_WIN32_
			ColorMapIter ci=m_color_map.find(buff);
			if(ci!=m_color_map.end())
			{
				int code = ci->second;
				m_current_color = buff;
				SetConsoleTextAttribute(hStdout, code);
			}
#endif
		}
		else
		{
			TransMapIter ti=m_trans_map.find(buff);
			if(ti!=m_trans_map.end())
			{
				trans+=ti->second;
			}
		}
	}
}

void ColorConsole::Print(const char* str)
{
	bool fn = false;
	const char* normal = str;
	const char* literal = 0;
	int len = 0;

	std::string trans;

	for(const char* p = str; *p; ++p)
	{
		char c=*p;
		if(c!='$')
		{
			++len;
		}
		else
		{
			if(!fn)
			{
				if(normal&&len)
				{
					PrintStr(normal, len, trans);
				}
				fn=true;
				literal = p+1;
				normal = 0;
				len = 0;
			}
			else
			{
				if(literal&&len)
				{
					Literal(literal, len, trans);
				}
				else if(literal&&!len)
				{
					PrintStr("$", 1, trans);
				}
				fn=false;
				normal = p+1;
				literal = 0;
				len = 0;
			}
		}
	}
	if(normal&&len)
	{
		PrintStr(normal, len, trans);
	}
	if(m_mode==1)
	{
		printf("%s", trans.c_str());
	}
	else if(m_mode==2)
	{
		CBPrintStr(trans.c_str());
	}
}
