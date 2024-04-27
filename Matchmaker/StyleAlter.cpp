#include "StyleAlter.h"
#include <QtWidgets\QMainWindow>
#include <QStatusBar>
#include<QObject>
#include<QObjectList>
#include <filesystem>
#include <QSettings>
#include<QHBoxLayout>
#include<QList>
#include<QLabel>
#include<QFileInfo>
#include<QString>
#include "../SARibbonBar/SAFramelessHelper.h"
#include "../SARibbonBar/SARibbonQuickAccessBar.h"
#include "../Log/LogCategory.h"
#include "../SARibbonBar/SARibbonMenu.h"

//#include "SQLite3/sqlite3.h"
//#include <sqlite3.h>

//#ifdef _DEBUG
//#pragma comment(lib, "sqlite3d.lib")
//#else
//#pragma comment(lib, "sqlite3.lib")
//#endif

//namespace fs = std::experimental::filesystem;
#pragma execution_character_set("utf-8")//让能够正常显示中文字符串

StyleAlter::~StyleAlter()
{
	delete(m_page);
	delete(m_pannel);
	delete(m_builder);
	delete(m_lstWidgetHisTory);
	delete(m_widget);
	delete(m_appMainMenu);
}

StyleAlter::StyleAlter(SARibbonMainWindow* window) //: SARibbonBar(window)
{
	m_builder = new SARibbonBuilder(window);
	m_page = new SARibbonCategory();
	m_page->setObjectName(QObject::tr("Unknown"));
	m_page->setWindowTitle(QObject::tr("Unknown"));
	m_pannel = new SARibbonPannel();
	m_pannel->setObjectName(QObject::tr("Unknown Controls"));
	m_pannel->setWindowTitle(QObject::tr("Unknown Controls"));
	m_page->addPannel(m_pannel);
}
//
//QList<QList<QString>> readHistoryInfo()
//{
//	std::string path = "";
//	QList<QList<QString>> historyFilesLst;
//	GetExePath(path);
//	QString historyFilePath = QString::fromStdString(path).toUtf8() + u8"plant/ADPCore/ADPAssetDetails.adp";
//	QFileInfo fileInfo(historyFilePath);
//	sqlite3* sqlConnect = nullptr;
//	if (fileInfo.isFile())
//	{
//		std::string utf8DBFilePath = historyFilePath.toStdString();
//		int result = sqlite3_open_v2(utf8DBFilePath.c_str(), &sqlConnect,
//			SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
//			| SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);
//		if (SQLITE_OK != result)
//		{
//			sqlConnect = nullptr;
//			LogInfoEx(__FUNCTION__, u8"plant/ADPCore/ADPAssetDetails.adp 打开失败");
//		}
//		//SQL语句
//		const char* sqlSentence = "SELECT id, src_file, cache_file, modify_time FROM adp_assets;";
//		sqlite3_stmt* stmt = nullptr;
//		int resultSent = sqlite3_prepare_v2(sqlConnect, sqlSentence, -1, &stmt, nullptr);
//		if (SQLITE_OK == resultSent)
//		{
//			while (sqlite3_step(stmt) == SQLITE_ROW)
//			{
//				const unsigned char* id = sqlite3_column_text(stmt, 0);
//				const unsigned char* srcFile = sqlite3_column_text(stmt, 1);
//				const unsigned char* cacheFile = sqlite3_column_text(stmt, 2);
//				const unsigned char* modifyTime = sqlite3_column_text(stmt, 3);
//				QList<QString>tmpLst;
//				tmpLst.push_back(QString::fromStdString((char*)id));
//				tmpLst.push_back(QString::fromStdString((char*)srcFile));
//				tmpLst.push_back(QString::fromStdString((char*)cacheFile));
//				tmpLst.push_back(QString::fromStdString((char*)modifyTime));
//				historyFilesLst.push_back(tmpLst);
//			}
//		}
//	}
//	else
//	{
//		LogInfoEx(__FUNCTION__, u8"plant/ADPCore/ADPAssetDetails.adp 不存在");
//	}
//	return historyFilesLst;
//}

StyleAlter* StyleAlter::install(CoreObject* coreObject, std::string& ribbonFilePath)
{
	SARibbonMainWindow* window = coreObject->getMainWindow();
	QList<QToolBar*> lstToolBar = coreObject->getListToolBar();
	// 该window已安装过QRibbon
	if (qobject_cast<StyleAlter*>(window->menuBar()) || window->isUseRibbon())
	{
		return NULL;
	}
	QMenuBar* menuBar = window->menuBar();
	auto ribbonWidget = new StyleAlter(window);
	ribbonWidget->initialize(window, menuBar, lstToolBar, ribbonFilePath);
	return ribbonWidget;
}

void StyleAlter::initialize(SARibbonMainWindow* window, QMenuBar* menuBar, QList<QToolBar*> lstToolBar, std::string& ribbonFilePath)
{
	getMenuBarMessage(window, menuBar);
	getToolBarMessage(window, lstToolBar);
	delete(menuBar);

	window->setMenuWidget(new SARibbonBar(window));


	window->setRibbonTheme(SARibbonMainWindow::NormalTheme);

	//window->setRibbonTheme(SARibbonMainWindow::Office2013);
	//window->setRibbonTheme(SARibbonMainWindow::NormalTheme);
	/*SAFramelessHelper* helper = window->framelessHelper();
	helper->setRubberBandOnResize(false);*/
	window->setWindowState(Qt::WindowMaximized);
	SAFramelessHelper* helper = window->framelessHelper();
	helper->setRubberBandOnResize(false);
	
	//将应用按钮设置为一个SARibbonApplicationButton 
	m_appButton = new SARibbonApplicationButton();
	window->ribbonBar()->setApplicationButton(m_appButton);
	m_appButton->setIcon(QIcon(":CorePlugin/Resources/appMain.png"));
	

	window->ribbonBar()->setMinimumMode(true);  //设置ribbon默认隐藏
	window->ribbonBar()->showMinimumModeButton(true);  //用于显示将ribbon栏收起来的图标
	//window->ribbonBar()->setWindowIcon(QIcon("E:/code3.0/Kernel/bin/bin_64/plant/SARibbonComponent/icon/test/folder.png"));
	
	window->setWindowState(Qt::WindowMaximized);
	
	buildSARibbonMenu(window, ribbonFilePath);
	if (nullptr != m_builder->m_vBoxLayAct)
	{
		m_appMainMenu = new SARibbonMenu();
		m_appButton->setMenu(m_appMainMenu);
		QHBoxLayout* hLayOut = new QHBoxLayout;
		m_vBoxLayActTwo = new QVBoxLayout;
		m_builder->m_vBoxLayAct->setSpacing(2);
		m_builder->m_vBoxLayAct->addStretch();

		//m_vBoxLayActTwo->addWidget(new QLabel(QObject::tr("Recently used solutions:")));//最近使用的解决方案：

		//m_lstWidgetHisTory = new QListWidget;
		//QList<QList<QString>> historyInfo = readHistoryInfo();
		//for (int index = 0; index < historyInfo.size(); index++)
		//{
		//	QListWidgetItem* item = new QListWidgetItem;
		//	QString tmpInfo = historyInfo[index][1].mid(historyInfo[index][1].lastIndexOf("/")+1);
		//	item->setText(tmpInfo);
		//	m_lstWidgetHisTory->addItem(item);
		//}
		//m_vBoxLayActTwo->addWidget(m_lstWidgetHisTory);
		//m_lstWidgetHisTory->setFixedWidth(300);

		hLayOut->addLayout(m_builder->m_vBoxLayAct);
		//hLayOut->addLayout(m_vBoxLayActTwo);
		hLayOut->setSpacing(2);
		m_appMainMenu->setLayout(hLayOut);
		m_appButton->setMenu(m_appMainMenu);
		//connect(m_appMainMenu, &SARibbonMenu::aboutToShow, this, &StyleAlter::showAppMain);
		//connect(m_appMainMenu, &SARibbonMenu::aboutToHide, this, &StyleAlter::hideAppMain);
	}
	else
	{
		m_appButton->hide();
	}
	QList<QAction*>lstAct = m_pannel->actions();
	if(!lstAct.empty())
	{
#ifdef _DEBUG
		window->ribbonBar()->addCategoryPage(m_page);
#endif
	}
}

void StyleAlter::getQAcions(QList<QAction*> lstAct, SARibbonBuilder* builder, CoreObject* coreObject)
{
	for (auto iterMenu : lstAct)
	{
		auto menu = iterMenu->menu();
		QList<QAction*> actions;
		// 当没有菜单时，表示为QAction
		if (!menu)
		{
			if (iterMenu->isSeparator() || !iterMenu->isVisible())
			{
				continue;
			}
			QString text = iterMenu->text();
			if (text.isEmpty()) //当菜单栏中action的text为空时，当做未知的控件
			{
				QString name = m_name + QString::number(m_num);
				m_num++;
				iterMenu->setText(name);
				iterMenu->setIcon(QIcon("plant/SARibbonComponent/icon/test/BusinessClose.ico"));
				m_pannel->addLargeAction(iterMenu);
			}
			else
			{
				QString ns = iterMenu->text();
				builder->addQAction(iterMenu->text().toStdString(), iterMenu);
				if (NULL != coreObject)
				{
					buildActionOrToolButton(coreObject, iterMenu);
				}
			}
		}
		else
		{
			actions = menu->actions();
			getQAcions(actions, builder, coreObject);
		}
	}
}

//获取之前版本的菜单栏中的组件信息
void StyleAlter::getMenuBarMessage(SARibbonMainWindow* window, QMenuBar* menuBar) 
{
	if (!menuBar)
	{
		return;
	}
	auto menus = menuBar->actions();
	getQAcions(menus, m_builder);
}

void StyleAlter::getToolBarMessage(SARibbonMainWindow* window, QList<QToolBar*> lstToolBar)
{
	for (QToolBar* tmpToolBar : lstToolBar)
	{
		QList<QAction*>tmpLstAct = tmpToolBar->actions();
		for (QAction* act : tmpLstAct)
		{
			if (!act->isWidgetType())
			{
				if (act->text().isEmpty() && !act->isSeparator() && act->isVisible())
				{
					QString name = m_name + QString::number(m_num);
					m_num++;
					act->setText(name);
					act->setToolTip(name);
					act->setIcon(QIcon("plant/SARibbonComponent/icon/test/BusinessClose.ico"));
					m_pannel->addLargeAction(act);
				}
				else
				{
					m_builder->addQAction(act->text().toStdString(), act);
				}
			}
		}
		window->removeToolBar(tmpToolBar);
	}
	lstToolBar.clear();
}

void StyleAlter::buildSARibbonMenu(SARibbonMainWindow* mWindow, std::string& ribbonFilePath)
{
	m_builder->readXMLConfig(mWindow, ribbonFilePath, m_pannel);
}
void StyleAlter::showRibbonBarRmenu(const QPoint& point)
{
	m_builder->getRMenu(point, "Default");
}

SARibbonBuilder* StyleAlter::getBuilder()
{
	return m_builder;
}

QMenu* hasMenu(const QList<CategoryTree*>& path, int index, QList<QAction*>lstAct)
{
	for (auto iterMenu : lstAct)
	{
		auto menu = iterMenu->menu();
		QList<QAction*> actions;
		// 当没有菜单时
		if (!menu)
		{
			continue;
		}
		else
		{
			QString menuName = menu->objectName();
			QString catTreeName = QString::fromStdString(path[index]->m_attrMp.at("name"));
			if (menuName == catTreeName)
			{
				return menu;
			}
			actions = menu->actions();
			QMenu* tmpMenu = hasMenu(path, index, actions);
			if (NULL != tmpMenu)
			{
				return tmpMenu;
			}
		}
	}
	return NULL;
}
bool hasAction(const QList<CategoryTree*>& path, QList<QAction*>lstAct)
{
	for (auto iterMenu : lstAct)
	{
		auto menu = iterMenu->menu();
		QList<QAction*> actions;
		// 当没有菜单时
		if (!menu)
		{
			QString text = iterMenu->text();
			if (path.back()->m_attrMp["name"] == text.toStdString())
			{
				return true;
			}
		}
		else
		{
			actions = menu->actions();
			return hasAction(path, actions);
		}
	}
	return false;
}
void reBuildAction(CategoryTree* tmpNode, QAction* action)
{
	if (!tmpNode->m_attrMp["text"].empty())
	{
		tmpNode->m_attrMp["name"] = tmpNode->m_attrMp["text"];
		action->setText(QString::fromStdString(tmpNode->m_attrMp["text"]));
	}
	if (!tmpNode->m_attrMp["icon"].empty())
	{
		action->setIcon(QIcon(QString::fromStdString(tmpNode->m_attrMp["icon"])));
	}
	if (!tmpNode->m_attrMp["toolTip"].empty())
	{
		action->setToolTip(QString::fromStdString(tmpNode->m_attrMp["toolTip"]));
	}
}
void reBuildMenu(CategoryTree* tmpNode, QMenu* menu)
{
	if (!tmpNode->m_attrMp["title"].empty())
	{
		menu->setTitle(QString::fromStdString(tmpNode->m_attrMp["title"]));
	}
	if (!tmpNode->m_attrMp["icon"].empty())
	{
		menu->setIcon(QIcon(QString::fromStdString(tmpNode->m_attrMp["icon"])));
	}
	if (!tmpNode->m_attrMp["name"].empty())
	{
		menu->setObjectName(QString::fromStdString(tmpNode->m_attrMp["name"]));
	}
}

void StyleAlter::buildAppButton(const QList<CategoryTree*>& path, int index, SARibbonMenu* menu, QAction* action)
{
	QList<QAction*>lstAct;
	if (NULL != menu)
	{
		lstAct = menu->actions();
	}
	bool hasAct = hasAction(path, lstAct);
	if (!hasAct)
	{
		CategoryTree* tmpNode = path[index];
		if (path.size() - 1 == index)
		{
			reBuildAction(tmpNode, action);
			menu->addAction(action);
		}
		else
		{
			QMenu* tmpMenu = hasMenu(path, index, lstAct);
			if (NULL == tmpMenu)
			{
				tmpMenu = new SARibbonMenu();
				if (NULL == menu)
				{
					menu = (SARibbonMenu*)tmpMenu;
				}
				else
				{
					menu->addMenu(tmpMenu);
				}
				reBuildMenu(tmpNode, tmpMenu);
				tmpMenu->addAction(action);
				tmpNode = path[index + 1];
				reBuildAction(tmpNode, action);
			}
			else
			{
				tmpNode = path[index + 1];
				reBuildAction(tmpNode, action);
				tmpMenu->addAction(action);
			}
		}
	}
	else
	{
		QString message = "ribbon 在菜单栏中该路径下已经存在：";
		message += m_builder->getControlMessage(action);
		LogInfoEx(message.toStdString());
	}
}
void StyleAlter::buildQuick(const QList<CategoryTree*>& path, int index, QAction* action, SARibbonQuickAccessBar*quick, SARibbonButtonGroupWidget* rightBar)
{
	QList<QAction*>lstAct;
	if (NULL != quick)
	{
		lstAct = quick->actions();
	}
	bool hasAct = hasAction(path, lstAct);
	if (!hasAct)
	{
		CategoryTree* tmpNode = path[index];
		if (path.size() - 1 == index)
		{
			reBuildAction(tmpNode, action);
			if (NULL != quick)
			{
				quick->addAction(action);
			}
			else
			{
				rightBar->addAction(action);
			}
		}
		else
		{
			QMenu* tmpMenu = hasMenu(path, index, lstAct);
			if (NULL == tmpMenu)
			{
				
				tmpMenu = new SARibbonMenu();
				tmpMenu->addAction(action);
				reBuildMenu(tmpNode, tmpMenu);
				tmpNode = path[index + 1];
				reBuildAction(tmpNode, action);
				if (NULL != quick)
				{
					quick->addMenu(tmpMenu);
				}
				else
				{
					rightBar->addMenu(tmpMenu);
				}
				
			}
			else
			{
				tmpNode = path[index + 1];
				reBuildAction(tmpNode, action);
				tmpMenu->addAction(action);
			}
		}
	}
	else
	{
		QString message = "ribbon 在菜单栏中该路径下已经存在：";
		message += m_builder->getControlMessage(action);
		LogInfoEx(message.toStdString());
	}
}
void StyleAlter::buildMenu(const QList<CategoryTree*>& path, int index, SARibbonPannel* pannel, QAction* action, QToolButton* toolBuntton)
{
	QList<QAction*>lstAct = pannel->actions();
	bool hasAct = hasAction(path, lstAct);
	if (!hasAct)
	{
		CategoryTree* tmpNode = path[index];
		if (path.size() - 1 == index)
		{
			rProportionMP comModel = m_builder->getConpentModel();
			reBuildAction(tmpNode, action);
			pannel->addAction(action,comModel[tmpNode->m_attrMp["IconModel"]]);
		}
		else
		{
			QMenu* tmpMenu = hasMenu(path, index, lstAct);
			if (NULL == tmpMenu)
			{
				tmpMenu = new SARibbonMenu();
				reBuildMenu(tmpNode, tmpMenu);
				tmpMenu->addAction(action);
				rProportionMP comModel = m_builder->getConpentModel();
				tButPopupModeMP toolModel = m_builder->getToolBntModel();
				pannel->addMenu(tmpMenu, comModel[tmpNode->m_attrMp["IconModel"]], toolModel[tmpNode->m_attrMp["ActionBtnModel"]]);//m_toolBntModel
				tmpNode = path[index + 1];
				reBuildAction(tmpNode, action);
			}
			else
			{
				tmpNode = path[index + 1];
				reBuildAction(tmpNode, action);
				tmpMenu->addAction(action);
			}
		}
	}
	else
	{
		QString message = "ribbon 在菜单栏中该路径下已经存在：";
		message += m_builder->getControlMessage(action);
		LogInfoEx(message.toStdString());
	}
}


void StyleAlter::hideAppMain()
{
	m_appButton->setStyleSheet("SARibbonApplicationButton{background:#555555}");
	m_appButton->setStyleSheet("SARibbonApplicationButton::hover{border - top:1px solid #333333;background: #a9a9a9;}");
	
}
//void StyleAlter::showAppMain()
//{
//	if (nullptr != m_vBoxLayActTwo)
//	{
//		m_lstWidgetHisTory->clear();
//		QList<QList<QString>> historyInfo = readHistoryInfo();
//		for (int index = 0; index < historyInfo.size(); index++)
//		{
//			int n = historyInfo[index][1].lastIndexOf("/");
//			QListWidgetItem* item = new QListWidgetItem;
//			QString tmpInfo = historyInfo[index][1].mid(historyInfo[index][1].lastIndexOf("/") + 1);
//			item->setText(tmpInfo);
//			m_lstWidgetHisTory->addItem(item);
//		}
//		m_vBoxLayActTwo->addWidget(m_lstWidgetHisTory);
//	}
//}
//void StyleAlter::showAppMain(const QPoint& point, QAbstractButton* appBut, QList<QString>& historyName)
//{
//	if (m_appMainMenu == NULL)
//	{
//		m_appMainMenu = new SARibbonMenu(appBut);
//		QHBoxLayout* hLayOut = new QHBoxLayout;
//		QVBoxLayout* vlayout1 = new QVBoxLayout;
//		QVBoxLayout* vlayout2 = new QVBoxLayout;
//		vlayout1->setSpacing(2);
//		for (QAction* act : m_builder->m_appMainActLst)
//		{
//			QPushButton* button1 = new QPushButton(act->text());
//			button1->setIcon(act->icon());
//			button1->setFlat(true);
//			vlayout1->addWidget(button1);
//			button1->setFixedSize(130, 35);
//			connect(button1, &QPushButton::clicked, act, &QAction::triggered);
//		}
//		vlayout1->addStretch();
//
//		vlayout2->addWidget(new QLabel("最近使用的解决方案："));
//		
//		m_lstWidgetHisTory = new QListWidget;
//		for (int index = 0; index < historyName.size(); index++)
//		{
//			QListWidgetItem* item = new QListWidgetItem;
//			item->setText(historyName[index]);
//			m_lstWidgetHisTory->addItem(item);
//		}
//		vlayout2->addWidget(m_lstWidgetHisTory);
//		m_lstWidgetHisTory->setFixedWidth(300);
//
//		hLayOut->addLayout(vlayout1);
//		hLayOut->addLayout(vlayout2);
//		hLayOut->setSpacing(2);
//		m_appMainMenu->setLayout(hLayOut);
//	}
//
//	bool bhide = m_appMainMenu->isHidden();
//	if (bhide)
//	{
//		m_appMainMenu->move(point);
//		m_appMainMenu->exec();
//	}
//}

void StyleAlter::buildPannel(const QList<CategoryTree*>& path, int index, SARibbonCategory* page, QAction* action, QToolButton* toolBuntton)
{
	QList<SARibbonPannel*>pannels = page->pannelList();
	bool hasPannel = false;
	for (SARibbonPannel* pannel : pannels)
	{
		std::string pannelName = pannel->objectName().toStdString();
		if (pannelName == path[index]->m_attrMp["name"])
		{
			hasPannel = true;
			if (NULL != action)
			{
				buildMenu(path, index + 1, pannel, action);
			}
			else
			{
				QString toolTip = toolBuntton->toolTip();
				toolBuntton->setText(toolTip);
				if (!path.back()->m_attrMp["text"].empty())
				{
					path.back()->m_attrMp["name"] = path.back()->m_attrMp["text"];
					toolBuntton->setText(QString::fromStdString(path.back()->m_attrMp["text"]));
				}
				if (!path.back()->m_attrMp["icon"].empty())
				{
					toolBuntton->setIcon(QIcon(QString::fromStdString(path.back()->m_attrMp["icon"])));
					toolBuntton->setIconSize(QSize(20, 21));
				}
				if (!path.back()->m_attrMp["toolTip"].empty())
				{
					toolBuntton->setToolTip(QString::fromStdString(path.back()->m_attrMp["toolTip"]));
				}
				toolBuntton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
				toolBuntton->show();
				pannel->addLargeWidget(toolBuntton);
			}
		}
	}
	if (!hasPannel)
	{
		SARibbonPannel* tmpPannel = new SARibbonPannel();
		tmpPannel->setObjectName(QString::fromStdString(path[index]->m_attrMp["name"]));
		tmpPannel->setWindowTitle(QString::fromStdString(path[index]->m_attrMp["windowTitle"]));
		page->addPannel(tmpPannel);
		if (NULL != action)
		{
			buildMenu(path, index + 1, tmpPannel, action);
		}
		else
		{
			QString toolTip = toolBuntton->toolTip();
			toolBuntton->setText(toolTip);
			if (!path.back()->m_attrMp["text"].empty())
			{
				path.back()->m_attrMp["name"] = path.back()->m_attrMp["text"];  //当改变以text为唯一标识时，防止该ACtion有多个时，找不到
				toolBuntton->setText(QString::fromStdString(path.back()->m_attrMp["text"]));
			}
			if (!path.back()->m_attrMp["icon"].empty())
			{
				toolBuntton->setIcon(QIcon(QString::fromStdString(path.back()->m_attrMp["icon"])));//添加图标
				toolBuntton->setIconSize(QSize(20,21));
			}
			if (!path.back()->m_attrMp["toolTip"].empty())
			{
				toolBuntton->setToolTip(QString::fromStdString(path.back()->m_attrMp["toolTip"]));
			}
			toolBuntton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
			toolBuntton->show();
			tmpPannel->addLargeWidget(toolBuntton);
		}
	}
}
void StyleAlter::buildPage(const QList<CategoryTree*>& path, int index, SARibbonBar* ribbonBar, QAction* action, QToolButton* toolBuntton)
{
	if (path.size() <= index - 1)
	{
		return;
	}
	QList<SARibbonCategory*>pages = ribbonBar->categoryPages();
	bool hasPage = false;
	for (SARibbonCategory* page : pages)
	{
		std::string pageName = page->objectName().toStdString();
		if (pageName == path[index]->m_attrMp["name"])
		{
			if (NULL != action)
			{
				buildPannel(path, index + 1, page, action);
			}
			else
			{
				buildPannel(path, index + 1, page, NULL, toolBuntton);
			}
			hasPage = true;
		}
	}
	if (!hasPage)
	{
		SARibbonCategory* tmpPage = new SARibbonCategory();
		tmpPage->setObjectName(QString::fromStdString(path[index]->m_attrMp["name"]));
		tmpPage->setWindowTitle(QString::fromStdString(path[index]->m_attrMp["windowTitle"]));
		ribbonBar->addCategoryPage(tmpPage);
		if (NULL != action)
		{
			buildPannel(path, index + 1, tmpPage, action);
		}
		else
		{
			buildPannel(path, index + 1, tmpPage, NULL, toolBuntton);
		}
	}
}
QList<CategoryTree*> getPath(CategoryTree* root)  //获取从根节点到叶子节点的路径
{
	QList<CategoryTree*>res;
	CategoryTree* node = root;
	while (node)
	{
		res.push_front(node);
		node = node->m_father;
	}
	return res;
}
void StyleAlter::getTreeNode(const std::string& name, CategoryTree* root, QList<CategoryTree*>& results)
{
	if (NULL == root)
	{
		return;
	}
	if (!root->m_attrMp["name"].empty() && name == root->m_attrMp["name"])  // && "QAction" == root->m_type
	{ 
		results.push_back(root);
		return;
	}
	for (int index = 0; index < root->m_childers.size(); index++)
	{
		getTreeNode(name, root->m_childers[index], results);
	}
}
void StyleAlter::buildActionOrToolButton(CoreObject* coreObject, QAction* action, QToolButton* toolButton)
{
	SARibbonBar* ribbonBar = coreObject->getMainWindow()->ribbonBar();

	//判断得到 Action 信息在读取配置文件后存储的 CategoryTree 结构中有没有
	CategoryTree* root = m_builder->getTreeRoot();
	QList<CategoryTree*> resultLst;
	std::string name;
	if (NULL != action)
	{
		name = action->text().toStdString();
	}
	else
	{
		name = toolButton->toolTip().toStdString();
	}
	getTreeNode(name, root, resultLst);
	if (resultLst.empty())
	{
		QString message = u8"ribbon: 在配置文件中没有找到：";
		QString name = m_name + QString::number(m_num);
		if (NULL != action)
		{
			message += m_builder->getControlMessage(action);
			if (action->text().isEmpty())
			{
				action->setText(name);
				m_num++;
			}
			action->setIcon(QIcon("plant/SARibbonComponent/icon/test/BusinessClose.ico"));
			m_pannel->addLargeAction(action);
		}
		else
		{
			message += m_builder->getControlMessage(toolButton);
			if (toolButton->text().isEmpty())
			{
				toolButton->setText(name);
				m_num++;
			}
			toolButton->setIcon(QIcon("plant/SARibbonComponent/icon/test/BusinessClose.ico"));
			m_pannel->addLargeWidget(toolButton);
		}
		LogInfoEx(message.toStdString());
	}
	for (int index = 0; index < resultLst.size(); index++)
	{
		QList<CategoryTree*>path = getPath(resultLst.at(index));
		//创建菜单栏中组件
		if ("page" == path[1]->m_treeType)
		{
			if (NULL != action)
			{
				buildPage(path, 1, ribbonBar, action);
			}
			else
			{
				buildPage(path, 1, ribbonBar, NULL, toolButton);
			}
		}
		else if ("quickAccessBar" == path[1]->m_treeType)
		{
			SARibbonQuickAccessBar*  tmpQuick = coreObject->getMainWindow()->ribbonBar()->quickAccessBar();
			buildQuick(path, 2, action, tmpQuick);
		}
		else if ("Rmenus" == path[1]->m_treeType)
		{
			m_builder->addRmenuAciotn(path[2]->m_attrMp["name"], action);
		}
		else if ("applicationButton" == path[1]->m_treeType)
		{
			/*SARibbonMenu* tmpMenu = m_builder->getAppMainMenu();
			buildAppButton(path, 2, tmpMenu, action);*/
			
		}
		else if ("rightButtonGroup" == path[1]->m_treeType)
		{
			SARibbonButtonGroupWidget* rightBar = coreObject->getMainWindow()->ribbonBar()->rightButtonGroup();
			buildQuick(path, 2, action, NULL, rightBar);
		}
	}
}
void StyleAlter::expendPluginAlter(CoreObject* coreObject)
{
	QList<QToolBar*>lstToolBar = coreObject->getListToolBar();
	for (QToolBar* tpToolBar : lstToolBar)  //将新增工具条内容风格转换
	{
		QList<QAction*>tmpLstAct = tpToolBar->actions();
		QObjectList childLst = tpToolBar->children();
		if (QObject::tr("ModelOperationToolbar") == tpToolBar->windowTitle())//模型操作工具条
		{
			for (auto* act : childLst)
			{
				QToolButton* toolBut = dynamic_cast<QToolButton*>(act);
				if (!toolBut)
					continue;
				buildActionOrToolButton(coreObject, NULL, toolBut);
			}
		}
		else
		{
			for (auto* act : tmpLstAct)
			{
				if (!act->isWidgetType() && !act->isSeparator() && act->isVisible())
				{
					m_builder->addQAction(act->text().toStdString(), act);
					//根据得到的Action新建page系列
					buildActionOrToolButton(coreObject, act);
				}
			}
		}
		coreObject->getMainWindow()->removeToolBar(tpToolBar);
	}
	lstToolBar.clear();
	//获取插件添加到menuBar中的内容
	QMenuBar* menuBar = coreObject->getMainWindow()->menuBar();
	if (!menuBar)
	{
		return;
	}
	auto menus = menuBar->actions();
	getQAcions(menus, m_builder, coreObject);
	menuBar->clear(); //清除添加的menuBar防止错误

	QList<QAction*>lstAct = m_pannel->actions();
	SARibbonBar* ribbon = coreObject->getMainWindow()->ribbonBar();
	if (!lstAct.empty() && NULL == ribbon->categoryByObjectName(QObject::tr("Unknown")))
	{
#ifdef _DEBUG
		ribbon->addCategoryPage(m_page);
#endif
	}
	else if (!lstAct.empty() && NULL != ribbon->categoryByObjectName(QObject::tr("Unknown")))
	{
		ribbon->removeCategory(m_page);
#ifdef _DEBUG
		ribbon->addCategoryPage(m_page);
#endif
	}
}

std::set<std::string>& StyleAlter::getHideDockWidgets()
{
	return m_builder->getHideDockWidgets();
}