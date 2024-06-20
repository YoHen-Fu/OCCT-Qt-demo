#ifndef IAPPCONTEXT_H
#define IAPPCONTEXT_H

#include <QObject>

class GuiApplication;

class IAppContext : public QObject
{
public:
    IAppContext(QObject* parent);
    virtual GuiApplication* getguiApplication() = 0;
// private:
//     GuiApplication* m_guiApplication = nullptr;
};

#endif // IAPPCONTEXT_H
