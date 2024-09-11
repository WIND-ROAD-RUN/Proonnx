#include"cfgr_CatalogueInitializer.h"

#include<filesystem>
#include <cassert>

namespace rw {
    namespace cfgr {
        std::unordered_map<std::string, std::string> CatalogueInitializer::m_workPlaceToPath;

        std::string CatalogueInitializer::findWorkPath(std::string workPlaceName)
        {
            std::string result;
            if (m_workPlaceToPath.find(workPlaceName) != m_workPlaceToPath.end()) {
                result = m_workPlaceToPath.at(workPlaceName);
            }
            assert(!result.empty());
            return result;
        }

        std::string CatalogueInitializer::pathAppend(std::string srcPath,std::string value)
        {
            std::filesystem::path result(srcPath);
            result.append(value);
            return result.string();
        }

        void CatalogueInitializer::setRootPath(const std::string& path)
        {
            m_rootPath = path;
        }

        void CatalogueInitializer::createDirectory(const std::string& directoryName)
        {
            using namespace std;
            filesystem::path rootPath(m_rootPath);
            rootPath.append(directoryName.c_str());
            auto isCreate=filesystem::create_directories(rootPath);
            m_workPlaceToPath.insert({ directoryName, rootPath.string() });
        }

    }
    
}


