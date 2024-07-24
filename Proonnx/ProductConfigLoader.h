#ifndef PRODUCTCONFIGLOADER_H_
#define PRODUCTCONFIGLOADER_H_

#include<string>

class ProductConfigModule;

struct ProductConfig {
	std::string productName{"XXXX"};
	long ExposureTime{10000};
	int gain{1};
	int rotateCount{0};
	std::pair<int, int> topLeftCorner{0,0};
	std::pair<int, int> upperRightCorner{ 0,0 };
	std::pair<int, int> leftLowerCorner{ 0,0 };
	std::pair<int, int> lowerRightCorner{ 0,0 };
};

struct RecognizeRange {
	std::pair<int, int> topLeftCorner{ 0,0 };
	std::pair<int, int> upperRightCorner{ 0,0 };
	std::pair<int, int> leftLowerCorner{ 0,0 };
	std::pair<int, int> lowerRightCorner{ 0,0 };
};

struct ProductProductInfo {
	int totalCount;
	int passCount;
	int outCount;
};

class ProductConfigLoader {
private:
	ProductConfigModule* m_productConfigModule{nullptr};
public:
	ProductConfigLoader();
	~ProductConfigLoader();
private:
	bool loadFile(const std::string& filePath);
public:
	void setNewFile(const std::string& filePath);
	bool saveFile(const std::string& filePath);
public:
	ProductConfig loadConfig(const std::string& filePath);
	ProductProductInfo loadProductProductInfo(const std::string& filePath);

	bool storeConfig(const ProductConfig& productConfig);
	bool storeProductProductInfo(const ProductProductInfo & productProductInfo);

};

#endif // !PRODUCTCONFIGLOADER_H_
