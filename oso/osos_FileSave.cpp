#include"osos_FileSave.h"

#include<iostream>
#include<fstream>

#include<cassert>

///	Third party library
#include"pugixml.hpp"
#include"json/json.h"

namespace rw {
	namespace oso
	{
		void 
            FileSave_XML::saveNodeWithAssembly_isDeprecated
            (pugi::xml_node& node,const  std::shared_ptr<ObjectStoreAssembly> & assembly)
		{
            auto child = node.append_child(assembly->getName().c_str());
            child.append_attribute("nodeType").set_value("assembly");
            for (const auto& item : assembly->getItems()) {
                if (auto objectAssembly = ObjectStoreCoreToAssembly(item)) {
                    saveNodeWithAssembly_isDeprecated(child, objectAssembly);
                }
                else if (auto objectItem = ObjectStoreCoreToItem(item)) {
                    saveNodeWithItem_Deprecated(child, objectItem);
                }
                else {
                    throw std::runtime_error("Unknown type which is the superclass of objectStoreCore");
                }
            }
		}

        void 
            FileSave_XML::saveNodeWithItem_Deprecated
            (pugi::xml_node& node, const std::shared_ptr<ObjectStoreItem> & item)
        {
            auto child = node.append_child(item->getName().c_str());
            if (item->getType() == ObjectDataItemStoreType::item_bool) {
                child.append_attribute("type").set_value("bool");
                child.append_attribute("nodeType").set_value("item") ;
                child.append_child("value").text() = item->getValueAsBool() ? "true" : "false";
            }
            else if (item->getType() == ObjectDataItemStoreType::item_double) {
                child.append_attribute("type").set_value("double") ;
                child.append_attribute("nodeType").set_value("item") ;
                child.append_child("value").text() = std::to_string(item->getValueAsDouble()).c_str();
            }
            else if (item->getType() == ObjectDataItemStoreType::item_int) {
                child.append_attribute("type").set_value("int") ;
                child.append_attribute("nodeType").set_value("item");
                child.append_child("value").text() = std::to_string(item->getValueAsInt()).c_str();
            }
            else if (item->getType() == ObjectDataItemStoreType::item_float) {
                child.append_attribute("type").set_value("float");
                child.append_attribute("nodeType").set_value("item") ;
                child.append_child("value").text() = std::to_string(item->getValueAsFloat()).c_str();
            }
            else if (item->getType() == ObjectDataItemStoreType::item_long) {
                child.append_attribute("type").set_value("long") ;
                child.append_attribute("nodeType").set_value("item") ;
                child.append_child("value").text() = std::to_string(item->getValueAsLong()).c_str();
            }
            else if (item->getType() == ObjectDataItemStoreType::item_string) {
                child.append_attribute("type").set_value("string") ;
                child.append_attribute("nodeType").set_value("item") ;
                child.append_child("value").text() = item->getValueAsString().c_str();
            }
            else {
                throw std::runtime_error("Unknown value type");
            }
        }

        void FileSave_XML::loadNodeWithAssembly_Deprecated(const pugi::xml_node& node, std::shared_ptr<ObjectStoreAssembly>& assembly)
        {
            assembly->setName(node.name());
            for (auto& child : node.children()) {
                if (child.attribute("nodeType").as_string() == std::string("assembly")) {
                    auto objectAssembly = std::make_shared<ObjectStoreAssembly>();
                    loadNodeWithAssembly_Deprecated(child, objectAssembly);
                    assembly->addItem(objectAssembly);
                }
                else if (child.attribute("nodeType").as_string() == std::string("item")) {
                    auto objectItem = std::make_shared<ObjectStoreItem>();
                    objectItem->setName(child.name());
                    loadNodeWithItem_Deprecated(child, objectItem);
                    assembly->addItem(objectItem);
                }
                else {
                    throw std::runtime_error("Unknown node type");
                }
            }
        }

        void FileSave_XML::loadNodeWithItem_Deprecated(const pugi::xml_node& node, std::shared_ptr<ObjectStoreItem>& item)
        {
            item->setName(node.name());
            if (node.attribute("type").as_string() == std::string("bool")) {
                item->setValueFromBool(node.child("value").text().as_string() == "true");
            }
            else if (node.attribute("type").as_string() == std::string("double")) {
                item->setValueFromDouble(node.child("value").text().as_double());
            }
            else if (node.attribute("type").as_string() == std::string("int")) {
                item->setValueFromInt(node.child("value").text().as_int());
            }
            else if (node.attribute("type").as_string() == std::string("float")) {
                item->setValueFromFloat(node.child("value").text().as_float());
            }
            else if (node.attribute("type").as_string() == std::string("long") ) {
                item->setValueFromLong(node.child("value").text().as_llong());
            }
            else if (node.attribute("type").as_string() == std::string("string")) {
                item->setValueFromString(node.child("value").text().as_string());
            }
            else {
                throw std::runtime_error("Unknown node type");
            }
        }

		void 
            FileSave_XML::save_Deprecated
            (const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly)
		{
			assert(fileName.extension()==std::string(".xml"));
            pugi::xml_document doc;
            auto rootNode=doc.append_child(assembly->getName().c_str());
            rootNode.append_attribute("nodeType").set_value("root");
			for (const auto & item:assembly->getItems()) {
                if (auto objectAssembly = ObjectStoreCoreToAssembly(item)) {
                    saveNodeWithAssembly_isDeprecated(rootNode, objectAssembly);
                }
                else if (auto objectItem = ObjectStoreCoreToItem(item)) {
                    saveNodeWithItem_Deprecated(rootNode, objectItem);
                }
                else {
                    throw std::runtime_error("Unknown node type which is instance of the superclass of objectStoreCore");
                }
			}

            doc.save_file(fileName.c_str());
		
		}

        std::shared_ptr<ObjectStoreAssembly> FileSave_XML::load_Deprecated(const std::filesystem::path& fileName)
        {
            auto assembly = std::make_shared<ObjectStoreAssembly>();
            pugi::xml_document doc;
            if (doc.load_file(fileName.c_str())) {
                auto rootNode = doc.find_child_by_attribute("nodeType", "root");;
                if (rootNode) {
                    auto name = rootNode.name();
                    assembly->setName(rootNode.name());
                    for (auto & child:rootNode.children()) {
                        if (child.attribute("nodeType").as_string() == std::string("assembly")) {
                            auto objectAssembly = std::make_shared<ObjectStoreAssembly>();
                            loadNodeWithAssembly_Deprecated(child, objectAssembly);
                            assembly->addItem(objectAssembly);
                        }
                        else if (child.attribute("nodeType").as_string() == std::string("item")) {
                            auto objectItem = std::make_shared<ObjectStoreItem>();
                            loadNodeWithItem_Deprecated(child, objectItem);
                            assembly->addItem(objectItem);
                        }
                        else {
                            throw std::runtime_error("Unknown node type which is instance of the superclass of objectStoreCore");
                        }
                    }
                }
                else {
                    throw std::runtime_error("root node can't find");
                }
            }
            else {
                throw std::runtime_error("file can't find");
            }
            return assembly;
        }

        void FileSave_XML::saveNodeWithAssembly(pugi::xml_node& node, const std::shared_ptr<ObjectStoreAssembly>& assembly)
        {
            auto child = node.append_child("assembly");
            child.append_attribute("name").set_value(assembly->getName().c_str());

            auto children = child.append_child("children");
            for (const auto& item : assembly->getItems()) {
                if (auto objectAssembly = ObjectStoreCoreToAssembly(item)) {
                    saveNodeWithAssembly(children, objectAssembly);
                }
                else if (auto objectItem = ObjectStoreCoreToItem(item)) {
                    saveNodeWithItem(children, objectItem);
                }
                else {
                    throw std::runtime_error("Unknown type which is the superclass of objectStoreCore");
                }
            }
        }

        void FileSave_XML::saveNodeWithItem(pugi::xml_node& node, const std::shared_ptr<ObjectStoreItem>& item)
        {
            auto child = node.append_child("item");
            child.append_attribute("name").set_value(item->getName().c_str());

            if (item->getType() == ObjectDataItemStoreType::item_bool) {
                child.append_child("value").text().set(item->getValueAsBool() ? "true" : "false");
                child.append_child("type").text().set("bool");
            }
            else if (item->getType() == ObjectDataItemStoreType::item_double) {
                child.append_child("value").text().set(std::to_string(item->getValueAsDouble()).c_str());
                child.append_child("type").text().set("double");
            }
            else if (item->getType() == ObjectDataItemStoreType::item_int) {
                child.append_child("value").text().set(std::to_string(item->getValueAsInt()).c_str()); 
                child.append_child("type").text().set("int");
            }
            else if (item->getType() == ObjectDataItemStoreType::item_float) {
                child.append_child("value").text().set(std::to_string(item->getValueAsFloat()).c_str()); 
                child.append_child("type").text().set("float");
            }
            else if (item->getType() == ObjectDataItemStoreType::item_long) {
                child.append_child("value").text().set(std::to_string(item->getValueAsLong()).c_str()); 
                child.append_child("type").text().set("long");
            }
            else if (item->getType() == ObjectDataItemStoreType::item_string) {
                child.append_child("value").text().set(item->getValueAsString().c_str()); 
                child.append_child("type").text().set("string");
            }
            else {
                throw std::runtime_error("Unknown value type");
            }
        }

        void FileSave_XML::loadNodeWithAssembly(const pugi::xml_node& node, std::shared_ptr<ObjectStoreAssembly>& assembly)
        {
            assembly->setName(node.attribute("name").as_string());
            for (const auto& child : node.child("children").children()) {
                if (child.name() == std::string("assembly")) {
                    auto objectAssembly = std::make_shared<ObjectStoreAssembly>();
                    loadNodeWithAssembly(child, objectAssembly);
                    assembly->addItem(objectAssembly);
                }
                else if (child.name() == std::string("item")) {
                    auto objectItem = std::make_shared<ObjectStoreItem>();
                    loadNodeWithItem(child, objectItem);
                    assembly->addItem(objectItem);
                }
                else {
                    throw std::runtime_error("Unknown node type which is instance of the superclass of objectStoreCore");
                }
            }
        }

        void FileSave_XML::loadNodeWithItem(const pugi::xml_node& node, std::shared_ptr<ObjectStoreItem>& item)
        {
            item->setName(node.attribute("name").as_string());
            if (node.child("type").text().as_string() == std::string("bool")) {
                item->setValueFromBool(node.child("value").text().as_string() == "true");
            }
            else if (node.child("type").text().as_string() == std::string("double")) {
                item->setValueFromDouble(node.child("value").text().as_double());
            }
            else if (node.child("type").text().as_string() == std::string("int")) {
                item->setValueFromInt(node.child("value").text().as_int());
            }
            else if (node.child("type").text().as_string() == std::string("float")) {
                item->setValueFromFloat(node.child("value").text().as_float());
            }
            else if (node.child("type").text().as_string() == std::string("long")) {
                item->setValueFromLong(node.child("value").text().as_llong());
            }
            else if (node.child("type").text().as_string() == std::string("string")) {
                item->setValueFromString(node.child("value").text().as_string());
            }
            else {
                throw std::runtime_error("Unknown node type");
            }
        }

        void FileSave_XML::save(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly)
        {
            assert(fileName.extension() == std::string(".xml"));
            pugi::xml_document doc;
            auto rootNode = doc.append_child("assembly");
            rootNode.append_attribute("name").set_value(assembly->getName().c_str());
            auto children = rootNode.append_child("children");

            for (const auto& item : assembly->getItems()) {
                if (auto objectAssembly = ObjectStoreCoreToAssembly(item)) {
                    saveNodeWithAssembly(children, objectAssembly);
                }
                else if (auto objectItem = ObjectStoreCoreToItem(item)) {
                    saveNodeWithItem(children, objectItem);
                }
                else {
                    throw std::runtime_error("Unknown node type which is instance of the superclass of objectStoreCore");
                }
            }

            doc.save_file(fileName.c_str());
        }

        std::shared_ptr<ObjectStoreAssembly> FileSave_XML::load(const std::filesystem::path& fileName)
        {
            auto assembly = std::make_shared<ObjectStoreAssembly>();
            pugi::xml_document doc;

            //check if the file exists
            if (!doc.load_file(fileName.c_str())) {
                throw std::runtime_error(std::string(fileName.string()+"can't find"));
            }

            //check if the root node exists
            auto rootNode = doc.child("assembly");
            if (!rootNode) {
                throw std::runtime_error("root node can't find");
            }

            auto name = rootNode.attribute("name").as_string();
            assembly->setName(name);

            auto childrenNode = rootNode.child("children");
            for (const auto & child : childrenNode.children()) {
                if (child.name() == std::string("assembly")) {
                    auto objectAssembly = std::make_shared<ObjectStoreAssembly>();
                    loadNodeWithAssembly(child, objectAssembly);
                    assembly->addItem(objectAssembly);
                }
                else if (child.name() == std::string("item")) {
                    auto objectItem = std::make_shared<ObjectStoreItem>();
                    loadNodeWithItem(child, objectItem);
                    assembly->addItem(objectItem);
                }
                else {
                    throw std::runtime_error("Unknown node type which is instance of the superclass of objectStoreCore");
                }
            }
            
            return assembly;

        }

        void FileSave_Json::save(const std::filesystem::path& fileName, std::shared_ptr<ObjectStoreAssembly> assembly)
        {
        }

        std::shared_ptr<ObjectStoreAssembly> FileSave_Json::load(const std::filesystem::path& fileName)
        {
            return std::shared_ptr<ObjectStoreAssembly>();
        }

}

}
