#pragma once

//#include "DeepWorldCore\DeepWorldApi\DeepWorldDef.h"
#include "../MatchMaker/Utility.h"
class ILogInterface;
class LogFrameWork
{
private:
	LogFrameWork();;

public:
	virtual ~LogFrameWork();

	static LogFrameWork* GetLogFrameWork();

	virtual ILogInterface* GetLogInterface();;

private:
	ILogInterface*		m_log_interface;
};


LogFrameWork* GetLogFrameWork();

