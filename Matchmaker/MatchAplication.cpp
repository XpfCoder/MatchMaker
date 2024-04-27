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
	// ����������
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
	// �������봰��
	auto frameWidget = new QWidget;
	setCentralWidget(frameWidget);
	
	QSplitter* frameSpliter = new QSplitter;
	frameSpliter->setOrientation(Qt::Horizontal);

	//��Ӵ�ֱ���֣��ڷ���Դ��ͼ ��:CentralWidget(��render),�£�resourceWidget
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
	// ��Ӳ˵�������
	menuBar()->setMinimumWidth(600);

	//SAFramelessHelper* helper = framelessHelper();
	//helper->setRubberBandOnResize(false);

	//setStatusBar(new QStatusBar());

	//SARibbonBar* ribbon = ribbonBar();
	////ͨ��setContentsMargins����ribbon���ܵļ��
	//ribbon->setContentsMargins(5, 0, 5, 0);
	////����applicationButton

	//ribbon->applicationButton()->setText(("File"));

	////�������ǩҳ - ͨ��addCategoryPage�����������
	//SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("Main"));
	//categoryMain->setObjectName(("categoryMain"));
	//categoryMain->hide();
	////���������ǩҳ - ֱ��new SARibbonCategory���
	//SARibbonCategory* categoryTool = new SARibbonCategory();
	//categoryTool->setCategoryName(tr("Tool"));
	//categoryTool->setObjectName(("categoryTool"));
	//categoryTool->hide();
	////createCategoryOther(categoryOther);
	//ribbon->addCategoryPage(categoryTool);


	////���ɾ����ǩҳ
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
	//ToolPannel->setObjectName(QString::fromStdString("���ù���"));
	//ToolPannel->setWindowTitle(QString::fromStdString("���ù���"));

	////QAction* actLocateRibbon = createAction(tr("��λ"), ":/CorePlugin/Resources/logo.png", "��λ");
	////actLocateRibbon->setCheckable(true);
	////ToolPannel->addLargeAction(actLocateRibbon);

	////categoryTool->addPannel(ToolPannel);


	//this->ribbonBar()->setMinimumMode(true);

	//setWindowIcon(QIcon(":/CorePlugin/Resources/logo.png"));//��ִ�г���ͼ��
	//QMenu* spawnerMenu = new QMenu(u8"���α༭");
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

