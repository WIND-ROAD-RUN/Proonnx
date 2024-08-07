#include"ProductConfigLoader.h"

#include"ProductConfigModule.h"
#include"spdlog/spdlog.h"

ProductConfigLoader::ProductConfigLoader()
{
	m_productConfigModule = new ProductConfigModule();
}

ProductConfigLoader::~ProductConfigLoader()
{
	delete m_productConfigModule;
}

void ProductConfigLoader::setNewFile(const std::string& filePath)
{
	m_productConfigModule->setNewFile(filePath);
}

ProductConfig ProductConfigLoader::loadProductConfig(const std::string& filePath)
{
	spdlog::info("Load Product config by "+ filePath);

	loadFile(filePath);
	ProductConfig result;
	result.ExposureTime = m_productConfigModule->readExposureTime();
	result.gain = m_productConfigModule->readGain();
	result.leftLowerCorner = m_productConfigModule->readLeftLowerCorner();
	result.lowerRightCorner = m_productConfigModule->readLowerRightCornerr();
	result.productName = m_productConfigModule->readProductName();
	result.rotateCount = m_productConfigModule->readRotateCount();
	result.topLeftCorner = m_productConfigModule->readTopLeftCorner();
	result.upperRightCorner = m_productConfigModule->readUpperRightCorner();

	return result;
}

ProductCountInfo ProductConfigLoader::loadProductCountInfo(const std::string& filePath)
{
	loadFile(filePath);
	ProductCountInfo result;
	result.outCount = m_productConfigModule->readOutCount();
	result.passCount = m_productConfigModule->readPassCount();
	result.totalCount = m_productConfigModule->readTotalCount();

	return result;
}

RejectAttribute ProductConfigLoader::loadRejectAttribute(const std::string& filePath)
{
	loadFile(filePath);
	RejectAttribute result;
	result.DisposalTime = m_productConfigModule->readDisposalTime();
	result.OffsetsNumber = m_productConfigModule->readOffsetsNumber();
	result.RejectDelay = m_productConfigModule->readRejectdelay();

	return result;
}

bool ProductConfigLoader::loadFile(const std::string& filePath)
{
	return m_productConfigModule->loadFile(filePath);
}

bool ProductConfigLoader::saveFile(const std::string& filePath)
{
	return m_productConfigModule->saveFile(filePath);
}

bool ProductConfigLoader::storeConfig(const ProductConfig& productConfig)
{
	bool result;
	result = m_productConfigModule->
		storeGain(productConfig.gain);
	result = m_productConfigModule->
		storeLeftLowerCorner(
productConfig.leftLowerCorner.first, productConfig.leftLowerCorner.second);
	result = m_productConfigModule->
		storeLowerRightCornerr(
productConfig.lowerRightCorner.first, productConfig.lowerRightCorner.second);
	result = m_productConfigModule->
		storeProductName(productConfig.productName);
	result = m_productConfigModule->
		storeExposureTime(productConfig.ExposureTime);
	result = m_productConfigModule->
		storeTopLeftCorner(
productConfig.topLeftCorner.first, productConfig.topLeftCorner.second);
	result = m_productConfigModule->
		storeUpperRightCorner(
productConfig.upperRightCorner.first, productConfig.upperRightCorner.second);
	result = m_productConfigModule
		->storeRotateCount(productConfig.rotateCount);
	return result;
}

bool ProductConfigLoader::storeProductProductInfo(const ProductCountInfo& productProductInfo)
{
	auto result = m_productConfigModule->storeOutCount(productProductInfo.outCount);
	result = m_productConfigModule->storeTotalCount(productProductInfo.totalCount);
	result = m_productConfigModule->storePassCount(productProductInfo.passCount);

	return result;
}

bool ProductConfigLoader::storeRejectAttribute(const RejectAttribute& productConfig)
{
	auto result = m_productConfigModule->storeDisposalTime(productConfig.DisposalTime);
	result = m_productConfigModule->storeOffsetsNumber(productConfig.OffsetsNumber);
	result = m_productConfigModule->storeRejectdelay(productConfig.RejectDelay);

	return result;
	
}
