#include "LogManager.h"
#include "DeepEyeCore/Utility.h"
#include <string>
#include <cstdarg>
#include <ctime>
#include <iomanip>
#include <filesystem>
namespace fs = std::experimental::filesystem;

//const int BUFSIZE = 10*1024* 1024;
//static char g_logBuffer[BUFSIZE] = {0};
//const char* LEVELINFO[] = { "(Error)\t","(Warn)\t","(Std) \t" ,"(Info) \t","(Insan)\t" };

LogManager::~LogManager()
{
	if (m_ostream.is_open())
	{
		m_ostream.close();
	}
}

void LogManager::setLoggingLevel(Resafety::DeepWorld::LoggingLevel level)
{
	if (level <= Resafety::DeepWorld::LoggingLevel::Start || level >= Resafety::DeepWorld::LoggingLevel::End)
	{
		return;
	}
	m_level = level;
}

bool LogManager::init(const std::string& logDir, const std::string& logFileName)
{
	std::string baseDir = std::string(logDir) + "LogFile\\";
	fs::path fspath = fs::u8path(baseDir);
	fs::create_directories(fspath);
	std::string logFilePath = baseDir + logFileName;
	if (m_ostream.is_open())
	{
		return false;
	}
	fs::path fsLogFilepath = fs::u8path(logFilePath);
	m_ostream.open(fsLogFilepath, std::ios_base::out | std::ios_base::trunc);
	PAF_ASSERT(m_ostream);
	return true;
}

void LogManager::logPrint(const char* msg, Resafety::DeepWorld::LoggingLevel level)
{
	if (m_level < level)
	{
		return;
	}
	time_t tt;
	time(&tt);
	tm t;
	localtime_s(&t, &tt);
	// clear with ""
	m_workstream.str("");
	// day/month/year hour:	minute:second 
	m_workstream
		<< std::setfill('0') << std::setw(2) << t.tm_mday << '/'
		<< std::setfill('0') << std::setw(2) << 1 + t.tm_mon << '/'
		<< std::setfill('0') << std::setw(4) << 1900 + t.tm_year << ' '
		<< std::setfill('0') << std::setw(2) << t.tm_hour << ':'
		<< std::setfill('0') << std::setw(2) << t.tm_min << ':'
		<< std::setfill('0') << std::setw(2) << t.tm_sec << ' ';
	PAF_ASSERT(level > Resafety::DeepWorld::LoggingLevel::Start && level < Resafety::DeepWorld::LoggingLevel::End);
	int l = static_cast<int>(level);
	m_workstream << LEVELINFO[l];
	m_workstream << msg << std::endl;
	m_ostream << m_workstream.str();
	m_ostream.flush();
}

void LogManager::logStandard(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(g_logBuffer, format, args);
	logPrint(g_logBuffer, Resafety::DeepWorld::LoggingLevel::Standard);
	va_end(args);
}

void LogManager::logWarning(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(g_logBuffer, format, args);
	logPrint(g_logBuffer, Resafety::DeepWorld::LoggingLevel::Warnings);
	va_end(args);
}

void LogManager::logError(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(g_logBuffer, format, args);
	logPrint(g_logBuffer, Resafety::DeepWorld::LoggingLevel::Errors);
	va_end(args);
}
