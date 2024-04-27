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

	// ���ͣ������
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget);

	// ��ӹ�����
	virtual void addToolBar(QToolBar* toolbar);

	// ��ȡ�˵���
	virtual QMenuBar* menuBar() const;

	// ��ȡ״̬��
	virtual QStatusBar* statusBar() const;

	virtual SARibbonMainWindow* getMainWindow();
public:
	QList<QToolBar*>getListToolBar();  //��ȡ����Ĺ�����List
	void expendPluginAlter();  //����չ����ķ��ת��
public slots:
	void alterStyle();

private:
	QList<QDockWidget*> m_dockWidgetLst;
	StyleAlter* m_styleAlter{ nullptr };
	QMenu* m_dockWidMenu{ nullptr };
	MatchAplication* m_matchView{ nullptr };
	//���ڱ�����ӵ�ToolBar
	QList<QToolBar*> m_lstToolBar;
};

