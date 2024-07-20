#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Proonnx.h"

#include"opencv/include/opencv2/opencv.hpp"
#include <opencv/include/opencv2/core/core.hpp>
#include <opencv/include/opencv2/highgui/highgui.hpp>
#include"ocrwork.h"
#include"MVS/Includes/MvCameraControl.h"
#include"mycamera.h"
#include"qdebug.h"

#include"LocalizationStringLoader-XML.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProonnxClass; };
QT_END_NAMESPACE

class Proonnx : public QMainWindow
{
	Q_OBJECT
private:
	LocalizationStringLoaderXML* m_locStrLoader{ nullptr };
private:
	void ini_ui();

	void ini_localizationStringLoader();

	void ini_localizationStringLoaderUI();

	void ini_connect();

	void des_com();

	//实例化一台相机
	MyCamera myCamera1;
	//实例化一个推理模型
	ocrwork o;
public:
	Proonnx(QWidget* parent = nullptr);

	~Proonnx();

private:
	Ui::ProonnxClass* ui;

private slots:
	void cBox_changeLanguage_index_change_on(int index);

	void pbt_addProductCongfig_clicked();

	void pbt_addCamera_clicked();

	void pbt_modCameraConfig_clicked();

	void pbt_modProductConfig_clicked();

	//相机回调函数
	void DispImgBuff1(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInf);


private:
	//初始化相机
	bool InitCamera(int index);

	int RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight);//RGB转BGR

	cv::Mat  Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData, bool& isok);//转Mat格式
	QImage cvMat2QImage(cv::Mat& mat);

};
