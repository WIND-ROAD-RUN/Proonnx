#ifndef LOCALIZATIONSTRINGLOADERXML_H_
#define LOCALIZATIONSTRINGLOADERXML_H_

#include"LocalizationStringLoaderAbstract.h"
#include<string>
#include <unordered_map>

/*Implemented as a singleton class*/
class LocalizationStringXML;

class LocalizationStringLoaderXML
    :public LocalizationStringLoaderAbstract {

private:
    static LocalizationStringLoaderXML* m_instance;

    LocalizationStringLoaderXML();

    LocalizationStringLoaderXML(const std::string& filePath, const std::string& language);

private:
    std::string m_XMLFilePath{};

private:
    std::unordered_map<std::string, std::string> m_map;

    std::string m_language{};

    LocalizationStringXML* m_LocalStringComponet{ nullptr };

public:
    static LocalizationStringLoaderXML*
        getInstance() {
        if (!m_instance) {
            m_instance = new LocalizationStringLoaderXML();
        }
        return m_instance;
    }

    static LocalizationStringLoaderXML*
        getInstance(const std::string& filePath, const std::string& language) {
        if (!m_instance) {
            m_instance = new LocalizationStringLoaderXML(filePath, language);
        }
        return m_instance;
    }

    ~LocalizationStringLoaderXML();

public:
    void setFilePath(const std::string& filePath) { m_XMLFilePath = filePath; }

    void setLanguage(const std::string& language) { m_language = language; }

public:
    // Inherit through LocalizationStringLoaderAbstract
    bool loadData() override;

    const std::string& getString(const std::string id) override;

};


#endif // !LOCALIZATIONSTRINGLOADERXML_H_

 