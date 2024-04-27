#include "CoreObject.h"
#include "StyleAlter.h"
#include <filesystem>
#include <QString>
#include "../SARibbonBar/SARibbonQuickAccessBar.h"
#include "../SARibbonBar/SARibbonTabBar.h"



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



	QToolBar* kk = new QToolBar;
	QMenu* pp = new QMenu(u8"工具箱(T)");
	this->menuBar()->addMenu(pp);
	this->addToolBar(kk);
	
	QAction* action = pp->addAction("test");
	kk->addAction(action);
	alterStyle();
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
	expendPluginAlter();  //加载插件时，将插件的风格转换为ribbon风格
}

// 添加停靠窗口
void CoreObject::addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget)
{
	m_matchView->addDockWidget(area, widget);
}

// 添加工具栏
void CoreObject::addToolBar(QToolBar* toolbar)
{
	m_matchView->addToolBar(toolbar);
	m_lstToolBar.push_back(toolbar);  //在向窗口工具栏添加toolbar时，将其保存起来以便获取其中信息
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

SARibbonMainWindow* CoreObject::getMainWindow()
{
	return m_matchView;
}

QList<QToolBar*> CoreObject::getListToolBar()  //获取保存的工具条List
{
	return m_lstToolBar;
}

void CoreObject::alterStyle()
{
	std::string path = "";
	GetExePath(path);


	//QString ribbonFilePath = QString::fromStdString(path) + "";
	std::string ribbonFilePath = path.append("plant/SARibbonComponent/ConfigXML/ADPSARibbonComponent.xml");
	std::filesystem::path temp = std::filesystem::absolute(std::filesystem::u8path(ribbonFilePath));
	if (std::filesystem::exists(temp) && !std::filesystem::is_directory(temp))
	{
		//转换菜单栏风格
		//m_lstToolBar.push_back(m_solutionWidget->getToolBar());
		m_styleAlter = StyleAlter::install(this, ribbonFilePath);
		//m_lstToolBar.clear();
		m_matchView->ribbonBar()->setContextMenuPolicy(Qt::CustomContextMenu);  //创建自己的右键响应

		m_dockWidMenu = new QMenu();

		/*
		m_dockWidgetLst = m_studioView->getDockWidgets();
		for (int index = 0; index < m_dockWidgetLst.size(); index++)
		{
			QAction* tmp = new QAction();
			tmp->setCheckable(true);
			m_dockWigActLst.push_back(tmp);
			m_dockWidMenu->addAction(tmp);
		}
		*/
		connect(getMainWindow()->ribbonBar(), &SARibbonBar::customContextMenuRequested,
			[&](const QPoint& point) {
				//hideDockWidgets();
				QPoint realPoint = QPoint(point.x() + m_matchView->x(), point.y() + m_matchView->y());
				m_dockWidMenu->move(realPoint);
				m_dockWidMenu->exec();
			});

		getMainWindow()->show();
		getMainWindow()->ribbonBar()->show();
		getMainWindow()->ribbonBar()->rightButtonGroup()->hide();
		getMainWindow()->ribbonBar()->quickAccessBar()->hide();
		getMainWindow()->ribbonBar()->setFixedHeight(36);
		getMainWindow()->ribbonBar()->ribbonTabBar()->hide();//隐藏相关组件
	}
}

void CoreObject::expendPluginAlter()  //扩展插件风格转换
{
	if (NULL != m_styleAlter)
	{
		m_styleAlter->expendPluginAlter(this);
	}
}