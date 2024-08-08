#include "cfgl_LocalizationStringLoader.h"

#include"cfgl_LocalizationStringModule.h"

namespace rw {
    namespace cfgl {
        LocalizationStringLoaderXML* LocalizationStringLoaderXML::m_instance = nullptr;

        LocalizationStringLoaderXML::LocalizationStringLoaderXML
        ()
            :m_LocalStringComponet(new LocalizationStringXML) {}

        LocalizationStringLoaderXML::LocalizationStringLoaderXML
        (const std::string& filePath, const std::string& language)
            :m_XMLFilePath(filePath),
            m_language(language),
            m_LocalStringComponet(new LocalizationStringXML(filePath)) {}

        LocalizationStringLoaderXML*
            LocalizationStringLoaderXML::getInstance
            ()
        {
            if (!m_instance) {
                m_instance = new LocalizationStringLoaderXML();
            }
            return m_instance;
        }

        LocalizationStringLoaderXML*
            LocalizationStringLoaderXML::getInstance
            (const std::string& filePath, const std::string& language)
        {
            if (!m_instance) {
                m_instance = new LocalizationStringLoaderXML(filePath, language);
            }
            return m_instance;
        }

        LocalizationStringLoaderXML::~LocalizationStringLoaderXML
        ()
        {
            delete m_LocalStringComponet;
        }

        bool
            LocalizationStringLoaderXML::loadData
            ()
        {
            m_LocalStringComponet->setFilePath(m_XMLFilePath);
            if (!m_LocalStringComponet->loadFile()) { return false; }
            return m_LocalStringComponet->getMap(m_map, m_language);
        }

        const std::string&
            LocalizationStringLoaderXML::getString
            (const std::string id)
        {
            return m_map[id];
        }
    }

}
