#include"ProductConfigModule.h"

#include"pugixml.hpp"
#include"spdlog/spdlog.h"

ProductConfigModule::ProductConfigModule()
{
	m_doc = new pugi::xml_document();
}

ProductConfigModule::~ProductConfigModule()
{
	delete m_doc;
}

void ProductConfigModule::setNewFile(const std::string filePath)
{
	auto productConfigNode = m_doc->append_child("ProductConfig");
	auto productNameNode = productConfigNode.append_child("ProductName");
	auto exposureTimeNode = productConfigNode.append_child("ExposureTime");
	auto gainNode = productConfigNode.append_child("Gain");
	auto rotateCountNode = productConfigNode.append_child("RotateCount");
	auto recognizeRangeNode = productConfigNode.append_child("RecognizeRange");

	auto topLeftCornerNode = recognizeRangeNode.append_child("TopLeftCorner");
	topLeftCornerNode.append_child("X");
	topLeftCornerNode.append_child("Y");

	auto upperRightCornerNode = recognizeRangeNode.append_child("UpperRightCorner");
	upperRightCornerNode.append_child("X");
	upperRightCornerNode.append_child("Y");

	auto leftLowerCornerNode = recognizeRangeNode.append_child("LeftLowerCorner");
	leftLowerCornerNode.append_child("X");
	leftLowerCornerNode.append_child("Y");

	auto lowerRightCornerNode = recognizeRangeNode.append_child("LowerRightCorner");
	lowerRightCornerNode.append_child("X");
	lowerRightCornerNode.append_child("Y");

	m_doc->save_file(filePath.c_str());
}

bool ProductConfigModule::loadFile(const std::string filePath)
{
	if (m_doc->load_file(filePath.c_str())) {
		return true;
	}
	return false;
}

bool ProductConfigModule::saveFile(const std::string filePath)
{
	if (m_doc->save_file(filePath.c_str())) {
		return true;
	}
	return false;
}

bool ProductConfigModule::storeProductName(const std::string& productName)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto productNameNode = productConfigNode.child("ProductName");
	if (!productNameNode) {
		return false;
	}
	auto storeResult=productNameNode.text().set(productName.c_str());
	if (!storeResult) {
		return false;
	}
	return true;
}

bool ProductConfigModule::storeExposureTime(int exposureTime)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto exposureTimeNode = productConfigNode.child("ExposureTime");
	if (!exposureTimeNode) {
		return false;
	}
	auto storeReslt=exposureTimeNode.text().set(exposureTime);
	if (!storeReslt) {
		return false;
	}
	return true;
}

bool ProductConfigModule::storeRotateCount(int count)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto rotateCountNode = productConfigNode.child("RotateCount");
	if (!rotateCountNode) {
		return false;
	}
	auto storeResult=rotateCountNode.text().set(count);
	if (!storeResult) {
		return false;
	}
	return true;
}

bool ProductConfigModule::storeGain(int gain)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto gainNode = productConfigNode.child("Gain");
	if (!gainNode) {
		return false;
	}
	auto storeResult=gainNode.text().set(gain);
	if (!storeResult) {
		return false;
	}
	return true;
}

bool ProductConfigModule::storeTopLeftCorner(int x, int y)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	if (!recognizeRangeNode) {
		return false;
	}
	auto topLeftCornerNode = recognizeRangeNode.child("TopLeftCorner");
	if (!topLeftCornerNode) {
		return false;
	}

	auto xNode = topLeftCornerNode.child("X");
	if (!xNode) {
		return false;
	}
	auto storeXResult=xNode.text().set(x);
	if (!storeXResult) {
		return false;
	}

	auto yNode = topLeftCornerNode.child("Y");
	if (!yNode) {
		return false;
	}
	auto storeyResult = yNode.text().set(y);
	if (!storeyResult) {
		return false;
	}
	return true;
}

bool ProductConfigModule::storeUpperRightCorner(int x, int y)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	if (!recognizeRangeNode) {
		return false;
	}
	auto upperRightCornerNode = recognizeRangeNode.child("UpperRightCorner");
	if (!upperRightCornerNode) {
		return false;
	}

	auto xNode = upperRightCornerNode.child("X");
	if (!xNode) {
		return false;
	}
	auto storeXResult=xNode.text().set(x);
	if (!storeXResult) {
		return false;
	}

	auto yNode = upperRightCornerNode.child("Y");
	if (!yNode) {
		return false;
	}
	auto storeYResult=yNode.text().set(y);
	if (!storeYResult) {
		return false;
	}
	return true;
}

bool ProductConfigModule::storeLeftLowerCorner(int x, int y)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	if (!recognizeRangeNode) {
		return false;
	}
	auto leftLowerCornerNode = recognizeRangeNode.child("LeftLowerCorner");
	if (!leftLowerCornerNode) {
		return false;
	}

	auto xNode = leftLowerCornerNode.child("X");
	if (!xNode) {
		return false;
	}
	auto storeXResult=xNode.text().set(x);
	if (!storeXResult) {
		return false;
	}

	auto yNode = leftLowerCornerNode.child("Y");
	if (!yNode) {
		return false;
	}
	auto storeYResult = yNode.text().set(y);
	if (!storeYResult) {
		return false;
	}
	return true;
}

bool ProductConfigModule::storeLowerRightCornerr(int x, int y)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	if (!recognizeRangeNode) {
		return false;
	}
	auto lowerRightCornerNode = recognizeRangeNode.child("LowerRightCorner");
	if (!lowerRightCornerNode) {
		return false;
	}

	auto xNode = lowerRightCornerNode.child("X");
	if (!xNode) {
		return false;
	}
	auto storeXResult=xNode.text().set(x);
	if (!storeXResult) {
		return false;
	}

	auto yNode = lowerRightCornerNode.child("Y");
	if (!yNode) {
		return false;
	}
	auto storeYResult = yNode.text().set(y);
	if (!storeYResult) {
		return false;
	}

	return true;
}

std::string ProductConfigModule::readProductName()
{
	std::string result;
	auto productConfigNode = m_doc->child("ProductConfig");
	auto productNameNode = productConfigNode.child("ProductName");
	result = productNameNode.text().as_string();

	return result;
}

int ProductConfigModule::readExposureTime()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto exposureTimeNode = productConfigNode.child("ExposureTime");
	auto result = exposureTimeNode.text().as_int();

	return result;
}

int ProductConfigModule::readRotateCount()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto rotateCountNode = productConfigNode.child("RotateCount");
	auto result = rotateCountNode.text().as_int();

	return result;
}

int ProductConfigModule::readGain()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto gainNode = productConfigNode.child("Gain");
	auto result = gainNode.text().as_int();

	return result;
}

std::pair<int, int> ProductConfigModule::readTopLeftCorner()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	auto topLeftCornerNode = recognizeRangeNode.child("TopLeftCorner");
	auto xNode = topLeftCornerNode.child("X");
	auto x = xNode.text().as_int();
	auto yNode = topLeftCornerNode.child("Y");
	auto y = yNode.text().as_int();

	return {x,y};
}

std::pair<int, int> ProductConfigModule::readUpperRightCorner()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	auto upperRightCornerNode = recognizeRangeNode.child("UpperRightCorner");
	auto xNode = upperRightCornerNode.child("X");
	auto x = xNode.text().as_int();
	auto yNode = upperRightCornerNode.child("Y");
	auto y = yNode.text().as_int();

	return { x,y };
}

std::pair<int, int> ProductConfigModule::readLeftLowerCorner()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	auto leftLowerCornerNode = recognizeRangeNode.child("LeftLowerCorner");
	auto xNode = leftLowerCornerNode.child("X");
	auto x = xNode.text().as_int();
	auto yNode = leftLowerCornerNode.child("Y");
	auto y = yNode.text().as_int();

	return { x,y };
}

std::pair<int, int> ProductConfigModule::readLowerRightCornerr()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	auto lowerRightCornerNode = recognizeRangeNode.child("LowerRightCorner");
	auto xNode = lowerRightCornerNode.child("X");
	auto x = xNode.text().as_int();
	auto yNode = lowerRightCornerNode.child("Y");
	auto y = yNode.text().as_int();

	return { x,y };
}
