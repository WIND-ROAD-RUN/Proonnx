#ifndef CFGR_CONFIGBEFORERUNTIMEMODULE_H_
#define CFGR_CONFIGBEFORERUNTIMEMODULE_H_

#include<string>

namespace pugi {
	class xml_document;
}

namespace rw {
	namespace cfgr {

		class ConfigBeforeRuntimeModule {
		private:
			pugi::xml_document* m_doc;

			std::string m_filePath;

		public:
			ConfigBeforeRuntimeModule();

			~ConfigBeforeRuntimeModule();

		public:
			void setNewFile(const std::string filePath);

			bool loadFile(const std::string filePath);

			bool storeCameraCount(int count);

			bool storeLanguage(const std::string& language);

			bool storeCameraConfig(const std::string& ip, const std::string& configPath);

			void saveFile(const std::string& filePath);
		public:
			int readCameraCount();

			std::string readLanguage();

			bool  readCameraConfig(const std::string& ip, std::string& configPath);
		};

	}

}

#endif // !CFGR_CONFIGBEFORERUNTIMEMODULE_H_
