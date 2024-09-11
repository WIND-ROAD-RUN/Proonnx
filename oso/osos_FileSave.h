#ifndef OSOS_FILESAVE_H_
#define OSOS_FILESAVE_H_

#include "oso_core.h"

#include<filesystem>
#include<memory>

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

        template<FileSaveStrategyType strategyType= FileSaveStrategyType::XML>
        class FileSave {
        private:
            std::shared_ptr<FileSave_Strategy> m_strategy;

            void initializeStrategy() {
                switch (strategyType) {
                case FileSaveStrategyType::XML:
                    m_strategy = std::make_unique<FileSave_XML>();
                    break;
                default:
                    throw std::invalid_argument("Unsupported strategy type");
                }
            }

        public:
            FileSave() {
                initializeStrategy();
            }

            ~FileSave() {}

        public:
            inline void save(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly);

            inline std::shared_ptr<ObjectStoreAssembly> load(const std::filesystem::path& fileName);
                
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
            void save(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly) override;

            std::shared_ptr<ObjectStoreAssembly> load(const std::filesystem::path& fileName) override;

        };

        template<FileSaveStrategyType strategyType>
        inline void  
            FileSave<strategyType>::save
            (const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly)
        {
            m_strategy->save(fileName, assembly);
        }

        template<FileSaveStrategyType strategyType>
        inline std::shared_ptr<ObjectStoreAssembly> 
            FileSave<strategyType>::load(const std::filesystem::path& fileName)
        {
            return m_strategy->load(fileName);
        }
    }
}

#endif // !OSOS_FILESAVE_H_
