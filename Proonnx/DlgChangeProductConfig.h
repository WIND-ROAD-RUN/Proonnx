﻿#pragma once

#include <QDialog>
#include "ui_DlgChangeProductConfig.h"

#include"oso/osop_OcrDateProductConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgChangeProductConfigClass; };
QT_END_NAMESPACE

class  ProductConfigLoader;
struct RecognizeRange;
class ImageIdentify;
namespace rw {
	namespace oulq {
		class LabelFrameSelectable;
	}
}

using namespace rw::oulq;

class RuntimeInfo;
class DlgChangeProductConfig : public QDialog
{
	Q_OBJECT
private:
    RuntimeInfo* m_runtimeInfo{ nullptr };

	rw::oso::OcrDataProductConfig _ocrDataProductCfg;
public:
	//记得移除
    std::string _ip{};
public:
    void setRuntimeInfo(RuntimeInfo* runtimeInfo) { m_runtimeInfo = runtimeInfo; }
private:
	QString m_filePath{};

	ProductConfigLoader* m_loader;

	RecognizeRange* m_recognizeRange{nullptr};

	int m_rotateCount{ 0 };
private:
	QString m_configBeforeRuntime{};
public:
	void setConfigBeforeRuntime(const QString& filePath);

private:
	LabelFrameSelectable* m_frameSelectLabel{nullptr};

private:
	int m_cameraIndex{ 1 };

	ImageIdentify* m_camera{ nullptr };

public:
	void setCameraIndex(int index) { m_cameraIndex = index; }

	void setCamera(ImageIdentify* camera);

	QString getProductName() { return ui->lEdit_productName->text(); }

public:
	explicit DlgChangeProductConfig(QWidget *parent = nullptr, int cameraIndex = 1);

	~DlgChangeProductConfig();

private:
	void ini_ui();
	void ini_localizationStringUI();
	void ini_connect();

public:
	void setFilePath(const QString& filePath) { m_filePath = filePath; }

	void readRuntimeInfo();

public:
	void setWindowSize(int wide, int height);

private:
	Ui::DlgChangeProductConfigClass *ui;

private:
	void closeEvent(QCloseEvent* event) override;

private slots:
	void selectionMade_complete(const QRect& rect);

	void pbt_saveProductConfig_clicked();

	void pbtn_saveProductConfig_clicked_refactor();

	void pbtn_spinImage_clicked();

	void pbtn_drawRecognitionRange_clicked();

	void sBox_exposureTime_value_change(int);

	void sBox_gain_value_change(int);

	void pbtn_updateCamera_clicked();
};
