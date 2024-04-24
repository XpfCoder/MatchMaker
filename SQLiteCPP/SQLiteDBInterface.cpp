#include "SQLiteDBInterface.h"
#include "SQLiteDBImpl.h"
#include <filesystem>


namespace fs = std::filesystem;
SQLiteDBInterface::SQLiteDBInterface()
{
	m_sqliteDBImpl = new SQLiteDBImpl();
}

SQLiteDBInterface::~SQLiteDBInterface()
{
	if (m_sqliteDBImpl)
	{
		uninit();
		delete m_sqliteDBImpl;
		m_sqliteDBImpl = nullptr;
	}
}

int SQLiteDBInterface::initV2(const std::string &dbFile, std::string& outStrErr, const int openType /*= 1*/, const int busyTimeOut /*= 0*/, const std::string& vfs /*= ""*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl || 0 != createDirectory(dbFile))
	{
		retVal = -1;
		return retVal;
	}
	;
	return m_sqliteDBImpl->initV2(dbFile, outStrErr, openType, busyTimeOut, vfs);
}

int SQLiteDBInterface::init(const std::string& dbFile, std::string& outStrErr, const int openType /*= 1*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl || 0 != createDirectory(dbFile))
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->init(dbFile, outStrErr, openType);
}

int SQLiteDBInterface::uninit()
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->uninit();
}

int SQLiteDBInterface::tableExist(const std::string& tablName)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->tableExist(tablName);
}

int SQLiteDBInterface::tableColumnExists(const std::string& tablName, const std::string& columnName)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->tableColumnExists(tablName, columnName);
}

int SQLiteDBInterface::getSingleValue(const std::string& strQuery, std::string& strResult, std::string& outStrErr)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getSingleValue(strQuery, strResult, outStrErr);
}

int SQLiteDBInterface::getDBFileName(std::string& strDBFileName, std::string& outStrErr)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getDBFileName(strDBFileName, outStrErr);
}

int SQLiteDBInterface::excDBSql(const std::string& inStrDBSql, int& outResultRow, std::string& outStrErr, TransactionFlag flag /*= TRANSACTION_FLAG_UNUSED*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	if (flag == TRANSACTION_FLAG_UNUSED)
	{
		return m_sqliteDBImpl->excDBSql(inStrDBSql, outResultRow, outStrErr);
	}
	else if (flag == TRANSACTION_FLAG_BEGIN)
	{
		if (!m_sqliteDBImpl->transactionBegin(outStrErr) && !m_sqliteDBImpl->excDBSql(inStrDBSql, outResultRow, outStrErr))
		{
			return retVal;
		}
		return -1;
	}
	else if (flag == TRANSACTION_FLAG_COMMIT)
	{		
		if (!m_sqliteDBImpl->excDBSql(inStrDBSql, outResultRow, outStrErr) && !m_sqliteDBImpl->transactionCommit(outStrErr))
		{
			return retVal;
		}
		return -1;
	}
	return retVal;
}

int SQLiteDBInterface::initStatement(const std::string& inStrQuerySql, std::string& outStrException, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->initStatement(inStrQuerySql, outStrException, statementName);
}

bool SQLiteDBInterface::statementExecQueryStep(const std::string& statementName /*= std::string("default")*/)
{
	bool retVal = false;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = false;
		return retVal;
	}
	return m_sqliteDBImpl->statementExecQueryStep(statementName);
}

int SQLiteDBInterface::statementExecQueryStep(std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->statementExecQueryStep(outStrErr, statementName);
}

int SQLiteDBInterface::statementReset(std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->statementReset(outStrErr, statementName);
}

int SQLiteDBInterface::getTableColumnName(const unsigned int& inColumnIndex, std::string& outStrName, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getTableColumnName(inColumnIndex, outStrName, outStrErr, statementName);
}

int SQLiteDBInterface::getTableColumnCount(unsigned int& outTotalColumnCount, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getTableColumnCount(outTotalColumnCount, outStrErr, statementName);
}

int SQLiteDBInterface::getIndex(const std::string& name, unsigned int& outIndex, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getIndex(name, outIndex, statementName);
}


int SQLiteDBInterface::bindNoCopy(const int index, const std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bindNoCopy(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const int index, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(index, outStrErr, statementName);
}

int SQLiteDBInterface::bindNoCopy(const std::string& name, const std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bindNoCopy(name, value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const std::string& name, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(name, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const int index, const int value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const int index, const unsigned int value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const int index, const long long value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const int index, const double value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const int index, const char* value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const int index, const std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const char* name, const int value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(std::string(name), value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const char* name, const unsigned int value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(std::string(name), value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const char* name, const long long value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(std::string(name), value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const char* name, const double value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(std::string(name), value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const char* name, const char* value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(std::string(name), value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const char* name, const std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(std::string(name), value, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const int index, const void* value, uint32_t valueSize, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(index, value, valueSize, outStrErr, statementName);
}

int SQLiteDBInterface::bind(const char* name, const void* value, uint32_t valueSize, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->bind(std::string(name), value, valueSize, outStrErr, statementName);
}

int SQLiteDBInterface::clearBindinds(std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->clearBindinds(outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const int index, int& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumn(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const int index, unsigned int& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumn(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const int index, long long& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumn(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const int index, double &value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumn(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const int index, std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumnString(index, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const char* name, int& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumn(name, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const char* name, std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumnString(name, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const char* name, double &value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumn(name, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const char* name, long long& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumn(name, value, outStrErr, statementName);
}

int SQLiteDBInterface::getColumn(const char* name, unsigned int& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumn(name, value, outStrErr, statementName);
}

bool SQLiteDBInterface::isExistStatement(const char* statementName)
{
	if (nullptr == m_sqliteDBImpl)
	{
		return false;
	}
	return m_sqliteDBImpl->isExistStatement(statementName);
}

int SQLiteDBInterface::addStatement(const char* statementName, const std::string& inStrQuerySql)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	if (m_sqliteDBImpl->isExistStatement(statementName))
	{
		return retVal;
	}
	if (!m_sqliteDBImpl->addStatement(statementName))
	{
		std::string outStrException;
		return initStatement(inStrQuerySql, outStrException, std::string(statementName));
	}
	return 1;
}

int SQLiteDBInterface::removeStatement(const char* statementName)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->removeStatement(statementName);
}

int SQLiteDBInterface::getAllStatement(std::vector<std::string> & statementList)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getAllStatement(statementList);
}

bool SQLiteDBInterface::statementExec(const std::string& statementName /*= std::string("default")*/)
{
	bool retVal = false;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = false;
		return retVal;
	}
	return m_sqliteDBImpl->statementExec(statementName);
}

int SQLiteDBInterface::statementExec(std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->statementExec(outStrErr, statementName);
}

int32_t SQLiteDBInterface::createDirectory(const std::string &filePath)
{
	fs::path temp = fs::absolute(fs::u8path(filePath));
	size_t dirPathLen = temp.u8string().length();
	if (dirPathLen > MAX_PATH_LEN || dirPathLen <= 0)
	{
		return -1;
	}

	fs::path fsDirectoryPath = temp.parent_path();
	if (!fs::exists(fsDirectoryPath))
	{
		return !fs::create_directories(fsDirectoryPath);
	}
	else
	{
		return 0;
	}
}

int SQLiteDBInterface::transactionCommit(std::string& outStrErr)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->transactionCommit(outStrErr);
}

int SQLiteDBInterface::transactionBegin(std::string& outStrErr)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->transactionBegin(outStrErr);
}

int SQLiteDBInterface::getColumnBlob(const int index, const void*& value, int& blobSize, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumnBlob(index, value,blobSize, outStrErr, statementName);
}

int SQLiteDBInterface::getColumnBlob(const char* name, const void*& value, int& blobSize, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (nullptr == m_sqliteDBImpl)
	{
		retVal = -1;
		return retVal;
	}
	return m_sqliteDBImpl->getColumnBlob(name, value, blobSize, outStrErr, statementName);
}


