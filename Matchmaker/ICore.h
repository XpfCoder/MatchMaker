#pragma once
#include <QMainWindow>
#include <QUndoCommand> 
#include <functional>


namespace Matchmaker
{

	class ICore
	{
	public:
		// ���ͣ������
		virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget) = 0;

		// ��ӹ�����
		virtual void addToolBar(QToolBar *toolbar) = 0;

		// ��ȡ�˵���
		virtual QMenuBar* menuBar() const = 0;

		// ��ȡ״̬��
		virtual QStatusBar* statusBar() const = 0;

		virtual QMainWindow* getMainWindow() = 0;

	
	};

}
