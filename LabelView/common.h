#pragma once

#include <unordered_map>
#include <QString>
#include <QPoint>
#include <QVector>
#include <QPoint>
#include <QSize>
#include <QPolygon>
#include <QLine>
//#include "rapidjson/document.h"
//#include <tuple>
#include <QFile>
//#include <opencv2/opencv.hpp>
#include <QImage>

// ��ע��Ϣ
struct BndBox
{
	QString name;  // Ŀ������
	float conf;    // ���Ŷ�(Ĭ��ֵС��0)
	int xmin;      // Ŀ���λ��(��ֱ�)
	int xmax;
	int ymin;
	int ymax;
	BndBox() :conf(-1), xmin(0), xmax(0), ymin(0), ymax(0) {}
};

struct ImageInfo
{
	QString file_path;      // ͼƬ·��
	QVector<BndBox> boxes;  // ��ע��Ϣ
};

struct Grid
{
	int horizon;
	int vertical;
	Grid() :horizon(1), vertical(1) {}
	int count() { return horizon * vertical; }
};

enum class GridView { increase, decrease };

// ��ȡ�ļ���������ͼƬ�ļ��ļ�
QStringList getImgFileNames(const QString& path);

// ��ȡ�ļ���������xml�ļ��ļ�
QStringList getXmlFileNames(const QString& path);

QPoint toRelativePoint(const QPoint& pt, const QSize& size);
QPoint toPixelPoint(const QPoint& pt, const QSize& size);
QPolygon toPixelPolygon(const QPolygon& polygon, const QSize& size);

// ����Ŀ���(��ֱ��������)
QVector<BndBox> getBndBox(const QString& label_file, const QSize& img_size);
