﻿#pragma once

#include <QDialog>
#include "ui_DlgSetProonnx.h"

namespace rw {
	namespace cfgl {
		class LocalizationStringLoaderXML;
	}
	namespace cfgr {
		class ConfigBeforeRuntimeLoader;
	}
}

QT_BEGIN_NAMESPACE
namespace Ui { class DlgSetProonnxClass; };
QT_END_NAMESPACE

using namespace rw::cfgl;
using namespace rw::cfgr;

class DlgSetProonnx : public QDialog
{
	Q_OBJECT
private:
	ConfigBeforeRuntimeLoader* m_configBeforeRuntimeLoader;

private:
	QString m_filePath{};

private:
	LocalizationStringLoaderXML* m_locStrLoader;

public:
	DlgSetProonnx(QWidget *parent = nullptr);

	~DlgSetProonnx();

public:
	void setFilePath(const QString& filePath) { m_filePath = filePath; }

	void iniComponet();

	void setWindowSize(int wide, int height);

private:
	void ini_ui();

	void ini_localizationStringUI();

	void ini_configBeforeRuntimeLoader(const std::string& filePath);


	void ini_uiFromConfigBeforeRuntime();


	void ini_connect();

private:
	Ui::DlgSetProonnxClass *ui;

private slots:
	void cBox_changeLanguage_index_change_on(int index);

	void pbtn_accept_clicked();

	void pbtn_cancel_clicked();

};
