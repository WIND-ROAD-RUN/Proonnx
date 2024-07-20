#pragma once

#include <QDialog>
#include "ui_DlgChangeProductConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgChangeProductConfigClass; };
QT_END_NAMESPACE

class DlgChangeProductConfig : public QDialog
{
	Q_OBJECT
private:
	QString m_filePath{};

public:
	DlgChangeProductConfig(QWidget *parent = nullptr);

	~DlgChangeProductConfig();

private:
	void ini_ui();

	void ini_connect();

public:
	void setFilePath(const QString& filePath) { m_filePath = filePath; }

	void iniUI();

private:
	Ui::DlgChangeProductConfigClass *ui;

};
