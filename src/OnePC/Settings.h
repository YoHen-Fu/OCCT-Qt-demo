#pragma once

// 第三方库
#include <QObject>
#include <QSettings>
// 标准库

// 自定义库

class Settings{
public:
    static Settings* instance();
    void setdpiScale(double dpiScale_);
    double getdpiScale();
private:
    Settings();
    double dpiScale_;
};
