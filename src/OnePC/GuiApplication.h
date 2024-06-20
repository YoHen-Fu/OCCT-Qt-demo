#ifndef ONEPC_H
#define ONEPC_H

#include <QWidget>
#include <QToolButton>
#include <QTreeWidget>

#include "ui_GuiApplication.h"
#include "GuiWidget.h"

#include "ModelTree.h"

//临时
#include "Command_api.h"

// class AppContext;

namespace Ui {
class OnePC;
}

class GuiApplication : public QMainWindow
{
    Q_OBJECT

public:
    explicit GuiApplication(QWidget *parent = nullptr);
    ~GuiApplication();
    void createCommands();
    void createMenus();
    void createModelTree();
    GuiWidget* getguiWidget(){ return guiWidget; };
    ModelTree* getmodelTree(){ return modelTree; };
    template<typename CmdType, typename... Args> void addCommand(Args... p) {
        m_cmdContainer.addNamedCommand<CmdType>(std::forward<Args>(p)...);
    }
private:
    Ui::GuiApplication *ui;
    GuiWidget* guiWidget;
    ModelTree* modelTree;
    CommandContainer m_cmdContainer;
    IAppContext* m_appContext = nullptr;
};

#endif // ONEPC_H
