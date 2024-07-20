#pragma once

#include <QDialog>
#include "ui_DlgAddCamera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgAddCameraClass; };
QT_END_NAMESPACE

class DlgAddCamera : public QDialog
{
	Q_OBJECT

public:
	DlgAddCamera(QWidget *parent = nullptr);

	~DlgAddCamera();

private:
	Ui::DlgAddCameraClass *ui;
};
