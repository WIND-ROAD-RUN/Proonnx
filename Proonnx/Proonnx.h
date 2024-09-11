#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Proonnx.h"

#include<QVector>
#include<QLabel>

namespace cv {
	class Mat;
}

class ImageIdentify;
class ocrwork;
class ConfigForImageSave;
namespace rw {
	namespace oulq {
		class LabelClickable;
	}
	namespace cfgl {
		class LocalizationStringLoaderXML;
	}
	namespace cfgr {
		class ConfigBeforeRuntimeLoader;
	}

}
class IndexButton;

QT_BEGIN_NAMESPACE
namespace Ui { class ProonnxClass; };
QT_END_NAMESPACE

using namespace rw::oulq;
using namespace rw::cfgl;
using namespace rw::cfgr;

class Proonnx : public QMainWindow
{
	Q_OBJECT
private:
	LocalizationStringLoaderXML* m_locStrLoader{ nullptr };

	ConfigBeforeRuntimeLoader* m_configBeforeRuntimeLoader{nullptr};

	ConfigForImageSave* m_configForImageSaveLoader{ nullptr };

private:
	QVector<LabelClickable *>* m_labelDisaplayCameraList{ nullptr };
	QVector<bool> m_labelDisaplayCameraListHasCamera;

	QVector<QLabel*>* m_labelDisaplayProductNameList{ nullptr };

	QVector<QLabel*>* m_labelDisaplayCheckInfoList{ nullptr };

	QVector<QLabel*>* m_labelProductCountList{ nullptr };

	QVector<QLabel*>* m_labelProductPassCountList{ nullptr };

	QVector<QLabel*>* m_labelProductOutCountList{ nullptr };

	QVector<ImageIdentify*> * m_imageIdentifyList{nullptr};

public:
	QVector<IndexButton*>* m_pbtnSetIsCheckList{ nullptr };

private:
	QString m_filePathConfigBeforeRuntimeLoader{};

private:
	void ini_ui();

	void ini_localizationStringLoader();

	void ini_localizationStringUI();

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

private:
	void pbt_addProductCongfig(int index);

	void pbt_modProductConfig(int index);

	void setCheckProduct_clicked(bool check);

	void pbtn_clearCount(int index);

private slots:
	void pbtn_setProonnx_clicked();

	void pbt_setIsCheckProduct_clicked();

	void pbtn_quit_clicked();

	void pbtn_testDlg_clicked();

	void clicked_label_clicked(int index);

	void pbt_setIsCheckProduct(int index);
};
