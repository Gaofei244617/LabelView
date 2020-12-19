#include "LabelView.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LabelView w;
    w.show();
    w.resize(850, 550);
    w.setHint("Drag Files Here");
    return a.exec();
}
