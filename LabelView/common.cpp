#include "common.h"
#include <algorithm>
#include <QMessageBox>
#include <QDomDocument>
#include <QDir>

// ��ȡ�ļ���������ͼƬ�ļ��ļ�
QStringList getImgFileNames(const QString& path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png"; // �ļ����͹���
    // �ļ���
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    for (auto& f : files)
    {
        f = path + "/" + f;
    }
    // ���ļ���
    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto& item : subDirs)
    {
        files.append(getImgFileNames(path + "/" + item));
    }
    return files;
}

// ��ȡ�ļ���������xml�ļ��ļ�
QStringList getXmlFileNames(const QString& path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.xml"; // �ļ����͹���
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    for (auto& f : files)
    {
        f = path + "/" + f;
    }
    // ���ļ���
    QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto& item : subDirs)
    {
        files.append(getXmlFileNames(path + "/" + item));
    }
    return files;
}


QPoint toRelativePoint(const QPoint& pt, const QSize& size)
{
    int x = static_cast<int>(pt.x() * 10000.0 / size.width());
    int y = static_cast<int>(pt.y() * 10000.0 / size.height());
    return QPoint(x, y);
}

QPoint toPixelPoint(const QPoint& pt, const QSize& size)
{
    int x = static_cast<int>(pt.x() / 10000.0 * size.width());
    int y = static_cast<int>(pt.y() / 10000.0 * size.height());
    return QPoint(x, y);
}

QPolygon toPixelPolygon(const QPolygon& polygon, const QSize& size)
{
    QPolygon pixelPolygon(polygon.size());
    std::transform(polygon.cbegin(), polygon.cend(), pixelPolygon.begin(), 
        [&size](const QPoint& pt) {return toPixelPoint(pt, size); });
    return pixelPolygon;
}

// ����Ŀ���
QVector<BndBox> getBndBox(const QString& label_file, const QSize& img_size)
{
    QFile label(label_file);
    QVector<BndBox> boxes;
    int width = img_size.width();
    int height = img_size.height();

    QDomDocument doc;
    if (!doc.setContent(&label, false))
    {
        QMessageBox::warning(nullptr, nullptr, "Can not parse this xml file\n");
        return boxes;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "annotation")
    {
        return boxes;
    }

    QDomNode child = root.firstChild();
    while (!child.isNull())
    {
        if (child.toElement().tagName() == "object")
        {
            BndBox box;
            QDomNode sub_child = child.toElement().firstChild();
            while (!sub_child.isNull())
            {
                if (sub_child.toElement().tagName() == "name")
                {
                    box.name = sub_child.toElement().text();
                }
                else if (sub_child.toElement().tagName() == "confidence")
                {
                    box.conf = sub_child.toElement().text().toFloat();
                }
                else if (sub_child.toElement().tagName() == "bndbox")
                {
                    QDomNode sub2_child = sub_child.toElement().firstChild();
                    while (!sub2_child.isNull())
                    {
                        int num = 0;
                        QString tagName = sub2_child.toElement().tagName();
                        if (tagName == "xmax")
                        {
                            num = sub2_child.toElement().text().toInt();
                            box.xmax = static_cast<int>(num * 10000.0 / width);
                        }
                        else if (tagName == "xmin")
                        {
                            num = sub2_child.toElement().text().toInt();
                            box.xmin = static_cast<int>(num * 10000.0 / width);
                        }
                        else if (tagName == "ymax")
                        {
                            num = sub2_child.toElement().text().toInt();
                            box.ymax = static_cast<int>(num * 10000.0 / height);
                        }
                        else if (tagName == "ymin")
                        {
                            num = sub2_child.toElement().text().toInt();
                            box.ymin = static_cast<int>(num * 10000.0 / height);
                        }
                        sub2_child = sub2_child.nextSibling();
                    }
                }
                sub_child = sub_child.nextSibling();
            }
            boxes.push_back(box);
        }
        child = child.nextSibling();
    }

    return boxes;
}
