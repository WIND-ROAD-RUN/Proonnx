#ifndef LOCALIZATIONSTRINGMODULE_H_
#define LOCALIZATIONSTRINGMODULE_H_

#include<string>

/*
The abstract interface of the LocalizationStringModule class system defines the necessary interface standards for storing localized strings
*/

class LocalizationStringModuleAbstract {
public:

	LocalizationStringModuleAbstract() {}
	~LocalizationStringModuleAbstract() {}

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

	virtual bool loadFile() = 0;//加载数据到类中
	virtual bool saveFile() = 0;//保存所有更改的数据
};

#endif // !HEADER_LOCALIZATIONSTRINGMODULE_H_

