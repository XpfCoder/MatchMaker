#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MatchAplication.h"
#include "../SARibbonBar/SARibbonMainWindow.h"

class MatchAplication : public SARibbonMainWindow
{
    Q_OBJECT

public:
    MatchAplication();
    ~MatchAplication();

    void init();
    void addTabifiedDockWidget(Qt::DockWidgetArea area, QDockWidget* dockwidget);
private:
    QDockWidget* m_maleShowWidget{ nullptr };
    QDockWidget* m_femaleShowWidget{ nullptr };
    QDockWidget* m_centerWidget{ nullptr };
    
private:
    QList<QDockWidget*> m_dockWidgets;
};
