#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Proonnx.h"

#include"MVS/Includes/MvCameraControl.h"
#include"ImageIdentify.h"
#include<QVector>
#include<QLabel>


namespace cv {
	class Mat;
}

class ImageIdentify;
class ocrwork;
class LocalizationStringLoaderXML;
class ConfigBeforeRuntimeLoader;
class ConfigForImageSave;

QT_BEGIN_NAMESPACE
namespace Ui { class ProonnxClass; };
QT_END_NAMESPACE

class Proonnx : public QMainWindow
{
	Q_OBJECT
private:
	LocalizationStringLoaderXML* m_locStrLoader{ nullptr };

	ConfigBeforeRuntimeLoader* m_configBeforeRuntimeLoader{nullptr};

	ConfigForImageSave* m_configForImageSave{ nullptr };

private:
	QVector<QLabel *>* m_disaplayCameraList{ nullptr };

	QVector<QLabel*>* m_disaplayProductNameList{ nullptr };

	QVector<QLabel*>* m_disaplayCheckInfoList{ nullptr };

	QVector<QLabel*>* m_productCountList{ nullptr };

	QVector<QLabel*>* m_productPassCountList{ nullptr };

	QVector<QLabel*>* m_productOutCountList{ nullptr };

	QVector<ImageIdentify*> * m_cameraList{nullptr};

private:
	QString m_configBeforeRuntimeLoaderFilePath{};

private:
	void ini_ui();

	void ini_localizationStringLoader();

	void ini_localizationStringLoaderUI();

private:
	void ini_configBeforeRuntimeLoader();

	void ini_configBeforeRuntime();

	void ini_configForImageSave();
private:
	void ini_gBox_monitoringDisplay();

	void ini_cameraList();

private:
	void ini_connect();

	void des_com();
private:
	void set_isCheckProduct(bool is);
private:
	bool checkCurrentIsAllFalse(const QVector<bool>& list);
public:
	Proonnx(QWidget* parent = nullptr);

	~Proonnx();


private:
	Ui::ProonnxClass* ui;
private:
	bool m_isSingleCheckProduct{false};
private:
	QVector<bool> get_isCheckProductList();

	void set_isCheckProductByList(const QVector<bool> & list);

	void set_allDoNotCheck();

private slots:
	void pbt_addProductCongfig_clicked();

	void pbt_modProductConfig_clicked();

	void pbtn_setProonnx_clicked();

	void pbt_setIsCheckProduct_clicked();

	void pbtn_clearCount_clicked();

	void pbtn_quit_clicked();
};
