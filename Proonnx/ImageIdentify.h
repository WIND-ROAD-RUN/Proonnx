#ifndef IMAGEIDENTIFY_H_
#define IMAGEIDENTIFY_H_

#include"opencv/include/opencv2/opencv.hpp"
#include <opencv/include/opencv2/core/core.hpp>
#include <opencv/include/opencv2/highgui/highgui.hpp>
#include"ocrwork.h"
#include"MVS/Includes/MvCameraControl.h"
#include"mycamera.h"
#include"qdebug.h"
#include<QAction>
#include<QLabel>
#include"ProductCheck.h"

struct ImageIdentifyUtilty
{
	static int 
		RGB2BGR
		(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight);

	static
		cv::Mat  ConvertMat
		(MV_FRAME_OUT_INFO_EX* pFrameInfo, unsigned char* pData);

	static
		QImage convcertImageFormCvMat
		(cv::Mat& mat);
};

class MonitorCamera;

class ImageIdentify
	:public QAction {
	Q_OBJECT
private:
	MyCamera* m_camera{nullptr};

	MonitorCamera* m_monitorCamera{nullptr};


	ocrwork* m_indentModel{nullptr};


private:
	QLabel* m_labelForImage{ nullptr };

	QLabel* m_dlgLabelForImage{nullptr};

public:
	void setDlgLabelForImage(QLabel * label);

	void deleteDlgLabelForImage() { m_dlgLabelForImage = nullptr; }

private:
	ProductCheck<std::vector<OCRResult>, QString> * m_productCheck;

private:
	QString m_standDate;

	std::string m_Ip;

public:
	explicit ImageIdentify(QLabel * label,const std::string & ip);

	~ImageIdentify();
private:
	void ini_connect();

public:
	bool InitCamera();

	void startMonitor();


public:
	bool iniCom(int index);

	void iniProductCheck
	(int m_kConsecutiveErrorThreshold,
		ProductCheck<std::vector<OCRResult>, QString>::Compare compare);

private slots:
	void DisplayImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInf);

};


#endif // !IMAGEIDENTIFY_H_
