#include"oso/oso_core.h"
#include"oso/osos_FileSave.h"
#include"oso/osop_OcrDateProductConfig.h"
#include"cfgr/cfgr_RuntimeConfig.h"
#include"oso/osos_FileSave.h"

#include<iostream>

using namespace rw::oso;
using namespace rw::cfgr;

int main()
{
    RutimeConfig runtimeConfig;
    runtimeConfig.cameraCount = 4;
    runtimeConfig.language = "CHN";
    runtimeConfig.cameraConfigs.push_back(std::make_pair("camera1", "config1"));
    runtimeConfig.cameraConfigs.push_back(std::make_pair("camera2", "config2"));
    runtimeConfig.cameraConfigs.push_back(std::make_pair("camera3", "config3"));
    runtimeConfig.cameraConfigs.push_back(std::make_pair("camera4", "config4"));

    ObjectStoreAssembly assembly = RutimeConfig::toObjectStoreAssembly(runtimeConfig);
    FileSave fileSave;
    fileSave.save(R"(C:\Users\34615\Desktop\test\Config\config.xml)",rw::oso::makeObjectStoreAssemblySharedPtr(assembly));

    auto a=fileSave.load(R"(C:\Users\34615\Desktop\test\Config\config.xml)");
    a->print(std::cout);

    
	return 0;
}