#include "LocalizationStringLoader-XML.h"

#include"LocalizationStringModule-XML.h"


LocalizationStringLoaderXML* LocalizationStringLoaderXML::m_instance = nullptr;

LocalizationStringLoaderXML::
LocalizationStringLoaderXML()
    :m_LocalStringComponet(new LocalizationStringXML) {}

LocalizationStringLoaderXML::
LocalizationStringLoaderXML
(const std::string& filePath, const std::string& language)
    :m_XMLFilePath(filePath), m_language(language), m_LocalStringComponet(new LocalizationStringXML(filePath)) {}

LocalizationStringLoaderXML::
~LocalizationStringLoaderXML() { delete m_LocalStringComponet; delete m_instance; }


bool LocalizationStringLoaderXML::loadData()
{
    m_LocalStringComponet->setFilePath(m_XMLFilePath);
    if (!m_LocalStringComponet->loadFile()) { return false; }
    return m_LocalStringComponet->getMap(m_map,m_language);
}

const std::string& LocalizationStringLoaderXML::getString(const std::string id)
{
    return m_map[id];
}
