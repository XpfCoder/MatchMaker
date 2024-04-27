

#include <stdio.h>
#include <iomanip>
#include <time.h>
#include <direct.h>

#include "LogFrameWork.h"
#include "./log_imp/LogImp.h"




LogFrameWork::LogFrameWork()
{
	m_log_interface = new CLogInterface();

}

LogFrameWork::~LogFrameWork()
{
	delete m_log_interface;
}

LogFrameWork* LogFrameWork::GetLogFrameWork()
{
	static LogFrameWork p ;
	return &p;
}


ILogInterface* LogFrameWork::GetLogInterface()
{
	return m_log_interface;
}


LogFrameWork* GetLogFrameWork()
{
	return LogFrameWork::GetLogFrameWork();
}
