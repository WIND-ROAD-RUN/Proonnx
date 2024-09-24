#ifndef OSO_CORE_H_
#define OSO_CORE_H_

#include<vector>
#include<memory>
#include<string>
#include<iostream>

namespace rw {
    namespace oso {
        class ObjectStoreCore {
        private:
            /// Class member std:: string m_name of class ObjectiStoreCore
            /// 
            /// This is a private member of class ObjectStoreCore.Means the name representation of the instance object
            /// , which can serve as the node name in the class composition structure
            /// 
            /// Class member std:: string m_name of class ObjectiStoreCore
            std::string m_name;
        public:
            ObjectStoreCore* parent{ nullptr };

        public:
            int level{ 0 };
            
            virtual void addLevel()  {
                level++;
            }

            virtual void subLevel() {
                level--;
            }

            virtual std::string getStoreType() const {
                return "core";
            }


        public:
            ObjectStoreCore();

            virtual ~ObjectStoreCore() = default;

        public:
            void setName(const std::string& name);

            std::string getName() const;

        public:
            virtual void print(std::ostream& os);

        };

        enum  class ObjectDataItemStoreType
        {
            item_string,
            item_int,
            item_long,
            item_float,
            item_double,
            item_bool
        };

        class ObjectStoreItem
            :public ObjectStoreCore {
        private:
            std::string m_value{"0"};
            ObjectDataItemStoreType m_type{ ObjectDataItemStoreType::item_int };
        public:
            ObjectStoreItem();

            virtual ~ObjectStoreItem() = default;
        public:
            void setValueFromString(const std::string& value);

            void setValueFromInt(int value);

            void setValueFromLong(long value);

            void setValueFromFloat(float value);

            void setValueFromDouble(double value);

            void setValueFromBool(bool value);

        public:
            std::string getValueAsString() const;

            int getValueAsInt() const;

            long getValueAsLong() const;

            float getValueAsFloat() const;

            double getValueAsDouble() const;

            bool getValueAsBool() const;

            ObjectDataItemStoreType getType() const;

            std::string getTypeString() const;

        public:
            // 通过 ObjectStoreCore 继承
            void print(std::ostream& os) override;

            virtual std::string getStoreType() const override {
                return "item";
            }

        };

        class ObjectStoreAssembly
            :public ObjectStoreCore {
        private:
            std::vector<std::shared_ptr<ObjectStoreCore>> m_items;

        public:
            ObjectStoreAssembly();

            virtual ~ObjectStoreAssembly() = default;

        public:
            void addItem(std::shared_ptr<ObjectStoreCore> item);

            void addItem(ObjectStoreAssembly && assembly);

            void addItem(ObjectStoreItem && item);

            void removeItem(std::shared_ptr<ObjectStoreCore> item);

        public:
            using size_type = std::vector<std::shared_ptr<ObjectStoreCore>>::size_type;

            std::shared_ptr<ObjectStoreCore> operator[](const size_type & i);

            const std::shared_ptr<ObjectStoreCore> at(const size_type& i)const;

        public:
            std::vector<std::shared_ptr<ObjectStoreCore>> getItems() const;

            const std::shared_ptr<ObjectStoreCore> getItem(const std::string& name) const;

        public:
            // 通过 ObjectStoreCore 继承
            void print(std::ostream& os) override;

            virtual void addLevel() override;

            virtual void subLevel() override;

            virtual std::string getStoreType() const override {
                return "assembly";
            }

        };

        inline std::shared_ptr<ObjectStoreAssembly> 
            makeObjectStoreAssemblySharedPtr
            (const ObjectStoreAssembly & assembly) {
            return std::make_shared<ObjectStoreAssembly>(assembly);
        }

        inline std::shared_ptr<ObjectStoreItem> 
            makeObjectStoreItemSharedPtr
            (const ObjectStoreItem& item) {
            return std::make_shared<ObjectStoreItem>(item);
        }

        inline std::shared_ptr<ObjectStoreAssembly> 
            ObjectStoreCoreToAssembly
            (std::shared_ptr<ObjectStoreCore> core) {
            std::shared_ptr<ObjectStoreAssembly> assembly = std::dynamic_pointer_cast<ObjectStoreAssembly>(core);

            return assembly;
        }

        inline std::shared_ptr<ObjectStoreItem> 
            ObjectStoreCoreToItem
            (std::shared_ptr<ObjectStoreCore> core) {
            std::shared_ptr<ObjectStoreItem> item = std::dynamic_pointer_cast<ObjectStoreItem>(core);

            return item;
        }

    }

}


#endif // !OSO_CORE_H_
