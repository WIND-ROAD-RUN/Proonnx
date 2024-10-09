#include "RuntimeInfo.h"

#include"cfgr/cfgr_CatalogueInitializer.h"
#include"cfgr/cfgr_RuntimeConfigLoader.h"
#include"oso/oso_core.h"
#include"oso/osop_OcrDateProductConfig.h"
#include"oso/osos_FileSave.h"
#include"MonitorCamera.h"



using namespace rw::oso;

void RuntimeInfo::ini()
{
    create_directory(R"(C:\Users\34615\Desktop\test)");
    read_runtimeConfig();


    //检索所有连接的相机并读取上次运行的配置文件若没有那么创建一个新的配置文件
    m_cameraIpList = MonitorCameraUtility::checkAllConnectCamera();

    FileSave fileLoader;
    for (const auto& item : m_cameraIpList) {
        std::string cameraConfigFilePath;
        auto hasConfig = m_runtimeConfigPtr->readCameraLastRunTimeConfig(item, cameraConfigFilePath);

        if (hasConfig) {
            OcrDataProductConfig cameraCfg = fileLoader.load(cameraConfigFilePath);
            m_ocrConfigs.emplace(item, std::make_shared<OcrDataProductConfig>(cameraCfg));
        }
        else {
            OcrDataProductConfig newCameraCfg;
            newCameraCfg.productName = "UNDEFINED" + item;
            std::string path = rw::cfgr::CatalogueInitializer::findWorkPath("ProductConfig");
            path = rw::cfgr::CatalogueInitializer::pathAppend(path, newCameraCfg.productName + ".xml");
            fileLoader.save(path, newCameraCfg);
            m_runtimeConfigPtr->addCameraLastRunTimeConfig(item, path);
            m_ocrConfigs.emplace(path, std::make_shared<OcrDataProductConfig>(newCameraCfg));
        }
    }
    this->saveRuntimeConfigFile();
}

void RuntimeInfo::create_directory(const std::string& rootPath)
{
    rw::cfgr::CatalogueInitializer catalogueIni;
    catalogueIni.setRootPath(rootPath);
    catalogueIni.createDirectory("Config");
    catalogueIni.createDirectory("ProductConfig");
    catalogueIni.createDirectory("HistoryImage");
}

void RuntimeInfo::read_runtimeConfig()
{
    auto runtimePath = rw::cfgr::CatalogueInitializer::findWorkPath("Config");
    runtimePath = rw::cfgr::CatalogueInitializer::pathAppend(runtimePath, "runtimeCfg.xml");

    m_runtimeConfigPath = runtimePath;

    bool loaderResult{ false };
    auto runtimeConfig = RuntimeConfigLoader::load(runtimePath, loaderResult);
    if (!loaderResult) {
        RutimeConfig config;
        config.cameraCount = 4;
        config.language = "CHN";
        RuntimeConfigLoader::save(runtimePath, config);
        m_runtimeConfigPtr = std::make_shared<RutimeConfig>(config);
    }
    else {
        m_runtimeConfigPtr = std::make_shared<RutimeConfig>(runtimeConfig);
    }
}

void RuntimeInfo::saveRuntimeConfigFile()
{
    rw::oso::FileSave fileSave;
    fileSave.save(m_runtimeConfigPath, RutimeConfig::toObjectStoreAssembly(*m_runtimeConfigPtr));
}
