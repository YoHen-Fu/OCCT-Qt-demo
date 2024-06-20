#include "AppContext.h"

AppContext::AppContext(GuiApplication* guiApplication)
    : IAppContext(guiApplication)
    , m_guiApplication{guiApplication}
{

}
