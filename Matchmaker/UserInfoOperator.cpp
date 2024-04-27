#include "UserInfoOperator.h"
#include <string>


UserInfoOperator* UserInfoOperator::m_userInfoOperator = nullptr;




UserInfoOperator::UserInfoOperator()
{
	std::string strErr;
	m_userInfoSqliteDB.reset(new SQLiteDBInterface);
	bool retVal = m_userInfoSqliteDB->init(getExePath() + "UserInfo.sqlite", strErr);

	if (retVal != 0)
	{
		LogErrorEx(__FUNCTION__, ": init UserInfo's DB failed.");
	}
	//createUserListTable();
	//initUserListDB();
}

UserInfoOperator::~UserInfoOperator()
{
}

UserInfoOperator* UserInfoOperator::getInterface()
{
	if (m_userInfoOperator == nullptr)
	{
		m_userInfoOperator = new UserInfoOperator;
	}
	return m_userInfoOperator;
}


bool UserInfoOperator::initUserListDB()
{
	std::string readSql = "select account, password, regTime from " +
		Resafety::DeepStar::S_USERACCOUNT + " where account = ?";
	std::string writeSql = "insert into " + Resafety::DeepStar::S_USERACCOUNT +
		" (account, password, regTime) values(?,?,?)";
	//std::string writeUpdateSql = "update " + Resafety::DeepStar::S_RESOURCELISTTABLE +
	//	" set md5=?,update_time=?,resource_group=?,resource_id=?, resource_update_flag=? where key=? and type=?";
	//std::string writeUpdateSql1 = "update " + Resafety::DeepStar::S_RESOURCELISTTABLE +
	//	" set md5=?, resource_update_flag =?, update_time=? where key=? and type=?";
	//std::string getResourceInfo = "select key, md5, update_time, type, resource_group, resource_update_flag, \
	//									resource_id from " + S_RESOURCELISTTABLE + " where key=?  and type=?";
	//std::string getResourceInfoWithoutType = "select key, md5, update_time, type, resource_group, resource_update_flag, \
	//									resource_id from " + S_RESOURCELISTTABLE + " where key=? ";
	//std::string updateUpdateFlag = "update " + Resafety::DeepStar::S_RESOURCELISTTABLE + " set resource_update_flag=" +
	//	std::to_string(DONTUPDATE) + " where key=?  and type=?";

	int retRead = UserInfoOperator::getInterface()->m_userInfoSqliteDB->addStatement("read", readSql);
	int retWrite = UserInfoOperator::getInterface()->m_userInfoSqliteDB->addStatement("write", writeSql);
	return 1;

}

bool UserInfoOperator::createUserListTable()
{
	std::string strErr;
	std::string createSql = "CREATE TABLE IF NOT EXISTS " + Resafety::DeepStar::S_USERACCOUNT +
		"(account TEXT, password TEXT, regTime TEXT, PRIMARY KEY (account))";
	int resultRow;
	int retVal = UserInfoOperator::getInterface()->m_userInfoSqliteDB->excDBSql(createSql, resultRow, strErr);
	if (0 != retVal)
	{
		LogErrorEx(__FUNCTION__, ": create ", Resafety::DeepStar::S_USERACCOUNT, " failed");
		return false;
	}
	//m_isFirstLoadRes = true;
	LogInfoEx(__FUNCTION__, ": create ", Resafety::DeepStar::S_USERACCOUNT, " table successed.");
	return true;
}

bool UserInfoOperator::isExistResource(const char* key)
{
	if (m_userInfoSqliteDB)
	{
		std::string strErr;
		bool res = false;
		int retV = m_userInfoSqliteDB->bind(1, key, strErr, std::string("read"));
		if (retV == 0 && m_userInfoSqliteDB->statementExecQueryStep("read"))
		{
			res = true;
		}
		return res;
	}
	return false;
}


bool UserInfoOperator::clearBind(const std::string& statement)
{
	if (!m_userInfoSqliteDB)
	{
		return false;
	}
	std::string strErr;
	int retV = 0;
	if (statement.empty())
	{
		retV |= m_userInfoSqliteDB->clearBindinds(strErr);
		retV |= m_userInfoSqliteDB->statementReset(strErr);
	}
	else
	{
		retV |= m_userInfoSqliteDB->clearBindinds(strErr, statement);
		retV |= m_userInfoSqliteDB->statementReset(strErr, statement);
	}
	if (retV == 0)
		return true;
	LogErrorEx(__FUNCTION__, ": clear bind failed.");
	return false;
}