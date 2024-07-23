#ifndef CONFIGBEFORERNTIMELOADER_H_
#define CONFIGBEFORERNTIMELOADER_H_

#include<string>

class ConfigBeforeRuntimeModule;

class ConfigBeforeRuntimeLoader {
private:
	ConfigBeforeRuntimeModule* m_configBeforeRuntimeModule{nullptr};

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
	int readCameraCount();

	std::string readLanguage();


};

#endif // !CONFIGBEFORERNTIMELOADER_H_
