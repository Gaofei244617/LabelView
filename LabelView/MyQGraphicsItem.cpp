#include "MyQGraphicsItem.h"
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QGraphicsScene>
#include <QGraphicsView>

MyQGraphicsItem::MyQGraphicsItem(QGraphicsItem* parent) :
	QGraphicsItem(parent)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

MyQGraphicsItem::MyQGraphicsItem(const QImage& img, QGraphicsItem* parent) :
	QGraphicsItem(parent), m_img(img)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

MyQGraphicsItem::~MyQGraphicsItem() {}

void MyQGraphicsItem::setObjBox(const QVector<BndBox>& boxes)
{
	this->m_boxes = boxes;
	this->update();
}

QRectF MyQGraphicsItem::boundingRect() const
{
	auto size = m_img.size();
	int w = size.width();
	int h = size.height();
	return QRectF(0, 0, w, h);
}

// ªÊÕº
void MyQGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	auto size = m_img.size();
	painter->drawImage(boundingRect(), m_img);

	// ªÊ÷∆ƒø±ÍøÚ
	if (m_boxes.size() > 0)
	{
		painter->setRenderHints(QPainter::Antialiasing, true); // øπæ‚≥›
		painter->setFont(QFont("Helvetica", 12));              // ±Í«©◊÷ÃÂ

		const int width = size.width();
		const int height = size.height();
		int x = 0, y = 0, w = 0, h = 0;
		for (const auto& box : m_boxes)
		{
			x = box.xmin / 10000.0 * width;;
			y = box.ymin / 10000.0 * height;
			w = (box.xmax - box.xmin) / 10000.0 * width;
			h = (box.ymax - box.ymin) / 10000.0 * height;

			QString label = box.name;
			if (box.conf > 0)
			{
				label = label + " " + QString::number(box.conf, 'f', 2);
			}
			painter->setPen(QPen(Qt::red, 1.2, Qt::DashDotLine));
			painter->drawRect(x, y, w, h);           // ƒø±ÍøÚ

			QFontMetrics fm = painter->fontMetrics();
			auto label_rect = fm.tightBoundingRect(label);
			int label_width = label_rect.width() + 4;
			int label_height = label_rect.height() + 4;
			QRect rect(x - 1, y - label_height, label_width, label_height);
			painter->fillRect(rect, Qt::red);        // ±Í«©±≥æ∞

			painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
			painter->drawText(x - 1, y - fm.descent(), label);   // ±Í«©
		}
	}
}