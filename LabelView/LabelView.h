#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LabelView.h"

class LabelView : public QMainWindow
{
    Q_OBJECT

public:
    LabelView(QWidget *parent = Q_NULLPTR);
    void actionOnKeyboard(QString key);
    void setHint(const QString& str);   // 设置初始化窗口提示文字

private:
    Ui::LabelViewClass* ui;
};
