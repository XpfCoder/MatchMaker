#include "SQLiteDBImpl.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
SQLiteDBImpl::SQLiteDBImpl()
{
	initPtr();
}

SQLiteDBImpl::~SQLiteDBImpl()
{
	uninit();
}

int SQLiteDBImpl::initV2(const std::string dbFile, std::string& outStrErr, const int openType /*= 1*/, const int busyTimeOut /*= 0*/, const std::string& vfs /*= ""*/)
{
	int retVal = 0;
	// check the param
	if (true == dbFile.empty())
	{
		retVal = 1;
		return retVal;
	}
	// initial the database
	if (nullptr == m_pDatabase)
	{
		retVal = 3;
		return retVal;
	}
	// initial base params
	try
	{
		int ret_init = m_pDatabase->initV2(dbFile.c_str(), outStrErr, openType, busyTimeOut, vfs);
		if (-1 == ret_init)
		{
			retVal = 4;

			return retVal;
		}
		// check the file status
		fs::path temp = fs::u8path(dbFile);
		std::fstream file(temp.string());
		if (false == file.good())
		{
			// file doesnt exist
			retVal = 2;
			return retVal;
		}
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 5;
		return retVal;
	}
	return retVal;
}

int SQLiteDBImpl::init(const std::string& dbFile, std::string& outStrErr, const int openType /*= 1*/)
{
	int retVal = 0;
	// check the param
	if (true == dbFile.empty())
	{
		retVal = 1;
		return retVal;
	}
	// initial the database
	if (nullptr == m_pDatabase)
	{
		retVal = 3;
		return retVal;
	}
	// initial base params
	try
	{
		int ret_init = m_pDatabase->init(dbFile.c_str(), outStrErr, openType);
		if (-1 == ret_init)
		{
			retVal = 4;

			return retVal;
		}
		// check the file status
		fs::path temp = fs::u8path(dbFile);
		std::fstream file(temp);
		if (false == file.good())
		{
			// file doesnt exist
			retVal = 2;
			return retVal;
		}
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 5;
		return retVal;
	}
	return retVal;
}

int SQLiteDBImpl::uninit()
{
	if (m_pStatementList.size()>0)
	{
		for (auto it : m_pStatementList)
		{
			delete it.second;
		}
		m_pStatementList.clear();
	}
	return 0;
}

int SQLiteDBImpl::tableExist(const std::string& tablName)
{
	int retVal = 0;
	if (nullptr == m_pDatabase)
	{
		retVal = -3;
		return retVal;
	}
	if (tablName.empty())
	{
		retVal = -2;
		return retVal;
	}
	m_pDatabase->tableExists(tablName) ? retVal = 0 : retVal = 1;
	return retVal;
}

int SQLiteDBImpl::tableColumnExists(const std::string& tablName, const std::string& columnName)
{
	int retVal = 0;
	if (nullptr == m_pDatabase)
	{
		retVal = -3;
		return retVal;
	}
	if (tablName.empty())
	{
		retVal = -2;
		return retVal;
	}
	m_pDatabase->tableColumnExists(tablName.c_str(),columnName.c_str()) ? retVal = 0 : retVal = 1;
	return retVal;
}

int SQLiteDBImpl::getSingleValue(const std::string& strQuery, std::string& strResult, std::string& outStrErr)
{
	int retVal = 0;
	if (strQuery.empty())
	{
		retVal = 1;
		return retVal;
	}
	if (nullptr == m_pDatabase)
	{
		retVal = 2;
		return retVal;
	}

	try
	{
		const std::string str = m_pDatabase->execAndGet(strQuery);
		strResult = str;
	}
	catch (SQLite::Exception& ex)
	{
		std::string str(ex.getErrorStr());
		outStrErr = str;
		retVal = 3;
	}
	return retVal;
}

int SQLiteDBImpl::getDBFileName(std::string& strDBFileName, std::string& outStrErr)
{
	int retVal = 0;
	if (nullptr == m_pDatabase)
	{
		retVal = -1;
		return retVal;
	}
	try
	{
		strDBFileName = m_pDatabase->getFilename();
	}
	catch (SQLite::Exception& ex)
	{
		std::string str(ex.getErrorStr());
		outStrErr = str;
	}
	return retVal;
}

int SQLiteDBImpl::excDBSql(const std::string& inStrDBSql, int& outResultRow, std::string& outStrErr)
{
	int retVal = 0;
	if (nullptr == m_pDatabase)
	{
		retVal = -4;
		return retVal;
	}

	try
	{
		//if not initialise the file
		if (true == m_pDatabase->getFilename().empty())
		{
			retVal = -2;
			return retVal;
		}
		//if sql string is empty
		if (true == inStrDBSql.empty())
		{
			retVal = -3;
			return retVal;
		}
		//execute the sql 
		outResultRow = m_pDatabase->exec(inStrDBSql);
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = ex.getErrorStr();
		retVal = -4;
		return retVal;
	}
	return retVal;
}

int SQLiteDBImpl::initStatement(const std::string& inStrQuerySql, std::string& outStrException, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;
	if (inStrQuerySql.empty())
	{
		retVal = 1;
		return retVal;
	}
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end())
	{
		retVal = 2;
		return retVal;
	}
	if (m_pDatabase->getFilename().empty())
	{
		retVal = 4;
		return retVal;
	}
	try
	{
		m_pStatementList.find(statementName)->second->initParams(m_pDatabase.get(), inStrQuerySql);
	}
	catch (SQLite::Exception& ex)
	{
		outStrException = std::string(ex.getErrorStr());
		retVal = 5;
		return retVal;
	}
	return retVal;
}

bool SQLiteDBImpl::statementExecQueryStep(const std::string& statementName /*= std::string("default")*/)
{
	bool retVal = true;

	// 2.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = false;
		return retVal;
	}

	try
	{
		retVal = m_pStatementList.find(statementName)->second->executeStep();
	}
	catch (SQLite::Exception& ex)
	{
		std::string str = std::string(ex.getErrorStr());
		retVal = false;
	}

	return retVal;
}

int SQLiteDBImpl::statementExecQueryStep(std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{

	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	// 2.call the function
	try
	{
		bool res = m_pStatementList.find(statementName)->second->executeStep();
		if (!res)
		{
			retVal = 3;
		}
	}
	catch (SQLite::Exception& ex)
	{
		retVal = 2;
		outStrErr = std::string(ex.getErrorStr());
	}

	return retVal;
}

bool SQLiteDBImpl::statementExec(const std::string& statementName /*= std::string("default")*/)
{
	bool retVal = true;

	// 2.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = false;
		return retVal;
	}

	try
	{
		retVal = m_pStatementList.find(statementName)->second->exec();
	}
	catch (SQLite::Exception& ex)
	{
		std::string str = std::string(ex.getErrorStr());
		retVal = false;
	}

	return retVal;
}

int SQLiteDBImpl::statementExec(std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	// 2.call the function
	try
	{
		retVal = m_pStatementList.find(statementName)->second->exec();
	}
	catch (SQLite::Exception& ex)
	{
		retVal = 2;
		outStrErr = std::string(ex.getErrorStr());
	}

	return retVal;
}

int SQLiteDBImpl::statementReset(std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		m_pStatementList.find(statementName)->second->reset();
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 2;
	}

	return retVal;
}

int SQLiteDBImpl::getTableColumnName(const unsigned int& inColumnIndex, std::string& outStrName, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		outStrName = m_pStatementList.find(statementName)->second->getColumnName(inColumnIndex);
	}
	catch (SQLite::Exception& ex)
	{
		retVal = 2;
		outStrErr = std::string(ex.getErrorStr());
	}

	return retVal;
}

int SQLiteDBImpl::getTableColumnCount(unsigned int& outTotalColumnCount, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		outTotalColumnCount = m_pStatementList.find(statementName)->second->getColumnCount();
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 2;
	}

	return retVal;
}

int SQLiteDBImpl::getIndex(const std::string& name, unsigned int& outIndex, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	// 2. name is empty,  error, return 2
	if (name.empty())
	{
		retVal = 2;
		return retVal;
	}

	// 3.
	outIndex = m_pStatementList.find(statementName)->second->getIndex(name.c_str());

	return retVal;
}



 
 int SQLiteDBImpl::bind(const int index, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
 {
 	int retVal = 0;
 
 	// 1.m_pDatabase is nullptr
 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
 	{
 		retVal = 1;
 		return retVal;
 	}
 
 	try
 	{
		m_pStatementList.find(statementName)->second->bind(index);
 	}
 	catch (SQLite::Exception& ex)
 	{
 		outStrErr = std::string(ex.getErrorStr());
 		retVal = 2;
 	}
 
 	return retVal;
 }

 int SQLiteDBImpl::bind(const int index, const void* value, uint32_t valueSize, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
 {
	 int retVal = 0;

	 // 1.m_pDatabase is nullptr
	 if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	 {
		 retVal = 1;
		 return retVal;
	 }

	 try
	 {
		 m_pStatementList.find(statementName)->second->bind(index, value, valueSize);
	 }
	 catch (SQLite::Exception& ex)
	 {
		 outStrErr = std::string(ex.getErrorStr());
		 retVal = 2;
	 }

	 return retVal;
 }

 int SQLiteDBImpl::bind(const std::string& name, const void* value, uint32_t valueSize, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
 {
	 int retVal = 0;

	 // 1.m_pDatabase is nullptr
	 if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	 {
		 retVal = 1;
		 return retVal;
	 }

	 try
	 {
		 m_pStatementList.find(statementName)->second->bindNoCopy(name, value, valueSize);
	 }
	 catch (SQLite::Exception& ex)
	 {
		 outStrErr = std::string(ex.getErrorStr());
		 retVal = 2;
	 }

	 return retVal;
 }

// int SQLiteDBImpl::bind(const int index, const int value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
// {
// 	int retVal = 0;
// 
// 	// 1.m_pDatabase is nullptr
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 
// 	try
// 	{
// 		m_pStatement->bind(index, value);
// 	}
// 	catch (SQLite::Exception& ex)
// 	{
// 		outStrErr = std::string(ex.getErrorStr());
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::bind(const int index, const unsigned int value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 
// 	// 1.m_pDatabase is nullptr
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 
// 	try
// 	{
// 		m_pStatement->bind(index, value);
// 	}
// 	catch (SQLite::Exception& ex)
// 	{
// 		outStrErr = std::string(ex.getErrorStr());
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::bind(const int index, const long long value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 
// 	// 1.m_pDatabase is nullptr
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 
// 	try
// 	{
// 		m_pStatement->bind(index, value);
// 	}
// 	catch (SQLite::Exception& ex)
// 	{
// 		outStrErr = std::string(ex.getErrorStr());
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::bind(const int index, const double value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 
// 	// 1.m_pDatabase is nullptr
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 
// 	try
// 	{
// 		m_pStatement->bind(index, value);
// 	}
// 	catch (SQLite::Exception& ex)
// 	{
// 		outStrErr = std::string(ex.getErrorStr());
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::bind(const int index, const char* value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 
// 	// 1.m_pDatabase is nullptr
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 
// 	try
// 	{
// 		m_pStatement->bind(index, value);
// 	}
// 	catch (SQLite::Exception& ex)
// 	{
// 		outStrErr = std::string(ex.getErrorStr());
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::bind(const int index, const std::string& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 
// 	// 1.m_pDatabase is nullptr
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 
// 	try
// 	{
// 		m_pStatement->bind(index, value);
// 	}
// 	catch (SQLite::Exception& ex)
// 	{
// 		outStrErr = std::string(ex.getErrorStr());
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }

//int SQLiteDBImpl::bind(const std::string& name, const int value, std::string& outStrErr)
//{
//	int retVal = 0;
//
//	// 1.m_pDatabase is nullptr
//	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
//	{
//		retVal = 1;
//		return retVal;
//	}
//
//	try
//	{
//		m_pStatement->bind(name, value);
//	}
//	catch (SQLite::Exception& ex)
//	{
//		outStrErr = std::string(ex.getErrorStr());
//		retVal = 2;
//	}
//
//	return retVal;
//}
//
//int SQLiteDBImpl::bind(const std::string& name, const unsigned int value, std::string& outStrErr)
//{
//	int retVal = 0;
//
//	// 1.m_pDatabase is nullptr
//	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
//	{
//		retVal = 1;
//		return retVal;
//	}
//
//	try
//	{
//		m_pStatement->bind(name, value);
//	}
//	catch (SQLite::Exception& ex)
//	{
//		outStrErr = std::string(ex.getErrorStr());
//		retVal = 2;
//	}
//
//	return retVal;
//}
//
//int SQLiteDBImpl::bind(const std::string& name, const long long value, std::string& outStrErr)
//{
//	int retVal = 0;
//
//	// 1.m_pDatabase is nullptr
//	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
//	{
//		retVal = 1;
//		return retVal;
//	}
//
//	try
//	{
//		m_pStatement->bind(name, value);
//	}
//	catch (SQLite::Exception& ex)
//	{
//		outStrErr = std::string(ex.getErrorStr());
//		retVal = 2;
//	}
//
//	return retVal;
//}
//
//int SQLiteDBImpl::bind(const std::string& name, const double value, std::string& outStrErr)
//{
//	int retVal = 0;
//
//	// 1.m_pDatabase is nullptr
//	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
//	{
//		retVal = 1;
//		return retVal;
//	}
//
//	try
//	{
//		m_pStatement->bind(name, value);
//	}
//	catch (SQLite::Exception& ex)
//	{
//		outStrErr = std::string(ex.getErrorStr());
//		retVal = 2;
//	}
//
//	return retVal;
//}
//
//int SQLiteDBImpl::bind(const std::string& name, const char* value, std::string& outStrErr)
//{
//	int retVal = 0;
//
//	// 1.m_pDatabase is nullptr
//	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
//	{
//		retVal = 1;
//		return retVal;
//	}
//
//	try
//	{
//		m_pStatement->bind(name, value);
//	}
//	catch (SQLite::Exception& ex)
//	{
//		outStrErr = std::string(ex.getErrorStr());
//		retVal = 2;
//	}
//
//	return retVal;
//}
//
//
//int SQLiteDBImpl::bind(const std::string& name, const std::string& value, std::string& outStrErr)
//{
//	int retVal = 0;
//
//	// 1.m_pDatabase is nullptr
//	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
//	{
//		retVal = 1;
//		return retVal;
//	}
//
//	try
//	{
//		m_pStatement->bind(name, value);
//	}
//	catch (SQLite::Exception& ex)
//	{
//		outStrErr = std::string(ex.getErrorStr());
//		retVal = 2;
//	}
//
//	return retVal;
//}

int SQLiteDBImpl::bindNoCopy(const int index, const std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		m_pStatementList.find(statementName)->second->bindNoCopy(index, value);
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 2;
	}

	return retVal;
}


int SQLiteDBImpl::bindNoCopy(const std::string& name, const std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		m_pStatementList.find(statementName)->second->bindNoCopy(name, value);
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 2;
	}

	return retVal;
}

int SQLiteDBImpl::bind(const std::string& name, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		m_pStatementList.find(statementName)->second->bind(name);
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 2;
	}

	return retVal;
}

int SQLiteDBImpl::clearBindinds(std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		m_pStatementList.find(statementName)->second->clearBindings();
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 2;
	}

	return retVal;
}

int SQLiteDBImpl::getColumnString(const int index, std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}
	try
	{
		value = m_pStatementList.find(statementName)->second->getColumn(index).getString();
	}
	catch (SQLite::Exception *e)
	{
		outStrErr = e->getErrorStr();
		retVal = 2;
	}

	return retVal;
}

int SQLiteDBImpl::getColumnString(const char* name, std::string& value, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}
	try
	{
		value = m_pStatementList.find(statementName)->second->getColumn(name).getString();
	}
	catch (SQLite::Exception *e)
	{
		outStrErr = e->getErrorStr();
		retVal = 2;
	}

	return retVal;
}

int SQLiteDBImpl::getColumnBlob(const int index, const void*& value, int& blobSize, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}
	try
	{
		value = m_pStatementList.find(statementName)->second->getColumn(index).getBlob();
		blobSize = m_pStatementList.find(statementName)->second->getColumn(index).getBytes();
	}
	catch (SQLite::Exception *e)
	{
		outStrErr = e->getErrorStr();
		retVal = 2;
	}

	return retVal;
}

int SQLiteDBImpl::getColumnBlob(const char* name, const void*& value, int& blobSize, std::string& outStrErr, const std::string& statementName /*= std::string("default")*/)
{
	int retVal = 0;

	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}
	try
	{
		value = m_pStatementList.find(statementName)->second->getColumn(name).getBlob();
		blobSize = m_pStatementList.find(statementName)->second->getColumn(name).getBytes();
	}
	catch (SQLite::Exception *e)
	{
		outStrErr = e->getErrorStr();
		retVal = 2;
	}

	return retVal;
}

// int SQLiteDBImpl::getColumn(const int index, int& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(index).getInt();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const int index, unsigned int& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(index).getUInt();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const int index, long long& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(index).getInt64();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const int index, double &value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(index).getDouble();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const int index, std::string& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(index).getString();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const char* name, int& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(std::string(name)).getInt();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const char* name, unsigned int& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(std::string(name)).getUInt();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const char* name, long long& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(std::string(name)).getInt64();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const char* name, double &value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(std::string(name)).getDouble();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }
// 
// int SQLiteDBImpl::getColumn(const char* name, std::string& value, std::string& outStrErr)
// {
// 	int retVal = 0;
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 	try
// 	{
// 		value = m_pStatement->getColumn(std::string(name)).getString();
// 	}
// 	catch (SQLite::Exception *e)
// 	{
// 		outStrErr = e->getErrorStr();
// 		retVal = 2;
// 	}
// 
// 	return retVal;
// }

int SQLiteDBImpl::transactionCommit(std::string& outStrErr)
{
	int retVal = 0;
	if (!m_pTransaction)
	{
		return retVal;
	}
	// 1.m_pDatabase is nullptr
	if (!m_pDatabase  || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		m_pTransaction->commit();
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 2;
	}

	return retVal; 
}

int SQLiteDBImpl::transactionBegin(std::string& outStrErr)
{
	int retVal = 0;
	if (!m_pTransaction)
	{
		m_pTransaction.reset(new SQLite::Transaction(*m_pDatabase));
	}
	// 1.m_pDatabase is nullptr
	if (!m_pDatabase || !m_pTransaction || m_pDatabase->getFilename().empty())
	{
		retVal = 1;
		return retVal;
	}

	try
	{
		m_pTransaction->begin();
	}
	catch (SQLite::Exception& ex)
	{
		outStrErr = std::string(ex.getErrorStr());
		retVal = 2;
	}

	return retVal;
}

bool SQLiteDBImpl::isExistStatement(const char* name)
{
	return m_pStatementList.find(name) != m_pStatementList.end();
}

int SQLiteDBImpl::addStatement(const char* name)
{
	if (!isExistStatement(name))
	{
		SQLite::Statement *stmt = new SQLite::Statement;
		m_pStatementList.insert(std::make_pair(name, stmt));
		return 0;
	}
	return 1;
}

int SQLiteDBImpl::removeStatement(const char* name)
{
	if (!isExistStatement(name))
	{
		SQLite::Statement *stmt = new SQLite::Statement;
		m_pStatementList.insert(std::make_pair(name, stmt));
		return 0;
	}
	return 1;
}

int SQLiteDBImpl::getAllStatement(std::vector<std::string> & statementList)
{
	if (m_pStatementList.size() > 0)
	{
		for (auto it : m_pStatementList)
		{
			statementList.push_back(it.first);
		}
		return 0;
	}
	return 1;
}

void SQLiteDBImpl::initPtr()
{
	m_pDatabase.reset(new SQLite::Database);
	SQLite::Statement *stmt = new SQLite::Statement;
	m_pStatementList.insert(std::make_pair("default", stmt));
	/*m_pStatement.reset(new SQLite::Statement);*/
}

// int SQLiteDBImpl::preQuery(const unsigned int& columnIndex)
// {
// 	int retVal = 0;
// 
// 	// 1.m_pDatabase is nullptr
// 	if (!m_pDatabase || m_pStatementList.find(statementName) == m_pStatementList.end() || m_pDatabase->getFilename().empty())
// 	{
// 		retVal = 1;
// 		return retVal;
// 	}
// 
// 	unsigned int count_column_total = m_pStatement->getColumnCount();
// 
// 	if ((0 >= count_column_total) || (columnIndex >= count_column_total))
// 	{
// 		retVal = 2;
// 		return retVal;
// 	}
// 
// 	return retVal;
// }

