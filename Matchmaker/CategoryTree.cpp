#include "CategoryTree.h"

CategoryTree::CategoryTree()
{

}
CategoryTree::~CategoryTree()
{

}

void CategoryTree::clearTree(CategoryTree* root)
{
	if (root)        //�жϵ�ǰ����Ϊ��
	{
		for (int idex = 0; idex < root->m_childers.size(); idex++)
		{
			clearTree(root->m_childers[idex]);
		}
		delete root;      //�ͷŵ�ǰ�����ռ�õ��ڴ� 
	}
}
