#ifndef OSOS_FILESAVE_H_
#define OSOS_FILESAVE_H_

#include "oso_core.h"

#include<filesystem>
#include<memory>
#include<string>
#include<cassert>

namespace pugi {
    class xml_node;
}

namespace rw {
    namespace oso {
        enum class FileSaveStrategyType {
            XML,
        };

        class FileSave_XML;
        class FileSave_Strategy;

        template<FileSaveStrategyType strategyType = FileSaveStrategyType::XML>
        class FileSave {
        private:
            std::shared_ptr<FileSave_Strategy> m_strategy;

            std::string m_extensionName;

            inline void initializeStrategy();

        public:
            FileSave() {
                initializeStrategy();
            }

            ~FileSave() {}

        public:
            inline void save(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly);

            inline void save(const std::filesystem::path& fileName, const ObjectStoreAssembly& assembly);

            inline void save(const std::filesystem::path& fileName, ObjectStoreAssembly&& assembly);

            inline std::shared_ptr<ObjectStoreAssembly> load(const std::filesystem::path& fileName);

            inline std::shared_ptr<ObjectStoreAssembly> load(const std::filesystem::path& fileName, bool& loadResult);
        };



        class FileSave_Strategy {
        public:
            virtual ~FileSave_Strategy() = default;

            virtual void save(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly) = 0;

            virtual std::shared_ptr<ObjectStoreAssembly> load(const std::filesystem::path& fileName) = 0;

        };

        class FileSave_XML
            :public FileSave_Strategy {
        public:
            FileSave_XML() = default;

            virtual ~FileSave_XML() = default;

        private:
            void saveNodeWithAssembly(pugi::xml_node& node, const std::shared_ptr<ObjectStoreAssembly>& assembly);

            void saveNodeWithItem(pugi::xml_node& node, const std::shared_ptr<ObjectStoreItem>& item);

        private:
            void loadNodeWithAssembly(const pugi::xml_node& node, std::shared_ptr<ObjectStoreAssembly>& assembly);

            void loadNodeWithItem(const pugi::xml_node& node, std::shared_ptr<ObjectStoreItem>& item);
        public:
            void save(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly)override;

            std::shared_ptr<ObjectStoreAssembly> load(const std::filesystem::path& fileName) override;
            //TODO：自定义类型到std::shared_ptr<ObjectStoreAssembly>的转换
        private:
            void saveNodeWithAssembly_isDeprecated(pugi::xml_node& node, const std::shared_ptr<ObjectStoreAssembly>& assembly);

            void saveNodeWithItem_Deprecated(pugi::xml_node& node, const std::shared_ptr<ObjectStoreItem>& item);

        private:
            void loadNodeWithAssembly_Deprecated(const pugi::xml_node& node, std::shared_ptr<ObjectStoreAssembly>& assembly);

            void loadNodeWithItem_Deprecated(const pugi::xml_node& node, std::shared_ptr<ObjectStoreItem>& item);

        public:
            void save_Deprecated(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly);

            std::shared_ptr<ObjectStoreAssembly> load_Deprecated(const std::filesystem::path& fileName);

        };

        class FileSave_Json
            :public FileSave_Strategy {
        public:


        public:
            // 通过 FileSave_Strategy 继承
            void save(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly) override;

            std::shared_ptr<ObjectStoreAssembly> load(const std::filesystem::path& fileName) override;

        };

        template<FileSaveStrategyType strategyType>
        inline void FileSave<strategyType>::initializeStrategy()
        {
            switch (strategyType) {
            case FileSaveStrategyType::XML:
                m_strategy = std::make_unique<FileSave_XML>();
                m_extensionName = ".xml";
                break;
            default:
                throw std::invalid_argument("Unsupported strategy type");
            }
        }

        template<FileSaveStrategyType strategyType>
        inline void
            FileSave<strategyType>::save
            (const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly)
        {
            assert(fileName.extension() == m_extensionName);
            m_strategy->save(fileName, assembly);
        }

        template<FileSaveStrategyType strategyType>
        inline void
            FileSave<strategyType>::save
            (const std::filesystem::path& fileName, const ObjectStoreAssembly& assembly)
        {
            assert(fileName.extension() == m_extensionName);
            m_strategy->save(fileName, makeObjectStoreAssemblySharedPtr(assembly));
        }

        template<FileSaveStrategyType strategyType>
        inline void FileSave<strategyType>::save(const std::filesystem::path& fileName, ObjectStoreAssembly&& assembly)
        {
            assert(fileName.extension() == m_extensionName);
            m_strategy->save(fileName, makeObjectStoreAssemblySharedPtr(assembly));
        }

        template<FileSaveStrategyType strategyType>
        inline std::shared_ptr<ObjectStoreAssembly>
            FileSave<strategyType>::load
            (const std::filesystem::path& fileName)
        {
            assert(fileName.extension() == m_extensionName);
            try {
                auto result = m_strategy->load(fileName);
                return result;
            }
            catch (const std::exception& ex) {
                return nullptr;
            }
        }

        template<FileSaveStrategyType strategyType>
        inline std::shared_ptr<ObjectStoreAssembly>
            FileSave<strategyType>::load
            (const std::filesystem::path& fileName, bool& loadResult)
        {
            assert(fileName.extension() == m_extensionName);
            try {
                auto result = m_strategy->load(fileName);
                loadResult = true;
                return result;
            }
            catch (const std::exception& ex) {
                loadResult = false;
                return nullptr;
            }

        }

    }
}

#endif // !OSOS_FILESAVE_H_
