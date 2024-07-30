#ifndef PRODUCTCONFIGLOADER_H_
#define PRODUCTCONFIGLOADER_H_

#include<string>

class ProductConfigModule;

struct ProductConfig {
	std::string productName{"XXXX"};

	long ExposureTime{10000};
	int gain{1};
	int rotateCount{0};

	std::pair<double, double> topLeftCorner{0,0};
	std::pair<double, double> upperRightCorner{ 0,0 };
	std::pair<double, double> leftLowerCorner{ 0,0 };
	std::pair<double, double> lowerRightCorner{ 0,0 };

};

struct RecognizeRange {
	std::pair<double, double> topLeftCorner{ 0,0 };
	std::pair<double, double> upperRightCorner{ 0,0 };
	std::pair<double, double> leftLowerCorner{ 0,0 };
	std::pair<double, double> lowerRightCorner{ 0,0 };
};

struct ProductCountInfo {
	int totalCount{ 0 };
	int passCount{ 0 };
	int outCount{ 0 };
};

struct RejectAttribute {
	int RejectDelay{ 0 };
	int OffsetsNumber{ 0 };
	int DisposalTime{ 0 };
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
	ProductConfig loadProductConfig(const std::string& filePath);

	ProductCountInfo loadProductCountInfo(const std::string& filePath);

	RejectAttribute loadRejectAttribute(const std::string& filePath);

public:
	bool storeConfig(const ProductConfig& productConfig);

	bool storeProductProductInfo(const ProductCountInfo & productProductInfo);

	bool storeRejectAttribute(const RejectAttribute& productConfig);

};

#endif // !PRODUCTCONFIGLOADER_H_
