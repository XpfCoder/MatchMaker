#include "StyleItemDelegate.h"
#include <qpainter.h>
//#include "StyleCommon.h"
#include "PublicDataObj.h"
static const int PicWidth = 200;
static const int PicHeight = 180;
static const int TextHeight = 20;
static const int Space = 3;

#pragma execution_character_set("utf-8")

StyleItemDelegate::StyleItemDelegate(QObject* parent /*= nullptr*/)
	:QStyledItemDelegate(parent)
{
	
}

void StyleItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.data(Qt::UserRole).canConvert<PublicDataObjPtr>())
	//if(true)
	{
		PublicDataObjPtr style = qvariant_cast<PublicDataObjPtr>(index.data(Qt::UserRole));
		painter->save();
		painter->fillRect(option.rect, QColor(100, 100, 100, 80));

		if (true)
		{
			painter->fillRect(option.rect, QColor(255, 0, 0, 100));
		}
		if (option.state & QStyle::State_Selected)
		{
			painter->fillRect(option.rect, QColor(0, 255, 0, 100));
		}
		if (option.state & QStyle::State_MouseOver)
		{
			painter->fillRect(option.rect, option.palette.highlight());
		}

		QRect rect = option.rect;
		//QPixmap mp(style->getPreview().c_str());
		QPixmap mp("D:/r_kernal_code/bin/bin_64/builtin/texture/00剖开看地下/00剖开看地下_00002.png");
		painter->drawPixmap(rect.x() + Space, rect.y() + Space, rect.width() - Space * 2, rect.height() - TextHeight - Space * 3, mp);
		painter->setFont(QFont(u8"微软雅黑", 12, QFont::Normal));
		painter->drawText(QRect(rect.x(), rect.y() + rect.height() - TextHeight - Space, rect.width(), TextHeight), Qt::AlignCenter | Qt::TextWordWrap, style->m_name.c_str());
		painter->restore();
	}
	else
	{
		QStyledItemDelegate::paint(painter, option, index);
	}                                                                                      
}

QSize StyleItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(PicWidth, PicHeight);
}



