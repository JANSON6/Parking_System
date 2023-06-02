#include "initializewidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QFont font;
//    font.setFamily("/usr/share/fonts/DroidSansFallback.ttf");
//    QApplication::setFont(font);

    InitializeWidget w;
    w.show();

    return a.exec();
}
