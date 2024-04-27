#pragma once
#include <qstyleditemdelegate.h>

class StyleItemDelegate : public QStyledItemDelegate
{
public:
	explicit StyleItemDelegate(QObject* parent = nullptr);

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

