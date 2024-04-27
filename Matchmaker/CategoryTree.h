#pragma once
#include<vector>
#include<string>
#include<map>

#pragma execution_character_set("utf-8")//���ܹ�������ʾ�����ַ���

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
	Tips = "����Wlkx/Wlp�ļ����򿪡�" TipTitle = "���ļ�" PreMsg = "��ѡ����Ҫ�򿪵��ļ�����Ҳ���Բ��رյ�ǰ�ļ�������£�ͬʱ�򿪶���ļ���ʾ��" ActionOk = "ģ�ͼ�����..." ActionFail = "ģ��û�м��أ�" HotKey = "Ctrl+O"*/

public:
	static void clearTree(CategoryTree* root);
};

