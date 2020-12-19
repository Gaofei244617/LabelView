#include "LabelView.h"

LabelView::LabelView(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::LabelViewClass())
{
    ui->setupUi(this);

    // 响应键盘输入
    QObject::connect(ui->graphicsView, &MyQGraphicsView::keyboardSignal, this, &LabelView::actionOnKeyboard);
}

void LabelView::actionOnKeyboard(QString key)
{
    if (key == "Esc" && this->isFullScreen())
    {
        setWindowState(Qt::WindowMaximized);      // 窗口最大化, 退出全屏
        return;
    }
    if (key == "Enter")
    {
        setWindowState(Qt::WindowFullScreen);     // 全屏显示
        return;
    }
}

// 设置初始化窗口提示文字
void LabelView::setHint(const QString& str)
{
    auto item = ui->graphicsView->m_scene->addText(str, QFont("Microsoft YaHei", 40, QFont::Bold));
    auto size = ui->graphicsView->size();
    int x = size.width() / 2 - item->boundingRect().width() / 2;
    int y = size.height() / 2 - item->boundingRect().height() / 2;
    item->setDefaultTextColor(QColor(220, 220, 220));
    item->setPos(x, y);
}
