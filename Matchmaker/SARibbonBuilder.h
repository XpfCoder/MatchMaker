#pragma once
#include <string>
#include<QWidget>
#include<QAction>
#include<map>
#include<QToolButton>
#include<qsizepolicy>
#include <QButtonGroup>
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../rapidxml-1.13/rapidxml_utils.hpp"
#include "../rapidxml-1.13/rapidxml_print.hpp"
#include "../SARibbonBar/SARibbonBar.h"
#include "../SARibbonBar/SARibbonPannelItem.h"
#include "../SARibbonBar/SARibbonComboBox.h"
#include "../SARibbonBar/SARibbonCategory.h"
#include "../SARibbonBar/SARibbonPannel.h"
#include "../SARibbonBar/SARibbonMainWindow.h"
#include "SARibbonRegistrater.hpp"
#include "CategoryTree.h"
#include "PopupMenu.h"
#include <set>

#pragma execution_character_set("utf-8")//让能够正常显示中文字符串
typedef std::map<std::string, SARibbonPannelItem::RowProportion> rProportionMP;
typedef std::map<std::string, QToolButton::ToolButtonPopupMode> tButPopupModeMP;

class SARibbonMenu;

class SARibbonBuilder:QObject
{
	Q_OBJECT
public:
	explicit SARibbonBuilder(QObject* parent);
	~SARibbonBuilder();
public:
	void readXMLConfig(SARibbonMainWindow* mWindow,const std::string& filePath, SARibbonPannel* pannel);
	void getRMenu(const QPoint& point, const std::string& rMneuName);
	void addQAction(const std::string& strKey, QAction* actValue);
	CategoryTree* getTreeRoot();
	std::map<std::string, SARibbonPannelItem::RowProportion>getConpentModel();
	std::map<std::string, QToolButton::ToolButtonPopupMode>getToolBntModel();

	template<class object>//要成员函数实现模版用法，而类不是模版类。
	QString getControlMessage(object* obj);
	std::set<std::string>& getHideDockWidgets();
	void addRmenuAciotn(std::string& name, QAction* action);
private:
	void readConfigBlock(const rapidxml::xml_node<>* root, std::string nodeName);
	void readConfigActionNode(const rapidxml::xml_node<>* node);
	void getConfigActionInfo(const rapidxml::xml_node<>* node);

	void readConfigRmenus(const rapidxml::xml_node<>* curNode);
	void readConfigHideDockWidgets(const rapidxml::xml_node<>* curNode);
	QList<QAction*> readConfigRmenusItem(const rapidxml::xml_node<>* itemNode, CategoryTree* rootMenu = NULL);
	QAction* getConfigRmenusItemInfo(const rapidxml::xml_node<>* item, std::string& iconModel, CategoryTree* rootMenu = NULL);

	void readRightButtonGroupItem(const rapidxml::xml_node<>* itemNode, CategoryTree* rootRightButtonGroup);
	void readRightButtonGroup(const rapidxml::xml_node<>* curNode);
	void readConfigRibbons(const rapidxml::xml_node<>* curNode);
	void readQuickTool(const rapidxml::xml_node<>* curNode);
	void readQuickToolItem(const rapidxml::xml_node<>* itemNode, CategoryTree* rootQuick);

	PopupMenu* readMenu(const rapidxml::xml_node<>* curNode, CategoryTree* rootMenu = NULL, std::string type = "");


	QVBoxLayout* readApplicationBut(const rapidxml::xml_node<>* curNode);
	void readApplicationButInfo(const rapidxml::xml_node<>* itemNode, QVBoxLayout* vBoxLayAct);
	void readRibbonMenu(const rapidxml::xml_node<>* curNode);
	
	void readCategory(const rapidxml::xml_node<>* curNode, CategoryTree* rootPage);
	bool readPanel(const rapidxml::xml_node<>* curNode, SARibbonCategory* page, CategoryTree* rootPannel);
	bool readPanelChildInfo(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel, CategoryTree* rootPannel);
	bool readSARibbonComboBox(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel);
	void readSARibbonComboBoxValue(const rapidxml::xml_node<>* curNode, SARibbonComboBox* comBox);
	bool readSARibbonLineEdit(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel);
	bool readSARibbonCheckBox(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel);
	bool readSARibbonButtonGroupWidget(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel);
	bool readSARibbonGallery(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel);
	bool readQButtonGroup(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel);
	bool readQRadioButton(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel, QButtonGroup* bntGrp);
	bool readQRadioButtonInfo(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel, QButtonGroup* bntGrp);
	
	void registerClass();
	void responseFunc(QAction* action, const std::string jsonStr);
private:
	std::map<std::string, QAction*>m_actionsMp;
	std::map<std::string, bool>m_usedActName;
	SARibbonBar* m_ribbon = nullptr; 

	SARibbonMainWindow* m_mWindow{ nullptr };


	rProportionMP m_conpentModel;
	tButPopupModeMP m_toolBntModel;
	std::map <std::string, bool>m_boolValue;
	std::map<std::string, QSizePolicy::Policy>m_sizePolicy;
	std::map<std::string, QList<QAction*>>m_actToRmenu;
	std::set<std::string> m_hideDockWidgets;

public:
	QVBoxLayout* m_vBoxLayAct{ nullptr };

	SARibbonMenu* m_appMainMenu = NULL;
public:
	QList<QAction*>m_appMainActLst;

	CategoryTree* m_treeRoot = NULL;
};
template<class object>
QString SARibbonBuilder::getControlMessage(object* obj)
{
	QString resultMess = "objectName:";
	resultMess += obj->objectName() + "; text:" + obj->text();
	resultMess += "; statusTip:" + obj->statusTip() + "; toolTip:" + obj->toolTip();
	resultMess += "; iconName:" + obj->icon().name();
	if (NULL != obj->parent())
	{
		resultMess += "; parent objectName:" + obj->parent()->objectName();
	}
	resultMess += "; shortcut:" + obj->shortcut().toString() + "; whatsThis:" + obj->whatsThis();
	return resultMess;
}
