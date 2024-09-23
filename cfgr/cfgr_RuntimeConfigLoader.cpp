#include "cfgr_RuntimeConfigLoader.h"

#include"oso/osos_FileSave.h"

namespace rw {
    namespace cfgr {
        RutimeConfig RuntimeConfigLoader::load(const std::filesystem::path& filePath)
        {
            oso::FileSave fileSave;
            auto filePath=fileSave.load(filePath);
        }

        void RuntimeConfigLoader::save(const std::filesystem::path& filePath, const RutimeConfig& config)
        {
            oso::FileSave fileSave;
            fileSave.save(filePath, oso::makeObjectStoreAssemblySharedPtr(RutimeConfig::toObjectStoreAssembly(config)));

        }
    }
}
