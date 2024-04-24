#pragma once
#include <QObject>
#include "ICore.h"
#include "MatchAplication.h"
class CoreObject : public QObject, public Matchmaker::ICore
{
	Q_OBJECT
private:
	CoreObject();
	~CoreObject();
public:

	void init();
	static CoreObject* instance();

	// 添加停靠窗口
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget);

	// 添加工具栏
	virtual void addToolBar(QToolBar* toolbar);

	// 获取菜单栏
	virtual QMenuBar* menuBar() const;

	// 获取状态栏
	virtual QStatusBar* statusBar() const;

	virtual QMainWindow* getMainWindow();

private:
	MatchAplication* m_matchView{ nullptr };
};

