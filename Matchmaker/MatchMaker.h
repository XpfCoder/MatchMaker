#pragma once
#include <QWidget>
#include "ui_MatchMaker.h"
class MatchMaker : public QWidget
{
	Q_OBJECT
public:
	MatchMaker();
	~MatchMaker();

private slots:
	void Login();
	void RegAccount();
private:
	Ui::MatchMaker ui;

	QWidget* m_registerAccount{ nullptr };
	QWidget* m_homePage{ nullptr };
};