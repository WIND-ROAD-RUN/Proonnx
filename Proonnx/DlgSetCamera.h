#pragma once

#include <QDialog>
#include "ui_DlgSetCamera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgSetCameraClass; };
QT_END_NAMESPACE

class DlgSetCamera : public QDialog
{
	Q_OBJECT

public:
	DlgSetCamera(QWidget *parent = nullptr);
	~DlgSetCamera();

private:
	Ui::DlgSetCameraClass* ui;
private:
	void ini_connect();
public:
	int m_select{0};
private slots:
	void pbt_addProduct_clicked();
	void pbt_changeProduct_clicked();
	void pbt_selectProduct_clicked();
	void pbt_clear_clicked();
};
