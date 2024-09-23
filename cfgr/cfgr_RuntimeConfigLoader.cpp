#include "cfgr_RuntimeConfigLoader.h"

#include"oso/osos_FileSave.h"

namespace rw {
    namespace cfgr {
        /*RutimeConfig RuntimeConfigLoader::load(const std::filesystem::path& filePath)
        {
            oso::FileSave fileSave;
            auto config=fileSave.load(filePath);
            return *config;
        }*/

        RutimeConfig RuntimeConfigLoader::load(const std::filesystem::path& filePath)
        {
            bool isLoadSuccess = false;
            oso::FileSave fileSave;
            auto config = fileSave.load(filePath, isLoadSuccess);
            if (!isLoadSuccess) {
                return RutimeConfig();
            }
            auto result=RutimeConfig::toRuntimeConfig(*config);
            return result;
        }

        void RuntimeConfigLoader::save(const std::filesystem::path& filePath, const RutimeConfig& config)
        {
            oso::FileSave fileSave;
            fileSave.save(filePath, oso::makeObjectStoreAssemblySharedPtr(RutimeConfig::toObjectStoreAssembly(config)));

        }
    }
}
