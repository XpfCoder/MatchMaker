#include "ImageItemModel.h"

#include <QIcon>


ImageItemModel::ImageItemModel(const std::vector<PublicDataObjPtr>& list)
	:QAbstractListModel()
{
	styleLists = list;
}

ImageItemModel::ImageItemModel()
	: QAbstractListModel()
{

}

ImageItemModel::~ImageItemModel()
{
	styleLists.clear();
}

int ImageItemModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return styleLists.size();
}

QVariant ImageItemModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (index.row() < 0 || index.row() >= rowCount())
		return QVariant();
	// 	if (role == Qt::DisplayRole)
	// 	{
	// 		return styleLists[index.row()]->getName().c_str();
	// 	}
	// 	if (role == Qt::DecorationRole)
	// 	{
	// 		return QIcon(styleLists[index.row()]->getPreview().c_str());
	// 	}
	if (role == Qt::UserRole)
	{
		return QVariant::fromValue(styleLists[index.row()]);
	}
	return QVariant();
}

int ImageItemModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 1;
}

Qt::ItemFlags ImageItemModel::flags(const QModelIndex& index) const
{
	return QAbstractListModel::flags(index);
}
