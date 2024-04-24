#include "RegisterAccount.h"
#include "../SQLiteCPP/SQLiteDBInterface.h"
#include "UserInfoOperator.h"
#include "../Log/LogCategory.h"
//#include "md5.h"


RegisterAccount::RegisterAccount()
{
	ui.setupUi(this);
	connect(ui.pushButton, &QPushButton::clicked, this, &RegisterAccount::RegisteredUser);
}

bool RegisterAccount::RegisteredUser()
{
	if (ui.passwordEdit->text() != ui.passwordOKEdit->text())
	{
		LogErrorEx(__FUNCTION__, ": Register Account failed, The two passwords entered are inconsistent");
		return false;
	}
	time_t now = time(NULL);
	std::string RegisterTime = time_str_2(now);
	std::string strErr;
	int retV = UserInfoOperator::getInterface()->m_userInfoSqliteDB->bind(1, ui.accountEdit->text().toStdString(), strErr, std::string("write"));
	retV |= UserInfoOperator::getInterface()->m_userInfoSqliteDB->bind(2, ui.passwordEdit->text().toStdString(), strErr, std::string("write"));
	retV |= UserInfoOperator::getInterface()->m_userInfoSqliteDB->bind(3, RegisterTime, strErr, std::string("write"));
	UserInfoOperator::getInterface()->m_userInfoSqliteDB->statementExec("write");
	return 1;
}


