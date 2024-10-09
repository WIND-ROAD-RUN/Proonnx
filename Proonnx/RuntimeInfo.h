#ifndef RUNTIMEINFO_H_
#define RUNTIMEINFO_H_

#include<memory>
#include<map>
#include<string>
#include<vector>

namespace rw {
	namespace cfgr {
		struct RutimeConfig;
	}
	namespace oso {
		class OcrDataProductConfig;
	}
}

using namespace rw::cfgr;

class RuntimeInfo {
public:
    std::shared_ptr<RutimeConfig> m_runtimeConfigPtr{ nullptr };

    std::map<std::string, std::shared_ptr<rw::oso::OcrDataProductConfig> > m_ocrConfigs;

    std::vector<std::string> m_cameraIpList;

public:
    std::string m_runtimeConfigPath{};

public:
    void ini();

public:
    void create_directory(const std::string& rootPath);

    void read_runtimeConfig();

public:
    void saveRuntimeConfigFile();

};


#endif // !RUNTIMEINFO_H_
