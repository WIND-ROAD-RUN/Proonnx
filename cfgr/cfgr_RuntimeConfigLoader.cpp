#include "cfgr_RuntimeConfigLoader.h"

#include"oso/osos_FileSave.h"

namespace rw {
    namespace cfgr {
        RutimeConfig 
            RuntimeConfigLoader::load
            (const std::filesystem::path& filePath, bool & isLoad)
        {
            bool isLoadSuccess = false;
            oso::FileSave fileSave;
            auto config = fileSave.load(filePath, isLoadSuccess);
            if (!isLoadSuccess) {
                isLoad = false;
                return RutimeConfig();
            }
            auto result=RutimeConfig::toRuntimeConfig(*config);
            isLoad = true;
            return result;
        }

        void 
            RuntimeConfigLoader::save
            (const std::filesystem::path& filePath, const RutimeConfig& config)
        {
            oso::FileSave fileSave;
            fileSave.save(filePath, oso::makeObjectStoreAssemblySharedPtr(RutimeConfig::toObjectStoreAssembly(config)));

        }
    }
}
