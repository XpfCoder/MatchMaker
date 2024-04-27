#include "MatchMaker.h"
#include <QMovie>
#include "RegisterAccount.h"
#include "../SQLiteCPP/SQLiteDBInterface.h"
#include "../Log/LogCategory.h"
#include "UserInfoOperator.h"
#include "HomePage.h"
#include <QMessagebox>

MatchMaker::MatchMaker()
{
    ui.setupUi(this);


    QImage image(QString(":/Matchmaker/image/marry.png"));
    ui.loginMarry->setPixmap(QPixmap::fromImage(image));
    ui.loginMarry->setScaledContents(true);
    ui.loginMarry->show();

    GetLogFrameWork()->GetLogInterface()->init(getExePath(), "Log.txt");
    UserInfoOperator::getInterface()->initUserListDB();
    UserInfoOperator::getInterface()->createUserListTable();



    connect(ui.pushButton_register, &QPushButton::clicked, this, &MatchMaker::RegAccount);
    connect(ui.pushButton_login, &QPushButton::clicked, this, &MatchMaker::Login);

}

MatchMaker::~MatchMaker()
{
    if (m_homePage)
    {
        delete m_homePage;
        m_homePage = nullptr;
    }
    if (m_registerAccount)
    {
        delete m_registerAccount;
        m_registerAccount = nullptr;
    }
}


void MatchMaker::Login()
{
    std::string strErr;

    UserInfoOperator::getInterface()->m_userInfoSqliteDB->clearBindinds(strErr);
    int retBind = UserInfoOperator::getInterface()->m_userInfoSqliteDB->bind(1, ui.accountEdit->text().toStdString(), strErr, std::string("read"));

    int retFind = UserInfoOperator::getInterface()->m_userInfoSqliteDB->statementExecQueryStep(std::string("read"));
    if (retFind)
    {
        std::string account;
        std::vector<std::string> StatementList;
        std::string password;
        UserInfoOperator::getInterface()->m_userInfoSqliteDB->getColumn("password", password, strErr, std::string("read"));
        if (!password.compare(ui.passwordEdit->text().toStdString()))
        {
            if (m_homePage == nullptr)
            {
                m_homePage = new HomePage(this);
            }
            m_homePage->show();
        }
        else
        {
            QMessageBox::warning(this, u8"ÌáÊ¾", u8"ÃÜÂë´íÎó!", "OK");
        }
    }
    else
    {
        QMessageBox::warning(this, u8"ÌáÊ¾", u8"Î´×¢²áÕËºÅ!", "OK");

    }
    UserInfoOperator::getInterface()->clearBind(std::string("read"));
}

void MatchMaker::RegAccount()
{
    if (m_registerAccount == nullptr)
    {
        m_registerAccount = new RegisterAccount;
    }
    m_registerAccount->show();
}
