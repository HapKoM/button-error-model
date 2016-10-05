#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    /*w.setStyleSheet("QPushButton {"
                    "background-color: #7F7F7F;"
                    "}");*/
    w.showFullScreen();

    return a.exec();
}
