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
#include"RollingAverage.h"

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
		(const cv::Mat & mat);

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



class MonitorCamera;
class ProductConfigLoader;
class ConfigForImageSave;
struct RecognizeRange;
struct RejectAttribute;

class ImageIdentify
	:public QAction {
	Q_OBJECT
private:
	MonitorCamera* m_monitorCamera{nullptr};

	ocrwork* m_indentModel{nullptr};

private:
	qint64  m_lastCapture_time{0};

	qint64 m_Capture_time_mid{200};

private:
	RecognizeRange * m_recognizeRange{ nullptr };

	RejectAttribute* m_rejectAttribute{ nullptr };
public:
	void setRecognizeRange(const RecognizeRange & range);

	void setRejectAttribute(const RejectAttribute & rejectAttribute);

private:
	void save_caputure_time();
private:
	bool is_saveCaputureTime{ false };

public:
	bool m_setIsCheck{false};

private:
	QLabel* m_labelForImage{ nullptr };

	QLabel* m_dlgLabelForImage{nullptr};
	
	QLabel* m_disaplayCheckInfo{nullptr};

public:
	void setDlgLabelForImage(QLabel * label);

	void deleteDlgLabelForImage() { m_dlgLabelForImage = nullptr; }

	void setDisaplayCheckInfo(QLabel* label);

public:
	QLabel* m_labelForProductCount{ nullptr };
	QString m_stringForProductCount{};
	int m_productCount{0};

	QLabel* m_labelForProductPassCount{ nullptr };
	QString m_stringForProductPassCount{};
	int m_productPassCount{ 0 };

	QLabel* m_labelForProductOutCount{ nullptr };
	QString m_stringForProductOutCount{};
	int m_productOutCount{ 0 };

	QLabel* m_labelForProductName{ nullptr };
public:
	QLabel* m_labelForNg{ nullptr };

public:
	void setProductCount(int total,int pass,int out);

private:
	ProductConfigLoader* m_productLoader{nullptr};

	ConfigForImageSave* m_configForImageSave{nullptr};

private:
	QString m_saveImageWorkPath{};

private:
	bool is_check{false};

public:
	void setIsCheckProduct(bool is) { is_check = is; }
 
	bool getIsCheckProduct() { return is_check; }

public:
	void iniCamera();

public:
	std::string m_productConfigFilePath{};

private:
	ProductCheck<std::vector<OCRResult>, QString>* m_productCheck{ nullptr };

	RollingAverage<qint64>* m_rollingAverage{ nullptr };

	QString m_standardDate;

public:
	void setStandDate(const QString & standardDate);

public:
	QString m_standDate;

	std::string m_Ip;

public:
	explicit ImageIdentify(QLabel * label,const std::string & ip);

	~ImageIdentify();

private:
	void ini_connect();

private:
	int m_rotateCount{0};

	int m_exposureTime{0};

	int m_gain{0};

public:
	int getExposureTime() { return m_exposureTime; }

	int getGain() { return m_gain; }

	int getRotateCont() { return m_rotateCount; }

public:
	void setRotateCount(int count);

public:
	bool connectCamera();

	void IniOcr();

	void startAcquisition();

	void stopAcquisition();

	bool setExposureTime(int exposureTime);

	bool setGain(int gain);

	void startMonitor();

	bool setHardwareTriggeredAcquisition();

	bool setSoftwareTriggeredAcquisition();

private:
	void display_image(cv::Mat& mat);

	void display_image(const cv::Mat& mat,QLabel * label);

	void display_dlgImage(cv::Mat& mat);

	void update_productInfo_label(bool check);

	void render_image(cv::Mat& matForImage,cv::Mat & matForDlg);

	std::vector<OCRResult> ocr_image(cv::Mat srcMat);

	bool check_productDate(const std::vector<OCRResult> & date);

	void change_check_state(bool check);

	cv::Mat rotate_image(const cv::Mat& image, int rotations);

	void save_image(bool productCheckResult,const QImage & image, bool isCrop);


	int set_IO_start(int time);

	void send_checkErrorSignal();

	void set_recognizeRange();

private slots:
	void DisplayImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInf);


};
#endif // !IMAGEIDENTIFY_H_
