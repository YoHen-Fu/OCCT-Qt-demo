#include "settings.h"


Settings* Settings::instance(){
    static Settings settings;
    return &settings;
}

Settings::Settings()
{

}

void Settings::setdpiScale(double dpiScale){
    dpiScale_ = dpiScale;
}
double Settings::getdpiScale(){
    return dpiScale_;
}

