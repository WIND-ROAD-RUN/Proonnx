#ifndef CONFIGFORIMAGESAVE_H_
#define CONFIGFORIMAGESAVE_H_

#include<QString>
#include<QImage>

class ConfigForImageSave {
private:
	QString m_today{};
	int m_saveDays{};
	QString m_currentFilePath{};
public:
	void setToday(const QString& today) { m_today = today; }
	void setSaveDays(int days) { m_saveDays = days; }
	void setCurrentFilePath(const QString& filePath) { m_currentFilePath = filePath; }
	QString getCurrentFilePath() { return m_currentFilePath; }
	void iniConfig();
private:
	int countSubdirectories();
public:
	bool createDirectory(const QString& folderName);
	bool createDirectory(const QString& folderPath, const QString& folderName);
	bool removeDirectory(const QString& folderName);
	QStringList getSubdirectoryNames();
	QStringList sortDates(const QStringList& dateList);
private:
	static ConfigForImageSave* m_instance;

	ConfigForImageSave();
public:
	static ConfigForImageSave*
		getInstance() {
		if (!m_instance) {
			m_instance = new ConfigForImageSave();
		}
		return m_instance;
	}
public:
	void saveImage(const QImage& image, const QString& directoryPath, const QString& fileName);


};



#endif // !CONFIGFORIMAGESAVE_H_
