#ifndef IMAGEIDENTIFYUTILTY_H_
#define IMAGEIDENTIFYUTILTY_H_

#include<string>
#include"opencv/include/opencv2/opencv.hpp"
#include <opencv/include/opencv2/core/core.hpp>
#include <opencv/include/opencv2/highgui/highgui.hpp>
#include<QImage>
#include"MVS/Includes/MvCameraControl.h"
#include"ocrwork.h"

struct ImageIdentifyUtilty
{
	static int
		RGB2BGR
		(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight);

	static cv::Mat
		ConvertMat
		(MV_FRAME_OUT_INFO_EX* pFrameInfo, unsigned char* pData);

	static QImage
		convcertImageFromCvMat
		(const cv::Mat& mat);

	static bool
		checkProduct
		(std::vector<OCRResult>& data, QString& standardDate);

	static bool
		isAlphanumericOrPunct
		(const char* str);

	static std::string
		replaceChar
		(const char* str, char oldChar, char newChar);

	static std::string
		trimToSubstring
		(std::string str1, const std::string& str2);

	static std::string
		getFirstNCharacters
		(const std::string& str, int n);

	static int
		hashSimilarity
		(const std::string& str1, const std::string& str2);

	static QString
		getCurrentTimeWithMilliseconds();

	static cv::Mat cropImage(const cv::Mat& image,
		const std::pair<double, double>& topLeft,
		const std::pair<double, double>& topRight,
		const std::pair<double, double>& bottomRight,
		const std::pair<double, double>& bottomLeft);

};

#endif // !IMAGEIDENTIFYUTILTY_H_
