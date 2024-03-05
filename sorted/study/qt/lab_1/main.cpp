#include <QApplication>

#include "mainwindow.h"

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QApplication::setStyle("fusion");

    MainWindow window;
    window.show();

    return QApplication::exec();
}
