#pragma once

#include <QDialog>
#include "ui_DlgAddProductConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgAddProductConfigClass; };
QT_END_NAMESPACE

struct RecognizeRange;
class FrameSelectLabel;
class ImageIdentify;

class DlgAddProductConfig : public QDialog
{
	Q_OBJECT
private:
	int m_rotateCount{ 0 };

	RecognizeRange * m_recognizeRange{nullptr};

private:
	FrameSelectLabel* m_frameSelectLabel;
private:
	QString m_configBeforeRuntime;
private:
	int m_cameraIndex{1};

	ImageIdentify* m_camera{nullptr};

public:
	explicit DlgAddProductConfig(QWidget* parent = nullptr);

	~DlgAddProductConfig();

public:
	void setCameraIndex(int index) { m_cameraIndex = index; }

	void setCamera(ImageIdentify* camera);

	void setConfigBeforeRuntime(const QString & filePath);

	QString getProductName() { return ui->lEdit_productName->text(); }

private:
	void ini_ui();
	void ini_localizationStringLoaderUI();
	void ini_connect();

private:
	Ui::DlgAddProductConfigClass *ui;

private slots:
	void pbt_saveProductConfig_clicked();

	void pbtn_spinImage_clicked();

	void pbtn_drawRecognitionRange_clicked();

	void selectionMade_complete(const QRect& rect);

	void sBox_exposureTime_value_change(int);

	void sBox_gain_value_change(int);
};
