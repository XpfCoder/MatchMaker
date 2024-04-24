/********************************************************************************
** Form generated from reading UI file 'MatchMaker.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATCHMAKER_H
#define UI_MATCHMAKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MatchMaker
{
public:
    QToolBar *mainToolBar;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *loginMarry;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *accountEdit;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *passwordEdit;
    QPushButton *pushButton_login;
    QLabel *label_password;
    QLabel *label_account;
    QPushButton *pushButton_register;

    void setupUi(QWidget *MatchMaker)
    {
        if (MatchMaker->objectName().isEmpty())
            MatchMaker->setObjectName(QString::fromUtf8("MatchMaker"));
        MatchMaker->resize(600, 400);
        MatchMaker->setMinimumSize(QSize(600, 400));
        MatchMaker->setMaximumSize(QSize(600, 400));
        MatchMaker->setAcceptDrops(true);
        mainToolBar = new QToolBar(MatchMaker);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setGeometry(QRect(0, 0, 2, 8));
        gridLayout_2 = new QGridLayout(MatchMaker);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(MatchMaker);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(frame);


        verticalLayout->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 60, -1, 60);
        loginMarry = new QLabel(MatchMaker);
        loginMarry->setObjectName(QString::fromUtf8("loginMarry"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(loginMarry->sizePolicy().hasHeightForWidth());
        loginMarry->setSizePolicy(sizePolicy);
        loginMarry->setMinimumSize(QSize(280, 0));

        verticalLayout_3->addWidget(loginMarry);


        horizontalLayout_2->addLayout(verticalLayout_3);


        horizontalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        groupBox = new QGroupBox(MatchMaker);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(300, 0));
        gridLayoutWidget = new QWidget(groupBox);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(-1, 9, 291, 281));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(40, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        accountEdit = new QLineEdit(gridLayoutWidget);
        accountEdit->setObjectName(QString::fromUtf8("accountEdit"));

        gridLayout->addWidget(accountEdit, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 4, 2, 1, 1);

        passwordEdit = new QLineEdit(gridLayoutWidget);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setAutoFillBackground(false);
        passwordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordEdit, 2, 2, 1, 1);

        pushButton_login = new QPushButton(gridLayoutWidget);
        pushButton_login->setObjectName(QString::fromUtf8("pushButton_login"));

        gridLayout->addWidget(pushButton_login, 3, 2, 1, 1);

        label_password = new QLabel(gridLayoutWidget);
        label_password->setObjectName(QString::fromUtf8("label_password"));
        label_password->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_password, 2, 1, 1, 1);

        label_account = new QLabel(gridLayoutWidget);
        label_account->setObjectName(QString::fromUtf8("label_account"));
        label_account->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_account, 1, 1, 1, 1);

        pushButton_register = new QPushButton(gridLayoutWidget);
        pushButton_register->setObjectName(QString::fromUtf8("pushButton_register"));
        pushButton_register->setStyleSheet(QString::fromUtf8(".QPushButton{text-align:left;\n"
"\n"
"background-color: rgba(0, 0, 0, 0);\n"
"	\n"
"color: rgb(102, 197, 255);\n"
"/*color: rgb(83, 71, 255);*/\n"
"font: 15px \"Microsoft YaHei UI\";\n"
"border:0px;\n"
"}\n"
".QPushButton:hover{\n"
"color: rgb(255, 24, 65);\n"
"font: 15px \"Microsoft YaHei UI\";\n"
"	text-decoration: underline;\n"
"}"));

        gridLayout->addWidget(pushButton_register, 4, 1, 1, 1);


        horizontalLayout_5->addWidget(groupBox);


        horizontalLayout->addLayout(horizontalLayout_5);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 5);

        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(MatchMaker);

        QMetaObject::connectSlotsByName(MatchMaker);
    } // setupUi

    void retranslateUi(QWidget *MatchMaker)
    {
        MatchMaker->setWindowTitle(QApplication::translate("MatchMaker", "\347\233\270\344\272\262\345\271\263\345\217\260", nullptr));
        loginMarry->setText(QString());
        groupBox->setTitle(QApplication::translate("MatchMaker", "\347\231\273\345\275\225", nullptr));
        passwordEdit->setText(QString());
        pushButton_login->setText(QApplication::translate("MatchMaker", "\347\231\273\345\275\225", nullptr));
        label_password->setText(QApplication::translate("MatchMaker", "\345\257\206\347\240\201", nullptr));
        label_account->setText(QApplication::translate("MatchMaker", "\350\264\246\345\217\267", nullptr));
        pushButton_register->setText(QApplication::translate("MatchMaker", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MatchMaker: public Ui_MatchMaker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATCHMAKER_H
