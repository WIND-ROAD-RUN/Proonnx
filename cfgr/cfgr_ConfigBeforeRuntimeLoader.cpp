#include "cfgr_ConfigBeforeRuntimeLoader.h"

#include"cfgr_ConfigBeforeRuntimeModule.h"

namespace rw {
    namespace cfgr {

        ConfigBeforeRuntimeLoader::ConfigBeforeRuntimeLoader
        ()
        {
            m_configBeforeRuntimeModule = new ConfigBeforeRuntimeModule;
        }

        ConfigBeforeRuntimeLoader::~ConfigBeforeRuntimeLoader
        ()
        {
            delete m_configBeforeRuntimeModule;
        }

        void
            ConfigBeforeRuntimeLoader::setNewFile
            (const std::string& filePath)
        {
            m_configBeforeRuntimeModule->setNewFile(filePath);
        }

        bool
            ConfigBeforeRuntimeLoader::loadFile
            (const std::string& filePath)
        {
            return m_configBeforeRuntimeModule->loadFile(filePath);
        }

        void
            ConfigBeforeRuntimeLoader::saveFile
            (const std::string& filePath)
        {
            m_configBeforeRuntimeModule->saveFile(filePath);
        }

        bool
            ConfigBeforeRuntimeLoader::storeCameraCount
            (int count)
        {
            return m_configBeforeRuntimeModule->storeCameraCount(count);
        }

        bool
            ConfigBeforeRuntimeLoader::storeLanguage
            (const std::string& language)
        {
            return m_configBeforeRuntimeModule->storeLanguage(language);
        }

        bool
            ConfigBeforeRuntimeLoader::storeCameraConfig
            (const std::string& ip, const std::string& configPath)
        {
            return m_configBeforeRuntimeModule->storeCameraConfig(ip, configPath);
        }

        bool
            ConfigBeforeRuntimeLoader::readCameraConfig
            (const std::string& ip, std::string& configPath)
        {
            return m_configBeforeRuntimeModule->readCameraConfig(ip, configPath);
        }

        int
            ConfigBeforeRuntimeLoader::readCameraCount
            ()
        {
            return m_configBeforeRuntimeModule->readCameraCount();
        }

        std::string
            ConfigBeforeRuntimeLoader::readLanguage
            ()
        {
            return m_configBeforeRuntimeModule->readLanguage();
        }
    }
}