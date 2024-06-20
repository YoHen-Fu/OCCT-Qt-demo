#include "logger.h"

MagLogger* MagLogger::instance(){
    static MagLogger magLogger;
    return &magLogger;
}

void MagLogger::updateLog(QTextEdit* messageShow){
    std::vector<std::string> contents = MagLogger::instance()->getAllLog();
    std::string str = std::accumulate(contents.begin(), contents.end(), std::string());
    messageShow->setPlainText(QString::fromStdString(str));
    QScrollBar *verticalScrollBar = messageShow->verticalScrollBar();
    // 将滚动条位置设置为最大值
    verticalScrollBar->setValue(verticalScrollBar->maximum());
}

std::string MagLogger::currentTime() {
    std::time_t currentTime = std::time(0);
    std::tm* timeinfo = std::localtime(&currentTime);
    std::stringstream timeStringStream;
    timeStringStream << (timeinfo->tm_year + 1900) << "-" // 年
                    << std::setw(2) << std::setfill('0') << (timeinfo->tm_mon + 1) << "-" // 月
                    << std::setw(2) << std::setfill('0') << timeinfo->tm_mday << " "  // 日
                    << std::setw(2) << std::setfill('0') << timeinfo->tm_hour << ":" // 时
                    << std::setw(2) << std::setfill('0') << timeinfo->tm_min << ":"  // 分
                    << std::setw(2) << std::setfill('0') << timeinfo->tm_sec; // 秒
    std::string formattedTime = timeStringStream.str();
    return "[" + formattedTime + "]\t";
}

void MagLogger::setDebugLog(const std::string& message) {
    debugLog.push_back(currentTime() + "DEBUG: " + message + "\n");
    magAllLog.push_back(currentTime() + "DEBUG: " + message + "\n");
}   


void MagLogger::setInfoLog(const std::string& message) {
    infoLog.push_back(currentTime() + "INFO: " + message + "\n");
    magAllLog.push_back(currentTime() + "INFO: " + message + "\n");
}

void MagLogger::setWarningLog(const std::string& message) {
    warningLog.push_back(currentTime() + "WARNING: " + message + "\n");
    magAllLog.push_back(currentTime() + "WARNING: " + message + "\n");
}

void MagLogger::setErrorLog(const std::string& message) {
    errorLog.push_back(currentTime() + "ERROR: " + message + "\n");
    magAllLog.push_back(currentTime() + "ERROR: " + message + "\n");
}

void MagLogger::exportLog(const std::string& fileName){
    std::ofstream ofs(fileName, std::ios::out);
    for (auto& log : magAllLog) {
        ofs << log;
    }
    ofs.close();
}


