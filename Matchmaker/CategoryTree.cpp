#include "CategoryTree.h"

CategoryTree::CategoryTree()
{

}
CategoryTree::~CategoryTree()
{

}

void CategoryTree::clearTree(CategoryTree* root)
{
	if (root)        //判断当前树不为空
	{
		for (int idex = 0; idex < root->m_childers.size(); idex++)
		{
			clearTree(root->m_childers[idex]);
		}
		delete root;      //释放当前结点所占用的内存 
	}
}
