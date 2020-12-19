#pragma once

#include <QGraphicsView>
#include <QPoint>
#include <QMouseEvent>
#include <QString>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QUrl>
#include <QGraphicsScene>
#include <QResizeEvent>
#include "MyQGraphicsItem.h"
#include <QGraphicsPixmapItem>
#include <QVector>

class MyQGraphicsView : public QGraphicsView
{
	Q_OBJECT 
public:
	explicit MyQGraphicsView(QWidget* parent = nullptr);

	void mouseMoveEvent(QMouseEvent* event);      // 鼠标移动 
	void mousePressEvent(QMouseEvent* event);     // 鼠标按下
	void mouseReleaseEvent(QMouseEvent* event);   // 鼠标释放
	void keyPressEvent(QKeyEvent* event);         // 键盘输入事件
	void wheelEvent(QWheelEvent* Event);          // 鼠标滚轮事件
	void dragEnterEvent(QDragEnterEvent* event);  // 拖动进入事件
	void dropEvent(QDropEvent* event);            // 拖拽释放
	void resizeEvent(QResizeEvent* event);        // 缩放事件

	void updateView();                            // 更新视图
	void actionOnOpenFile(QString filePath);
	void updateGrid(GridView state);              // 更新视图网格

signals:
	void mouseMoveSignal(QPoint pt, QPoint pt2);
	void openFileSignal(QString str);
	void keyboardSignal(QString key);

public:
	QGraphicsScene* m_scene;                // 视图场景

protected:
	Grid m_grid;                            // 显示网格
	QGraphicsItem* m_item;                  // 存储拖入的单个图片
	QVector<QString> m_images;              // 图片路径
	int m_imgIndex;                         // 当前展示的最后一张图片的索引位置(无图片展示时<0)
	QVector<QString> m_labels;              // 标注文件路径
	QVector<QGraphicsItem*> m_pixmapItems;
};
