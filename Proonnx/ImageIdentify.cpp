#include"ImageIdentify.h"
#include"MonitorCamera.h"

#include<QPainter>
#include"qdebug.h"
void ImageIdentify::setDlgLabelForImage(QLabel* label)
{
	m_dlgLabelForImage = label;
}

void ImageIdentify::setDisaplayCheckInfo(QLabel* label)
{
	m_disaplayCheckInfo = label;
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
}

void ImageIdentify::ini_connect()
{
	QObject::connect(m_monitorCamera, SIGNAL(ImgCallBackSignal(unsigned char*, MV_FRAME_OUT_INFO_EX*)),
		this, SLOT(DisplayImage(unsigned char*, MV_FRAME_OUT_INFO_EX*)));
}

bool ImageIdentify::InitCamera()
{
	m_monitorCamera->setIp(m_Ip);
	return m_monitorCamera->connectCamera();
}

void ImageIdentify::IniOcr()
{
	m_indentModel->initial();
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

void ImageIdentify::iniProductCheck(int m_kConsecutiveErrorThreshold, ProductCheck<std::vector<OCRResult>, QString>::Compare compare)
{
	m_productCheck = new ProductCheck<std::vector<OCRResult>, QString>(m_kConsecutiveErrorThreshold, compare);
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
	bool checkResult{false};
	change_check_state(checkResult);
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

