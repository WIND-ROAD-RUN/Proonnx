#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Proonnx.h"

#include"MVS/Includes/MvCameraControl.h"
#include"ImageIdentify.h"
#include"ClickableLabel.h"
#include<QVector>
#include<QLabel>
#include"IndexButton.h"


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
	QVector<ClickableLabel *>* m_disaplayCameraList{ nullptr };

	QVector<QLabel*>* m_disaplayProductNameList{ nullptr };

	QVector<QLabel*>* m_disaplayCheckInfoList{ nullptr };

	QVector<QLabel*>* m_productCountList{ nullptr };

	QVector<QLabel*>* m_productPassCountList{ nullptr };

	QVector<QLabel*>* m_productOutCountList{ nullptr };

	QVector<ImageIdentify*> * m_cameraList{nullptr};
public:
	QVector<IndexButton*>* m_setIsCheckPbtnList{ nullptr };

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
private:
	void select_config(int index);

private slots:
	void pbt_addProductCongfig_clicked();
	void pbt_addProductCongfig(int index);

	void pbt_modProductConfig_clicked();
	void pbt_modProductConfig(int index);

	void pbtn_setProonnx_clicked();

	void pbt_setIsCheckProduct_clicked();
	void pbt_setIsCheckProduct(int index);
	void setCheckProduct_clicked(bool check);

	void pbtn_clearCount_clicked();
	void pbtn_clearCount(int index);

	void pbtn_quit_clicked();

	void clicked_label_clicked(int index);

};
