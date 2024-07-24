#include"ImageIdentify.h"

#include <cctype> 
#include <cstring> 
#include <unordered_set>
#include <algorithm>
#include<QPainter>

#include"qdebug.h"
#include"ProductCheck.h"
#include"ProductConfigLoader.h"
#include"MonitorCamera.h"


void ImageIdentify::setDlgLabelForImage(QLabel* label)
{
	m_dlgLabelForImage = label;
}

void ImageIdentify::setDisaplayCheckInfo(QLabel* label)
{
	m_disaplayCheckInfo = label;
}

void ImageIdentify::iniCamera()
{
	m_productLoader = new ProductConfigLoader();

	auto cameraConfig = m_productLoader->loadConfig(m_productConfigFilePath);
	setExposureTime(cameraConfig.ExposureTime);
	setGain(cameraConfig.ExposureTime);

	if (m_labelForProductName) {
		m_labelForProductName->setText(QString::fromStdString(cameraConfig.productName));
	}

	auto productConfig= m_productLoader->loadProductProductInfo(m_productConfigFilePath);

	if (m_labelForProductCount) {
		m_stringForProductCount=m_labelForProductCount->text();
		m_labelForProductCount->setText(m_stringForProductCount+ QString::number(productConfig.totalCount));
		m_productCount = productConfig.totalCount;
	}
	if (m_labelForProductOutCount) {
		m_stringForProductOutCount = m_labelForProductOutCount->text();
		m_labelForProductOutCount->setText(m_stringForProductOutCount+ QString::number(productConfig.outCount));
		m_productOutCount = productConfig.outCount;
	}
	if (m_labelForProductPassCount) {
		m_stringForProductPassCount= m_labelForProductPassCount->text();
		m_labelForProductPassCount->setText(m_stringForProductPassCount+ QString::number(productConfig.passCount));
		m_productPassCount= productConfig.passCount;
	}
}

void ImageIdentify::setStandDate(const QString& standardDate)
{
	m_standardDate = standardDate;
}

ImageIdentify::ImageIdentify(QLabel* label, const std::string& ip)
{
	m_labelForImage = label;
	m_Ip = ip;
	m_monitorCamera = new MonitorCamera();
	m_indentModel = new ocrwork();
	ini_connect();
}

ImageIdentify::~ImageIdentify()
{
	m_monitorCamera->stopAcquisition();
	delete m_indentModel;
	delete m_labelForImage;
	delete m_labelForImage;
	delete m_monitorCamera;
	delete m_productCheck;
	delete m_productLoader;
}

void ImageIdentify::ini_connect()
{
	QObject::connect(m_monitorCamera, SIGNAL(ImgCallBackSignal(unsigned char*, MV_FRAME_OUT_INFO_EX*)),
		this, SLOT(DisplayImage(unsigned char*, MV_FRAME_OUT_INFO_EX*)));
}

bool ImageIdentify::connectCamera()
{
	m_monitorCamera->setIp(m_Ip);
	return m_monitorCamera->connectCamera();
}


void ImageIdentify::IniOcr()
{
	m_indentModel->initial();
	m_productCheck = new ProductCheck<std::vector<OCRResult>, QString>(5, ImageIdentifyUtilty::checkProduct);
}

void ImageIdentify::startAcquisition()
{
	m_monitorCamera->startAcquisition();
}

void ImageIdentify::stopAcquisition()
{
	m_monitorCamera->stopAcquisition();
}

bool ImageIdentify::setExposureTime(int exposureTime)
{
	return m_monitorCamera->setExposureTime(exposureTime);
}

bool ImageIdentify::setGain(int gain)
{
	return m_monitorCamera->setGain(gain);
}

void ImageIdentify::startMonitor()
{
	m_monitorCamera->startAcquisition();
}


void ImageIdentify::display_image(cv::Mat& mat)
{
	QImage im =
		ImageIdentifyUtilty::convcertImageFormCvMat(mat);

	if (im.isNull()) {
		qDebug() << "cvMat2QImage failed";
		return;
	}
	// Convert QImage to QPixmap and display it on QLabel
	QPixmap pixmap = QPixmap::fromImage(im);

	if (pixmap.isNull()) {
		qDebug() << "QPixmap::fromImage failed";
		return;
	}

	pixmap = pixmap.scaled(m_labelForImage->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	m_labelForImage->setPixmap(pixmap);
	m_labelForImage->setScaledContents(true); // Fill the image with QLabel

	if (m_dlgLabelForImage) {
		m_dlgLabelForImage->setPixmap(pixmap);
		m_dlgLabelForImage->setScaledContents(true); // Fill the image with QLabel
	}
}

void ImageIdentify::update_productInfo_label(bool check)
{
	++m_productCount;
	m_labelForProductCount->setText(m_stringForProductCount + QString::number(m_productCount));
	if (check) {
		++m_productPassCount;
		m_labelForProductPassCount->setText(m_stringForProductPassCount + QString::number(m_productPassCount));
	}
	else {
		++m_productOutCount;
		m_labelForProductOutCount->setText(m_stringForProductOutCount + QString::number(m_productOutCount));
	}
	ProductProductInfo config;
	config.totalCount = m_productCount;
	config.passCount = m_productPassCount;
	config.outCount = m_productOutCount;
	
	m_productLoader->storeProductProductInfo(config);
	m_productLoader->saveFile(m_productConfigFilePath);
}

void ImageIdentify::render_image(cv::Mat& image)
{
	display_image(image);
}

std::vector<OCRResult> ImageIdentify::ocr_image(cv::Mat srcMat)
{
	std::vector<OCRResult> result;
	m_indentModel->testOcr(srcMat, result);
	m_indentModel->drawView(srcMat, result);
	return result;
}

bool ImageIdentify::check_productDate(const std::vector<OCRResult>& date)
{
	
}

void ImageIdentify::change_check_state(bool check)
{
	auto palette = m_disaplayCheckInfo->palette();
	if (check) {
		palette.setColor(QPalette::WindowText, Qt::green); 
		m_disaplayCheckInfo->setPalette(palette);
		m_disaplayCheckInfo->setText("OK");
	}
	else {
		palette.setColor(QPalette::WindowText, Qt::red);
		m_disaplayCheckInfo->setPalette(palette);
		m_disaplayCheckInfo->setText("NG");
	}
}

void ImageIdentify::DisplayImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo)
{
	// Convert image data to cv:: Mat format
	cv::Mat mat;
	mat= ImageIdentifyUtilty::ConvertMat(pFrameInfo, pData);

	if (mat.empty()) {
		qDebug() << "mat is empty";
		return;
	}
	auto recognizeResult = ocr_image(mat);

	if (is_check) {
		auto checkResult = m_productCheck->check(recognizeResult, m_standardDate);
		if (checkResult == ProductCheckUtilty::ProductCheckInfo::WITHIN_THRESHOLD) {
			change_check_state(true);
			update_productInfo_label(true);
		}
		else {
			change_check_state(false);
			update_productInfo_label(false);
		}
	}
	render_image(mat);
}

int 
ImageIdentifyUtilty::RGB2BGR
(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight)
{
	if (NULL == pRgbData)
	{
		return MV_E_PARAMETER;
	}

	for (unsigned int j = 0; j < nHeight; j++)
	{
		for (unsigned int i = 0; i < nWidth; i++)
		{
			unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
			pRgbData[j * (nWidth * 3) + i * 3] = pRgbData[j * (nWidth * 3) + i * 3 + 2];
			pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
		}
	}

	return MV_OK;
}

cv::Mat ImageIdentifyUtilty::ConvertMat(MV_FRAME_OUT_INFO_EX* pFrameInfo, unsigned char* pData)
{
	cv::Mat mat;
	switch (pFrameInfo->enPixelType) {
	case PixelType_Gvsp_RGB8_Packed:
		mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
		cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB); // convert color format
		break;
	case PixelType_Gvsp_Mono8:
		mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
		break;
	case PixelType_Gvsp_YUV422_Packed:
		mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC2, pData);
		cv::cvtColor(mat, mat, cv::COLOR_YUV2RGB_Y422);
		break;
	case PixelType_Gvsp_YUV422_YUYV_Packed:
		mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC2, pData);
		cv::cvtColor(mat, mat, cv::COLOR_YUV2RGB_YUYV);
		break;
	case PixelType_Gvsp_BayerGB8:
		mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
		cv::cvtColor(mat, mat, cv::COLOR_BayerGB2RGB);
		break;
	case PixelType_Gvsp_BayerGR8: 
		mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
		cv::cvtColor(mat, mat, cv::COLOR_BayerGR2RGB);
		break;
	default:
		qDebug() << "Unsupported pixel type";
		return mat;
	}
	return mat;
}

QImage ImageIdentifyUtilty::convcertImageFormCvMat(cv::Mat& mat)
{
	QImage im;
	if (mat.type() == CV_8UC3) {
		im = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
	}
	else if (mat.type() == CV_8UC1) {
		im = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
	}
	else {
		qDebug() << "Unsupported cv::Mat type";
		return im;
	}
	return im;
}

bool ImageIdentifyUtilty::checkProduct(std::vector<OCRResult>& data, QString& standardDate)
{
	bool result{false};
	for (const auto &item: data) {
		if (!ImageIdentifyUtilty::isAlphanumericOrPunct(item.text)) {
			continue;
		}
		auto replaceDate= ImageIdentifyUtilty::replaceChar(item.text,'Q','0');
		replaceDate = trimToSubstring(replaceDate, ImageIdentifyUtilty::getFirstNCharacters(standardDate.toStdString(),3));
		result= ImageIdentifyUtilty::hashSimilarity(replaceDate, standardDate.toStdString())>=90;
	}
	return result;
}

bool ImageIdentifyUtilty::isAlphanumericOrPunct(const char* str)
{
	if (str == nullptr) {
		return false; 
	}

	for (size_t i = 0; i < strlen(str); ++i) {
		char ch = str[i];
		if (!std::isalnum(ch) && !std::ispunct(ch)) {
			return false; 
		}
	}
	return true; 
}

std::string ImageIdentifyUtilty::replaceChar(const char* str, char oldChar, char newChar)
{
	if (str == nullptr) {
		return ""; // 检查空指针，返回空字符串
	}

	std::string result = str; // 将 const char* 转换为 std::string

	// 遍历字符串并替换字符
	for (size_t i = 0; i < result.length(); ++i) {
		if (result[i] == oldChar) {
			result[i] = newChar; // 替换字符
		}
	}

	return result; // 返回更改后的字符串
}

std::string ImageIdentifyUtilty::trimToSubstring(std::string str1, const std::string& str2)
{
	// 查找 str2 在 str1 中的位置
	size_t pos = str1.find(str2);

	// 如果找到 str2，返回从该位置开始的子字符串
	if (pos != std::string::npos) {
		return str1.substr(pos); // 返回从 pos 开始的子字符串
	}

	// 如果未找到，返回原始 str1
	return str1;
}

std::string ImageIdentifyUtilty::getFirstNCharacters(const std::string& str, int n)
{
	// 确保 n 不超过字符串长度
	if (n < 0) {
		return ""; // 如果 n 为负，返回空字符串
	}
	return str.substr(0, std::min(n, static_cast<int>(str.length())));
}

int ImageIdentifyUtilty::hashSimilarity(const std::string& str1, const std::string& str2)
{
	std::unordered_set<char> set1;
	std::unordered_set<char> set2;

	// 将 str1 中的字符添加到 set1
	for (char ch : str1) {
		set1.insert(ch);
	}

	// 将 str2 中的字符添加到 set2
	for (char ch : str2) {
		set2.insert(ch);
	}

	// 计算交集大小
	std::unordered_set<char> intersection;
	for (char ch : set1) {
		if (set2.find(ch) != set2.end()) {
			intersection.insert(ch);
		}
	}

	// 计算并集大小
	std::unordered_set<char> unionSet = set1;
	unionSet.insert(set2.begin(), set2.end());

	// 计算 Jaccard 相似度
	double jaccardIndex = static_cast<double>(intersection.size()) / unionSet.size();

	// 返回相似度的百分比（0-100）
	return static_cast<int>(jaccardIndex * 100);
}

