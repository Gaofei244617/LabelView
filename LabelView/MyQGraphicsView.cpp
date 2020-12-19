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

// 鼠标移动事件
void MyQGraphicsView::mouseMoveEvent(QMouseEvent* event) {}

// 鼠标按下事件
void MyQGraphicsView::mousePressEvent(QMouseEvent* event) {}

// 鼠标单击释放事件
void MyQGraphicsView::mouseReleaseEvent(QMouseEvent* event) {}

// 缩放事件
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

// 拖拽释放事件
void MyQGraphicsView::dropEvent(QDropEvent* event)
{
	const QMimeData* qm = event->mimeData();          // 获取MIMEData
	QString filePath = qm->urls()[0].toLocalFile();   // 获取拖动文件的本地路径
	actionOnOpenFile(filePath);
	if (m_imgIndex < 0 && m_images.size() > 0)
	{
		m_imgIndex = 0;
	}
	updateView();  // 更新视图
}

// 键盘输入事件
void MyQGraphicsView::keyPressEvent(QKeyEvent* event)
{
	int step = m_grid.horizon * m_grid.vertical;
	// 键盘左键
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
	// 键盘右键
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
	// 回车键
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
	{
		emit keyboardSignal("Enter");
		return;
	}
	// Esc键
	if (event->key() == Qt::Key_Escape)
	{
		emit keyboardSignal("Esc");
		return;
	}

	// Ctrl + s
	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
	{
		QPixmap img = this->grab();  // 抓取图片
		QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存图片"), tr("image.jpg"), tr("Image Files (*.jpg)"));
		if (!fileName.isNull())
		{
			img.save(fileName);      // 保存图片
		}

		return;
	}
}

// 鼠标滚轮事件
void MyQGraphicsView::wheelEvent(QWheelEvent* Event)
{
	// 向上滚动
	if ((Event->modifiers() == Qt::ControlModifier) && (Event->angleDelta().y() > 0))
	{
		updateGrid(GridView::decrease);
		updateView();
	}

	// 向下滚动
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

// 更新视图网格
void MyQGraphicsView::updateGrid(GridView state)
{
	if (state == GridView::increase)
	{
		// 最多10*10
		m_grid.horizon = std::min(10, m_grid.horizon + 1);
		m_grid.vertical = std::min(10, m_grid.vertical + 1);
	}
	else if (state == GridView::decrease)
	{
		// 最少1*1
		m_grid.horizon = std::max(1, m_grid.horizon - 1);
		m_grid.vertical = std::max(1, m_grid.vertical - 1);
	}
}

// 更新视图
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
		item->setToolTip(m_images[i]);  // 鼠标悬停提示
		m_scene->addItem(item);
		m_pixmapItems.push_back(item);
		int x = (i - idx1) % m_grid.horizon * (w + 1);
		int y = (i - idx1) / m_grid.horizon * (h + 1);
		m_pixmapItems.back()->setPos(x, y);

		QFileInfo fi(m_images[i]);
		//auto file_name = fi.fileName();         // 文件名(含后缀)
		auto file_name = fi.completeBaseName();  // 文件名(不含后缀)
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

// 拖拽文件释放
void MyQGraphicsView::actionOnOpenFile(QString filePath)
{
	QFileInfo file_info(filePath);

	// 是文件夹
	if (file_info.isDir())
	{
		auto img_list = getImgFileNames(filePath);
		m_images.reserve(m_images.size() + img_list.size());
		for (auto& f : img_list)
		{
			m_images.push_back(std::move(f));
		}
		auto label_list = getXmlFileNames(filePath);  // 含绝对路径
		m_labels.reserve(label_list.size() + m_labels.size());
		for (auto& f : label_list)
		{
			m_labels.push_back(std::move(f));
		}

		// 删除拖入的单张图片
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

		QString suffix = file_info.suffix();   // 文件后缀名
		if (suffix == "jpg" || suffix == "png" || suffix == "bmp")
		{
			m_grid.horizon = 1;
			m_grid.vertical = 1;
			QImage img(filePath);
			ori_img_size = img.size();
			m_item = new MyQGraphicsItem(img.scaled(w, h));
			m_item->setToolTip(filePath);  // 鼠标悬停提示
		}
		if (suffix == "xml" && m_item != nullptr)
		{
			auto boxes = getBndBox(filePath, ori_img_size);
			static_cast<MyQGraphicsItem*>(m_item)->setObjBox(boxes);
		}
	}
}