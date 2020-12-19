#include "MyQGraphicsView.h"
#include "MyQGraphicsScene.h"
#include <QMessageBox>
#include "common.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include <QFileDialog>

MyQGraphicsView::MyQGraphicsView(QWidget* parent)
	:QGraphicsView(parent),
	m_item(nullptr),
	m_scene(new MyQGraphicsScene(this)),
	m_imgIndex(-1)
{
	this->setStyleSheet("padding: 0px; border: 0px;");
	this->setScene(m_scene);
	this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

// ����ƶ��¼�
void MyQGraphicsView::mouseMoveEvent(QMouseEvent* event) {}

// ��갴���¼�
void MyQGraphicsView::mousePressEvent(QMouseEvent* event) {}

// ��굥���ͷ��¼�
void MyQGraphicsView::mouseReleaseEvent(QMouseEvent* event) {}

// �����¼�
void MyQGraphicsView::resizeEvent(QResizeEvent* event)
{
	auto w = this->width();
	auto h = this->height();
	m_scene->setSceneRect(0, 0, w, h);
	updateView();
}

void MyQGraphicsView::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

// ��ק�ͷ��¼�
void MyQGraphicsView::dropEvent(QDropEvent* event)
{
	const QMimeData* qm = event->mimeData();          // ��ȡMIMEData
	QString filePath = qm->urls()[0].toLocalFile();   // ��ȡ�϶��ļ��ı���·��
	actionOnOpenFile(filePath);
	if (m_imgIndex < 0 && m_images.size() > 0)
	{
		m_imgIndex = 0;
	}
	updateView();  // ������ͼ
}

// ���������¼�
void MyQGraphicsView::keyPressEvent(QKeyEvent* event)
{
	int step = m_grid.horizon * m_grid.vertical;
	// �������
	if (event->key() == Qt::Key_Left)
	{
		if (m_item != nullptr)
		{
			m_scene->removeItem(m_item);
			delete m_item;
			m_item = nullptr;
		}
		if (m_imgIndex <= m_grid.count() - 1)
		{
			QMessageBox::about(nullptr, nullptr, "No more images");
			return;
		}
		m_imgIndex = std::max(int(m_grid.count() - 1), m_imgIndex - step);
		updateView();
		return;
	}
	// �����Ҽ�
	if (event->key() == Qt::Key_Right)
	{
		if (m_item != nullptr)
		{
			m_scene->removeItem(m_item);
			delete m_item;
			m_item = nullptr;
		}
		if (m_imgIndex == m_images.size() - 1)
		{
			QMessageBox::about(nullptr, nullptr, "No more images");
			return;
		}
		m_imgIndex = std::min(int(m_images.size() - 1), m_imgIndex + step);
		updateView();
		return;
	}
	// �س���
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
	{
		emit keyboardSignal("Enter");
		return;
	}
	// Esc��
	if (event->key() == Qt::Key_Escape)
	{
		emit keyboardSignal("Esc");
		return;
	}

	// Ctrl + s
	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
	{
		QPixmap img = this->grab();  // ץȡͼƬ
		QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("����ͼƬ"), tr("image.jpg"), tr("Image Files (*.jpg)"));
		if (!fileName.isNull())
		{
			img.save(fileName);      // ����ͼƬ
		}

		return;
	}
}

// �������¼�
void MyQGraphicsView::wheelEvent(QWheelEvent* Event)
{
	// ���Ϲ���
	if ((Event->modifiers() == Qt::ControlModifier) && (Event->angleDelta().y() > 0))
	{
		updateGrid(GridView::decrease);
		updateView();
	}

	// ���¹���
	if ((Event->modifiers() == Qt::ControlModifier) && (Event->angleDelta().y() < 0))
	{
		updateGrid(GridView::increase);
		if (m_imgIndex < m_grid.count() - 1)
		{
			m_imgIndex = std::min(int(m_images.size() - 1), m_grid.count() - 1);
		}
		updateView();
	}
}

// ������ͼ����
void MyQGraphicsView::updateGrid(GridView state)
{
	if (state == GridView::increase)
	{
		// ���10*10
		m_grid.horizon = std::min(10, m_grid.horizon + 1);
		m_grid.vertical = std::min(10, m_grid.vertical + 1);
	}
	else if (state == GridView::decrease)
	{
		// ����1*1
		m_grid.horizon = std::max(1, m_grid.horizon - 1);
		m_grid.vertical = std::max(1, m_grid.vertical - 1);
	}
}

// ������ͼ
void MyQGraphicsView::updateView()
{
	if (m_item != nullptr)
	{
		m_scene->addItem(m_item);
		return;
	}

	int w = m_scene->sceneRect().width() / m_grid.horizon;
	int h = m_scene->sceneRect().height() / m_grid.vertical;

	for (auto item : m_pixmapItems)
	{
		m_scene->removeItem(item);
		delete item;
	}
	m_pixmapItems.clear();

	int idx1 = std::max(0, m_imgIndex + 1 - m_grid.count());
	int idx2 = std::min(m_imgIndex, int(m_images.size() - 1));
	for (int i = idx1; i <= idx2; i++)
	{
		QImage img(m_images[i]);
		QSize ori_img_size = img.size();
		QGraphicsItem* item = new MyQGraphicsItem(img.scaled(w - 1, h - 1));
		item->setToolTip(m_images[i]);  // �����ͣ��ʾ
		m_scene->addItem(item);
		m_pixmapItems.push_back(item);
		int x = (i - idx1) % m_grid.horizon * (w + 1);
		int y = (i - idx1) / m_grid.horizon * (h + 1);
		m_pixmapItems.back()->setPos(x, y);

		QFileInfo fi(m_images[i]);
		//auto file_name = fi.fileName();         // �ļ���(����׺)
		auto file_name = fi.completeBaseName();  // �ļ���(������׺)
		for (const auto& label : m_labels)
		{
			if (file_name == QFileInfo(label).completeBaseName())
			{
				int ww = ori_img_size.width();
				int hh = ori_img_size.height();
				auto boxes = getBndBox(label, QSize(ww, hh));
				static_cast<MyQGraphicsItem*>(m_pixmapItems.back())->setObjBox(boxes);
				break;
			}
		}
	}
}

// ��ק�ļ��ͷ�
void MyQGraphicsView::actionOnOpenFile(QString filePath)
{
	QFileInfo file_info(filePath);

	// ���ļ���
	if (file_info.isDir())
	{
		auto img_list = getImgFileNames(filePath);
		m_images.reserve(m_images.size() + img_list.size());
		for (auto& f : img_list)
		{
			m_images.push_back(std::move(f));
		}
		auto label_list = getXmlFileNames(filePath);  // ������·��
		m_labels.reserve(label_list.size() + m_labels.size());
		for (auto& f : label_list)
		{
			m_labels.push_back(std::move(f));
		}

		// ɾ������ĵ���ͼƬ
		if (m_item != nullptr)
		{
			m_scene->removeItem(m_item);
			delete m_item;
			m_item = nullptr;
		}
		return;
	}

	if (file_info.isFile())
	{
		int w = m_scene->sceneRect().width();
		int h = m_scene->sceneRect().height();
		static QSize ori_img_size(0, 0);

		QString suffix = file_info.suffix();   // �ļ���׺��
		if (suffix == "jpg" || suffix == "png" || suffix == "bmp")
		{
			m_grid.horizon = 1;
			m_grid.vertical = 1;
			QImage img(filePath);
			ori_img_size = img.size();
			m_item = new MyQGraphicsItem(img.scaled(w, h));
			m_item->setToolTip(filePath);  // �����ͣ��ʾ
		}
		if (suffix == "xml" && m_item != nullptr)
		{
			auto boxes = getBndBox(filePath, ori_img_size);
			static_cast<MyQGraphicsItem*>(m_item)->setObjBox(boxes);
		}
	}
}