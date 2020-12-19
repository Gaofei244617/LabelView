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

	void mouseMoveEvent(QMouseEvent* event);      // ����ƶ� 
	void mousePressEvent(QMouseEvent* event);     // ��갴��
	void mouseReleaseEvent(QMouseEvent* event);   // ����ͷ�
	void keyPressEvent(QKeyEvent* event);         // ���������¼�
	void wheelEvent(QWheelEvent* Event);          // �������¼�
	void dragEnterEvent(QDragEnterEvent* event);  // �϶������¼�
	void dropEvent(QDropEvent* event);            // ��ק�ͷ�
	void resizeEvent(QResizeEvent* event);        // �����¼�

	void updateView();                            // ������ͼ
	void actionOnOpenFile(QString filePath);
	void updateGrid(GridView state);              // ������ͼ����

signals:
	void mouseMoveSignal(QPoint pt, QPoint pt2);
	void openFileSignal(QString str);
	void keyboardSignal(QString key);

public:
	QGraphicsScene* m_scene;                // ��ͼ����

protected:
	Grid m_grid;                            // ��ʾ����
	QGraphicsItem* m_item;                  // �洢����ĵ���ͼƬ
	QVector<QString> m_images;              // ͼƬ·��
	int m_imgIndex;                         // ��ǰչʾ�����һ��ͼƬ������λ��(��ͼƬչʾʱ<0)
	QVector<QString> m_labels;              // ��ע�ļ�·��
	QVector<QGraphicsItem*> m_pixmapItems;
};
