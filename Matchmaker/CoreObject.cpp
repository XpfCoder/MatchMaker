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
	////将alterStyle信号与槽函数链接起来

	//m_pPrinter = new QPrinter;
	//connect(m_solutionWidget, &SolutionWidget::alterStyle, this, &CoreObject::showStyle);
	//alterStyle();
}

CoreObject::~CoreObject()
{
	// QMainWindow可能不需要释放，否则同级顶部窗口会先被释放，但仍然是其子对象。
	// 手动释放，会造成对象的重复释放。不释放，无法进入其他子对象的析构函数。
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

// 添加停靠窗口
void CoreObject::addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget)
{
	m_matchView->addDockWidget(area, widget);
}

// 添加工具栏
void CoreObject::addToolBar(QToolBar* toolbar)
{

}

// 获取菜单栏
QMenuBar* CoreObject::menuBar() const
{
	return m_matchView->menuBar();
}

// 获取状态栏
QStatusBar* CoreObject::statusBar() const
{
	return m_matchView->statusBar();
}

QMainWindow* CoreObject::getMainWindow()
{
	return m_matchView;
}