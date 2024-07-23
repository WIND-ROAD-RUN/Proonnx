#pragma once

#include <QDialog>
#include "ui_DlgChangeProductConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgChangeProductConfigClass; };
QT_END_NAMESPACE

class  ProductConfigLoader;
struct RecognizeRange;
class FrameSelectLabel;
class ImageIdentify;

class DlgChangeProductConfig : public QDialog
{
	Q_OBJECT
private:
	QString m_filePath{};

	ProductConfigLoader* m_loader;

	RecognizeRange* m_recognizeRange{nullptr};

	int m_rotateCount{ 0 };

private:
	FrameSelectLabel* m_frameSelectLabel{nullptr};

private:
	int m_cameraIndex{ 1 };

	ImageIdentify* m_camera{ nullptr };

public:
	void setCameraIndex(int index) { m_cameraIndex = index; }

	void setCamera(ImageIdentify* camera);

public:
	explicit DlgChangeProductConfig(QWidget *parent = nullptr, int cameraIndex = 1);

	~DlgChangeProductConfig();

private:
	void ini_ui();

	void ini_connect();

	void ini_configLoader();

public:
	void setFilePath(const QString& filePath) { m_filePath = filePath; }

	void iniUI();

private:
	Ui::DlgChangeProductConfigClass *ui;

private slots:
	void selectionMade_complete(const QRect& rect);

	void pbt_saveProductConfig_clicked();

	void pbtn_spinImage_clicked();

	void pbtn_drawRecognitionRange_clicked();
};
