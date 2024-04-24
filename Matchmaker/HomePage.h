#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HomePage.h"

class HomePage : public QWidget
{
    Q_OBJECT

public:
    HomePage(QWidget *parent = Q_NULLPTR);
    ~HomePage();
private:
    Ui::HomePage ui;
};
