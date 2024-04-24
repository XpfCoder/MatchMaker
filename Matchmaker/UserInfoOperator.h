#pragma once
#include "../SQLiteCPP/SQLiteDBInterface.h"
#include <memory>
#include "../Log/LogCategory.h"
#include "PreDeclare.h"
static std::string getExePath()
{
	std::string path;
	char fileName[MAX_PATH];
	GetModuleFileNameA(0, fileName, sizeof(fileName));
	std::string exe = fileName;
	path = exe.substr(0, exe.rfind("\\") + 1);
	return path;
}

class UserInfoOperator
{
public:
	
	static UserInfoOperator* getInterface();
	~UserInfoOperator();
	bool initUserListDB();
	bool createUserListTable();
	bool isExistResource(const char* key);
	std::unique_ptr<SQLiteDBInterface> m_userInfoSqliteDB;
private:
	UserInfoOperator();
	static UserInfoOperator* m_userInfoOperator;
};
