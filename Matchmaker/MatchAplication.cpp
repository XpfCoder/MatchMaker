#include "MatchAplication.h"
#include "RegisterAccount.h"
#include "../SQLiteCPP/SQLiteDBInterface.h"
#include "../Log/LogCategory.h"
#include "UserInfoOperator.h"
#include <QMessagebox>
#include <QMovie>
#include <QFile>
#include <QDockWidget>
#include <QToolBar>
#include <QHBoxLayout>
#include <QSplitter>
MatchAplication::MatchAplication()
{
	init();

}

MatchAplication::~MatchAplication()
{


}

void MatchAplication::init()
{
	// 设置主窗口
	setWindowTitle(u8"Matchmaker");
	setMinimumSize(1280, 720);
	setWindowState(Qt::WindowMinimized);

	//QString qssPath = QCoreApplication::applicationDirPath() +
	//	"/../../SARibbonBar/resource/qss_DarkStyle/DSblack.css";
	//QFile AppStyleFile(qssPath);
	//if (AppStyleFile.open(QFile::ReadOnly))
	//{
	//	static_cast<QApplication*>(QCoreApplication::instance())->setStyleSheet(AppStyleFile.readAll());
	//}
	// 
	// 
	// 设置中央窗口
	auto frameWidget = new QWidget;
	setCentralWidget(frameWidget);
	
	QSplitter* frameSpliter = new QSplitter;
	frameSpliter->setOrientation(Qt::Horizontal);

	//添加垂直布局，摆放资源视图 上:CentralWidget(含render),下：resourceWidget
	auto frameLayout = new QVBoxLayout;
	frameLayout->addWidget(frameSpliter);
	frameLayout->setContentsMargins(0, 0, 0, 0);

	frameWidget->setLayout(frameLayout);

	m_toolBar = new QToolBar;
	addToolBar(m_toolBar);
	m_maleShowWidget = new QDockWidget;
	m_femaleShowWidget = new QDockWidget;

	addTabifiedDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_maleShowWidget);
	addTabifiedDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_femaleShowWidget);

	m_maleShowWidget->setWindowTitle("Male");
	m_femaleShowWidget->setWindowTitle("Female");

}

void MatchAplication::addTabifiedDockWidget(Qt::DockWidgetArea area, QDockWidget* dockwidget)
{
	addDockWidget(area, dockwidget);
	//int count = tabifiedDockWidgets(dockwidget).count();
	for (auto firstWidget : m_dockWidgets)
	{
		if (dockWidgetArea(firstWidget) == area)
		{
			if (tabifiedDockWidgets(dockwidget).count() == 0)
			{
				tabifyDockWidget(firstWidget, dockwidget);
			}
			else
			{
				auto backWidget = tabifiedDockWidgets(firstWidget).back();
				tabifyDockWidget(backWidget, dockwidget);
			}
			break;
		}
	}
	m_dockWidgets.append(dockwidget);
}

