#ifndef CFGR_RUNTIMECONFIG_H_
#define CFGR_RUNTIMECONFIG_H_

#include<string>
#include<vector>

namespace rw {
    namespace oso {
        class ObjectStoreAssembly;
        class ObjectStoreAssembly;
    }
}

namespace rw {
    namespace cfgr {
        struct RutimeConfig
        {
            using cameraConfig = std::pair<std::string, std::string>;
        public:
            int cameraCount{ 4 };
            std::string language{"CHN"};

            std::vector<cameraConfig> cameraConfigs;

        public:
            bool readCameraLastRunTimeConfig(const std::string& ip, std::string& configPath);

            void addCameraLastRunTimeConfig(const std::string& ip, const std::string& configPath);

        public:
            static oso::ObjectStoreAssembly toObjectStoreAssembly(const RutimeConfig& runtimeConfig);
            static RutimeConfig toRuntimeConfig(const oso::ObjectStoreAssembly& assembly);
            
        };
    
    }

}

#endif // !CFGR_RUNTIMECONFIG_H_
