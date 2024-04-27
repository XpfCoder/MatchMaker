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
#include <QListView>
#include "../SARibbonBar/SARibbonBar.h"
#include "../SARibbonBar/SARibbonCategory.h"
#include "../SARibbonBar/SARibbonPannel.h"
#include "../SARibbonBar/SARibbonToolButton.h"
#include "../SARibbonBar/SAFramelessHelper.h"
#include "StyleItemDelegate.h"
#include <QTreeWidget>
#include "PublicDataObj.h"
#include "ImageItemModel.h"
MatchAplication::MatchAplication() : SARibbonMainWindow(nullptr, false)
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

	m_maleShowWidget = new QDockWidget;
	m_femaleShowWidget = new QDockWidget;

	m_maleShowWidget->setWindowTitle("Male");
	m_femaleShowWidget->setWindowTitle("Female");
	addTabifiedDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_maleShowWidget);
	addTabifiedDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_femaleShowWidget);
	QListView* listView = new QListView;
	std::vector<PublicDataObjPtr> strlist;
	QStringList num;
	for (int i = 0; i < 5; i++)
	{
		auto dataObj = std::make_shared<PublicDataObj>();
		dataObj->m_name = "testNAME";
		dataObj->m_unitPath = "testPATH";
		dataObj->m_imagePath = "";
		strlist.push_back(dataObj);
	}

	listView->setGridSize(QSize(220, 200));
	listView->setContextMenuPolicy(Qt::CustomContextMenu);
	listView->setItemDelegate(new StyleItemDelegate);
	ImageItemModel* model = new ImageItemModel(strlist);
	listView->setModel(model);
	listView->setAcceptDrops(true);
	m_maleShowWidget->setWidget(listView);
	// 添加菜单工具栏
	menuBar()->setMinimumWidth(600);

	//SAFramelessHelper* helper = framelessHelper();
	//helper->setRubberBandOnResize(false);

	//setStatusBar(new QStatusBar());

	//SARibbonBar* ribbon = ribbonBar();
	////通过setContentsMargins设置ribbon四周的间距
	//ribbon->setContentsMargins(5, 0, 5, 0);
	////设置applicationButton

	//ribbon->applicationButton()->setText(("File"));

	////添加主标签页 - 通过addCategoryPage工厂函数添加
	//SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("Main"));
	//categoryMain->setObjectName(("categoryMain"));
	//categoryMain->hide();
	////添加其他标签页 - 直接new SARibbonCategory添加
	//SARibbonCategory* categoryTool = new SARibbonCategory();
	//categoryTool->setCategoryName(tr("Tool"));
	//categoryTool->setObjectName(("categoryTool"));
	//categoryTool->hide();
	////createCategoryOther(categoryOther);
	//ribbon->addCategoryPage(categoryTool);


	////添加删除标签页
	//SARibbonCategory* categoryDelete = new SARibbonCategory();
	//categoryDelete->setContentsMargins(50, 50, 50, 50);
	//categoryDelete->setCategoryName(tr("Delete"));
	//categoryDelete->setObjectName(("categoryDelete"));

	//categoryDelete->hide();
	//ribbon->addCategoryPage(categoryDelete);

	//SARibbonCategory* categorySize = new SARibbonCategory();

	//categorySize->setCategoryName(("Editer"));
	//categorySize->setObjectName(tr("categoryEditor"));
	//categorySize->setSizeIncrement(5, 8);
	//categorySize->hide();
	//ribbon->addCategoryPage(categorySize);


	//setMinimumWidth(500);

	////showMaximized();
	//SARibbonPannel* ToolPannel = new SARibbonPannel();
	//ToolPannel->setObjectName(QString::fromStdString("常用工具"));
	//ToolPannel->setWindowTitle(QString::fromStdString("常用工具"));

	////QAction* actLocateRibbon = createAction(tr("定位"), ":/CorePlugin/Resources/logo.png", "定位");
	////actLocateRibbon->setCheckable(true);
	////ToolPannel->addLargeAction(actLocateRibbon);

	////categoryTool->addPannel(ToolPannel);


	//this->ribbonBar()->setMinimumMode(true);

	//setWindowIcon(QIcon(":/CorePlugin/Resources/logo.png"));//可执行程序图标
	//QMenu* spawnerMenu = new QMenu(u8"地形编辑");
	//this->menuBar()->addMenu(spawnerMenu);

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

