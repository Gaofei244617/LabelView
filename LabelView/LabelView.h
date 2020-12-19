#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LabelView.h"

class LabelView : public QMainWindow
{
    Q_OBJECT

public:
    LabelView(QWidget *parent = Q_NULLPTR);
    void actionOnKeyboard(QString key);
    void setHint(const QString& str);   // ���ó�ʼ��������ʾ����

private:
    Ui::LabelViewClass* ui;
};
