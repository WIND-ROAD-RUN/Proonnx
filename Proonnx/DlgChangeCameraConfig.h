#pragma once

#include <QDialog>
#include "ui_DlgChangeCameraConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgChangeCameraConfigClass; };
QT_END_NAMESPACE

class DlgChangeCameraConfig : public QDialog
{
	Q_OBJECT

public:
	DlgChangeCameraConfig(QWidget *parent = nullptr);

	~DlgChangeCameraConfig();

private:
	Ui::DlgChangeCameraConfigClass *ui;

};
