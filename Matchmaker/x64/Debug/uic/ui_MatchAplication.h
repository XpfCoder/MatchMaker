/********************************************************************************
** Form generated from reading UI file 'MatchAplication.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATCHAPLICATION_H
#define UI_MATCHAPLICATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MatchAplication
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MatchAplication)
    {
        if (MatchAplication->objectName().isEmpty())
            MatchAplication->setObjectName(QString::fromUtf8("MatchAplication"));
        MatchAplication->resize(800, 600);
        centralwidget = new QWidget(MatchAplication);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MatchAplication->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MatchAplication);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 18));
        MatchAplication->setMenuBar(menubar);
        statusbar = new QStatusBar(MatchAplication);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MatchAplication->setStatusBar(statusbar);

        retranslateUi(MatchAplication);

        QMetaObject::connectSlotsByName(MatchAplication);
    } // setupUi

    void retranslateUi(QMainWindow *MatchAplication)
    {
        MatchAplication->setWindowTitle(QApplication::translate("MatchAplication", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MatchAplication: public Ui_MatchAplication {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATCHAPLICATION_H
