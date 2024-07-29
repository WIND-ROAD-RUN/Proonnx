#ifndef LOGMODULE_H_
#define LOGMODULE_H_

#include<string>

class LogFileModule {
public:
	LogFileModule() {}
	~LogFileModule() {}
public:
	void iniCom();
	
	void info(const std::string & str) ;
	void error(const std::string& str);
	void warn(const std::string& str);
};

#endif // !LOGMODULE_H_
