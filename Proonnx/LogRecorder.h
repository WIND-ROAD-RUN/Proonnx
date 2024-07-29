#ifndef LOGRECORDER_H_
#define LOGRECORDER_H_

#include"LogSystemUtilty.h"
#include<string>

class LogFileModule;
struct ProductConfig;
struct RecognizeRange;
struct ProductCountInfo;
struct RejectAttribute;

class LogRecorder {
private:
	LogFileModule* m_logFileModule{nullptr};
private:
    static LogRecorder* m_instance;

    explicit LogRecorder(LogFileModule* logRecorder);
public:
    static LogRecorder*getInstance();

    ~LogRecorder();
public:
    void iniCom();
public:
    void info(const std::string& str);
    void info(const RecognizeRange& str);
    void info(const ProductConfig& str);
    void info(const ProductCountInfo& str);
    void info(const RejectAttribute& str);
    void error(const std::string& str);
    void warn(const std::string& str);
};


#endif // !LOGRECORDER_H_
