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

// 标注信息
struct BndBox
{
	QString name;  // 目标类型
	float conf;    // 置信度(默认值小于0)
	int xmin;      // 目标框位置(万分比)
	int xmax;
	int ymin;
	int ymax;
	BndBox() :conf(-1), xmin(0), xmax(0), ymin(0), ymax(0) {}
};

struct ImageInfo
{
	QString file_path;      // 图片路径
	QVector<BndBox> boxes;  // 标注信息
};

struct Grid
{
	int horizon;
	int vertical;
	Grid() :horizon(1), vertical(1) {}
	int count() { return horizon * vertical; }
};

enum class GridView { increase, decrease };

// 获取文件夹下所有图片文件文件
QStringList getImgFileNames(const QString& path);

// 获取文件夹下所有xml文件文件
QStringList getXmlFileNames(const QString& path);

QPoint toRelativePoint(const QPoint& pt, const QSize& size);
QPoint toPixelPoint(const QPoint& pt, const QSize& size);
QPolygon toPixelPolygon(const QPolygon& polygon, const QSize& size);

// 解析目标框(万分比相对坐标)
QVector<BndBox> getBndBox(const QString& label_file, const QSize& img_size);
