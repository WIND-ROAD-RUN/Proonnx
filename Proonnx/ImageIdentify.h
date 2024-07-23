#ifndef IMAGEIDENTIFY_H_
#define IMAGEIDENTIFY_H_

#include"opencv/include/opencv2/opencv.hpp"
#include <opencv/include/opencv2/core/core.hpp>
#include <opencv/include/opencv2/highgui/highgui.hpp>
#include"ocrwork.h"
#include"MVS/Includes/MvCameraControl.h"
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

	MonitorCamera* m_monitorCamera{nullptr};


	ocrwork* m_indentModel{nullptr};


private:
	QLabel* m_labelForImage{ nullptr };

	QLabel* m_dlgLabelForImage{nullptr};
	
	QLabel* m_disaplayCheckInfo{nullptr};
public:
	void setDlgLabelForImage(QLabel * label);

	void deleteDlgLabelForImage() { m_dlgLabelForImage = nullptr; }

	void setDisaplayCheckInfo(QLabel* label);

private:
	ProductCheck<std::vector<OCRResult>, QString> * m_productCheck;

public:
	QString m_standDate;

	std::string m_Ip;

public:
	explicit ImageIdentify(QLabel * label,const std::string & ip);

	~ImageIdentify();
private:
	void ini_connect();

public:
	bool InitCamera();

	void IniOcr();

	void startAcquisition();

	void stopAcquisition();

	bool setExposureTime(int exposureTime);

	bool setGain(int gain);

	void startMonitor();

public:

	void iniProductCheck
	(int m_kConsecutiveErrorThreshold,
		ProductCheck<std::vector<OCRResult>, QString>::Compare compare);

private:
	void display_image(cv::Mat& image);
	void render_image(cv::Mat& image);
	std::vector<OCRResult> ocr_image(cv::Mat srcMat);
	void change_check_state(bool check);
private slots:
	void DisplayImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInf);


};


#endif // !IMAGEIDENTIFY_H_
