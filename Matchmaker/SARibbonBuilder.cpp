#include "SARibbonBuilder.h"
#include <QtWidgets\qradiobutton>
#include<QCoreApplication>
#include <QtCore\qpluginloader.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "../Log/LogCategory.h"
#include "../SARibbonBar/SARibbonPannelOptionButton.h"
#include "../SARibbonBar/SARibbonLineEdit.h"
#include "../SARibbonBar/SARibbonCheckBox.h"
#include "../SARibbonBar/SARibbonButtonGroupWidget.h"
#include "../SARibbonBar/SARibbonGalleryGroup.h"
#include "../SARibbonBar/SARibbonGallery.h"
#include "../SARibbonBar/SARibbonMainWindow.h"
#include "../SARibbonBar/SARibbonQuickAccessBar.h"
#include "../SARibbonBar/SARibbonMenu.h"
#include "../SARibbonBar/SARibbonMainWindow.h"
#include "../SARibbonBar/SARibbonApplicationButton.h"
#include "../SARibbonBar/SARibbonElementManager.h"
#include "../SARibbonBar/SARibbonMenu.h"
#include "PopupMenu.h"
#include <filesystem>
#include <fstream>

#pragma execution_character_set("utf-8")//让能够正常显示中文字符串

SARibbonBuilder::SARibbonBuilder(QObject* parent) //: QObject(parent)
{
	registerClass();
	m_conpentModel.insert(std::make_pair("large", SARibbonPannelItem::Large));
	m_conpentModel.insert(std::make_pair("medium", SARibbonPannelItem::Medium));
	m_conpentModel.insert(std::make_pair("small", SARibbonPannelItem::Small));
	m_conpentModel.insert(std::make_pair("", SARibbonPannelItem::None));

	m_toolBntModel.insert(std::make_pair("delayedPopup", QToolButton::DelayedPopup));
	m_toolBntModel.insert(std::make_pair("menuButtonPopup", QToolButton::MenuButtonPopup));
	m_toolBntModel.insert(std::make_pair("instantPopup", QToolButton::InstantPopup));

	m_boolValue["false"] = false;
	m_boolValue["true"] = true;

	m_sizePolicy.insert(std::make_pair("fixed", QSizePolicy::Fixed));
	m_sizePolicy.insert(std::make_pair("minimum", QSizePolicy::Minimum));
	m_sizePolicy.insert(std::make_pair("maximum", QSizePolicy::Maximum));
	m_sizePolicy.insert(std::make_pair("preferred", QSizePolicy::Preferred));
	m_sizePolicy.insert(std::make_pair("minimumExpanding", QSizePolicy::MinimumExpanding));
	m_sizePolicy.insert(std::make_pair("expanding", QSizePolicy::Expanding));
	m_sizePolicy.insert(std::make_pair("ignored", QSizePolicy::Ignored));
	m_treeRoot = new CategoryTree();

	//m_vBoxLayAct = new QVBoxLayout();
}

void SARibbonBuilder::registerClass()
{
	REGISTER_SARIBBON(QAction);
	REGISTER_SARIBBON(QMenu);
	REGISTER_SARIBBON(QRadioButton);
	REGISTER_SARIBBON(QButtonGroup);

	REGISTER_SARIBBON(SARibbonQuickAccessBar);
	REGISTER_SARIBBON(SARibbonMenu);
	REGISTER_SARIBBON(SARibbonCategory);
	REGISTER_SARIBBON(SARibbonPannel);
	REGISTER_SARIBBON(SARibbonToolButton);
	REGISTER_SARIBBON(SARibbonGallery);
	REGISTER_SARIBBON(SARibbonGalleryGroup);
	REGISTER_SARIBBON(SARibbonComboBox);
	REGISTER_SARIBBON(SARibbonLineEdit);
	REGISTER_SARIBBON(SARibbonCheckBox);
	REGISTER_SARIBBON(SARibbonButtonGroupWidget); 
	REGISTER_SARIBBON(SARibbonApplicationButton);
}
SARibbonBuilder::~SARibbonBuilder()
{
	CategoryTree::clearTree(m_treeRoot);
	//delete m_vBoxLayAct;
}

//template<class T>
//void setTextAttributes(T* component, const rapidxml::xml_node<>* node)
//{
//	rapidxml::xml_attribute<>* attr = node->first_attribute("title");
//	if (attr)
//	{
//		QString qText = attr->value();
//		std::string text = qText.toStdString();
//		component->setText(qText);
//	}
//}
//template<class T>
//void setWinTitleAttributes(T* component, const rapidxml::xml_node<>* node, CategoryTree* root = NULL)
//{
//	rapidxml::xml_attribute<>* attr = node->first_attribute("windowTitle");
//	if (attr)
//	{
//		component->setWindowTitle(QString::fromStdString(attr->value()));
//		if (root)
//		{
//			root->m_windowTitle = attr->value();
//		}
//	}
//	attr = node->first_attribute("windowIcon");
//	if (attr)
//	{
//		component->setWindowIcon(QIcon(attr->value()));
//	}
//}
std::string getIconModelAttributes(const rapidxml::xml_node<>* node)
{
	rapidxml::xml_attribute<>* attr = node->first_attribute("IconModel");
	std::string result = "";
	if (attr)
	{
		result = node->first_attribute("IconModel")->value();
	}
	return result;
}
//template<class T>
//void setTitleAttributs(T* component, const rapidxml::xml_node<>* node, CategoryTree* root = NULL)
//{
//	rapidxml::xml_attribute<>* attr = node->first_attribute("title");
//	if (attr)
//	{
//		component->setTitle(attr->value());
//		if (root)
//		{
//			root->m_title = attr->value();
//		}
//	}
//}
//template<class T>
//void setCompAttributes(T* component, const rapidxml::xml_node<>* node, CategoryTree* root = NULL)
//{
//	rapidxml::xml_attribute<>* attr = node->first_attribute("name");
//	component->setObjectName((QString::fromStdString(attr->value())));
//	if (root)
//	{
//		QString name = attr->value();
//		root->m_name = name.toStdString();
//	}
//	attr = node->first_attribute("icon");
//	if (attr)
//	{
//		component->setIcon(QIcon(QString::fromStdString(attr->value())));
//		if (root)
//		{
//			root->m_icon = attr->value();
//		}
//	}
//}

template<class T>
void setPolicyAttr(T* component, const rapidxml::xml_node<>* node, const std::map<std::string, QSizePolicy::Policy>& sizePolicy)
{
	rapidxml::xml_attribute<>* attrPolHor = node->first_attribute("sizePolicy_hor");
	rapidxml::xml_attribute<>* attrPolVer = node->first_attribute("sizePolicy_ver");
	if (attrPolHor != nullptr && attrPolVer != nullptr)
	{
		QSizePolicy::Policy hor, ver;
		auto iterH = sizePolicy.find(attrPolHor->value());
		if (iterH != sizePolicy.end())
		{
			hor = (*iterH).second;
		}
		auto iterV = sizePolicy.find(attrPolVer->value());
		if (iterV != sizePolicy.end())
		{
			ver = (*iterV).second;
		}
		component->setSizePolicy(hor, ver);
	}
	/*rapidxml::xml_attribute<>* attr = node->first_attribute("enable");
	if (attr)
	{
		bool flag = true;
		if ("false" == attr->value())
		{
			flag = false;
		}
		component->setEnabled(flag);
	}*/
}

template<class T>
std::string setControlProperty(T* component, const rapidxml::xml_node<>* node, CategoryTree* root = nullptr)
{
	std::string result = "";
	std::map<std::string, bool>flagMp;
	flagMp.insert(std::make_pair("true", true));
	flagMp.insert(std::make_pair("false", false));
	for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute())
	{
		std::string tmpKey = attr->name(), tmpValue = attr->value();
		if ("icon" == tmpKey || "windowIcon" == tmpKey)
		{
			component->setProperty(tmpKey.c_str(), QIcon(QString::fromStdString(tmpValue)));
		}
		else if ("IconModel" == tmpKey)
		{
			result = node->first_attribute("IconModel")->value();
		}
		else if ("name" == tmpKey)
		{
			component->setObjectName((QString::fromStdString(attr->value())));
		}
		else if ("enable" == tmpKey)
		{
			component->setProperty(tmpKey.c_str(), flagMp[tmpValue]);
		}
		else
		{
			component->setProperty(tmpKey.c_str(), QString::fromStdString(tmpValue));
		}
		if (nullptr != root)
		{
			root->m_attrMp.insert(std::make_pair(tmpKey, tmpValue));
		}
	}
	return result;
}


void SARibbonBuilder::responseFunc(QAction* action, const std::string name)
{
	QString pluginName = action->property("plugin").toString();
	QString funcJson = action->property("JSON").toString();
	//Resafety::DeepWorldStudio::IPlugin* tmpPlugin = Resafety::DeepWorldStudio::PluginManager::instance()->getPlugin(pluginName.toStdString());
	//if (tmpPlugin)
	//{
	//	tmpPlugin->pluginHandle(funcJson.toStdString());
	//}
	//else
	//{
	//	LogErrorEx(__FUNCTION__, "：" + pluginName.toStdString() + "插件获取失败。");
	//}
}

void responseBntGroupFunc(QButtonGroup* bntGroup)
{

}

CategoryTree* SARibbonBuilder::getTreeRoot()
{
	return m_treeRoot;
}
void SARibbonBuilder::getRMenu(const QPoint& point, const std::string& rMneuName)
{
	QMenu* menuTmp = (QMenu*)ClassFactory::getInstance().createObjectByName("QMenu");
	if (!menuTmp)
	{
		LogErrorEx(__FUNCTION__, "：SARibbonMenu 组件创建失败。");
		return;
	}
	auto iter = m_actToRmenu.find(rMneuName);
	if (iter == m_actToRmenu.end())
	{
		LogErrorEx(__FUNCTION__, "：右键菜单名错误。");
		return;
	}
	menuTmp->addActions((*iter).second);
	menuTmp->move(point);
	menuTmp->exec();
}

QAction* SARibbonBuilder::getConfigRmenusItemInfo(const rapidxml::xml_node<>* item, std::string& iconModel, CategoryTree* rootMenu)
{
	QString tmpValue = item->first_attribute("name")->value();
	std::string attrNodeValue = tmpValue.toStdString();
	if (m_actionsMp.find(attrNodeValue) == m_actionsMp.end())
	{
		if (NULL != rootMenu)
		{
			CategoryTree* rootAct = new CategoryTree();
			for (auto attr = item->first_attribute(); attr != NULL; attr = attr->next_attribute())
			{
				std::string tmpKey = attr->name(), tmpValue = attr->value();
				rootAct->m_attrMp.insert(std::make_pair(tmpKey, tmpValue));
			}
			rootMenu->m_childers.push_back(rootAct);
			rootAct->m_father = rootMenu;
		}
		return nullptr;
	}
	QAction* tmpAct = m_actionsMp[attrNodeValue];
	/*for (auto attr = item->first_attribute(); attr != NULL; attr = attr->next_attribute())
	{
		std::string tmpKey = attr->name(), tmpValue = attr->value();
		if ("icon" == tmpKey)
		{
			tmpAct->setProperty(tmpKey.c_str(), QIcon(QString::fromStdString(tmpValue)));
		}
		else
		{
			tmpAct->setProperty(tmpKey.c_str(), QString::fromStdString(tmpValue));
		}
	}*/
	iconModel = setControlProperty(tmpAct, item);
	m_usedActName.insert(std::make_pair(attrNodeValue, true));
	return tmpAct;
}


QList<QAction*> SARibbonBuilder::readConfigRmenusItem(const rapidxml::xml_node<char>* itemNode, CategoryTree* rootMenu)
{
	QList<QAction*> resultAct;
	for (auto tmpNode = itemNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		std::string iconModel = "";
		QAction* tmpAct = getConfigRmenusItemInfo(tmpNode, iconModel, rootMenu);
		if (tmpAct)
		{
			resultAct.push_back(tmpAct);
		}
	}
	return resultAct;
}
void SARibbonBuilder::readConfigRmenus(const rapidxml::xml_node<>* curNode)
{
	curNode = curNode->first_node();
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, "：Rmenu 节点获取失败。");
		return;
	}
	for (auto tmpNode = curNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())  //不同地方进行右键点击时
	{
		rapidxml::xml_node<char>* chileNode = tmpNode->first_node();
		CategoryTree* menuTree = new CategoryTree();
		menuTree->m_treeType = "Rmenus";
		
		QList<QAction*> tmpQActions = readConfigRmenusItem(chileNode, menuTree);
		std::string rMenuName = tmpNode->first_attribute("name")->value();
		//menuTree->m_name = rMenuName;
		menuTree->m_attrMp.insert(std::make_pair("name", rMenuName));
		if (!menuTree->m_childers.empty())
		{
			m_treeRoot->m_childers.push_back(menuTree);
			menuTree->m_father = m_treeRoot;
		}
		if (!tmpQActions.isEmpty())
		{
			m_actToRmenu.insert(std::make_pair(rMenuName, tmpQActions));
		}
	}
}

bool SARibbonBuilder::readQRadioButtonInfo(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel, QButtonGroup* bntGrp)
{
	std::string nodeTypeValue = curNode->first_attribute("type")->value();
	QRadioButton* tmpRadioBut = (QRadioButton*)ClassFactory::getInstance().createObjectByName(nodeTypeValue);
	if (!tmpRadioBut)
	{
		LogErrorEx(__FUNCTION__, "：QRadioButton 组件创建失败。");
		return false;
	}
	/*setCompAttributes(tmpRadioBut, curNode);
	setTextAttributes(tmpRadioBut, curNode);


	rapidxml::xml_attribute<>* attr = curNode->first_attribute("isChecked");
	if (attr)
	{
		tmpRadioBut->setChecked(m_boolValue[attr->value()]);
	}
	std::string nodeIconModelValue = getIconModelAttributes(curNode);*/



	std::string nodeIconModelValue = setControlProperty(tmpRadioBut, curNode);
	panel->addWidget(tmpRadioBut, m_conpentModel[nodeIconModelValue]);
	rapidxml::xml_attribute<>* attr = curNode->first_attribute("index");
	int index = 0;
	if (attr)
	{
		index = atoi(attr->value());
	}
	bntGrp->addButton(tmpRadioBut, index);
	return true;
}
bool SARibbonBuilder::readQRadioButton(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel, QButtonGroup* bntGrp)
{
	bool flag = false;
	for (auto tmpNode = curNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		if (readQRadioButtonInfo(tmpNode, panel, bntGrp))
		{
			flag = true;
		}
	}
	return flag;
}
bool SARibbonBuilder::readQButtonGroup(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel)
{
	std::string nodeTypeValue = curNode->first_attribute("type")->value();
	QButtonGroup* tmpBntGrp = (QButtonGroup*)ClassFactory::getInstance().createObjectByName(nodeTypeValue);
	if (!tmpBntGrp)
	{
		LogErrorEx(__FUNCTION__, "：QButtonGroup 组件创建失败。");
		return false;
	}
	rapidxml::xml_node<char>* childNode = curNode->first_node();
	if (!readQRadioButton(childNode, panel, tmpBntGrp))
	{
		return false;
	}
	rapidxml::xml_attribute<>* attr = curNode->first_attribute("responseFunc");
	if (attr)
	{
		tmpBntGrp->setProperty("JSON", attr->value());
		connect(tmpBntGrp, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [=]() {
			responseBntGroupFunc(tmpBntGrp);  //QButtonGroup类型响应事件
			});
	}
	return true;
}
bool SARibbonBuilder::readSARibbonGallery(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel)
{
	SARibbonGallery* gallery = panel->addGallery();
	SARibbonGalleryGroup* group = gallery->addGalleryGroup();
	rapidxml::xml_node<>* childNode = curNode->first_node();
	if (!childNode)
	{
		LogErrorEx(__FUNCTION__, "：Gallery 子节点获取失败。");
		return false;
	}
	QString galleryGroupValue = childNode->value();
	QByteArray bufArray = galleryGroupValue.toUtf8();
	QJsonDocument jDoc = QJsonDocument::fromJson(bufArray);
	int number = 0;
	if (jDoc.isObject())
	{
		QJsonObject jObj = jDoc.object();
		QJsonValue dataValue = jObj.value("params");
		if (dataValue.isArray())
		{
			QJsonArray paramsValues = dataValue.toArray();
			for (int index = 0; index < paramsValues.size(); index++)
			{
				QJsonValue paramValue = paramsValues.at(index);
				QJsonArray value = paramValue.toArray();
				group->addItem(value.at(0).toString(), QIcon(value.at(1).toString()));
				number++;
			}
		}
	}
	if (number > 0)
		return true;
	return false;
}
bool SARibbonBuilder::readSARibbonButtonGroupWidget(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel)
{
	std::string nodeTypeValue = curNode->first_attribute("type")->value();
	SARibbonButtonGroupWidget* tmpBntGrpWid = (SARibbonButtonGroupWidget*)ClassFactory::getInstance().createObjectByName(nodeTypeValue);
	if (!tmpBntGrpWid)
	{
		LogErrorEx(__FUNCTION__, "：SARibbonButtonGroupWidget 组件创建失败。");
		return false;
	}
	rapidxml::xml_node<>* tmpNode = curNode->first_node();
	std::string nodeType = tmpNode->first_attribute("type")->value();
	if (tmpNode != nullptr && "QAction" == nodeType)
	{
		QList<QAction*>lstAct = readConfigRmenusItem(tmpNode);
		if (lstAct.isEmpty())
			return false;
		tmpBntGrpWid->addActions(lstAct);
	}
	/*std::string nodeIconModelValue = getIconModelAttributes(curNode);
	rapidxml::xml_attribute<char>* attr = curNode->first_attribute("name");
	tmpBntGrpWid->setObjectName((QString::fromStdString(attr->value())));
	setWinTitleAttributes(tmpBntGrpWid, curNode);*/


	std::string nodeIconModelValue = setControlProperty(tmpBntGrpWid, curNode);
	panel->addWidget(tmpBntGrpWid, m_conpentModel[nodeIconModelValue]);
	return true;
}
bool SARibbonBuilder::readSARibbonCheckBox(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel)
{
	std::string nodeTypeValue = curNode->first_attribute("type")->value();
	SARibbonCheckBox* tmpCheckedBox = (SARibbonCheckBox*)ClassFactory::getInstance().createObjectByName(nodeTypeValue);
	if (!tmpCheckedBox)
	{
		LogErrorEx(__FUNCTION__, "：SARibbonCheckBox 组件创建失败。");
		return false;
	}
	/*setWinTitleAttributes(tmpCheckedBox, curNode);
	setTextAttributes(tmpCheckedBox, curNode);
	std::string nodeIconModelValue = getIconModelAttributes(curNode);*/


	std::string nodeIconModelValue = setControlProperty(tmpCheckedBox, curNode);
	setPolicyAttr(tmpCheckedBox, curNode, m_sizePolicy);
	panel->addWidget(tmpCheckedBox, m_conpentModel[nodeIconModelValue]);
	return true;
}
bool SARibbonBuilder::readSARibbonLineEdit(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel)
{
	std::string nodeTypeValue = curNode->first_attribute("type")->value();
	SARibbonLineEdit* tmpLineEd = (SARibbonLineEdit*)ClassFactory::getInstance().createObjectByName(nodeTypeValue);
	if (!tmpLineEd)
	{
		LogErrorEx(__FUNCTION__, "：SARibbonLineEdit 组件创建失败。");
		return false;
	}
	/*setWinTitleAttributes(tmpLineEd, curNode);
	setTextAttributes(tmpLineEd, curNode);
	std::string nodeIconModelValue = getIconModelAttributes(curNode);*/

	std::string nodeIconModelValue = setControlProperty(tmpLineEd, curNode);
	setPolicyAttr(tmpLineEd, curNode, m_sizePolicy);
	panel->addWidget(tmpLineEd, m_conpentModel[nodeIconModelValue]);
	return true;
}

void SARibbonBuilder::readSARibbonComboBoxValue(const rapidxml::xml_node<>* curNode, SARibbonComboBox* comBox)
{
	QStringList resultString;
	QString comBoxValue = curNode->value();
	QByteArray bufValue = comBoxValue.toUtf8();
	QJsonDocument jdValue = QJsonDocument::fromJson(bufValue);
	if (jdValue.isObject())
	{
		QJsonObject joValue = jdValue.object();
		QJsonValue dataValue = joValue.value("params");
		if (dataValue.isArray())
		{
			QJsonArray dataArrValue = dataValue.toArray();
			for (int index = 0; index < dataArrValue.size(); index++)
			{
				resultString.append(dataArrValue.at(index).toString());
			}
		}
	}
	comBox->addItems(resultString);
}
bool SARibbonBuilder::readSARibbonComboBox(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel)
{
	std::string nodeTypeValue = curNode->first_attribute("type")->value();
	SARibbonComboBox* tmpComBox = (SARibbonComboBox*)ClassFactory::getInstance().createObjectByName(nodeTypeValue);
	if (!tmpComBox)
	{
		LogErrorEx(__FUNCTION__, "：SARibbonComboBox 组件创建失败。");
		return false;
	}
	/*setWinTitleAttributes(tmpComBox, curNode);


	std::string nodeIconModelValue = getIconModelAttributes(curNode);*/

	setPolicyAttr(tmpComBox, curNode, m_sizePolicy);
	std::string nodeIconModelValue = setControlProperty(tmpComBox, curNode);
	readSARibbonComboBoxValue(curNode, tmpComBox);
	panel->addWidget(tmpComBox, m_conpentModel[nodeIconModelValue]);
	return true;
}
bool SARibbonBuilder::readPanelChildInfo(const rapidxml::xml_node<>* curNode, SARibbonPannel* panel, CategoryTree* rootPannel)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, "：Panel 子节点获取失败。");
		return false;
	}
	std::string nodeTypeValue = curNode->first_attribute("type")->value();
	std::string nodeIconModelValue = getIconModelAttributes(curNode);
	if ("SARibbonMenu" == nodeTypeValue)
	{
		CategoryTree* rootMenu = new CategoryTree();
		SARibbonMenu* tmpMenu = readMenu(curNode, rootMenu);
		std::string popupMode = "instantPopup";
		rapidxml::xml_attribute<>* attr = curNode->first_attribute("ActionBtnModel");
		if (attr)
		{
			std::string tmp = attr->value();
			tButPopupModeMP::iterator iters = m_toolBntModel.find(tmp);
			if (iters != m_toolBntModel.end())
			{
				popupMode = iters->first;
			}
		}
		if (!tmpMenu->isEmpty())
		{
			panel->addMenu(tmpMenu, m_conpentModel[nodeIconModelValue], m_toolBntModel[popupMode]);
			delete rootMenu;
			return true;
		}
		else if(!(rootMenu->m_childers.empty()))
		{
			/*rootMenu->m_iconModel = nodeIconModelValue;
			rootMenu->m_toolBntModel = popupMode;*/

			rootMenu->m_attrMp.insert(std::make_pair("IconModel", nodeIconModelValue));
			rootMenu->m_attrMp.insert(std::make_pair("ActionBtnModel", popupMode));
			rootPannel->m_childers.push_back(rootMenu);
			rootMenu->m_father = rootPannel;
		}
	}
	else if ("QAction" == nodeTypeValue)
	{
		if (!curNode->first_node()) //表示没有子节点
		{
			std::string iconModel = "";
			QAction* tmpQAc = getConfigRmenusItemInfo(curNode, iconModel, rootPannel);
			if (NULL != tmpQAc)
			{
				panel->addAction(tmpQAc, m_conpentModel[nodeIconModelValue]);
				return true;
			}
		}
		else  //pannel->action->menu->action
		{
			//获取属性信息，一定拥有的属性：name、type
			std::string iconModel = "";
			QAction* tmpQAc = getConfigRmenusItemInfo(curNode, iconModel);
			if (!tmpQAc)
			{
				LogErrorEx(__FUNCTION__, "：QAction获取失败。");
				return false;
			}
			CategoryTree* rootMenu = new CategoryTree();
			SARibbonMenu* tmpMenu = readMenu(curNode, rootMenu);
			rapidxml::xml_node<char>* btnNode = curNode->first_node("ActionBtn");
			rapidxml::xml_attribute<>* attr = btnNode->first_attribute("ActionBtnModel");
			if (!tmpMenu->isEmpty())
			{
				tmpQAc->setMenu(tmpMenu);
				SARibbonToolButton* btn = panel->addAction(tmpQAc, m_conpentModel[nodeIconModelValue]);
				btn->setObjectName(btnNode->first_attribute("name")->value());
				if (attr)
				{
					btn->setPopupMode(m_toolBntModel[attr->value()]);
				}
				attr = btnNode->first_attribute("BtnCheckd");
				if (attr)
				{
					btn->setCheckable(m_boolValue[attr->value()]);
				}
				delete rootMenu;
				return true;
			}
			else if (!(rootMenu->m_childers.empty()))
			{
				//rootMenu->m_iconModel = nodeIconModelValue;
				rootMenu->m_attrMp.insert(std::make_pair("IconModel", nodeIconModelValue));
				if (attr)
				{
					rootMenu->m_attrMp.insert(std::make_pair("ActionBtnModel", attr->value()));
					//rootMenu->m_toolBntModel = attr->value();
				}
				rootPannel->m_childers.push_back(rootMenu);
				rootMenu->m_father = rootPannel;
			}
		}
	}
	else if ("SARibbonComboBox" == nodeTypeValue)
	{
		return readSARibbonComboBox(curNode, panel);
	}
	else if ("SARibbonLineEdit" == nodeTypeValue)
	{
		return readSARibbonLineEdit(curNode, panel);
	}
	else if ("SARibbonCheckBox" == nodeTypeValue)
	{
		return readSARibbonCheckBox(curNode, panel);
	}
	else if ("QButtonGroup" == nodeTypeValue)
	{
		return readQButtonGroup(curNode, panel);
	}
	else if("SARibbonButtonGroupWidget" == nodeTypeValue)
	{
		return readSARibbonButtonGroupWidget(curNode, panel);
	}
	else if ("SARibbonGallery" == nodeTypeValue)
	{
		return readSARibbonGallery(curNode, panel);
	}
	else if ("Separator" == nodeTypeValue)
	{
		panel->addSeparator();
	}
	return false;
}
bool SARibbonBuilder::readPanel(const rapidxml::xml_node<>* curNode, SARibbonCategory* page, CategoryTree* rootPannel)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, "Panel 节点获取失败。");
		return false;
	}
	SARibbonPannel* pannel = (SARibbonPannel*)ClassFactory::getInstance().createObjectByName(curNode->first_attribute("type")->value());
	if (!pannel)
	{
		LogErrorEx(__FUNCTION__, "SARibbonPannel 组件创建失败。");
		return false;
	}
	/*std::string name = curNode->first_attribute("name")->value();
	pannel->setObjectName(QString::fromStdString(name));
	rootPannel->m_name = name;
	std::string type = curNode->first_attribute("type")->value();
	rootPannel->m_type = type;
	setWinTitleAttributes(pannel, curNode, rootPannel);*/

	setControlProperty(pannel, curNode, rootPannel);
	pannel->setEnabled(true);
	
	rapidxml::xml_node<char>* childNode = curNode->first_node();
	bool flag = false;
	for (auto tmpNode = childNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		if (readPanelChildInfo(tmpNode, pannel, rootPannel))
		{
			flag = true;
		}
	}

	if (flag)
	{
		page->addPannel(pannel);
	}
	return flag;
}
void SARibbonBuilder::readCategory(const rapidxml::xml_node<>* curNode, CategoryTree* rootPage)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, ": Category 节点获取失败。");
		return;
	}
	SARibbonCategory* page = (SARibbonCategory*)ClassFactory::getInstance().createObjectByName(curNode->first_attribute("type")->value());
	if (!page)
	{
		LogErrorEx(__FUNCTION__, ": SARibbonCategory 组件创建失败。");
		return;
	}
	/*std::string name = curNode->first_attribute("name")->value();
	page->setObjectName(QString::fromStdString(name));
	rootPage->m_name = name;
	std::string type = curNode->first_attribute("type")->value();
	rootPage->m_type = type;
	setWinTitleAttributes(page, curNode, rootPage);*/
	setControlProperty(page, curNode, rootPage);

	rootPage->m_treeType = "page";

	rapidxml::xml_node<char>* childNode = curNode->first_node();
	bool flag = false;
	for (auto tmpNode = childNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		CategoryTree* rootPannel = new CategoryTree();
		if (readPanel(tmpNode, page, rootPannel))
		{
			flag = true;
		}
		if (!(rootPannel->m_childers.empty()))
		{
			rootPage->m_childers.push_back(rootPannel);
			rootPannel->m_father = rootPage;
		}
		else
		{
			delete rootPannel;
		}
	}
	if (flag)
	{
		m_ribbon->addCategoryPage(page);
	}
}
void SARibbonBuilder::readRibbonMenu(const rapidxml::xml_node<>* curNode)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, ": RibbonMenu 节点获取失败。");
		return;
	}
	rapidxml::xml_node<char>* childNode = curNode->first_node();
	for (auto tmpNode = childNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		CategoryTree* page = new CategoryTree();
		readCategory(tmpNode, page);
		if (!(page->m_childers.empty()))
		{
			page->m_father = m_treeRoot;
			m_treeRoot->m_childers.push_back(page);
		}
		else
		{
			delete page;
		}
	}
}
void SARibbonBuilder::readApplicationButInfo(const rapidxml::xml_node<>* itemNode, QVBoxLayout* vBoxLayAct)
{
	/*SARibbonPannel* pannel = new SARibbonPannel();
	vBoxLayAct->addWidget(pannel);*/
	for (auto tmpNode = itemNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		std::string attrTypeValue = tmpNode->first_attribute("type")->value();
		if ("QAction" == attrTypeValue)
		{
			std::string iconModel = "";
			QAction* act = getConfigRmenusItemInfo(tmpNode, iconModel);
			if (nullptr == act)
			{
				LogInfoEx(__FUNCTION__, u8"获取Action为空");
				return;
			}
			//pannel->addAction(act);
			SARibbonApplicationButton* button1 = new SARibbonApplicationButton(act->text());
			button1->setIcon(act->icon());
			button1->setFlat(true);
			button1->setFixedSize(130, 35);
			connect(button1, &SARibbonApplicationButton::clicked, act, &QAction::triggered);
			vBoxLayAct->addWidget(button1);
			
		}
		else if ("SARibbonMenu" == attrTypeValue)
		{
			PopupMenu* menu = readMenu(tmpNode);
			//pannel->addMenu(menu, m_conpentModel["large"], m_toolBntModel["menuButtonPopup"]);
			SARibbonApplicationButton* button1 = new SARibbonApplicationButton(menu->windowTitle());
			button1->setIcon(menu->icon());
			button1->setFlat(true);
			button1->setFixedSize(130, 35);
			button1->setMenu(menu);
			menu->setPushBnt(button1);
			menu->setWindows(m_mWindow);
			vBoxLayAct->addWidget(button1);
		}
	}
}
QVBoxLayout* SARibbonBuilder::readApplicationBut(const rapidxml::xml_node<>* curNode)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, ": MainMenu 节点获取失败。");
		return NULL;
	}
	QAbstractButton* tmpBut = m_ribbon->applicationButton();
	/*setCompAttributes(tmpBut, curNode);
	setTextAttributes(tmpBut, curNode);*/
	setControlProperty(tmpBut, curNode);
	rapidxml::xml_node<char>* childNode = curNode->first_node();
	if (!childNode)
	{
		LogErrorEx(__FUNCTION__, ": Amenu 节点获取失败。");
		return NULL;
	}
	QVBoxLayout* vBoxLayAct = new QVBoxLayout();
	CategoryTree* mainMenuTree = new CategoryTree();
	mainMenuTree->m_treeType = "applicationButton";

	readApplicationButInfo(childNode, vBoxLayAct);
	return vBoxLayAct;
}


PopupMenu* SARibbonBuilder::readMenu(const rapidxml::xml_node<>* curNode, CategoryTree* rootMenu, std::string type)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, "：Menus 节点获取失败。");
		return nullptr;
	}
	PopupMenu* tmpMenu = new PopupMenu(nullptr);
	//SARibbonMenu* tmpMenu = (SARibbonMenu*)ClassFactory::getInstance().createObjectByName(curNode->first_attribute("type")->value());
	if (!tmpMenu)
	{
		LogErrorEx(__FUNCTION__, ": SARibbonMenu 组件创建失败。");
		return nullptr;
	}
	/*setTitleAttributs(tmpMenu, curNode, rootMenu);
	setCompAttributes(tmpMenu, curNode, rootMenu);
	setWinTitleAttributes(tmpMenu, curNode, rootMenu);*/

	setControlProperty(tmpMenu, curNode);
	rapidxml::xml_node<>* childNode = curNode->first_node();
	QList<QAction*> tmpActLst = readConfigRmenusItem(childNode, rootMenu);
	tmpMenu->addActions(tmpActLst);

	

	if ("AppMainMenu" == type)
	{
		m_appMainActLst = tmpActLst;
	}

	return tmpMenu;
}
void SARibbonBuilder::readQuickToolItem(const rapidxml::xml_node<>* itemNode, CategoryTree* rootQuick)
{
	SARibbonQuickAccessBar* tmpQuickBar = m_ribbon->quickAccessBar();
	for (auto tmpNode = itemNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		std::string attrTypeValue = tmpNode->first_attribute("type")->value();
		if("QAction" == attrTypeValue)
		{
			std::string iconModel = "";
			tmpQuickBar->addAction(getConfigRmenusItemInfo(tmpNode, iconModel, rootQuick));
		}
		else if ("SARibbonMenu" == attrTypeValue)
		{
			tmpQuickBar->addMenu((SARibbonMenu*)readMenu(tmpNode, rootQuick));
		}
	}
}
void SARibbonBuilder::readQuickTool(const rapidxml::xml_node<>* curNode)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, ": QuickToolbar 节点获取失败。");
		return;
	}
	SARibbonQuickAccessBar* tmpQuickBar = m_ribbon->quickAccessBar();
	tmpQuickBar->setObjectName(QString::fromStdString(curNode->first_attribute("name")->value()));
	for (auto attr = curNode->first_attribute(); attr != NULL; attr = attr->next_attribute())
	{
		std::string tmpKey = attr->name(), tmpValue = attr->value();
		if ("windowIcon" == tmpKey)
		{
			tmpQuickBar->setProperty(tmpKey.c_str(), QIcon(tmpValue.c_str()));
		}
		else
		{
			tmpQuickBar->setProperty(tmpKey.c_str(), tmpValue.c_str());
		}
	}
	//setWinTitleAttributes(tmpQuickBar, curNode);
	rapidxml::xml_node<char>* childNode = curNode->first_node();
	CategoryTree* quickTree = new CategoryTree();
	quickTree->m_treeType = "quickAccessBar";
	readQuickToolItem(childNode, quickTree);
	if (!quickTree->m_childers.empty())
	{
		m_treeRoot->m_childers.push_back(quickTree);
		quickTree->m_father = m_treeRoot;
	}
}
void SARibbonBuilder::readRightButtonGroupItem(const rapidxml::xml_node<>* itemNode, CategoryTree* rootRightButtonGroup)
{
	SARibbonButtonGroupWidget* rightBar = m_ribbon->rightButtonGroup();
	for (auto tmpNode = itemNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		std::string attrTypeValue = tmpNode->first_attribute("type")->value();
		if ("QAction" == attrTypeValue)
		{
			std::string iconModel = "";
			rightBar->addAction(getConfigRmenusItemInfo(tmpNode, iconModel, rootRightButtonGroup));
		}
		else if ("SARibbonMenu" == attrTypeValue)
		{
			rightBar->addMenu((SARibbonMenu*)readMenu(tmpNode, rootRightButtonGroup));
		}
	}
}
void SARibbonBuilder::readRightButtonGroup(const rapidxml::xml_node<>* curNode)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, ": RightButtonGroup 节点获取失败。");
		return;
	}
	SARibbonButtonGroupWidget* rightBar = m_ribbon->rightButtonGroup();
	/*rightBar->setObjectName(QString::fromStdString(curNode->first_attribute("name")->value()));
	setWinTitleAttributes(rightBar, curNode);*/

	setControlProperty(rightBar, curNode);
	rapidxml::xml_node<char>* childNode = curNode->first_node();
	CategoryTree* rightButtonGroup = new CategoryTree();
	rightButtonGroup->m_treeType = "rightButtonGroup";
	readRightButtonGroupItem(childNode, rightButtonGroup);
	if (!rightButtonGroup->m_childers.empty())
	{
		m_treeRoot->m_childers.push_back(rightButtonGroup);
		rightButtonGroup->m_father = m_treeRoot;
	}
}
void SARibbonBuilder::readConfigHideDockWidgets(const rapidxml::xml_node<>* curNode)
{
	if (!curNode)
	{
		LogErrorEx(__FUNCTION__, ": QuickToolbar 节点获取失败。");
		return;
	}

	for (auto item = curNode->first_node(); item != NULL; item = item->next_sibling())
	{
		m_hideDockWidgets.insert(item->first_attribute("name")->value());
	}

}

void SARibbonBuilder::readConfigRibbons(const rapidxml::xml_node<>* curNode)
{
	readQuickTool(curNode->first_node("AppQuickToolbar"));

	//m_vBoxLayAct = readApplicationBut(curNode->first_node("AppMainMenu"));

	//m_appMainMenu = readMainMenu(curNode->first_node("AppMainMenu"));//暂时禁用该按钮，后续可放开


	readRightButtonGroup(curNode->first_node("RightButtonGroup"));
	readRibbonMenu(curNode->first_node("RibbonMenu"));
}

void SARibbonBuilder::getConfigActionInfo(const rapidxml::xml_node<>* node)
{
	//获取属性信息，一定拥有的属性：name、type
	rapidxml::xml_attribute<>* attr = node->first_attribute("name");
	std::string nodeName = attr->value();
	attr = node->first_attribute("type");

	QAction* tmpAction = (QAction*)ClassFactory::getInstance().createObjectByName(attr->value());
	if (!tmpAction)
	{
		LogErrorEx(__FUNCTION__, ": " + nodeName + "节点创建失败。");
		return;
	}
	m_actionsMp.insert(std::make_pair(nodeName, tmpAction));  //得到组件对象，并保存

	attr = node->first_attribute("HotKey");
	if (attr)
	{
		tmpAction->setShortcut(QKeySequence(QLatin1String(attr->value())));
	}
	for (auto attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute())
	{
		std::string tmpKey = attr->name(), tmpValue = attr->value();
		if ("name" != tmpKey)
		{
			if ("icon" == tmpKey)
			{
				tmpAction->setProperty(tmpKey.c_str(), QIcon(tmpValue.c_str()));
			}
			else
			{
				tmpAction->setProperty(tmpKey.c_str(), tmpValue.c_str());
			}
		}
	}
	tmpAction->setProperty("JSON", node->value());
	connect(tmpAction, &QAction::triggered, [=]() {
		responseFunc(tmpAction, nodeName);
	});
}
void SARibbonBuilder::readConfigActionNode(const rapidxml::xml_node<>* node)
{
	rapidxml::xml_node<>*childNode = node->first_node();  //获取Action节点
	for (auto tmpNode = childNode; tmpNode != NULL; tmpNode = tmpNode->next_sibling())
	{
		getConfigActionInfo(tmpNode);
	}
}

void SARibbonBuilder::readConfigBlock(const rapidxml::xml_node<>* root, std::string nodeName)
{
	rapidxml::xml_node<>* blockNode = root->first_node(nodeName.c_str());
	if (!blockNode) //判断节点Block是否存在
	{
		LogErrorEx(__FUNCTION__, ": " + nodeName+"节点获取失败。");
		return;
	}
	if ("Actions" == nodeName)
	{
		readConfigActionNode(blockNode);
	}
	else if ("Rmenus" == nodeName)
	{
		readConfigRmenus(blockNode);
	}
	else if ("Ribbons" == nodeName)
	{
		readConfigRibbons(blockNode);
	}
	else if ("HideDockWidgets" == nodeName)
	{
		readConfigHideDockWidgets(blockNode);
	}
}
//SARibbonBar* ribbon
void SARibbonBuilder::readXMLConfig(SARibbonMainWindow* mWindow, const std::string& filePath, SARibbonPannel* pannel)
{
	m_ribbon = mWindow->ribbonBar();
	m_mWindow = mWindow;
	try
	{
		std::filesystem::path fspath = std::filesystem::u8path(filePath);
		std::basic_ifstream<char> stream(fspath, std::ios::binary);
		rapidxml::file<> fdoc(stream);
		stream.close();
		// 用file文件读入缓冲区

		// 解析获取DOM实例
		rapidxml::xml_document<> doc;
		doc.parse<0>(fdoc.data());

		rapidxml::xml_node<>* root = doc.first_node();  //获取xml节点，即根节点；
		readConfigBlock(root, "Actions");
		readConfigBlock(root, "Rmenus");
		readConfigBlock(root, "Ribbons");
		readConfigBlock(root, "HideDockWidgets");
		doc.clear();
		for (std::map<std::string, QAction*>::iterator iter = m_actionsMp.begin(); iter != m_actionsMp.end(); iter++)
		{
			if (m_usedActName.find(iter->first) == m_usedActName.end())
			{
				QAction* action = iter->second;
				QString message = "ribbon 配置了，但没有使用的Action：\n";
				message += getControlMessage(action);
				pannel->addLargeAction(action);
				LogInfoEx(message.toStdString());
			}
		}
	}
	catch (rapidxml::parse_error e)
	{
		LogErrorEx(__FUNCTION__, ":",e.what());
		return;
	}
}
void SARibbonBuilder::addQAction(const std::string& strKey, QAction* actValue)
{
	if (actValue)
	{
		m_actionsMp.insert(std::make_pair(strKey, actValue));
	}
}

std::map<std::string, SARibbonPannelItem::RowProportion> SARibbonBuilder::getConpentModel()
{
	return m_conpentModel;
}
std::map<std::string, QToolButton::ToolButtonPopupMode> SARibbonBuilder::getToolBntModel()
{
	return m_toolBntModel;
}

void SARibbonBuilder::addRmenuAciotn(std::string& name, QAction* action)
{
	if (m_actToRmenu.find(name) == m_actToRmenu.end())
	{
		QList<QAction*>tmpLst;
		tmpLst.append(action);
		m_actToRmenu.insert(std::make_pair(name, tmpLst));
	}
	else
	{
		m_actToRmenu[name].append(action);
	}
}

std::set<std::string>& SARibbonBuilder::getHideDockWidgets()
{
	return m_hideDockWidgets;
}