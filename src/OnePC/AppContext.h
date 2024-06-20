#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "IAppContext.h"
#include "GuiApplication.h"

class AppContext : public IAppContext
{
public:
    AppContext(GuiApplication* guiApplication);
    GuiApplication* getguiApplication(){ return m_guiApplication; }
private:
    GuiApplication* m_guiApplication = nullptr;
};

#endif // APPCONTEXT_H
