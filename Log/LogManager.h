#pragma once
#include "../DeepWorldApi/LogManagerInterface.h"
#include <fstream>
#include <sstream>
#include <string>

//class LogManager : public Resafety::DeepWorld::ILogManager
//{
//public:
//	~LogManager();
//public:
//	virtual void setLoggingLevel(Resafety::DeepWorld::LoggingLevel level) override;
//	virtual bool init(const std::string& logDir,const std::string& logFileName) override;
//public:
//	void logStandard(const char* format, ...);
//	void logError(const char* format, ...);
//	void logWarning(const char* format, ...);
//private:
//	void logPrint(const char* msg, Resafety::DeepWorld::LoggingLevel level);
//private:
//	Resafety::DeepWorld::LoggingLevel m_level{ Resafety::DeepWorld::LoggingLevel::Standard };
//	std::ofstream m_ostream;
//	std::ostringstream m_workstream;
//};

