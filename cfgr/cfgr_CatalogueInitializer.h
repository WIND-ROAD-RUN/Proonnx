#ifndef CFGR_CATALOGUEINITIALIZER_H_
#define CFGR_CATALOGUEINITIALIZER_H_

#include<string>
#include<unordered_map>

namespace rw {
    namespace cfgr {

        class CatalogueInitializer {
        private:
            std::string m_rootPath{};
            
        private:
            static std::unordered_map<std::string, std::string> m_workPlaceToPath;

        public:
            static std::string findWorkPath(std::string workPlaceName);

            static std::string pathAppend(std::string srcPath,std::string value);

        public:
            void setRootPath(const std::string& path);

        public:
            void createDirectory(const std::string & directoryPath);

        };

    }

}

#endif // !CFGR_CATALOGUEINITIALIZER_H_
