#pragma once
#include <qabstractitemmodel.h>
#include <QStringListModel>
#include "PublicDataObj.h"
class ImageItemModel :
	public QAbstractListModel
{
public:
	ImageItemModel();
	//ImageItemModel(const std::vector<QString>& list);
	ImageItemModel(const std::vector<PublicDataObjPtr>& list);
	~ImageItemModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;

private:
	std::vector<PublicDataObjPtr> styleLists;
};

