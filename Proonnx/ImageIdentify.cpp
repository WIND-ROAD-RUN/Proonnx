#include"ImageIdentify.h"

#include <cctype> 
#include <cstring> 
#include <unordered_set>
#include <algorithm>
#include<QPainter>
#include<QDate>
#include <QtConcurrent/QtConcurrent>

#include"qdebug.h"
#include"ProductCheck.h"
#include"ProductConfigLoader.h"
#include"MonitorCamera.h"
#include"DateTransform.h"
#include"ConfigForImageSave.h"



void ImageIdentify::setRecognizeRange(const RecognizeRange& range)
{
	m_recognizeRange->leftLowerCorner = range.leftLowerCorner;
	m_recognizeRange->lowerRightCorner = range.lowerRightCorner;
	m_recognizeRange->topLeftCorner = range.topLeftCorner;
	m_recognizeRange->upperRightCorner = range.upperRightCorner;
}

void ImageIdentify::setRejectAttribute(const RejectAttribute& rejectAttribute)
{
	m_rejectAttribute = new RejectAttribute;
	m_rejectAttribute->DisposalTime=rejectAttribute.DisposalTime;
	m_rejectAttribute->OffsetsNumber = rejectAttribute.OffsetsNumber;
	m_rejectAttribute->RejectDelay = rejectAttribute.RejectDelay;
}

void ImageIdentify::save_caputure_time()
{
	qint64 currentTimeInMs = QDateTime::currentMSecsSinceEpoch();
	if (!is_saveCaputureTime) {
		m_lastCapture_time = currentTimeInMs;
		is_saveCaputureTime = true;
		return;		//Not recording for the first time
	}
	else {
		m_Capture_time_mid = m_rollingAverage->addData(currentTimeInMs - m_lastCapture_time);
		m_lastCapture_time = currentTimeInMs;
	}
	qDebug() << m_Capture_time_mid;
}

void ImageIdentify::setDlgLabelForImage(QLabel* label)
{
	m_dlgLabelForImage = label;
}

void ImageIdentify::setDisaplayCheckInfo(QLabel* label)
{
	m_disaplayCheckInfo = label;
}

void ImageIdentify::setProductCount(int total, int pass, int out)
{
	m_productCount = 0;
	m_productOutCount = 0;
	m_productPassCount = 0;
	m_labelForProductCount->setText(m_stringForProductCount + QString::number(m_productCount));
	m_labelForProductPassCount->setText(m_stringForProductPassCount + QString::number(m_productPassCount));
	m_labelForProductOutCount->setText(m_stringForProductOutCount + QString::number(m_productOutCount));

	ProductCountInfo config;
	config.totalCount = m_productCount;
	config.passCount = m_productPassCount;
	config.outCount = m_productOutCount;

	m_productLoader->storeProductProductInfo(config);
	m_productLoader->saveFile(m_productConfigFilePath);
	
}

void ImageIdentify::iniCamera()
{
	m_productLoader = new ProductConfigLoader();
	m_rollingAverage = new RollingAverage<qint64>(10);

	auto cameraConfig = m_productLoader->loadProductConfig(m_productConfigFilePath);
	setExposureTime(cameraConfig.ExposureTime);
	setGain(cameraConfig.gain);
	setRotateCount(cameraConfig.rotateCount);
	m_recognizeRange = new RecognizeRange();
	m_recognizeRange->topLeftCorner= cameraConfig.topLeftCorner;
	m_recognizeRange->leftLowerCorner = cameraConfig.leftLowerCorner;
	m_recognizeRange->lowerRightCorner = cameraConfig.lowerRightCorner;
	m_recognizeRange->upperRightCorner = cameraConfig.upperRightCorner;
	setRejectAttribute(m_productLoader->loadRejectAttribute(m_productConfigFilePath));

	if (m_labelForProductName) {
		m_labelForProductName->setText(QString::fromStdString(cameraConfig.productName));
	}

	auto productConfig= m_productLoader->loadProductCountInfo(m_productConfigFilePath);

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
	m_configForImageSave = ConfigForImageSave::getInstance();
	m_configForImageSave->createDirectory(QString::fromStdString(cameraConfig.productName));
	m_saveImageWorkPath = m_configForImageSave->getCurrentFilePath() + '/' + QString::fromStdString(cameraConfig.productName);
	m_configForImageSave->createDirectory(m_saveImageWorkPath,"Pass");
	m_configForImageSave->createDirectory(m_saveImageWorkPath, "NG");
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
	delete m_monitorCamera;
	delete m_productCheck;
	delete m_productLoader;
	delete m_recognizeRange;
	delete m_rollingAverage;
	delete m_rejectAttribute;
}

void ImageIdentify::ini_connect()
{
	QObject::connect(m_monitorCamera, SIGNAL(ImgCallBackSignal(unsigned char*, MV_FRAME_OUT_INFO_EX*)),
		this, SLOT(DisplayImage(unsigned char*, MV_FRAME_OUT_INFO_EX*)));
}

void ImageIdentify::setRotateCount(int count)
{
	m_rotateCount = count;
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
	m_exposureTime = exposureTime;
	return m_monitorCamera->setExposureTime(exposureTime);
}

bool ImageIdentify::setGain(int gain)
{
	m_gain = gain;
	return m_monitorCamera->setGain(gain);
}

void ImageIdentify::startMonitor()
{
	m_monitorCamera->startAcquisition();
}

bool ImageIdentify::setHardwareTriggeredAcquisition()
{
	return m_monitorCamera->setHardwareTriggeredAcquisition();
}

bool ImageIdentify::setSoftwareTriggeredAcquisition()
{
	return m_monitorCamera->setSoftwareTriggeredAcquisition();
}


void ImageIdentify::display_image(cv::Mat& mat)
{
	QImage im =
		ImageIdentifyUtilty::convcertImageFromCvMat(mat);

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

void ImageIdentify::display_dlgImage(cv::Mat& mat)
{
	if (m_dlgLabelForImage) {
		//auto recognizeResult = ocr_image(mat);
		QImage im =
			ImageIdentifyUtilty::convcertImageFromCvMat(mat);

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
	ProductCountInfo config;
	config.totalCount = m_productCount;
	config.passCount = m_productPassCount;
	config.outCount = m_productOutCount;
	
	m_productLoader->storeProductProductInfo(config);
	m_productLoader->saveFile(m_productConfigFilePath);
}

void ImageIdentify::render_image(cv::Mat& matForImage, cv::Mat& matForDlg)
{
	display_image(matForImage);
	display_dlgImage(matForDlg);
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

cv::Mat ImageIdentify::rotate_image(const cv::Mat& image, int rotations)
{
	int actualRotations = rotations % 4;

	cv::Mat rotatedImage = image;
	for (int i = 0; i < actualRotations; ++i) {
		cv::rotate(rotatedImage, rotatedImage, cv::ROTATE_90_CLOCKWISE);
	}
	return rotatedImage;
}

void ImageIdentify::save_image(bool productCheckResult, const QImage& image)
{
	if (productCheckResult) {
		auto filePath = m_saveImageWorkPath + QString("/Pass");
		m_configForImageSave->saveImage
		(image, 
			filePath,
			DateTransFormUtilty::removeSymbolsAndSpaces
			(ImageIdentifyUtilty::getCurrentTimeWithMilliseconds())+QString(".jpg"));
	}
	else {
		auto filePath = m_saveImageWorkPath + QString("/NG");
		m_configForImageSave->saveImage
		(image, 
			filePath, 
			DateTransFormUtilty::removeSymbolsAndSpaces
			(ImageIdentifyUtilty::getCurrentTimeWithMilliseconds())+QString(".jpg"));
	}
}

int ImageIdentify::set_IO_start(int time)
{
	return m_monitorCamera->setIOStart(time);
}

void ImageIdentify::send_checkErrorSignal()
{
	QtConcurrent::run([this]() {
		/*int sleepTime =( m_Capture_time_mid * ((double)m_rejectAttribute->RejectDelay / (double)m_rejectAttribute->DisposalTime))* m_rejectAttribute->DisposalTime;
		QThread::msleep(sleepTime);
		set_IO_start(m_rejectAttribute->RejectDelay);*/
		int sleepTime = (m_Capture_time_mid * m_rejectAttribute->OffsetsNumber)+m_rejectAttribute->RejectDelay;
		QThread::msleep(sleepTime);
		set_IO_start((double)m_rejectAttribute->DisposalTime);
		});
	
}

void ImageIdentify::set_recognizeRange()
{

}

void ImageIdentify::DisplayImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo)
{
	// Convert image data to cv:: Mat format
	cv::Mat nativeMat;
	nativeMat= ImageIdentifyUtilty::ConvertMat(pFrameInfo, pData);

	if (nativeMat.empty()) {
		qDebug() << "mat is empty";
		return;
	}

	nativeMat = rotate_image(nativeMat,m_rotateCount);
	cv::Mat matToSave;
	nativeMat.copyTo(matToSave);
	matToSave =
		ImageIdentifyUtilty::cropImage(matToSave,
			m_recognizeRange->topLeftCorner,
			m_recognizeRange->upperRightCorner,
			m_recognizeRange->lowerRightCorner,
			m_recognizeRange->leftLowerCorner);
	cv::Mat matToRecognize;
	matToSave.copyTo(matToRecognize);
	set_recognizeRange();
	auto recognizeResult = ocr_image(matToRecognize);

	
	if (is_check) {
		auto checkResult = m_productCheck->check(recognizeResult, m_standardDate);
		save_caputure_time();
		if (checkResult == ProductCheckUtilty::ProductCheckInfo::WITHIN_THRESHOLD) {
			change_check_state(true);
			update_productInfo_label(true);
			save_image(true, ImageIdentifyUtilty::convcertImageFromCvMat(matToSave));
		}
		else {
			change_check_state(false);
			update_productInfo_label(false);
			save_image(false, ImageIdentifyUtilty::convcertImageFromCvMat(matToSave));
			send_checkErrorSignal();
		}
	}
	else {
		is_saveCaputureTime = false;
	}
	render_image(matToRecognize, nativeMat);
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
	case PixelType_Gvsp_BayerRG8:
		mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
		cv::cvtColor(mat, mat, cv::COLOR_BayerBG2RGB); 
		break;
	default:
		qDebug() << "Unsupported pixel type";
		qDebug() << pFrameInfo->enPixelType;
		return mat;
	}
	return mat;
}

QImage ImageIdentifyUtilty::convcertImageFromCvMat(cv::Mat& mat)
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
		return ""; 
	}

	std::string result = str; 

	
	for (size_t i = 0; i < result.length(); ++i) {
		if (result[i] == oldChar) {
			result[i] = newChar; 
		}
	}

	return result; 
}

std::string ImageIdentifyUtilty::trimToSubstring(std::string str1, const std::string& str2)
{
	
	size_t pos = str1.find(str2);


	if (pos != std::string::npos) {
		return str1.substr(pos); 
	}

	return str1;
}

std::string ImageIdentifyUtilty::getFirstNCharacters(const std::string& str, int n)
{
	if (n < 0) {
		return ""; 
	}
	return str.substr(0, std::min(n, static_cast<int>(str.length())));
}

int ImageIdentifyUtilty::hashSimilarity(const std::string& str1, const std::string& str2)
{
	std::unordered_set<char> set1;
	std::unordered_set<char> set2;


	for (char ch : str1) {
		set1.insert(ch);
	}

	for (char ch : str2) {
		set2.insert(ch);
	}

	std::unordered_set<char> intersection;
	for (char ch : set1) {
		if (set2.find(ch) != set2.end()) {
			intersection.insert(ch);
		}
	}

	std::unordered_set<char> unionSet = set1;
	unionSet.insert(set2.begin(), set2.end());

	double jaccardIndex = static_cast<double>(intersection.size()) / unionSet.size();

	return static_cast<int>(jaccardIndex * 100);
}

QString ImageIdentifyUtilty::getCurrentTimeWithMilliseconds()
{
	QDateTime current = QDateTime::currentDateTime(); 
	return current.toString("yyyy-MM-dd HH:mm:ss.zzz"); 
}

cv::Mat ImageIdentifyUtilty::cropImage(const cv::Mat& image, const std::pair<double, double>& topLeft, const std::pair<double, double>& topRight, const std::pair<double, double>& bottomRight, const std::pair<double, double>& bottomLeft)
{
	int width = image.cols;
	int height = image.rows;

	cv::Point2f tl(topLeft.first * width, topLeft.second * height);
	cv::Point2f tr(topRight.first * width, topRight.second * height);
	cv::Point2f br(bottomRight.first * width, bottomRight.second * height);
	cv::Point2f bl(bottomLeft.first * width, bottomLeft.second * height);

	std::vector<cv::Point2f> points = { tl, tr, br, bl };

	cv::Rect boundingRect = cv::boundingRect(points);

	cv::Mat croppedImage = image(boundingRect);

	return croppedImage;
}

