#include"ProductConfigModule.h"

#include"pugixml.hpp"
#include"spdlog/spdlog.h"
#include<qdebug>

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

	auto productCountInfoNode = productConfigNode.append_child("ProductCountInfo");
	auto totalCountNode = productCountInfoNode.append_child("TotalCount");
	totalCountNode.text().set(0);
	auto passCountNode = productCountInfoNode.append_child("PassCount");
	passCountNode.text().set(0);
	auto outCountNode = productCountInfoNode.append_child("OutCount");
	outCountNode.text().set(0);

	auto rejectAttributeNode = productConfigNode.append_child("RejectAttribute");
	auto rejectDelayNode = rejectAttributeNode.append_child("RejectDelay");
	rejectDelayNode.text().set(0);
	auto offsetsNumberNode = rejectAttributeNode.append_child("OffsetsNumber");
	offsetsNumberNode.text().set(0);
	auto oisposalTimeNode = rejectAttributeNode.append_child("DisposalTime");
	oisposalTimeNode.text().set(0);

	auto recognizeRangeNode = productConfigNode.append_child("RecognizeRange");

	auto topLeftCornerNode = recognizeRangeNode.append_child("TopLeftCorner");
	topLeftCornerNode.append_child("X").text().set(0);
	topLeftCornerNode.append_child("Y").text().set(0);

	auto upperRightCornerNode = recognizeRangeNode.append_child("UpperRightCorner");
	upperRightCornerNode.append_child("X").text().set(0);
	upperRightCornerNode.append_child("Y").text().set(0);

	auto leftLowerCornerNode = recognizeRangeNode.append_child("LeftLowerCorner");
	leftLowerCornerNode.append_child("X").text().set(0);
	leftLowerCornerNode.append_child("Y").text().set(0);

	auto lowerRightCornerNode = recognizeRangeNode.append_child("LowerRightCorner");
	lowerRightCornerNode.append_child("X").text().set(0);
	lowerRightCornerNode.append_child("Y").text().set(0);

	auto result= m_doc->save_file(filePath.c_str());
	qDebug() << result;
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

bool ProductConfigModule::storeRejectdelay(int rejectDelay)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto rejectAttributeNode = productConfigNode.child("RejectAttribute");
	if (!rejectAttributeNode) {
		return false;
	}
	auto rejectDelayNode = rejectAttributeNode.child("RejectDelay");
	if (!rejectDelayNode) {
		return false;
	}
	return rejectDelayNode.text().set(rejectDelay);
}

bool ProductConfigModule::storeOffsetsNumber(int offsetsNumber)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto rejectAttributeNode = productConfigNode.child("RejectAttribute");
	if (!rejectAttributeNode) {
		return false;
	}
	auto offsetsNumberNode = rejectAttributeNode.child("OffsetsNumber");
	if (!offsetsNumberNode) {
		return false;
	}
	return offsetsNumberNode.text().set(offsetsNumber);
}

bool ProductConfigModule::storeDisposalTime(int disposalTime)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto rejectAttributeNode = productConfigNode.child("RejectAttribute");
	if (!rejectAttributeNode) {
		return false;
	}
	auto disposalTimeNode = rejectAttributeNode.child("DisposalTime");
	if (!disposalTimeNode) {
		return false;
	}
	return disposalTimeNode.text().set(disposalTime);
}

bool ProductConfigModule::storeTopLeftCorner(double x, double y)
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

bool ProductConfigModule::storeUpperRightCorner(double x, double y)
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

bool ProductConfigModule::storeLeftLowerCorner(double x, double y)
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

bool ProductConfigModule::storeLowerRightCornerr(double x, double y)
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

bool ProductConfigModule::storeTotalCount(int count)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto productProductInfoNode = productConfigNode.child("ProductCountInfo");
	if (!productProductInfoNode) {
		return false;
	}
	auto totalCountNode = productProductInfoNode.child("TotalCount");
	if (!totalCountNode) {
		return false;
	}
	auto storeResult=totalCountNode.text().set(count);

	return storeResult;

}

bool ProductConfigModule::storePassCount(int count)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto productProductInfoNode = productConfigNode.child("ProductCountInfo");
	if (!productProductInfoNode) {
		return false;
	}
	auto passCountNode = productProductInfoNode.child("PassCount");
	if (!passCountNode) {
		return false;
	}
	auto storeResult = passCountNode.text().set(count);

	return storeResult;
}

bool ProductConfigModule::storeOutCount(int count)
{
	auto productConfigNode = m_doc->child("ProductConfig");
	if (!productConfigNode) {
		return false;
	}
	auto productProductInfoNode = productConfigNode.child("ProductCountInfo");
	if (!productProductInfoNode) {
		return false;
	}
	auto outCountNode = productProductInfoNode.child("OutCount");
	if (!outCountNode) {
		return false;
	}
	auto storeResult = outCountNode.text().set(count);

	return storeResult;
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

int ProductConfigModule::readRejectdelay()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto rejectAttributeNode = productConfigNode.child("RejectAttribute");
	auto rejectDelayNode = rejectAttributeNode.child("RejectDelay");
	auto result = rejectDelayNode.text().as_int();

	return result;
}

int ProductConfigModule::readOffsetsNumber()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto rejectAttributeNode = productConfigNode.child("RejectAttribute");
	auto offsetsNumberNode = rejectAttributeNode.child("OffsetsNumber");
	auto result = offsetsNumberNode.text().as_int();

	return result;
}

int ProductConfigModule::readDisposalTime()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto rejectAttributeNode = productConfigNode.child("RejectAttribute");
	auto disposalTimeNode = rejectAttributeNode.child("DisposalTime");
	auto result = disposalTimeNode.text().as_int();

	return result;
}

std::pair<double, double> ProductConfigModule::readTopLeftCorner()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	auto topLeftCornerNode = recognizeRangeNode.child("TopLeftCorner");
	auto xNode = topLeftCornerNode.child("X");
	auto x = xNode.text().as_double();
	auto yNode = topLeftCornerNode.child("Y");
	auto y = yNode.text().as_double();

	return {x,y};
}

std::pair<double, double> ProductConfigModule::readUpperRightCorner()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	auto upperRightCornerNode = recognizeRangeNode.child("UpperRightCorner");
	auto xNode = upperRightCornerNode.child("X");
	auto x = xNode.text().as_double();
	auto yNode = upperRightCornerNode.child("Y");
	auto y = yNode.text().as_double();

	return { x,y };
}

std::pair<double, double> ProductConfigModule::readLeftLowerCorner()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	auto leftLowerCornerNode = recognizeRangeNode.child("LeftLowerCorner");
	auto xNode = leftLowerCornerNode.child("X");
	auto x = xNode.text().as_double();
	auto yNode = leftLowerCornerNode.child("Y");
	auto y = yNode.text().as_double();

	return { x,y };
}

std::pair<double, double> ProductConfigModule::readLowerRightCornerr()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto recognizeRangeNode = productConfigNode.child("RecognizeRange");
	auto lowerRightCornerNode = recognizeRangeNode.child("LowerRightCorner");
	auto xNode = lowerRightCornerNode.child("X");
	auto x = xNode.text().as_double();
	auto yNode = lowerRightCornerNode.child("Y");
	auto y = yNode.text().as_double();

	return { x,y };
}

int ProductConfigModule::readTotalCount()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto productProductInfoNode = productConfigNode.child("ProductCountInfo");
	auto totalCountNode = productProductInfoNode.child("TotalCount");
	return totalCountNode.text().as_int();
}

int ProductConfigModule::readPassCount()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto productProductInfoNode = productConfigNode.child("ProductCountInfo");
	auto passCountNode = productProductInfoNode.child("PassCount");
	return passCountNode.text().as_int();
}

int ProductConfigModule::readOutCount()
{
	auto productConfigNode = m_doc->child("ProductConfig");
	auto productProductInfoNode = productConfigNode.child("ProductCountInfo");
	auto outCountNode = productProductInfoNode.child("OutCount");
	return outCountNode.text().as_int();
}
