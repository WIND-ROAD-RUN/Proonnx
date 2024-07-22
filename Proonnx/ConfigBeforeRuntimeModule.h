#ifndef CONFIGBEFORERUNTIMEMODULE_H_
#define CONFIGBEFORERUNTIMEMODULE_H_

#include<string>

namespace pugi {
	class xml_document;
}

class ConfigBeforeRuntimeModule {
private:
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
	void saveFile(const std::string& filePath);

	int readCameraCount();
	std::string readLanguage();
};


#endif // !CONFIGBEFORERUNTIMEMODULE_H_
