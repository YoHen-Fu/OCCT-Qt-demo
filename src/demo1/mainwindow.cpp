#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 创建一个名为m_occWidget的OCCWidget实例
    m_occWidget = new OCCWidget(this);
    // 将OCCWidget设置为主窗口的中央部件
    this->setCentralWidget(m_occWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
