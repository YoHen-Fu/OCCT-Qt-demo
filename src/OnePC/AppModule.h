#ifndef APPMODULE_H
#define APPMODULE_H

#include "Settings.h"
#include "logger.h"
#include "GuiApplication.h"

class AppModule
{
public:
    static AppModule* instance();
    Settings* getSettings(){ return Settings::instance(); }
    MagLogger* getLogger(){ return MagLogger::instance(); }
    // void setGuiApplication(GuiApplication* guiApp){ m_guiApp = guiApp; }
    // GuiApplication* getGuiApplication(){ return m_guiApp; }
private:
    AppModule();
    // GuiApplication* m_guiApp;
};

#endif // APPMODULE_H
