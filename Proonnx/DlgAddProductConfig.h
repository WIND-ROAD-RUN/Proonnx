#pragma once

#include <QDialog>
#include "ui_DlgAddProductConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgAddProductConfigClass; };
QT_END_NAMESPACE

struct RecognizeRange;
class FrameSelectLabel;

class DlgAddProductConfig : public QDialog
{
	Q_OBJECT
private:
	int m_rotateCount{ 0 };
	RecognizeRange * m_recognizeRange{nullptr};
private:
	FrameSelectLabel* m_frameSelectLabel;
public:
	DlgAddProductConfig(QWidget *parent = nullptr);

	~DlgAddProductConfig();

private:
	void ini_ui();

	void ini_connect();
private:
	Ui::DlgAddProductConfigClass *ui;

private slots:
	void pbt_saveProductConfig_clicked();

	void pbtn_spinImage_clicked();

	void pbtn_drawRecognitionRange_clicked();

	void selectionMade_complete(const QRect& rect);
};
