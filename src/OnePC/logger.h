/**
 * @file logger.h
 * @author YoHen Fu (alittlefu@163.com)
 * @brief The MagLogger class is used to log messages to a file and to a vector of strings.
*/

#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>

#include <QString>
#include <QScrollBar>
#include <QTextEdit>

/**
 * @brief MagLogger类用于记录日志信息.
 * 
 * MagLogger类用于记录日志信息, 包括debug, info, warning, error, fatal等等级别的日志信息.
 * 日志信息会被记录到字符串数组中.
 * 日志信息会被输出到字符串数组中, 但可以通过调用getDebugLog(), getInfoLog(), getWarningLog(), getErrorLog(), getFatalLog()等函数来获取日志信息.
*/

class MagLogger {
public:
    static MagLogger* instance();
    std::string currentTime();
    /**
     * @brief 记录debug级别的日志信息.
     * 
     * 记录日志信息, 并将日志信息添加到日志信息数组中.
     * @param message debug级别的日志信息.
    */
    void setDebugLog(const std::string& message);
    /**
     * @brief 记录info级别的日志信息.
     * 
     * 记录日志信息, 并将日志信息添加到日志信息数组中.
     * @param message info级别的日志信息.
    */
    void setInfoLog(const std::string& message);
    /**
     * @brief 记录warning级别的日志信息.
     * 
     * 记录日志信息, 并将日志信息添加到日志信息数组中.
     * @param message info级别的日志信息.
    */
    void setWarningLog(const std::string& message);
    /**
     * @brief 记录error级别的日志信息.
     * 
     * 记录日志信息, 并将日志信息添加到日志信息数组中.
     * @param message error级别的日志信息.
    */
    void setErrorLog(const std::string& message);
    /**
     * @brief 记录fatal级别的日志信息.
     * 
     * 记录日志信息, 并将日志信息添加到日志信息数组中.
     * @param message fatal级别的日志信息.
    */
    void setFatalLog(const std::string& message);
    /**
     * @brief 获取日志debug级别的日志信息数组.
     * 
     * 获取debug级别的日志信息数组.
     * @return debug级别的日志信息数组.
    */
    std::vector<std::string> getDebugLog() const { return debugLog; }
    /**
     * @brief 获取日志info级别的日志信息数组.
     * 
     * 获取info级别的日志信息数组.
     * @return info级别的日志信息数组.
    */
    std::vector<std::string> getInfoLog() const { return infoLog; }
    /**
     * @brief 获取日志warning级别的日志信息数组.
     * 
     * 获取warning级别的日志信息数组.
     * @return warning级别的日志信息数组.
    */
    std::vector<std::string> getWarningLog() const { return warningLog; }
    /**
     * @brief 获取日志error级别的日志信息数组.
     * 
     * 获取error级别的日志信息数组.
     * @return error级别的日志信息数组.
    */
    std::vector<std::string> getErrorLog() const { return errorLog; }
    /**
     * @brief 获取日志fatal级别的日志信息数组.
     *        
     * 获取fatal级别的日志信息数组.
     * @return fatal级别的日志信息数组.
    */
    std::vector<std::string> getFatalLog() const { return fatalLog; }
    /**
     * @brief 获取所有级别的日志信息数组.
     * 
     * 获取所有级别的日志信息数组.
     * @return 所有级别的日志信息数组.
    */
    std::vector<std::string> getAllLog() const { return magAllLog; }
public:
    /**
     * @brief 输出日志信息到文件.
     * 
     * 输出日志信息到文件, 文件名为"fileName.log".
     * @param fileName 日志文件名.
    */
    void exportLog(const std::string& fileName);

    void updateLog(QTextEdit* messageShow);
private:
    /**
     * @brief 构造函数.
     *
     * 构造函数, 初始化日志信息数组.
    */
    MagLogger() {}
    /**
     * @brief 析构函数.
     *
     * 析构函数, 释放日志信息数组.
    */
    std::vector<std::string> magAllLog;
    std::vector<std::string> debugLog;
    std::vector<std::string> infoLog;
    std::vector<std::string> warningLog;
    std::vector<std::string> errorLog;
    std::vector<std::string> fatalLog;
};
