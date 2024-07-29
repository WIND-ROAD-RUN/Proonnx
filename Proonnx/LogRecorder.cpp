#include"LogRecorder.h"

#include"LogModule.h"
#include"ProductConfigLoader.h"

LogRecorder* LogRecorder::m_instance = nullptr;

LogRecorder* LogRecorder::getInstance()
{
    if (!m_instance) {
        m_instance = new LogRecorder(new LogFileModule());
    }
    return m_instance;
}

LogRecorder::LogRecorder(LogFileModule* logFileRecorder) {
    m_logFileModule = logFileRecorder;
}

LogRecorder::~LogRecorder()
{
}

void LogRecorder::iniCom()
{
    m_logFileModule->iniCom();
}

void LogRecorder::info(const std::string& str)
{
    m_logFileModule->info(str);
}

void LogRecorder::info(const RecognizeRange& str)
{
    info("RecognizeRange is:");
    info("leftLowerCorner: "+ std::to_string(str.leftLowerCorner.first)+","+ std::to_string(str.leftLowerCorner.second));
    info("lowerRightCorner:"+std::to_string(str.lowerRightCorner.first) + "," + std::to_string(str.lowerRightCorner.second));
    info("topLeftCorner:" + std::to_string(str.topLeftCorner.first) + "," + std::to_string(str.topLeftCorner.second));
    info("upperRightCorner:" + std::to_string(str.upperRightCorner.first) + "," + std::to_string(str.upperRightCorner.second));
}

void LogRecorder::info(const ProductConfig& str)
{
    info("ProductConfig is:");
    info("ProductName:"+ str.productName);
    info("ExposureTime:" + std::to_string(str.ExposureTime));
    info("Gain:" + std::to_string(str.gain));
    info("rotateCount:" + std::to_string(str.rotateCount));
}

void LogRecorder::info(const ProductCountInfo& str)
{
    info("ProductCountInfo is:");
    info("TotalCount:" + std::to_string(str.totalCount));
    info("PassCount:" + std::to_string(str.passCount));
    info("OutCount:" + std::to_string(str.outCount));
}

void LogRecorder::info(const RejectAttribute& str)
{
    info("RejectAttribute is:");
    info("DisposalTime:" + std::to_string(str.DisposalTime));
    info("OffsetsNumber:" + std::to_string(str.OffsetsNumber));
    info("RejectDelay:" + std::to_string(str.RejectDelay));
}

void LogRecorder::error(const std::string& str)
{
    m_logFileModule->error(str);
}

void LogRecorder::warn(const std::string& str)
{
    m_logFileModule->warn(str);
}
