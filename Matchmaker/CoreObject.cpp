#include "CoreObject.h"


CoreObject* CoreObject::instance()
{
	static CoreObject obj;
	return &obj;
}

CoreObject::CoreObject()
{
	m_matchView = new MatchAplication();
	//m_solutionWidget = new SolutionWidget(m_studioView);
	//m_renderWidget = new RenderWidget(m_studioView);
	//m_propertyWidget = new PropertyEditWidget(m_studioView);


	//m_resourceWidget = new ResourceViewWidget(m_studioView);
	//m_newHomePageWidget = new NewHomePageWidget(m_studioView);
	////��alterStyle�ź���ۺ�����������

	//m_pPrinter = new QPrinter;
	//connect(m_solutionWidget, &SolutionWidget::alterStyle, this, &CoreObject::showStyle);
	//alterStyle();
}

CoreObject::~CoreObject()
{
	// QMainWindow���ܲ���Ҫ�ͷţ�����ͬ���������ڻ��ȱ��ͷţ�����Ȼ�����Ӷ���
	// �ֶ��ͷţ�����ɶ�����ظ��ͷš����ͷţ��޷����������Ӷ��������������
	//if (m_studioView)
	//{
	//	delete m_studioView;
	//}
	//if (nullptr != m_dockWidMenu)
	//{
	//	delete m_dockWidMenu;
	//}

	//delete m_pPrinter;
}


void CoreObject::init()
{
	m_matchView->show();
}

// ���ͣ������
void CoreObject::addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget)
{
	m_matchView->addDockWidget(area, widget);
}

// ��ӹ�����
void CoreObject::addToolBar(QToolBar* toolbar)
{

}

// ��ȡ�˵���
QMenuBar* CoreObject::menuBar() const
{
	return m_matchView->menuBar();
}

// ��ȡ״̬��
QStatusBar* CoreObject::statusBar() const
{
	return m_matchView->statusBar();
}

QMainWindow* CoreObject::getMainWindow()
{
	return m_matchView;
}