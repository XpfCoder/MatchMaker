#pragma once
#include<vector>
#include<string>
#include<map>

#pragma execution_character_set("utf-8")//让能够正常显示中文字符串

class CategoryTree
{
public:
	CategoryTree();
	~CategoryTree();
	CategoryTree* m_father = nullptr;
	std::vector<CategoryTree*> m_childers;
	std::string m_treeType = "";
	/*string m_name = "", m_type = "";
	string m_treeType = "", m_iconModel = "";
	string m_conpentModel = "", m_toolBntModel = "";


	string m_windowTitle = "", m_icon = "";
	string m_text = "", m_title = "";
	string m_toolTip = "";*/
	std::map<std::string, std::string>m_attrMp;

	/*enable = "0"
	Tips = "加载Wlkx/Wlp文件并打开。" TipTitle = "打开文件" PreMsg = "请选择需要打开的文件，您也可以不关闭当前文件的情况下，同时打开多个文件显示！" ActionOk = "模型加载中..." ActionFail = "模型没有加载！" HotKey = "Ctrl+O"*/

public:
	static void clearTree(CategoryTree* root);
};

