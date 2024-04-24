#pragma once
#include <QMainWindow>
#include <QUndoCommand> 
#include <functional>


namespace Matchmaker
{

	class ICore
	{
	public:
		// 添加停靠窗口
		virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget) = 0;

		// 添加工具栏
		virtual void addToolBar(QToolBar *toolbar) = 0;

		// 获取菜单栏
		virtual QMenuBar* menuBar() const = 0;

		// 获取状态栏
		virtual QStatusBar* statusBar() const = 0;

		virtual QMainWindow* getMainWindow() = 0;

	
	};

}
