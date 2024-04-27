#pragma once
#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <QMenu>
#include "../SARibbonBar/SARibbonMenu.h"
#include "../SARibbonBar/SARibbonMainWindow.h"
class QPushButton;
class QWidget;

class PopupMenu : public SARibbonMenu
{
	Q_OBJECT
public:
	explicit PopupMenu(QPushButton* button, QWidget* parent = 0);
	void showEvent(QShowEvent* event);
	void setPushBnt(QPushButton* button);
	void setWindows(SARibbonMainWindow* windows);
private:
	QPushButton* m_pushBnt{ nullptr };
	SARibbonMainWindow* m_windows{ nullptr };
};

#endif // POPUPMENU_H

