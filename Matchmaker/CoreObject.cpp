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
	////��alterStyle�ź���ۺ�����������

	//m_pPrinter = new QPrinter;
	//connect(m_solutionWidget, &SolutionWidget::alterStyle, this, &CoreObject::showStyle);



	QToolBar* kk = new QToolBar;
	QMenu* pp = new QMenu(u8"������(T)");
	this->menuBar()->addMenu(pp);
	this->addToolBar(kk);
	
	QAction* action = pp->addAction("test");
	kk->addAction(action);
	alterStyle();
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
	expendPluginAlter();  //���ز��ʱ��������ķ��ת��Ϊribbon���
}

// ���ͣ������
void CoreObject::addDockWidget(Qt::DockWidgetArea area, QDockWidget* widget)
{
	m_matchView->addDockWidget(area, widget);
}

// ��ӹ�����
void CoreObject::addToolBar(QToolBar* toolbar)
{
	m_matchView->addToolBar(toolbar);
	m_lstToolBar.push_back(toolbar);  //���򴰿ڹ��������toolbarʱ�����䱣�������Ա��ȡ������Ϣ
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

SARibbonMainWindow* CoreObject::getMainWindow()
{
	return m_matchView;
}

QList<QToolBar*> CoreObject::getListToolBar()  //��ȡ����Ĺ�����List
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
		//ת���˵������
		//m_lstToolBar.push_back(m_solutionWidget->getToolBar());
		m_styleAlter = StyleAlter::install(this, ribbonFilePath);
		//m_lstToolBar.clear();
		m_matchView->ribbonBar()->setContextMenuPolicy(Qt::CustomContextMenu);  //�����Լ����Ҽ���Ӧ

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
		getMainWindow()->ribbonBar()->ribbonTabBar()->hide();//����������
	}
}

void CoreObject::expendPluginAlter()  //��չ������ת��
{
	if (NULL != m_styleAlter)
	{
		m_styleAlter->expendPluginAlter(this);
	}
}