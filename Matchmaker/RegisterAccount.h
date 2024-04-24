#pragma once
#include <QWidget>
#include "ui_RegisterAccountWidget.h"
class RegisterAccount : public QWidget
{ 
	Q_OBJECT
public:
	RegisterAccount();


private slots:
	bool RegisteredUser();
private:
	Ui::RegAccount ui;
};