#include "AppModule.h"

AppModule* AppModule::instance(){
    static AppModule* appModule;
    return appModule;
}

