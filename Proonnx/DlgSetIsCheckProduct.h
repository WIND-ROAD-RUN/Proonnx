#pragma once

#include <QDialog>
#include "ui_DlgSetIsCheckProduct.h"

#include<QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class DlgSetIsCheckProductClass; };
QT_END_NAMESPACE

class DlgSetIsCheckProduct : public QDialog
{
	Q_OBJECT

public:
	DlgSetIsCheckProduct(QWidget *parent = nullptr);

	~DlgSetIsCheckProduct();

private:
	void ini_ui();

	void ini_localizationStringLoaderUI();

	void ini_connect();

private:
	QVector<bool> m_isCheckedlist;

public:
	void iniGBox_cameraList(const QVector<bool> & list);

	QVector<bool> getCurrentIsCheckList();

	QVector<bool> getNativeIsCheckList() { return m_isCheckedlist; }

	void setWindowSize(int wide, int height);

private:
	Ui::DlgSetIsCheckProductClass *ui;

private slots:
	void pbtn_ok_clicked();

	void pbtn_cancel_clicked();

};
