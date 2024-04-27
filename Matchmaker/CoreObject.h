#pragma once
#include <QObject>
#include "ICore.h"
#include "MatchAplication.h"
class StyleAlter;
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

	virtual SARibbonMainWindow* getMainWindow();
public:
	QList<QToolBar*>getListToolBar();  //获取保存的工具条List
	void expendPluginAlter();  //将扩展插件的风格转换
public slots:
	void alterStyle();

private:
	QList<QDockWidget*> m_dockWidgetLst;
	StyleAlter* m_styleAlter{ nullptr };
	QMenu* m_dockWidMenu{ nullptr };
	MatchAplication* m_matchView{ nullptr };
	//用于保存添加的ToolBar
	QList<QToolBar*> m_lstToolBar;
};

