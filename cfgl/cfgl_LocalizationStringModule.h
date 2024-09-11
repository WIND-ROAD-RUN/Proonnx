#ifndef CFGL_LOCALIZATIONSTRINGMODULE_H_
#define CFGL_LOCALIZATIONSTRINGMODULE_H_

#include<string>
#include <unordered_map>

namespace pugi {
	class xml_document;
}

namespace rw
{
	namespace cfgl
	{
		class LocalizationStringModule_Abstract {
		public:

			LocalizationStringModule_Abstract() {}

			~LocalizationStringModule_Abstract() {}

		public:
			/*
				This interface queries the corresponding localized string based on the ID and local, and if it is found, it should return a string
			If no query is found, return an empty string. This interface should be called virtual boolean localString
			(std:: string&target, const std:: string&id, const std:: string&local="CHN")=0; Implementation
			*/
			virtual std::string localString(const std::string& id, const std::string& language = "CHN") = 0;

			/*
				This interface queries the corresponding localized string based on the ID and local. If it is found, assign it to the target and return true. If it is not found, return false,
			And the target should not change.
			*/
			virtual bool localString(std::string& target, const std::string& id, const std::string& language = "CHN") = 0;

			/*
			Save localized strings, with parameters corresponding to id (string id), language (stored language), src (stored string), and save them if the corresponding node cannot be found
		Return true, if there is already a corresponding node, return false
			*/
			virtual bool storeLocalString(const std::string& id, const std::string& language, const std::string& src) = 0;

			/*
				Change the localized string, with parameters corresponding to id (string id), language (stored language), src (string to be changed to), and update them once the corresponding node is found
			Return true, if there is no corresponding node, return false
			*/
			virtual bool changeLocalString(const std::string& id, const std::string& language, const std::string& src) = 0;

			virtual bool loadFile() = 0;
			virtual bool saveFile() = 0;
		};

		class LocalizationStringXML
			:public LocalizationStringModule_Abstract {
			/*
			LocalizationStringXML is a subclass of LocalizationStringModulaAbstract that stores localized strings as data files using XML,
			Follow the interface of abstract base classes.
			*/

			/*
			be careful!!!
			The implementation of this class relies on the third-party library pugixml. If the project does not have a third-party library installed, please use the project's VNet package manager
			Install the third library Pugixml for the project or download the source file and add it to the project, and build it yourself
			*/
		private:
			std::string m_filePath{};
			pugi::xml_document* m_doc{ nullptr };
		public:
			LocalizationStringXML(const std::string& filePath);//It is best to initialize the class using this method
			LocalizationStringXML();
			~LocalizationStringXML();
		public:
			void setFilePath(const std::string& filePath) { m_filePath = filePath; }//Set file path
			/*
		Convert data into a map*/
			bool getMap(std::unordered_map<std::string, std::string>& map, const std::string& language);
		public:
			// Inherit through LocalizationStringModulaAbstract
			std::string localString(const std::string& id, const std::string& language) override;

			bool localString(std::string& target, const std::string& id, const std::string& language) override;

			bool storeLocalString(const std::string& id, const std::string& language, const std::string& src) override;

			/*Load data file*/
			bool loadFile()override;

			/*Save the data in memory in a data file*/
			bool saveFile() override;

			// Inherit through LocalizationStringModulaAbstract
			bool changeLocalString(const std::string& id, const std::string& language, const std::string& src) override;
		};
	}
}



#endif
