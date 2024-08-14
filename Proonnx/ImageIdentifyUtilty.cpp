#include"ImageIdentifyUtilty.h"

#include<unordered_set>
#include<QDate>

int ImageIdentifyUtilty::RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight)
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

QImage ImageIdentifyUtilty::convcertImageFromCvMat(const cv::Mat& mat)
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
	bool result{ false };
	for (const auto& item : data) {
		if (!ImageIdentifyUtilty::isAlphanumericOrPunct(item.text)) {
			continue;
		}
		auto replaceDate = ImageIdentifyUtilty::replaceChar(item.text, 'Q', '0');
		replaceDate = trimToSubstring(replaceDate, ImageIdentifyUtilty::getFirstNCharacters(standardDate.toStdString(), 3));
		result = ImageIdentifyUtilty::hashSimilarity(replaceDate, standardDate.toStdString()) >= 90;
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
