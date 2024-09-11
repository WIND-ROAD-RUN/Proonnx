#ifndef CFGL_LOCALIZATIONSTRINGLOADER_H_
#define CFGL_LOCALIZATIONSTRINGLOADER_H_

#include<string>
#include <unordered_map>

namespace rw {
    namespace cfgl {
        class LocalizationStringLoader_Abstract {
        public:
            virtual bool loadData() = 0;

            virtual const std::string& getString(const std::string id) = 0;

        };

        /*Implemented as a singleton class*/
        class LocalizationStringXML;

        class LocalizationStringLoaderXML
            :public LocalizationStringLoader_Abstract {

        private:
            static LocalizationStringLoaderXML* m_instance;

            LocalizationStringLoaderXML();

            LocalizationStringLoaderXML(const std::string& filePath, const std::string& language);

        private:
            std::string m_XMLFilePath{};

        private:
            std::unordered_map<std::string, std::string> m_map;

            std::string m_language{};

            LocalizationStringXML* m_LocalStringComponet{ nullptr };

        public:
            static LocalizationStringLoaderXML*
                getInstance();

            static LocalizationStringLoaderXML*
                getInstance(const std::string& filePath, const std::string& language);

            ~LocalizationStringLoaderXML();

        public:
            void setFilePath(const std::string& filePath) { m_XMLFilePath = filePath; }

            void setLanguage(const std::string& language) { m_language = language; }

        public:
            // Inherit through LocalizationStringLoaderAbstract
            bool loadData() override;

            const std::string& getString(const std::string id) override;

        };

    }
}


#endif // !CFGL_LOCALIZATIONSTRINGLOADER_H_
