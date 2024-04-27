#include "PopupMenu.h"
#include <QPushButton>

PopupMenu::PopupMenu(QPushButton* button, QWidget* parent) : SARibbonMenu(parent), m_pushBnt(button)
{
}

void PopupMenu::showEvent(QShowEvent* event)
{
	QRect p = this->geometry();
	QRect geo = m_pushBnt->geometry();
	if (nullptr != m_pushBnt && nullptr != m_windows)
	{
		this->move(m_pushBnt->x() + geo.width() + m_windows->x(), m_pushBnt->y() + 
			geo.height() / 2 + p.height() / 2 + m_windows->y());
	}
}
void PopupMenu::setPushBnt(QPushButton* button)
{
	m_pushBnt = button;
}
void PopupMenu::setWindows(SARibbonMainWindow* windows)
{
	m_windows = windows;
}