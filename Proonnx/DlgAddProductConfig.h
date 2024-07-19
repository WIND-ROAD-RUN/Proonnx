#pragma once

#include <QDialog>
#include "ui_DlgAddProductConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgAddProductConfigClass; };
QT_END_NAMESPACE

class DlgAddProductConfig : public QDialog
{
	Q_OBJECT

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
};
