#ifndef LOCALIZATIONSTRINGMODULE_XML_H_
#define LOCALIZATIONSTRINGMODULE_XML_H_

/*
LocalizationStringXML is a subclass of LocalizationStringModulaAbstract that stores localized strings as data files using XML,
Follow the interface of abstract base classes.
*/

/*
    be careful!!!
The implementation of this class relies on the third-party library pugixml. If the project does not have a third-party library installed, please use the project's VNet package manager
Install the third library Pugixml for the project or download the source file and add it to the project, and build it yourself
*/

#include"LocalizationStringModuleAbstact.h"

#include<pugixml.hpp>
#include<string>
#include <unordered_map>

class LocalizationStringXML
    :public LocalizationStringModuleAbstract {
private:
    std::string m_filePath{};
    pugi::xml_document  m_doc;
public:
    LocalizationStringXML(const std::string& filePath) :m_filePath(filePath) {}//It is best to initialize the class using this method
    LocalizationStringXML() {}
    ~LocalizationStringXML() {}
public:
    void setFilePath(const std::string& filePath) { m_filePath = filePath; }//Set file path
    /*
Convert data into a map*/
    bool getMap(std::unordered_map<std::string, std::string>& map, const std::string& language);
public:
    // Inherit through LocalizationStringModulaAbstract
    std::string localString(const std::string& id, const std::string& language) override;

    bool localString(std::string& target, const std::string& id, const std::string& language) override;

    bool storeLocalString(const std::string& id, const std::string& language, const std::string& src) override;

    /*Load data file*/
    bool loadFile()override;

    /*Save the data in memory in a data file*/
    bool saveFile() override;

    // Inherit through LocalizationStringModulaAbstract
    bool changeLocalString(const std::string& id, const std::string& language, const std::string& src) override;
};


#endif // !LOCALIZATIONSTRINGMODULE_XML_H_

