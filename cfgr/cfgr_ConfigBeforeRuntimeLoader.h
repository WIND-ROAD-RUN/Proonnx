#ifndef CFGR_CONFIGBEFORERUNTIMELOADER_H_
#define CFGR_CONFIGBEFORERUNTIMELOADER_H_

#include<string>

namespace rw {
	namespace cfgr {

		class ConfigBeforeRuntimeModule;

		class ConfigBeforeRuntimeLoader {
		private:
			ConfigBeforeRuntimeModule* m_configBeforeRuntimeModule{ nullptr };

		public:
			ConfigBeforeRuntimeLoader();

			~ConfigBeforeRuntimeLoader();

		public:
			void setNewFile(const std::string& filePath);

			bool loadFile(const std::string& filePath);

			void saveFile(const std::string& filePath);

		public:
			bool storeCameraCount(int count);

			bool storeLanguage(const std::string& language);
		public:
			bool storeCameraConfig(const std::string& ip, const std::string& configPath);

			bool readCameraConfig(const std::string& ip, std::string& configPath);

		public:
			int readCameraCount();

			std::string readLanguage();


		};

	}
}


#endif // !CFGR_CONFIGBEFORERUNTIMELOADER_H_
