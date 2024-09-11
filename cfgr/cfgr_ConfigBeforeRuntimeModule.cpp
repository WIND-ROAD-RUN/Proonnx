#include "cfgr_ConfigBeforeRuntimeModule.h"

#include"pugixml.hpp"

namespace rw {
    namespace cfgr {

        ConfigBeforeRuntimeModule::ConfigBeforeRuntimeModule()
        {
            m_doc = new pugi::xml_document();
        }

        ConfigBeforeRuntimeModule::~ConfigBeforeRuntimeModule()
        {
            delete m_doc;
        }

        void
            ConfigBeforeRuntimeModule::setNewFile
            (const std::string filePath)
        {
            auto configBeforeRuntimeModuleNode = m_doc->append_child("ConfigBeforeRuntimeModule");
            auto cameraCountNode = configBeforeRuntimeModuleNode.append_child("CameraCount");
            cameraCountNode.text().set(4);
            auto languageNode = configBeforeRuntimeModuleNode.append_child("Language");
            languageNode.text().set("CHN");

            auto result = m_doc->save_file(filePath.c_str());
        }

        bool
            ConfigBeforeRuntimeModule::loadFile
            (const std::string filePath)
        {
            auto loadFileResutlt = m_doc->load_file(filePath.c_str());

            if (loadFileResutlt) {
                return true;
            }
            return false;
        }

        bool
            ConfigBeforeRuntimeModule::storeCameraCount
            (int count)
        {
            auto configBeforeRuntimeModuleNode = m_doc->child("ConfigBeforeRuntimeModule");
            if (!configBeforeRuntimeModuleNode) {
                return false;
            }
            auto cameraCountNode = configBeforeRuntimeModuleNode.child("CameraCount");
            if (!cameraCountNode) {
                return false;
            }
            auto storeResult = cameraCountNode.text().set(count);

            return storeResult;
        }

        bool
            ConfigBeforeRuntimeModule::storeLanguage
            (const std::string& language)
        {
            auto configBeforeRuntimeModuleNode = m_doc->child("ConfigBeforeRuntimeModule");
            if (!configBeforeRuntimeModuleNode) {
                return false;
            }
            auto languageNode = configBeforeRuntimeModuleNode.child("Language");
            if (!languageNode) {
                return false;
            }
            auto storeResult = languageNode.text().set(language.c_str());

            return storeResult;
        }

        bool
            ConfigBeforeRuntimeModule::storeCameraConfig
            (const std::string& ip, const std::string& configPath)
        {
            auto configBeforeRuntimeModuleNode = m_doc->child("ConfigBeforeRuntimeModule");
            if (!configBeforeRuntimeModuleNode) {
                return false;
            }
            auto cameraConfigNode = configBeforeRuntimeModuleNode.find_child_by_attribute("ip", ip.c_str());
            if (cameraConfigNode) {
                auto storeResult = cameraConfigNode.text().set(configPath.c_str());
                return storeResult;
            }
            else {
                cameraConfigNode = configBeforeRuntimeModuleNode.append_child("CameraConfig");
                if (!cameraConfigNode) { return false; }
                cameraConfigNode.append_attribute("ip").set_value(ip.c_str());
                auto storeResult = cameraConfigNode.text().set(configPath.c_str());
                return storeResult;
            }
        }

        void
            ConfigBeforeRuntimeModule::saveFile
            (const std::string& filePath)
        {
            auto result = m_doc->save_file(filePath.c_str());
        }

        int
            ConfigBeforeRuntimeModule::readCameraCount
            ()
        {
            int result{};
            auto configBeforeRuntimeModuleNode = m_doc->child("ConfigBeforeRuntimeModule");
            auto cameraCountNode = configBeforeRuntimeModuleNode.child("CameraCount");
            result = cameraCountNode.text().as_int();

            return result;
        }

        std::string
            ConfigBeforeRuntimeModule::readLanguage
            ()
        {
            std::string result{};
            auto configBeforeRuntimeModuleNode = m_doc->child("ConfigBeforeRuntimeModule");
            auto languageNode = configBeforeRuntimeModuleNode.child("Language");
            result = languageNode.text().as_string();

            return result;
        }

        bool
            ConfigBeforeRuntimeModule::readCameraConfig
            (const std::string& ip, std::string& configPath)
        {
            auto configBeforeRuntimeModuleNode = m_doc->child("ConfigBeforeRuntimeModule");
            if (!configBeforeRuntimeModuleNode) {
                return false;
            }
            auto cameraConfigNode = configBeforeRuntimeModuleNode.find_child_by_attribute("ip", ip.c_str());
            if (cameraConfigNode) {
                configPath = cameraConfigNode.text().as_string();
                return true;
            }
            else {
                return false;
            }
        }
    }
}
