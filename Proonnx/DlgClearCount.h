#pragma once

#include <QDialog>
#include "ui_DlgClearCount.h"

#include<QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class DlgClearCountClass; };
QT_END_NAMESPACE

class DlgClearCount : public QDialog
{
	Q_OBJECT

public:
	DlgClearCount(QWidget *parent = nullptr);
	~DlgClearCount();
private:
	void ini_ui();

	void ini_localizationStringLoaderUI();

	void ini_connect();

public:
	void iniGBox_cameraList(int cameraCount);

	QVector<bool> getIsCheckList();

private:
	Ui::DlgClearCountClass *ui;
	
private slots:
	void pbtn_ok_clicked();

	void pbtn_cancel_clicked();
};
