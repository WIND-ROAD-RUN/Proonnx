#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Proonnx.h"

#include"LocalizationStringLoader-XML.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProonnxClass; };
QT_END_NAMESPACE

class Proonnx : public QMainWindow
{
	Q_OBJECT
private:
	LocalizationStringLoaderXML* m_locStrLoader{ nullptr };
private:
	void ini_ui();
	void ini_localizationStringLoader();
	void ini_localizationStringLoaderUI();

	void ini_connect();

	void des_com();
public:
	Proonnx(QWidget* parent = nullptr);
	~Proonnx();
	

private:
	Ui::ProonnxClass* ui;
private slots:
	void cBox_changeLanguage_index_change(int index);
	void pbt_addProductCongfig_clicked();
};
