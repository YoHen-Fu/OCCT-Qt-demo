#include "Command_api.h"
#include "GuiApplication.h"

// Command类
Command::Command(IAppContext* appContext)
    : m_appContext(appContext)
{

}
void Command::setAction(QAction* action)
{
    m_action = action;
    QObject::connect(action, &QAction::triggered, this, &Command::execute);
}

// 工具栏命令
Command_ShowPC::Command_ShowPC(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/OnePC.png"), "显示笔记本", this);
    // QAction* action = new QAction(QIcon(":/figures/OnePC.png"), "显示笔记本", guiWidget);
    this->setAction(action);
}
void Command_ShowPC::execute(){
    auto guiApplication = this->getappContext()->getguiApplication();
    guiApplication->getguiWidget()->getModel().ShowPC();
    // this->getguiWidget()->getModel().ShowPC();
    // std::map<std::string, uint> model_infs = this->getmodelTree()->init(&this->getguiWidget()->getModel());
    // for(auto model_inf : model_infs){
    //     // 创建根节点
    //     QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->TreeWidget);
    //     rootItem->setText(model_inf.second, QString::fromStdString(model_inf.first));
    //     rootItem->setCheckState(0, Qt::Checked);
    // }
}

Command_HidePC::Command_HidePC(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/OnePC.png"), "隐藏笔记本", this);
    this->setAction(action);

}
void Command_HidePC::execute(){
    auto guiApplication = this->getappContext()->getguiApplication();
    guiApplication->getguiWidget()->getModel().HidePC();
}

Command_ViewFit::Command_ViewFit(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/FitView.png"), "适应屏幕", this);
    this->setAction(action);
}
void Command_ViewFit::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->FitView();
}

Command_ViewFront::Command_ViewFront(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/FrontView.png"), "正视图", this);
    this->setAction(action);

}
void Command_ViewFront::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->FrontView();
}

Command_ViewLeft::Command_ViewLeft(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/LeftView.png"), "左视图", this);
    this->setAction(action);

}
void Command_ViewLeft::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->LeftView();
}

Command_ViewTop::Command_ViewTop(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/TopView.png"), "俯视图", this);
    this->setAction(action);

}
void Command_ViewTop::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->TopView();
}

Command_ViewIsometric::Command_ViewIsometric(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/IsometricView.png"), "等轴测视图", this);
    this->setAction(action);

}
void Command_ViewIsometric::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->IsometricView();
}
// 开合动画
Command_OpenPCAct::Command_OpenPCAct(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/OnePC.png"), "打开笔记本", this);
    this->setAction(action);
}
void Command_OpenPCAct::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->getModel().OnePCOpen();
}

Command_ClosePCAct::Command_ClosePCAct(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/OnePC.png"), "合上笔记本", this);
    this->setAction(action);
}

void Command_ClosePCAct::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->getModel().OnePCClose();
}
// 框线
Command_ShowFrame::Command_ShowFrame(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/OnePC.png"), "显示框线", this);
    this->setAction(action);
}
void Command_ShowFrame::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->getModel().ShowFrame();
}

Command_HideFrame::Command_HideFrame(IAppContext* appContext)
    : Command{appContext}
{
    QAction* action = new QAction(QIcon(":/figures/OnePC.png"), "隐藏框线", this);
    this->setAction(action);
}
void Command_HideFrame::execute(){
    auto guiWidget = this->getappContext()->getguiApplication()->getguiWidget();
    guiWidget->getModel().HideFrame();
}

// CommandContainer类
CommandContainer::CommandContainer(IAppContext* appContext)
    : m_appContext{appContext}
{

}
void CommandContainer::setAppContext(IAppContext* appContext)
{
    // assert(!m_appContext && m_mapCommand.empty());
    m_appContext = appContext;
}
void CommandContainer::addCommand_impl(std::string_view name, Command* cmd)
{
    auto [it, ok] = m_mapCommand.insert({ name, cmd });
}

Command* CommandContainer::findCommand(std::string_view name) const
{
    auto it = m_mapCommand.find(name);
    return it != m_mapCommand.cend() ? it->second : nullptr;
}

QAction* CommandContainer::findCommandAction(std::string_view name) const
{
    auto cmd = this->findCommand(name);
    return cmd ? cmd->action() : nullptr;
}
