#ifndef __CONSOLE_WIN32_lksjcvkie92j4jf82__
#define __CONSOLE_WIN32_lksjcvkie92j4jf82__


//#include <platform/platform_shared/BiboFrame/BiboPlatform.h>
//#include <platform/platform_shared/BiboFrame/BTLBASIC.h>

//#include <third_party/mmgr/mmgr.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

	#include <Windows.h>
	#define _IS_WIN32_ 1
	#define FOREGROUND_BLACK 0x0000
	#define FOREGROUND_WHITE (FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED)	//0x0007
	#define FOREGROUND_LESS_BLUE (FOREGROUND_BLUE|FOREGROUND_GREEN)				//0x0003
	#define FOREGROUND_YELLOW (FOREGROUND_GREEN|FOREGROUND_RED)					//0x0006
	#define FOREGROUND_PINK (FOREGROUND_BLUE|FOREGROUND_RED)					//0x0005

	#define FOREGROUND_HI_BLUE      (FOREGROUND_BLUE|FOREGROUND_INTENSITY)		//0x0009
	#define FOREGROUND_HI_GREEN     (FOREGROUND_GREEN|FOREGROUND_INTENSITY)		//0x0010
	#define FOREGROUND_HI_RED       (FOREGROUND_RED|FOREGROUND_INTENSITY)		//0x0012
	#define FOREGROUND_HI_WHITE		(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY)	//15
	#define FOREGROUND_HI_LESS_BLUE (FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY)			//11
	#define FOREGROUND_HI_YELLOW (FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY)				//14	
	#define FOREGROUND_HI_PINK (FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY)				//13	

#else
#endif

#include <vector>
#include <string>
#include <map>
#include <stdio.h>

typedef void (*pfnPrint)(const char* str);

class ColorConsole
{
private:
	typedef std::map<std::string, int> ColorMap;
	typedef ColorMap::iterator ColorMapIter;
	ColorMap m_color_map;
	typedef std::map<std::string, std::string> TransMap;
	typedef TransMap::iterator TransMapIter;
	TransMap m_trans_map;

	std::vector<std::string> m_colorstack;
	std::string m_current_color;

	int m_mode;//0 = windows console, 1 = linux consle, 2 = maple console

	pfnPrint CBPrintStr;
#if _IS_WIN32_
	HANDLE hStdout;
#endif

public:
	//construct 
	ColorConsole();

	void Print(const char* instr);
	void Open(bool open)
	{
#if _IS_WIN32_
		if(open)
		{
			if(m_mode==0)
			{
				AllocConsole();
				hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
				//关闭快速编辑，防止阻塞，如果需要复制命令行里的内容按ctrl+m 临时打开快速编辑功能。
				SetConsoleMode(hStdout, /*ENABLE_QUICK_EDIT_MODE | */ENABLE_EXTENDED_FLAGS);
				SetConsoleOutputCP(CP_UTF8);

				//SetConsoleOutputCP(65001);
				CONSOLE_FONT_INFOEX info = { 0 }; // 以下设置字体来支持中文显示。
				info.cbSize = sizeof(info);
				info.dwFontSize.Y = 16; // leave X as zero
				info.FontWeight = FW_BOLD;
				wcscpy_s(info.FaceName, L"MS Gothic");
				SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
			}
		}
		else
		{
			if(m_mode==0)
			{
				FreeConsole();
				hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
			}
		}
#endif
	}
	void SetMode(int m)
	{
		m_mode = m;

		errno_t err;
		FILE* fp;
		if( (err = safe_fopen(&fp, "no_console.ini", "r")) ==0)
		{
			fclose(fp);
			printf("no_console.ini found, so not opening console\n");
		}
		else
		{
#ifdef NO_CONSOLE_IN_BASEUTIL
#else
			Open(true);
#endif
		}
	}
	void SetPrintCallBack(pfnPrint pfn)
	{
		CBPrintStr = pfn;
		if(pfn)
		{
			m_mode = 2;
		}
	}
protected:	
	void PrintStr(const char* str, int len, std::string& trans)
	{
		if(m_mode==0)
		{
#if _IS_WIN32_
			if(hStdout)
			{
				DWORD temp;
				WriteFile(hStdout, str, len, (LPDWORD)&temp, NULL);
			}
			else
			{
				//for(int i=0;i<len;++i)
			}
#endif
		}
		else
		{
			trans.append(str, len);
		}
	}
	void Literal(const char* str, int len, std::string& trans);
};

#endif
