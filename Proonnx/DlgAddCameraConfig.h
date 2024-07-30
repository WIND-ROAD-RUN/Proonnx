#pragma once

#include <QDialog>
#include "ui_DlgAddCameraConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgAddCameraConfigClass; };
QT_END_NAMESPACE

class DlgAddCameraConfig : public QDialog
{
	Q_OBJECT

public:
	DlgAddCameraConfig(QWidget *parent = nullptr);
	~DlgAddCameraConfig();

private:
	Ui::DlgAddCameraConfigClass *ui;
};
