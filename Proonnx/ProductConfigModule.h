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
	bool storeRejectdelay(int rejectDelay);

	bool storeOffsetsNumber(int offsetsNumber);

	bool storeDisposalTime(int disposalTime);

public:
	bool storeTopLeftCorner(double x, double y);

	bool storeUpperRightCorner(double x, double y);

	bool storeLeftLowerCorner(double x, double y);

	bool storeLowerRightCornerr(double x, double y);

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
	int readRejectdelay();

	int readOffsetsNumber();

	int readDisposalTime();

public:
	std::pair<double, double> readTopLeftCorner();

	std::pair<double, double> readUpperRightCorner();

	std::pair<double, double> readLeftLowerCorner();

	std::pair<double, double> readLowerRightCornerr();

public:
	int readTotalCount();

	int readPassCount();

	int readOutCount();

};



#endif // !PRODUCTCONFIGMODULE_H_
