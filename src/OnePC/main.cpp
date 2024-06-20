#include <QApplication>

#include "AppModule.h"
#include "Settings.h"
#include "GuiApplication.h"


static int runApp(QApplication* application){
    auto appModule = AppModule::instance();

    auto settings = appModule->getSettings();
    settings->setdpiScale(static_cast<double> (application->devicePixelRatio()));

    GuiApplication* GuiApp = new GuiApplication();
    GuiApp->setAttribute(Qt::WA_DeleteOnClose);
    // appModule->setGuiApplication(GuiApp);
    GuiApp->show();
    GuiApp->setWindowTitle("OnePC");
    const QIcon icon(":/figures/OnePC.png");
    GuiApp->setWindowIcon(icon);
    return application->exec();
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    return runApp(&application);
}
