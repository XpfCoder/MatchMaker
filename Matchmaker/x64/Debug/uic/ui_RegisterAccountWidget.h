/********************************************************************************
** Form generated from reading UI file 'RegisterAccountWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERACCOUNTWIDGET_H
#define UI_REGISTERACCOUNTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegAccount
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_6;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *accountEdit;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *passwordEdit;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;
    QLineEdit *passwordOKEdit;

    void setupUi(QWidget *RegAccount)
    {
        if (RegAccount->objectName().isEmpty())
            RegAccount->setObjectName(QString::fromUtf8("RegAccount"));
        RegAccount->resize(400, 322);
        verticalLayoutWidget = new QWidget(RegAccount);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(-1, 9, 401, 311));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 1, 1, 1, 1);

        accountEdit = new QLineEdit(verticalLayoutWidget);
        accountEdit->setObjectName(QString::fromUtf8("accountEdit"));

        gridLayout->addWidget(accountEdit, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 5, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 3, 1, 1);

        passwordEdit = new QLineEdit(verticalLayoutWidget);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));

        gridLayout->addWidget(passwordEdit, 2, 2, 1, 1);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 2, 1, 1, 1);

        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 1, 1, 1);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 4, 2, 1, 1);

        passwordOKEdit = new QLineEdit(verticalLayoutWidget);
        passwordOKEdit->setObjectName(QString::fromUtf8("passwordOKEdit"));

        gridLayout->addWidget(passwordOKEdit, 3, 2, 1, 1);


        verticalLayout_6->addLayout(gridLayout);


        verticalLayout_5->addLayout(verticalLayout_6);

        verticalLayout_5->setStretch(0, 1);

        retranslateUi(RegAccount);

        QMetaObject::connectSlotsByName(RegAccount);
    } // setupUi

    void retranslateUi(QWidget *RegAccount)
    {
        RegAccount->setWindowTitle(QApplication::translate("RegAccount", "Form", nullptr));
        label->setText(QApplication::translate("RegAccount", "\350\264\246\345\217\267", nullptr));
        label_2->setText(QApplication::translate("RegAccount", "\345\257\206\347\240\201", nullptr));
        label_3->setText(QApplication::translate("RegAccount", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        pushButton->setText(QApplication::translate("RegAccount", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegAccount: public Ui_RegAccount {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERACCOUNTWIDGET_H
