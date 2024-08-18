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
	m_rejectAttribute->DisposalTime = rejectAttribute.DisposalTime;
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
	m_recognizeRange->topLeftCorner = cameraConfig.topLeftCorner;
	m_recognizeRange->leftLowerCorner = cameraConfig.leftLowerCorner;
	m_recognizeRange->lowerRightCorner = cameraConfig.lowerRightCorner;
	m_recognizeRange->upperRightCorner = cameraConfig.upperRightCorner;
	setRejectAttribute(m_productLoader->loadRejectAttribute(m_productConfigFilePath));

	if (m_labelForProductName) {
		m_labelForProductName->setText(QString::fromStdString(cameraConfig.productName));
	}

	auto productConfig = m_productLoader->loadProductCountInfo(m_productConfigFilePath);

	if (m_labelForProductCount) {
		m_stringForProductCount = m_labelForProductCount->text();
		m_labelForProductCount->setText(m_stringForProductCount + QString::number(productConfig.totalCount));
		m_productCount = productConfig.totalCount;
	}
	if (m_labelForProductOutCount) {
		m_stringForProductOutCount = m_labelForProductOutCount->text();
		m_labelForProductOutCount->setText(m_stringForProductOutCount + QString::number(productConfig.outCount));
		m_productOutCount = productConfig.outCount;
	}
	if (m_labelForProductPassCount) {
		m_stringForProductPassCount = m_labelForProductPassCount->text();
		m_labelForProductPassCount->setText(m_stringForProductPassCount + QString::number(productConfig.passCount));
		m_productPassCount = productConfig.passCount;
	}
	m_configForImageSaveLoader = ConfigForImageSave::getInstance();
	m_configForImageSaveLoader->createDirectory(QString::fromStdString(cameraConfig.productName));
	m_saveImageWorkPath = m_configForImageSaveLoader->getCurrentFilePath() + '/' + QString::fromStdString(cameraConfig.productName);
	m_configForImageSaveLoader->createDirectory(m_saveImageWorkPath, "Pass");
	m_configForImageSaveLoader->createDirectory(m_saveImageWorkPath + "/Pass", "Native");
	m_configForImageSaveLoader->createDirectory(m_saveImageWorkPath + "/Pass", "Crop");

	m_configForImageSaveLoader->createDirectory(m_saveImageWorkPath, "NG");
	m_configForImageSaveLoader->createDirectory(m_saveImageWorkPath + "/NG", "Native");
	m_configForImageSaveLoader->createDirectory(m_saveImageWorkPath + "/NG", "Crop");
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


void ImageIdentify::display_image(const cv::Mat& mat)
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

	pixmap = pixmap.scaled(m_labelForImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	m_labelForImage->setPixmap(pixmap);
	m_labelForImage->setScaledContents(true); // Fill the image with QLabel

	if (m_dlgLabelForImage) {
		m_dlgLabelForImage->setPixmap(pixmap);
		m_dlgLabelForImage->setScaledContents(true); // Fill the image with QLabel
	}
}

void ImageIdentify::display_image(const cv::Mat& mat, QLabel* label)
{
	if (!label) {
		return;
	}
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
	label->setPixmap(pixmap);
	label->setScaledContents(true); // Fill the image with QLabel
}

void ImageIdentify::display_dlgImage(const cv::Mat& mat)
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

void ImageIdentify::render_image(const cv::Mat & matForImage, const cv::Mat & matForDlg)
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
	return false;
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

void ImageIdentify::save_image(bool productCheckResult, const QImage& image, bool isCrop)
{
	if (productCheckResult) {
		if (isCrop) {
			//ÇÐ¸îÂß¼­
			/*auto filePath = m_saveImageWorkPath + QString("/Pass/Crop");
			m_configForImageSaveLoader->saveImage
			(image,
				filePath,
				DateTransFormUtilty::removeSymbolsAndSpaces
				(ImageIdentifyUtilty::getCurrentTimeWithMilliseconds()) + QString(".jpg"));*/
		}
		else {
			auto filePath = m_saveImageWorkPath + QString("/Pass/Native");
			m_configForImageSaveLoader->saveImage
			(image,
				filePath,
				DateTransFormUtilty::removeSymbolsAndSpaces
				(ImageIdentifyUtilty::getCurrentTimeWithMilliseconds()) + QString(".jpg"));
		}

	}
	else {
		if (isCrop) {
			auto filePath = m_saveImageWorkPath + QString("/NG/Crop");
			m_configForImageSaveLoader->saveImage
			(image,
				filePath,
				DateTransFormUtilty::removeSymbolsAndSpaces
				(ImageIdentifyUtilty::getCurrentTimeWithMilliseconds()) + QString(".jpg"));
		}
		else {
			auto filePath = m_saveImageWorkPath + QString("/NG/Native");
			m_configForImageSaveLoader->saveImage
			(image,
				filePath,
				DateTransFormUtilty::removeSymbolsAndSpaces
				(ImageIdentifyUtilty::getCurrentTimeWithMilliseconds()) + QString(".jpg"));
		}
	}
}

int ImageIdentify::set_IO_start(int time)
{
	return m_monitorCamera->setIOStart(time);
}

void ImageIdentify::send_checkErrorSignal()
{
	QtConcurrent::run([this]() {
		int sleepTime = (m_Capture_time_mid * m_rejectAttribute->RejectDelay*0.01)*m_rejectAttribute->OffsetsNumber;
		QThread::msleep(sleepTime);
		set_IO_start((double)m_rejectAttribute->DisposalTime);
		});

}

void ImageIdentify::set_recognizeRange()
{

}

void ImageIdentify::DisplayImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo)
{
	QtConcurrent::run([=]() {
	save_caputure_time();
	cv::Mat nativeMat;
	nativeMat = ImageIdentifyUtilty::ConvertMat(pFrameInfo, pData);

	if (nativeMat.empty()) {
		qDebug() << "mat is empty";
		return;
	}

	nativeMat = rotate_image(nativeMat, m_rotateCount);
	cv::Mat matToSave;
	nativeMat.copyTo(matToSave);
	/*matToSave =
		ImageIdentifyUtilty::cropImage(matToSave,
			m_recognizeRange->topLeftCorner,
			m_recognizeRange->upperRightCorner,
			m_recognizeRange->lowerRightCorner,
			m_recognizeRange->leftLowerCorner);*/
	cv::Mat matToRecognize;
	matToSave.copyTo(matToRecognize);
	set_recognizeRange();
	//auto recognizeResult = ocr_image(matToRecognize);
	if (is_check) {
			//ÅÐ¶ÏÊ¶±ðÂß¼­
				auto recognizeResult = ocr_image(matToRecognize);/////////
				auto checkResult = m_productCheck->check(recognizeResult, m_standardDate);
				if (checkResult == ProductCheckUtilty::ProductCheckInfo::WITHIN_THRESHOLD) {
					change_check_state(true);
					update_productInfo_label(true);
					save_image(true, ImageIdentifyUtilty::convcertImageFromCvMat(nativeMat), false);
					save_image(true, ImageIdentifyUtilty::convcertImageFromCvMat(matToSave), true);
				}
				else {
					change_check_state(false);
					update_productInfo_label(false);
					save_image(false, ImageIdentifyUtilty::convcertImageFromCvMat(nativeMat), false);
					save_image(true, ImageIdentifyUtilty::convcertImageFromCvMat(matToSave), true);
					//ÉèÖÃio´¥·¢
					//send_checkErrorSignal();
					display_image(matToRecognize,m_labelForNg);
				}

				QMetaObject::invokeMethod(qApp, [this, matToRecognize, nativeMat]
					{

						render_image(matToRecognize, nativeMat);





					});
		
	}
	else {
		is_saveCaputureTime = false;
		QMetaObject::invokeMethod(qApp, [this, matToRecognize, nativeMat]
			{

				render_image(matToRecognize, nativeMat);





			});


	}


	//debug
	/*static int exposureTime= 50000;
	exposureTime += 5000;
	if (exposureTime>85000) {
		exposureTime = 50000;
	}
	m_monitorCamera->setExposureTime(exposureTime);*/
		});
}

