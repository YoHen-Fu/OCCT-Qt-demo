#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    mainWindow.resize(800,600);
    return app.exec();
}
