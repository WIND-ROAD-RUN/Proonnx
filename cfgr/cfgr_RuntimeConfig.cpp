#include "cfgr_RuntimeConfig.h"

#include"oso/oso_core.h"

namespace rw {
    namespace cfgr {
        oso::ObjectStoreAssembly RutimeConfig::toObjectStoreAssembly(const RutimeConfig& runtimeConfig)
        {
            oso::ObjectStoreAssembly assembly;
            assembly.setName("runtimeConfig");

            oso::ObjectStoreItem cameraCount;
            cameraCount.setName("cameraCount");
            cameraCount.setValueFromInt(runtimeConfig.cameraCount);
            assembly.addItem(oso::makeObjectStoreItemSharedPtr(cameraCount));

            oso::ObjectStoreItem language;
            language.setName("language");
            language.setValueFromString(runtimeConfig.language);
            assembly.addItem(oso::makeObjectStoreItemSharedPtr(language));

            oso::ObjectStoreAssembly cameraCfgs;
            cameraCfgs.setName("cameraConfigs");
            for (const auto & item:runtimeConfig.cameraConfigs) {
                oso::ObjectStoreItem cameraConfig;
                cameraConfig.setName(item.first);
                cameraConfig.setValueFromString(item.second);
                cameraCfgs.addItem(oso::makeObjectStoreItemSharedPtr(cameraConfig));
            }

            assembly.addItem(oso::makeObjectStoreAssemblySharedPtr(cameraCfgs));

            return assembly;
            
        }

        RutimeConfig RutimeConfig::toRuntimeConfig(const oso::ObjectStoreAssembly& assembly)
        {
            RutimeConfig runtimeConfig;
            runtimeConfig.cameraCount = oso::ObjectStoreCoreToItem(assembly.getItem("cameraCount"))->getValueAsInt();
            runtimeConfig.language = oso::ObjectStoreCoreToItem(assembly.getItem("language"))->getValueAsString();

            auto  cameraCfgs = oso::ObjectStoreCoreToAssembly(assembly.getItem("cameraConfigs"));
            for (const auto& item : cameraCfgs->getItems()) {
                auto storeItem = oso::ObjectStoreCoreToItem(item);
                runtimeConfig.cameraConfigs.push_back(std::make_pair(storeItem->getName(), storeItem->getValueAsString()));
            }

            return runtimeConfig;
        }
    }
}
