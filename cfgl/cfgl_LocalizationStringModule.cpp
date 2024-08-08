#include "cfgl_LocalizationStringModule.h"

#include<pugixml.hpp>
namespace rw
{
    namespace cfgl {
        LocalizationStringXML::LocalizationStringXML
        (const std::string& filePath)
            :m_filePath(filePath), m_doc(new pugi::xml_document)
        {

        }
        LocalizationStringXML::LocalizationStringXML()
            :m_doc(new pugi::xml_document)
        {

        }
        LocalizationStringXML::~LocalizationStringXML()
        {
            delete m_doc;
        }
        bool
            LocalizationStringXML::getMap
            (std::unordered_map<std::string, std::string>& map, const std::string& language)
        {
            auto LocalStringNode = m_doc->child("MyLocalizationStringModele_strings");
            if (!LocalStringNode) { return false; }
            pugi::xml_node languagaNode{};
            for (const auto& stringNode : LocalStringNode.children()) {
                languagaNode = stringNode.find_child_by_attribute("language", language.c_str());
                map[stringNode.attribute("id").value()] = languagaNode.text().get();
            }
            return true;
        }

        std::string LocalizationStringXML::localString(const std::string& id, const std::string& language)
        {
            std::string result{};
            localString(result, id, language);
            return result;
        }

        bool LocalizationStringXML::localString(std::string& target, const std::string& id, const std::string& language)
        {
            auto LocalStringNode = m_doc->child("MyLocalizationStringModele_strings");
            auto stringNode = LocalStringNode.find_child_by_attribute("id", id.c_str());
            if (stringNode.empty()) { return false; }

            auto languageNode = stringNode.find_child_by_attribute("language", language.c_str());
            if (languageNode.empty()) { return false; }
            target = move(std::string(languageNode.text().get()));

            return true;
        }

        bool LocalizationStringXML::storeLocalString(const std::string& id, const std::string& language, const std::string& src)
        {
            auto LocalStringNode = m_doc->child("MyLocalizationStringModele_strings");
            auto findStringNode = LocalStringNode.find_child_by_attribute("id", id.c_str());
            if (!findStringNode) {
                auto newStringNode = LocalStringNode.append_child("string");
                newStringNode.append_attribute("id").set_value(id.c_str());

                auto newLanguage = newStringNode.append_child("language");
                newLanguage.append_attribute("language").set_value(language.c_str());

                return newLanguage.text().set(src.c_str());
            }
            else {
                if (!findStringNode.find_child_by_attribute("language", language.c_str())) {
                    auto newLanguage = findStringNode.append_child("language");
                    newLanguage.append_attribute("language").set_value(language.c_str());

                    return newLanguage.text().set(src.c_str());
                }
            }
            return false;
        }

        bool LocalizationStringXML::loadFile()
        {
            if (m_doc->load_file(m_filePath.c_str())) { return true; }
            else { return false; }
        }

        bool LocalizationStringXML::saveFile()
        {
            return m_doc->save_file(m_filePath.c_str());
        }

        bool LocalizationStringXML::changeLocalString(const std::string& id, const std::string& language, const std::string& src)
        {
            auto LocalStringNode = m_doc->child("MyLocalizationStringModele_strings");

            auto findStringNode = LocalStringNode.find_child_by_attribute("id", id.c_str());
            if (!findStringNode) { return false; }

            auto findLanguageNode = findStringNode.find_child_by_attribute("language", language.c_str());
            if (!findLanguageNode) { return false; }

            return findLanguageNode.text().set(src.c_str());
        }

    }

}
