/********************************************************************************
** Form generated from reading UI file 'HomePage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMEPAGE_H
#define UI_HOMEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HomePage
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *HomePageTitle;
    QSpacerItem *horizontalSpacer;
    QLabel *logo;
    QHBoxLayout *horizontalLayout_4;
    QGridLayout *gridLayout;
    QPushButton *maleDoor;
    QGridLayout *gridLayout_2;
    QPushButton *famaleDoor;

    void setupUi(QWidget *HomePage)
    {
        if (HomePage->objectName().isEmpty())
            HomePage->setObjectName(QString::fromUtf8("HomePage"));
        HomePage->resize(800, 600);
        HomePage->setMinimumSize(QSize(800, 600));
        HomePage->setMaximumSize(QSize(800, 600));
        verticalLayoutWidget = new QWidget(HomePage);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 801, 606));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        HomePageTitle = new QPushButton(verticalLayoutWidget);
        HomePageTitle->setObjectName(QString::fromUtf8("HomePageTitle"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(HomePageTitle->sizePolicy().hasHeightForWidth());
        HomePageTitle->setSizePolicy(sizePolicy);
        HomePageTitle->setMinimumSize(QSize(200, 40));
        HomePageTitle->setMaximumSize(QSize(400, 80));
        HomePageTitle->setLayoutDirection(Qt::LeftToRight);
        HomePageTitle->setStyleSheet(QString::fromUtf8(".QPushButton\n"
"{text-align:left;\n"
"background-color: rgba(0, 0, 0, 0);\n"
"color: rgb(102, 197, 255);\n"
"font: 60px \"Microsoft YaHei UI\";\n"
"text-align : center;\n"
"border:0px;\n"
"}\n"
".QPushButton:hover{\n"
"color: rgb(255, 24, 65);\n"
"font: 60px \"Microsoft YaHei UI\";\n"
"text-align : center;\n"
"}"));

        horizontalLayout_5->addWidget(HomePageTitle);

        horizontalSpacer = new QSpacerItem(10, 100, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        logo = new QLabel(verticalLayoutWidget);
        logo->setObjectName(QString::fromUtf8("logo"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(logo->sizePolicy().hasHeightForWidth());
        logo->setSizePolicy(sizePolicy1);
        logo->setMinimumSize(QSize(150, 150));
        logo->setMaximumSize(QSize(150, 150));
        logo->setStyleSheet(QString::fromUtf8("background-image: url(\":/Matchmaker/image/marry.png\");\n"
"background-repeat: no-repeat; \n"
"background-position: left center;\n"
""));
        logo->setMargin(0);

        horizontalLayout_5->addWidget(logo);

        horizontalLayout_5->setStretch(0, 7);
        horizontalLayout_5->setStretch(1, 1);
        horizontalLayout_5->setStretch(2, 2);

        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(2, 2, 2, 2);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        maleDoor = new QPushButton(verticalLayoutWidget);
        maleDoor->setObjectName(QString::fromUtf8("maleDoor"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(maleDoor->sizePolicy().hasHeightForWidth());
        maleDoor->setSizePolicy(sizePolicy2);
        maleDoor->setLayoutDirection(Qt::LeftToRight);
        maleDoor->setStyleSheet(QString::fromUtf8(".QPushButton{text-align:left;\n"
"background-color: rgba(0, 0, 0, 0);\n"
"image:url(\":/Matchmaker/image/male.png\");\n"
"color: rgb(102, 197, 255);\n"
"/*color: rgb(83, 71, 255);*/\n"
"font: 15px \"Microsoft YaHei UI\";\n"
"text-align : center;\n"
"text-align : bottom;\n"
"}\n"
".QPushButton:hover{\n"
"color: rgb(155, 24, 165);\n"
"font: 15px \"Microsoft YaHei UI\";\n"
"border: 2px solid rgba(155, 124, 65,100);\n"
"text-align : center;\n"
"text-align : bottom;\n"
"}"));

        gridLayout->addWidget(maleDoor, 0, 0, 1, 1);


        horizontalLayout_4->addLayout(gridLayout);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        famaleDoor = new QPushButton(verticalLayoutWidget);
        famaleDoor->setObjectName(QString::fromUtf8("famaleDoor"));
        sizePolicy.setHeightForWidth(famaleDoor->sizePolicy().hasHeightForWidth());
        famaleDoor->setSizePolicy(sizePolicy);
        famaleDoor->setMinimumSize(QSize(0, 438));
        famaleDoor->setStyleSheet(QString::fromUtf8(".QPushButton{text-align:left;\n"
"background-color: rgba(0, 0, 0, 0);\n"
"image:url(\":/Matchmaker/image/female.png\");\n"
"color: rgb(102, 197, 255);\n"
"/*color: rgb(83, 71, 255);*/\n"
"font: 15px \"Microsoft YaHei UI\";\n"
"text-align : center;\n"
"text-align : bottom;\n"
"}\n"
".QPushButton:hover{\n"
"color: rgb(155, 24, 165);\n"
"font: 15px \"Microsoft YaHei UI\";\n"
"border: 2px solid rgba(155, 124, 65,100);\n"
"text-align : center;\n"
"text-align : bottom;\n"
"}"));

        gridLayout_2->addWidget(famaleDoor, 0, 0, 1, 1);


        horizontalLayout_4->addLayout(gridLayout_2);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 5);

        retranslateUi(HomePage);

        QMetaObject::connectSlotsByName(HomePage);
    } // setupUi

    void retranslateUi(QWidget *HomePage)
    {
        HomePage->setWindowTitle(QApplication::translate("HomePage", "Form", nullptr));
        HomePageTitle->setText(QApplication::translate("HomePage", "\347\233\270\344\272\262\347\256\241\347\220\206\345\271\263\345\217\260", nullptr));
        logo->setText(QString());
        maleDoor->setText(QApplication::translate("HomePage", "Male", nullptr));
        famaleDoor->setText(QApplication::translate("HomePage", "Famale", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HomePage: public Ui_HomePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEPAGE_H
