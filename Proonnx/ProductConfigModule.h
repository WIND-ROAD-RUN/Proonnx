#ifndef PRODUCTCONFIGMODULE_H_
#define PRODUCTCONFIGMODULE_H_

#include<string>

namespace pugi {
	class xml_document;
}

class ProductConfigModule {
private:
	pugi::xml_document * m_doc;
	std::string m_filePath;
public:
	ProductConfigModule();
	~ProductConfigModule();
public:
	void setNewFile(const std::string filePath);
	bool loadFile(const std::string filePath);
	bool saveFile(const std::string filePath);
public:
	bool storeProductName(const std::string & productName);
	bool storeExposureTime(int recognitionRange);
	bool storeRotateCount(int count);
	bool storeGain(int gain);
public:
	bool storeTopLeftCorner(int x,int y);
	bool storeUpperRightCorner(int x, int y);
	bool storeLeftLowerCorner(int x, int y);
	bool storeLowerRightCornerr(int x, int y);
public:
	bool storeTotalCount(int count);
	bool storePassCount(int count);
	bool storeOutCount(int count);
public:
	std::string readProductName();
	int readExposureTime();
	int readRotateCount();
	int readGain();
public:
	std::pair<int,int> readTopLeftCorner();
	std::pair<int, int> readUpperRightCorner();
	std::pair<int, int> readLeftLowerCorner();
	std::pair<int, int> readLowerRightCornerr();
public:
	int readTotalCount();
	int readPassCount();
	int readOutCount();
};



#endif // !PRODUCTCONFIGMODULE_H_
