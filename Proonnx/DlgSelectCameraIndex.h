#pragma once

#include <QDialog>
#include "ui_DlgSelectCameraIndex.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgSelectCameraIndexClass; };
QT_END_NAMESPACE

class DlgSelectCameraIndex : public QDialog
{
	Q_OBJECT

public:
	explicit DlgSelectCameraIndex(QWidget *parent = nullptr,int cameraCount=1);

	~DlgSelectCameraIndex();

private:
	int m_cameraCount{0};

private:
	QString m_configBeforeRuntime{};

public:
	void setConfigBeforeRuntime(const QString & filePath);

	void setWindowSize(int wide, int height);

private:
	void ini_ui();

	void ini_localizationStringLoaderUI();

	void ini_connect();

private:
	Ui::DlgSelectCameraIndexClass *ui;

public:
	int m_indexIndex{ 1 };

private slots:
	void pbtn_ok_clicked();

	void pbtn_cancel();

};
