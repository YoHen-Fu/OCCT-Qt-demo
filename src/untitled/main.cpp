#include <QApplication>
#include <QWidget>

#include "myopengl.h"


int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QtWidgetsApplication1 window;
    window.show();

    return app.exec();
}
