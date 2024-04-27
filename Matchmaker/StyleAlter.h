#pragma once
#include <QMenuBar>
#include <QToolBar>
#include<QListWidget>
#include<QWidget>
#include "../SARibbonBar/SARibbonBar.h"
#include "../SARibbonBar/SARibbonPannelItem.h"
#include "../SARibbonBar/SARibbonComboBox.h"
#include "../SARibbonBar/SARibbonCategory.h"
#include "../SARibbonBar/SARibbonPannel.h"
#include "../SARibbonBar/SARibbonMainWindow.h"
#include "../SARibbonBar/SARibbonButtonGroupWidget.h"
#include "../SARibbonBar/SARibbonApplicationButton.h"
#include "SARibbonBuilder.h"
#include "CoreObject.h"
#include <windows.h>

void GetExePath(std::string& path);

class StyleAlter : public QObject
{
	Q_OBJECT
public:
	~StyleAlter();
    StyleAlter(SARibbonMainWindow* window);
    SARibbonBuilder* getBuilder();
    static StyleAlter* install(CoreObject* coreObject, std::string& ribbonFilePath);
    void expendPluginAlter(CoreObject* coreObject);
    std::set<std::string>& getHideDockWidgets();
public slots:
	void showRibbonBarRmenu(const QPoint& point);
    //void showAppMain(); //const QPoint& point, QAbstractButton* appBut
    void hideAppMain();
private:
    /**
     * ��װ��ʵ����QMainWindowʵ����
     * �Զ�����QMainWindow�Ĳ˵�������Ribbon����
     */
    void initialize(SARibbonMainWindow* window, QMenuBar* menuBar, QList<QToolBar*> lstToolBar, std::string& ribbonFilePath);

    void getToolBarMessage(SARibbonMainWindow* window, QList<QToolBar*> lstToolBar);
	void getMenuBarMessage(SARibbonMainWindow* window, QMenuBar* menuBar);
	void buildSARibbonMenu(SARibbonMainWindow* mWindow, std::string& ribbonFilePath);
    //void buildToolButton(QToolButton* action, CoreObject* coreObject);
    void buildActionOrToolButton(CoreObject* coreObject, QAction* action = NULL, QToolButton* toolButton = NULL);
    void getQAcions(QList<QAction*> lstAct, SARibbonBuilder* builder, CoreObject* coreObject = NULL);
    void getTreeNode(const std::string& name, CategoryTree* root, QList<CategoryTree*>& results);
    void buildPage(const QList<CategoryTree*>& path, int index, SARibbonBar* ribbonBar, QAction* action = NULL, QToolButton* toolBuntton = NULL);
    void buildPannel(const QList<CategoryTree*>& path, int index, SARibbonCategory* page, QAction* action = NULL, QToolButton* toolBuntton = NULL);
    void buildMenu(const QList<CategoryTree*>& path, int index, SARibbonPannel* pannel, QAction* action = NULL, QToolButton* toolBuntton = NULL);
    void buildQuick(const QList<CategoryTree*>& path, int index, QAction* action, SARibbonQuickAccessBar* quick = NULL, SARibbonButtonGroupWidget* rightBar = NULL);
    void buildAppButton(const QList<CategoryTree*>& path, int index, SARibbonMenu* menu, QAction* action);
private:
    SARibbonBuilder* m_builder = NULL;
    SARibbonCategory* m_page = NULL;
    SARibbonPannel* m_pannel = NULL;
    QString m_name = QObject::tr("Unknown Controls");
    int m_num = 1;
    SARibbonMenu* m_appMainMenu = NULL;
    SARibbonApplicationButton* m_appButton = nullptr;

    QVBoxLayout* m_vBoxLayActTwo{ nullptr };

	QListWidget* m_lstWidgetHisTory = NULL;
	QWidget* m_widget = NULL;
};

inline void GetExePath(std::string& path)
{
    char fileName[MAX_PATH];
    /*��ȡ��ǰ�����Ѽ���ģ����ļ�������·������ģ������ɵ�ǰ���̼��ء�
    �����Ҫ��ȡ��һ���Ѽ���ģ����ļ�·��������ʹ��GetModuleFileNameEx������*/
    GetModuleFileNameA(0, fileName, sizeof(fileName));  //��ȡ�ļ���
    char* end = strrchr(fileName, '\\');
    path.assign(fileName, end + 1);
}