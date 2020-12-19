#pragma once

#include <QGraphicsItem>
#include <QImage>
#include "common.h"

class MyQGraphicsItem :public QGraphicsItem
{
public:
    MyQGraphicsItem(QGraphicsItem* parent = nullptr);
    MyQGraphicsItem(const QImage& img, QGraphicsItem* parent = nullptr);

    ~MyQGraphicsItem();
    void setObjBox(const QVector<BndBox>& boxes);

protected:
    QRectF boundingRect()const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    QVector<BndBox> m_boxes;
    QImage m_img;
};