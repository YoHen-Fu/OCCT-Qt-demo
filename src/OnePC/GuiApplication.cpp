#include "GuiApplication.h"
#include "logger.h"
#include "AppContext.h"

GuiApplication::GuiApplication(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GuiApplication)
    , guiWidget(new GuiWidget(this))
{
    ui->setupUi(this);

    m_appContext = new AppContext(this);
    m_cmdContainer.setAppContext(m_appContext);

    QList<int> MainWindowSplitterScale{0, 500};
    QList<int> WindowSplitterScale{600, 100};
    ui->MainWindowSplitter->setSizes(MainWindowSplitterScale);
    ui->WindowSplitter->setSizes(WindowSplitterScale);
    ui->MessageShow->setReadOnly(true);
    ui->GUIWidget->addWidget(guiWidget);
    // ui->TreeWidget->hide();

    MagLogger::instance()->setInfoLog("程序启动成功！");
    MagLogger::instance()->updateLog(ui->MessageShow);  //更新显示日志信息于窗口

    // this->createModelTree();
    this->createCommands();
    this->createMenus();




    // connect(ShowPC, &QAction::triggered, this->guiWidget, [&](){
    //     this->guiWidget->getModel().ShowPC();
    //     std::map<std::string, uint> model_infs = modelTree->init(&this->guiWidget->getModel());
    //     for(auto model_inf : model_infs){
    //         // 创建根节点
    //         QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->TreeWidget);
    //         rootItem->setText(model_inf.second, QString::fromStdString(model_inf.first));
    //         rootItem->setCheckState(0, Qt::Checked);
    //     }
    // });
}

GuiApplication::~GuiApplication()
{
    MagLogger::instance()->exportLog("OnePC.log");  //将日志写入文件
    delete ui;
    ui = nullptr;
}

void GuiApplication::createCommands(){
    // "file" commands
    this->addCommand<Command_ShowPC>();
    this->addCommand<Command_HidePC>();
    // "view" commands
    this->addCommand<Command_ViewFit>();
    this->addCommand<Command_ViewFront>();
    this->addCommand<Command_ViewLeft>();
    this->addCommand<Command_ViewTop>();
    this->addCommand<Command_ViewIsometric>();
    // "animation" commands
    this->addCommand<Command_OpenPCAct>();
    this->addCommand<Command_ClosePCAct>();
    // "frame" commands
    this->addCommand<Command_ShowFrame>();
    this->addCommand<Command_HideFrame>();
}
void GuiApplication::createMenus(){
    auto fnAddAction = [=](QToolBar* toolBar, std::string_view commandName) {
        ui->toolBar->addAction(m_cmdContainer.findCommandAction(commandName));
    };
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fnAddAction(ui->toolBar, Command_ShowPC::Name);
    fnAddAction(ui->toolBar, Command_HidePC::Name);
    ui->toolBar->addSeparator();  //插入分隔线
    fnAddAction(ui->toolBar, Command_ViewFit::Name);
    fnAddAction(ui->toolBar, Command_ViewFront::Name);
    fnAddAction(ui->toolBar, Command_ViewLeft::Name);
    fnAddAction(ui->toolBar, Command_ViewTop::Name);
    fnAddAction(ui->toolBar, Command_ViewIsometric::Name);
    ui->toolBar->addSeparator();  //插入分隔线
    fnAddAction(ui->toolBar, Command_OpenPCAct::Name);
    fnAddAction(ui->toolBar, Command_ClosePCAct::Name);
    ui->toolBar->addSeparator();  //插入分隔线
    fnAddAction(ui->toolBar, Command_ShowFrame::Name);
    fnAddAction(ui->toolBar, Command_HideFrame::Name);
}

void GuiApplication::createModelTree(){
    modelTree = new ModelTree(this);
    ui->TreeWidget->setColumnCount(1); // 设置树的列数
    QFont font("Arial", 10, QFont::Normal); // 创建一个字体对
    ui->TreeWidget->setFont(font);
    // 创建树的头部
    ui->TreeWidget->setHeaderLabels(QStringList() << "文件名称");
}

