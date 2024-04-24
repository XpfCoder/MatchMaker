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

	// ���ͣ������
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget);

	// ��ӹ�����
	virtual void addToolBar(QToolBar* toolbar);

	// ��ȡ�˵���
	virtual QMenuBar* menuBar() const;

	// ��ȡ״̬��
	virtual QStatusBar* statusBar() const;

	virtual QMainWindow* getMainWindow();

private:
	MatchAplication* m_matchView{ nullptr };
};

