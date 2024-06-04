#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include "occwidget.h"

namespace Ui {
class MainWindow;
}
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    OCCWidget* m_occWidget = nullptr;
};

#endif // MAINWINDOW_H
