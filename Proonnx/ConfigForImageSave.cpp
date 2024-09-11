#include "ConfigForImageSave.h"

#include<QDir>
#include<qDebug>
#include<QDate>
#include<filesystem>

ConfigForImageSave* ConfigForImageSave::m_instance = nullptr;

void ConfigForImageSave::iniConfig()
{
    createDirectory(m_today);
    auto saveDirectoryList = sortDates(getSubdirectoryNames());
    auto currentCount = countSubdirectories();
    if (currentCount >m_saveDays) {
        auto deleteCount = currentCount - m_saveDays;
        for (int i = 0;i< deleteCount;++i) {
            removeDirectory(saveDirectoryList[i]);
        }
    }
    std::filesystem::path path(m_currentFilePath.toStdString());
    path.append(m_today.toStdString());
    setCurrentFilePath(QString::fromStdString(path.string()));
}

int ConfigForImageSave::countSubdirectories()
{
    QDir dir(m_currentFilePath);
    
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    return dir.entryList().count(); 
}

bool ConfigForImageSave::createDirectory(const QString& folderName)
{
    QDir dir(m_currentFilePath);
    
    if (!dir.exists(folderName)) {
        return dir.mkdir(folderName); 
    }
    return false; 
}

bool ConfigForImageSave::createDirectory(const QString& folderPath, const QString& folderName)
{
    QDir dir(folderPath);

    if (!dir.exists(folderName)) {
        return dir.mkdir(folderName);
    }
    return false;
}

bool ConfigForImageSave::removeDirectory(const QString& folderName)
{
    QDir dir(m_currentFilePath);
    
    QString fullPath = dir.filePath(folderName);

    
    if (dir.exists(fullPath)) {
        return dir.rmdir(fullPath); 
    }
    return false; 
}

QStringList ConfigForImageSave::getSubdirectoryNames()
{
    QDir dir(m_currentFilePath);
    
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    return dir.entryList(); 
}

QStringList ConfigForImageSave::sortDates(const QStringList& dateList)
{
    QStringList sortedList = dateList; 
    std::sort(sortedList.begin(), sortedList.end(), [](const QString& a, const QString& b) {
        return QDate::fromString(a, "yyyy-MM-dd") < QDate::fromString(b, "yyyy-MM-dd");
        });
    return sortedList; 
}

ConfigForImageSave::ConfigForImageSave()
{

}

void ConfigForImageSave::saveImage(const QImage& image, const QString& directoryPath, const QString& fileName)
{
    QString filePath = QDir(directoryPath).filePath(fileName); 
    if (!image.save(filePath)) {
        qWarning("Failed to save image to %s", qPrintable(filePath));
    }
}
