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
	Ui::DlgAddProductConfigClass *ui;
};
