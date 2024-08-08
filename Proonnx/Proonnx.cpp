#include "Proonnx.h"

#include<QMessageBox>
#include<QComboBox>
#include<QFileDialog>
#include<QDate>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QVBoxLayout>

#include"mycamera.h"
#include"MonitorCamera.h"

#include"DlgAddProductConfig.h"
#include"DlgChangeProductConfig.h"
#include"DlgSetProonnx.h"
#include"DlgSelectCameraIndex.h"
#include"LocalizationStringLoader-XML.h"
#include"ConfigBeforeRuntimeLoader.h"
#include"ProductConfigLoader.h"
#include"DlgSetIsCheckProduct.h"
#include"ConfigForImageSave.h"
#include"DateTransform.h"
#include"DlgClearCount.h"
#include"LogRecorder.h"
#include"DlgSetCamera.h"
#include"DlgForTest.h"

static LogRecorder* LOGRECORDER = LogRecorder::getInstance();

void Proonnx::ini_ui()
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->showFullScreen();

	this->setWindowFlags(this->windowFlags() & ~Qt::WindowMinMaxButtonsHint);

	ini_configBeforeRuntimeLoader();
	ini_localizationStringLoader();
	ini_localizationStringLoaderUI();
	ini_gBox_monitoringDisplay();

	ui->ledit_currentDate->setText(QDate::currentDate().toString("yyyy/MM/dd"));
	auto font = ui->ledit_currentDate->font();
	font.setPointSize(20);
	ui->ledit_currentDate->setFont(font);
	ui->ledit_currentDate->setEnabled(false);

	ini_configForImageSave();
	ini_configBeforeRuntime();


	//////////////////////
	ui->pbt_modProductConfig->setVisible(false); 
	ui->pbtn_clearCount->setVisible(false);
	ui->pbt_addProductCongfig->setVisible(false);
	ui->pbtn_selectProducrConfig->setVisible(false);

}

void Proonnx::ini_localizationStringLoader()
{
	m_locStrLoader = LocalizationStringLoaderXML::getInstance();

	QString filePath = "/languageString.xml";
	auto  currentFilePath = QDir::currentPath();
	filePath = currentFilePath + filePath;
	m_locStrLoader->setFilePath(filePath.toStdString());
	m_locStrLoader->setLanguage(m_configBeforeRuntimeLoader->readLanguage());

	LOGRECORDER->info("Loading localization string config file at" + filePath.toStdString() + " with language :" + m_configBeforeRuntimeLoader->readLanguage());

	auto loadStrDataResult = m_locStrLoader->loadData();
	if (!loadStrDataResult) {
		LOGRECORDER->error("Cannot find localization string config file!");
	}

}

void Proonnx::ini_localizationStringLoaderUI()
{
	ui->gBox_monitoringDisplay->setTitle(QString::fromStdString(m_locStrLoader->getString("1")));
	ui->gBox_setPutton->setTitle(QString::fromStdString(m_locStrLoader->getString("2")));
	ui->pbt_modProductConfig->setText(QString::fromStdString(m_locStrLoader->getString("3")));
	ui->pbt_addProductCongfig->setText(QString::fromStdString(m_locStrLoader->getString("5")));
	ui->pbtn_setProonnx->setText(QString::fromStdString(m_locStrLoader->getString("20")));
	ui->gBox_currenctDate->setTitle(QString::fromStdString(m_locStrLoader->getString("19")));


}

void Proonnx::ini_configBeforeRuntimeLoader()
{
	m_configBeforeRuntimeLoader = new ConfigBeforeRuntimeLoader();

	m_configBeforeRuntimeLoader = new ConfigBeforeRuntimeLoader();

	QString filePath = "/config/ConfigBeforeRuntimeLoader.xml";
	auto  currentFilePath = QDir::currentPath();
	filePath = currentFilePath + filePath;

	m_configBeforeRuntimeLoaderFilePath = filePath;

	LOGRECORDER->info("Loading config of runtime at:" + filePath.toStdString());

	auto loadResult = m_configBeforeRuntimeLoader->loadFile(filePath.toStdString());
	if (!loadResult) {
		LOGRECORDER->warn("Cannot find config file!");
		LOGRECORDER->info("Create new config file at:" + filePath.toStdString());
		m_configBeforeRuntimeLoader->setNewFile(filePath.toStdString());
	}
}

void Proonnx::ini_configBeforeRuntime()
{
	LOGRECORDER->info("Initialize camera...................");
	ini_cameraList();

	ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("30")));
}

void Proonnx::ini_configForImageSave()
{
	LOGRECORDER->info("Create a new historical image folder and clean up old ones");
	m_configForImageSave = ConfigForImageSave::getInstance();
	m_configForImageSave->setToday(QString::fromStdString(DateTransFormUtilty::replaceSlashWithDash(ui->ledit_currentDate->text().toStdString())));
	m_configForImageSave->setSaveDays(7);
	auto currentPath = QDir::currentPath();
	currentPath = currentPath + "/historyImages";
	m_configForImageSave->setCurrentFilePath(currentPath);
	m_configForImageSave->iniConfig();
}

void Proonnx::ini_gBox_monitoringDisplay()
{
	auto cameraCount = m_configBeforeRuntimeLoader->readCameraCount();

	LOGRECORDER->info("Set the window layout to display the camera window as " + std::to_string(cameraCount));

	m_disaplayCameraList = new QVector<ClickableLabel*>;
	m_disaplayCheckInfoList = new QVector<QLabel*>;
	m_disaplayProductNameList = new QVector<QLabel*>;
	m_productCountList = new QVector<QLabel*>;
	m_productPassCountList = new QVector<QLabel*>;
	m_productOutCountList = new QVector<QLabel*>;
	m_productCountList = new QVector<QLabel*>;
	m_setIsCheckPbtnList = new QVector<IndexButton*>;
	QGridLayout* gBox_monitoringDisplayLayout = new QGridLayout(this);

	// Calculate the number of rows and columns to make them as close as possible
	int rows = static_cast<int>(std::sqrt(cameraCount));
	int cols = (cameraCount + rows - 1) / rows; // Round Up

	for (int i = 0; i < cameraCount; i++) {
		ClickableLabel* label = new ClickableLabel;
		label->m_index = i;
		QObject::connect(label, SIGNAL(clicked(int)), this, SLOT(clicked_label_clicked(int)));
		label->setText(QString::fromStdString(m_locStrLoader->getString("26")) + QString::number(i + 1));
		label->setAlignment(Qt::AlignCenter); // Center the label text
		label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expand tags to fill the layout
		label->setScaledContents(true); // Fill the image with labels
		m_disaplayCameraList->append(label);
		int row = i / cols;
		int col = i % cols;

		QHBoxLayout* hBoxLayout1 = new QHBoxLayout(this);
		QLabel* checkInfo = new QLabel();
		checkInfo->setText("OK");
		m_disaplayCheckInfoList->append(checkInfo);
		hBoxLayout1->addWidget(checkInfo);

		QLabel* ProductName = new QLabel();
		m_disaplayProductNameList->append(ProductName);
		ProductName->setText(QString::fromStdString(m_locStrLoader->getString("7")));
		hBoxLayout1->addWidget(ProductName);

		QHBoxLayout* hBoxLayoutProductCheckInfo = new QHBoxLayout(this);
		QLabel* productCount = new QLabel();
		productCount->setText(QString::fromStdString(m_locStrLoader->getString("27")));
		m_productCountList->append(productCount);
		hBoxLayoutProductCheckInfo->addWidget(productCount);

		QLabel* productPassCount = new QLabel();
		m_productPassCountList->append(productPassCount);
		productPassCount->setText(QString::fromStdString(m_locStrLoader->getString("28")));
		hBoxLayoutProductCheckInfo->addWidget(productPassCount);

		QLabel* productOutCount = new QLabel();
		m_productOutCountList->append(productOutCount);
		productOutCount->setText(QString::fromStdString(m_locStrLoader->getString("29")));
		hBoxLayoutProductCheckInfo->addWidget(productOutCount);

		IndexButton* indexButton = new IndexButton();
		indexButton->m_index = i;
		indexButton->setText(QString::fromStdString(m_locStrLoader->getString("30")));
		m_setIsCheckPbtnList->append(indexButton);
		hBoxLayoutProductCheckInfo->addWidget(indexButton);
		QObject::connect(indexButton,SIGNAL(clickedWithIndex(int)),this,SLOT(pbt_setIsCheckProduct(int)));

		QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
		vBoxLayout->addLayout(hBoxLayout1);
		vBoxLayout->addWidget(label);
		vBoxLayout->addLayout(hBoxLayoutProductCheckInfo);

		gBox_monitoringDisplayLayout->addLayout(vBoxLayout, row, col);
	}

	ui->gBox_monitoringDisplay->setLayout(gBox_monitoringDisplayLayout);
}

void Proonnx::ini_cameraList()
{
	m_cameraList = new QVector<ImageIdentify*>;
	auto devList = MonitorCameraUtility::checkAllConnectCamera();

	for (int i = 0; i < m_disaplayCameraList->size(); i++) {
		auto item = (*m_disaplayCameraList)[i];
		if (i < devList.size()) {
			LOGRECORDER->info("Camera: " + devList[i]);
			auto imageIdentify = new ImageIdentify(item, devList[i]);
			imageIdentify->setDisaplayCheckInfo((*m_disaplayCheckInfoList)[i]);
			imageIdentify->m_labelForProductCount = (*m_productCountList)[i];
			imageIdentify->m_labelForProductOutCount = (*m_productOutCountList)[i];
			imageIdentify->m_labelForProductPassCount = (*m_productPassCountList)[i];
			imageIdentify->m_labelForNg = ui->label_ng;
			imageIdentify->setStandDate(ui->ledit_currentDate->text());
			imageIdentify->IniOcr();
			auto connectResult = imageIdentify->connectCamera();
			if (connectResult) {
				(*m_disaplayCameraList)[i]->m_enbaleClicked = true;
				std::string cameraConfigFilePath;
				auto readResult = m_configBeforeRuntimeLoader->readCameraConfig(devList[i], cameraConfigFilePath);
				if (readResult) {
					imageIdentify->m_productConfigFilePath = cameraConfigFilePath;
					LOGRECORDER->info("Read camera prodcut file path as: " + cameraConfigFilePath);
				}
				else {
					ProductConfigLoader productConfigLoader;
					ProductConfig newConfig;
					newConfig.productName = "UNDEFINED" + (devList[i]);
					newConfig.ExposureTime = 10000;
					newConfig.gain = 1;
					QString filePath = "/ProductConfig/";
					auto  currentFilePath = QDir::currentPath();
					filePath = currentFilePath + filePath + QString::fromStdString(newConfig.productName) + ".xml";

					LOGRECORDER->warn("Camera last run configuration not found, default configuration will be generated at:" + filePath.toStdString());

					imageIdentify->m_productConfigFilePath = filePath.toStdString();
					productConfigLoader.setNewFile(filePath.toStdString());
					productConfigLoader.storeConfig(newConfig);
					productConfigLoader.saveFile(filePath.toStdString());
					m_configBeforeRuntimeLoader->storeCameraConfig(devList[i], filePath.toStdString());
					m_configBeforeRuntimeLoader->saveFile(m_configBeforeRuntimeLoaderFilePath.toStdString());
				}
				imageIdentify->m_labelForProductName = (*m_disaplayProductNameList)[i];
				imageIdentify->iniCamera();
				imageIdentify->startMonitor();
				imageIdentify->setSoftwareTriggeredAcquisition();
				imageIdentify->m_setIsCheck = false;
				imageIdentify->setIsCheckProduct(false);
			}
			else {
				LOGRECORDER->warn("Disconnected by ip : " + devList[i]);
			}
			m_cameraList->append(imageIdentify);
		}
		else {
			auto imageIdentify = new ImageIdentify(item, "disconnecd");
			m_cameraList->append(imageIdentify);


			item->setText(QString::fromStdString(m_locStrLoader->getString("21")));

			LOGRECORDER->warn("No connected cameras detected at the location" + std::to_string(i));

		}


	}

	if (devList.size() == 0) {
		ui->pbtn_clearCount->setEnabled(false);
		ui->pbt_addProductCongfig->setEnabled(false);
		ui->pbt_modProductConfig->setEnabled(false);
		ui->pbt_setIsCheckProduct->setEnabled(false);
	}
}

void Proonnx::ini_connect()
{
	QObject::connect(ui->pbt_addProductCongfig, SIGNAL(clicked()),
		this, SLOT(pbt_addProductCongfig_clicked()));
	QObject::connect(ui->pbt_modProductConfig, SIGNAL(clicked()),
		this, SLOT(pbt_modProductConfig_clicked()));
	QObject::connect(ui->pbtn_setProonnx, SIGNAL(clicked()),
		this, SLOT(pbtn_setProonnx_clicked()));
	QObject::connect(ui->pbt_setIsCheckProduct, SIGNAL(clicked()),
		this, SLOT(pbt_setIsCheckProduct_clicked()));
	QObject::connect(ui->pbtn_clearCount, SIGNAL(clicked()),
		this, SLOT(pbtn_clearCount_clicked()));
	QObject::connect(ui->pbtn_quit, SIGNAL(clicked()),
		this, SLOT(pbtn_quit_clicked()));
	QObject::connect(ui->pbtn_testDlg, SIGNAL(clicked()),
		this, SLOT(pbtn_testDlg_clicked()));
}

void Proonnx::des_com()
{
	delete m_configBeforeRuntimeLoader;
	for (auto& item : *m_disaplayCameraList) {
		delete item;
	}
	delete m_disaplayCameraList;


	for (auto& item : *m_cameraList) {
		delete item;
	}
	delete m_cameraList;

	for (auto& item : *m_disaplayCheckInfoList) {
		delete item;
	}
	delete m_disaplayCheckInfoList;

	for (auto& item : *m_disaplayProductNameList) {
		delete item;
	}
	delete m_disaplayProductNameList;

	for (auto& item : *m_productCountList) {
		delete item;
	}
	delete m_productCountList;

	for (auto& item : *m_productPassCountList) {
		delete item;
	}
	delete m_productPassCountList;

	for (auto& item : *m_productOutCountList) {
		delete item;
	}
	delete m_productOutCountList;
	delete m_configForImageSave;

	for (auto& item : *m_setIsCheckPbtnList) {
		delete item;
	}
	delete m_setIsCheckPbtnList;
}

void Proonnx::set_isCheckProduct(bool is)
{
	for (auto& item : *m_cameraList) {
		item->setIsCheckProduct(is);
	}
}

bool Proonnx::checkCurrentIsAllFalse(const QVector<bool>& list)
{
	bool result{ true };
	for (const auto& item : list) {
		if (item) {
			result = false;
		}
	}
	return result;
}

Proonnx::Proonnx(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::ProonnxClass())
{
	ui->setupUi(this);
	LOGRECORDER->info("###############################");
	LOGRECORDER->info("           Inilize             ");
	LOGRECORDER->info("###############################");
	ini_ui();
	ini_connect();

	LOGRECORDER->info("###############################");
	LOGRECORDER->info("    Inilize     Complete            ");
	LOGRECORDER->info("###############################");

	//
	pbt_setIsCheckProduct_clicked();
}

Proonnx::~Proonnx()
{
	delete ui;
}

QVector<bool> Proonnx::get_isCheckProductList()
{
	QVector<bool> result;
	for (const auto& item : *m_cameraList) {
		result.append(item->getIsCheckProduct());
	}
	return result;
}

void Proonnx::set_isCheckProductByList(const QVector<bool>& list)
{
	for (int i = 0; i < list.size(); ++i) {
		(*m_cameraList)[i]->setIsCheckProduct(list[i]);
		if (list[i]) {
			(*m_cameraList)[i]->setHardwareTriggeredAcquisition();
			LOGRECORDER->info("Camera  start recognizing at" + std::to_string(i));
		}
		else {
			(*m_cameraList)[i]->setSoftwareTriggeredAcquisition();
			LOGRECORDER->info("Camera  stops recognizing at" + std::to_string(i));
		}
	}
}

void Proonnx::set_allDoNotCheck()
{
	for (auto& item : (*m_cameraList)) {
		item->setIsCheckProduct(false);
	}
}

void Proonnx::select_config(int index)
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	QFileDialog
		fileDlg(this, QString::fromStdString(loader->getString("22")),
			"", QString::fromStdString(loader->getString("23")));
	if (fileDlg.exec() == QFileDialog::Accepted) {
		auto fileName = fileDlg.selectedFiles().first();
		ProductConfigLoader loader;
		auto productConfig = loader.loadProductConfig(fileName.toStdString());
		(*m_cameraList)[index]->setExposureTime(productConfig.ExposureTime);
		(*m_cameraList)[index]->setGain(productConfig.gain);
		(*m_cameraList)[index]->setRotateCount(productConfig.rotateCount);
		auto recognizeRange = loader.loadRejectAttribute(fileName.toStdString());
		(*m_cameraList)[index]->setRejectAttribute(recognizeRange);

		RecognizeRange range;
		range.leftLowerCorner = productConfig.leftLowerCorner;
		range.lowerRightCorner = productConfig.lowerRightCorner;
		range.topLeftCorner = productConfig.topLeftCorner;
		range.upperRightCorner = productConfig.upperRightCorner;
		(*m_cameraList)[index]->setRecognizeRange(range);

		
		(*m_disaplayProductNameList)[index]->setText(QString::fromStdString(productConfig.productName));
		
		auto countConfig = loader.loadProductCountInfo(fileName.toStdString());
		(*m_cameraList)[index]->setProductCount(countConfig.totalCount, countConfig.passCount, countConfig.outCount);
		m_configBeforeRuntimeLoader->storeCameraConfig((*m_cameraList)[index]->m_Ip, fileName.toStdString());
	}
}

void Proonnx::pbt_addProductCongfig(int index)
{
	LOGRECORDER->info("----------------------------------------------");
	LOGRECORDER->info("Open the Add Product Configuration dialog box");
	auto isCheckList = get_isCheckProductList();
	set_isCheckProduct(false);
	DlgAddProductConfig dlgAddProductConfig(this);
	dlgAddProductConfig.setWindowSize(this->width() * 0.75, this->height() * 0.75);

	dlgAddProductConfig.setCameraIndex(index + 1);
	dlgAddProductConfig.setCamera(m_cameraList->at(index));

	dlgAddProductConfig.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);

	auto dlgResult = dlgAddProductConfig.exec();
	if (dlgResult == QDialog::Accepted) {
		auto productName = dlgAddProductConfig.getProductName();
		(*m_disaplayProductNameList)[index]->setText(productName);
	}
	set_isCheckProductByList(isCheckList);
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbt_modProductConfig_clicked()
{
	LOGRECORDER->info("----------------------------------------------");
	LOGRECORDER->info("Opened the dialog box for modifying product configuration");

	m_configBeforeRuntimeLoader->loadFile(m_configBeforeRuntimeLoaderFilePath.toStdString());
	auto isCheckList = get_isCheckProductList();
	set_isCheckProduct(false);
	int cameraCount = m_cameraList->size();
	if (cameraCount == 1) {
		DlgChangeProductConfig dlg;
		dlg.setWindowSize(this->width() * 0.75, this->height() * 0.75);
		std::string path;
		auto readResult = m_configBeforeRuntimeLoader->readCameraConfig(m_cameraList->at(0)->m_Ip, path);
		if (readResult) {
			dlg.setFilePath(QString::fromStdString(path));
			dlg.setCameraIndex(1);
			dlg.setCamera(m_cameraList->at(0));
			dlg.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);
			dlg.iniUI();
			auto dlgResult = dlg.exec();
			if (dlgResult == QDialog::Accepted) {
				auto productName = dlg.getProductName();
				(*m_disaplayProductNameList)[0]->setText(productName);
			}
		}
		else {
			LOGRECORDER->warn("No corresponding camera configuration detected, possibly due to data file corruption. Now jump to the configuration file selection window");
			QFileDialog fileDlg(this, QString::fromStdString(m_locStrLoader->getString("22")), "", QString::fromStdString(m_locStrLoader->getString("23")));
			if (fileDlg.exec() == QFileDialog::Accepted) {
				auto filePath = fileDlg.selectedFiles().first();
				dlg.setFilePath(filePath);
				dlg.setCameraIndex(1);
				dlg.setCamera(m_cameraList->at(0));
				dlg.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);
				dlg.iniUI();
				auto dlgResult = dlg.exec();
				if (dlgResult == QDialog::Accepted) {
					auto productName = dlg.getProductName();
					(*m_disaplayProductNameList)[0]->setText(productName);
				}
			}
		}
	}
	else {
		LOGRECORDER->info("If the number of cameras is greater than 1, enter the selection dialog box");
		DlgSelectCameraIndex dlgSelectCameraIndex(this, cameraCount);
		dlgSelectCameraIndex.setWindowSize(this->width() * 0.75, this->height() * 0.75);
		dlgSelectCameraIndex.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);
		auto selectCareraIndexResult = dlgSelectCameraIndex.exec();

		if (selectCareraIndexResult == QDialog::Accepted) {
			auto cameraIndex = dlgSelectCameraIndex.m_indexIndex;
			LOGRECORDER->info("Selected camera at:" + std::to_string(cameraIndex));

			DlgChangeProductConfig dlg;
			dlg.setWindowSize(this->width() * 0.75, this->height() * 0.75);
			std::string path;
			auto readResult = m_configBeforeRuntimeLoader->readCameraConfig(m_cameraList->at(cameraIndex - 1)->m_Ip, path);
			if (readResult) {
				dlg.setFilePath(QString::fromStdString(path));
				dlg.setCameraIndex(cameraIndex);
				dlg.setCamera(m_cameraList->at(cameraIndex - 1));
				dlg.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);
				dlg.iniUI();
				auto dlgResult = dlg.exec();
				if (dlgResult == QDialog::Accepted) {
					auto productName = dlg.getProductName();
					(*m_disaplayProductNameList)[cameraIndex - 1]->setText(productName);
				}
			}
			else {
				QFileDialog fileDlg(this, QString::fromStdString(m_locStrLoader->getString("22")), "", QString::fromStdString(m_locStrLoader->getString("23")));
				if (fileDlg.exec() == QFileDialog::Accepted) {
					auto filePath = fileDlg.selectedFiles().first();
					dlg.setFilePath(filePath);
					dlg.setCameraIndex(cameraIndex);
					dlg.setCamera(m_cameraList->at(cameraIndex - 1));
					dlg.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);
					dlg.iniUI();
					auto dlgResult = dlg.exec();
					if (dlgResult == QDialog::Accepted) {
						auto productName = dlg.getProductName();
						(*m_disaplayProductNameList)[cameraIndex - 1]->setText(productName);
					}
				}
			}
		}
	}
	set_isCheckProductByList(isCheckList);
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbt_modProductConfig(int index)
{
	LOGRECORDER->info("----------------------------------------------");
	LOGRECORDER->info("Opened the dialog box for modifying product configuration");
	auto isCheckList = get_isCheckProductList();
	set_isCheckProduct(false);
	DlgChangeProductConfig dlg;
	dlg.setWindowSize(this->width() * 0.75, this->height() * 0.75);
	std::string path;
	auto readResult = m_configBeforeRuntimeLoader->readCameraConfig(m_cameraList->at(index)->m_Ip, path);
	if (readResult) {
		dlg.setFilePath(QString::fromStdString(path));
		dlg.setCameraIndex(index + 1);
		dlg.setCamera(m_cameraList->at(index));
		dlg.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);
		dlg.iniUI();
		auto dlgResult = dlg.exec();
		if (dlgResult == QDialog::Accepted) {
			auto productName = dlg.getProductName();
			(*m_disaplayProductNameList)[index]->setText(productName);
		}
	}
	else {
		LOGRECORDER->warn("No corresponding camera configuration detected, possibly due to data file corruption. Now jump to the configuration file selection window");
		QFileDialog fileDlg(this, QString::fromStdString(m_locStrLoader->getString("22")), "", QString::fromStdString(m_locStrLoader->getString("23")));
		if (fileDlg.exec() == QFileDialog::Accepted) {
			auto filePath = fileDlg.selectedFiles().first();
			dlg.setFilePath(filePath);
			dlg.setCameraIndex(index + 1);
			dlg.setCamera(m_cameraList->at(index));
			dlg.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);
			dlg.iniUI();
			auto dlgResult = dlg.exec();
			if (dlgResult == QDialog::Accepted) {
				auto productName = dlg.getProductName();
				(*m_disaplayProductNameList)[index]->setText(productName);
			}
		}
	}
	set_isCheckProductByList(isCheckList);
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbtn_setProonnx_clicked()
{
	LOGRECORDER->info("----------------------------------------------");
	LOGRECORDER->info("Open the settings window");
	DlgSetProonnx dlg;
	dlg.setWindowSize(this->width() * 0.75, this->height() * 0.75);
	dlg.setFilePath(m_configBeforeRuntimeLoaderFilePath);
	dlg.iniComponet();
	dlg.exec();
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbt_setIsCheckProduct_clicked()
{
	LOGRECORDER->info("----------------------------------------------");
	if (!m_isSingleCheckProduct) {
		ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("31")));
		set_isCheckProduct(true);
		m_isSingleCheckProduct = true;
		setCheckProduct_clicked(true);
		LOGRECORDER->info("Camera all begins to recognize ");
		for (auto & item:*m_setIsCheckPbtnList) {
			item->setText(QString::fromStdString(m_locStrLoader->getString("31")));
		}

		for (int i = 0;i< m_cameraList->size();i++) {
			(*m_disaplayCameraList)[i]->m_enbaleClicked = false;
		}

	}
	else {
		ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("30")));
		set_isCheckProduct(false);
		m_isSingleCheckProduct = false;
		setCheckProduct_clicked(false);
		LOGRECORDER->info("Camera  all stops recognizing ");
		for (auto& item : *m_setIsCheckPbtnList) {
			item->setText(QString::fromStdString(m_locStrLoader->getString("30")));
		}
		for (int i = 0; i < m_cameraList->size(); i++) {
			(*m_disaplayCameraList)[i]->m_enbaleClicked = true;
		}
	}
	/*if (m_cameraList->size() == 1) {
		if (!m_isSingleCheckProduct) {
			ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("31")));
			set_isCheckProduct(true);
			m_isSingleCheckProduct = true;
			(*m_cameraList)[0]->setHardwareTriggeredAcquisition();
			ui->pbt_addProductCongfig->setEnabled(false);
			ui->pbt_modProductConfig->setEnabled(false);
			LOGRECORDER->info("Camera begins to recognize at:0");

		}
		else {
			ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("30")));
			set_isCheckProduct(false);
			m_isSingleCheckProduct = false;
			(*m_cameraList)[0]->setSoftwareTriggeredAcquisition();
			ui->pbt_addProductCongfig->setEnabled(true);
			ui->pbt_modProductConfig->setEnabled(true);
			LOGRECORDER->info("Camera  stops recognizing at:0");
		}
	}
	else {
		DlgSetIsCheckProduct dlg;
		dlg.setWindowSize(this->width() * 0.75, this->height() * 0.75);
		auto checkList = get_isCheckProductList();
		set_allDoNotCheck();
		dlg.iniGBox_cameraList(checkList);
		auto dlgResult = dlg.exec();

		if (dlgResult == QDialog::Accepted) {
			auto currentCheckList = dlg.getCurrentIsCheckList();
			set_isCheckProductByList(currentCheckList);
			auto pbtnEnabel = checkCurrentIsAllFalse(currentCheckList);
			ui->pbt_addProductCongfig->setEnabled(pbtnEnabel);
			ui->pbt_modProductConfig->setEnabled(pbtnEnabel);

		}
		else {
			auto nativeCheckList = dlg.getNativeIsCheckList();
			set_isCheckProductByList(nativeCheckList);
			auto pbtnEnabel = checkCurrentIsAllFalse(nativeCheckList);
			ui->pbt_addProductCongfig->setEnabled(pbtnEnabel);
			ui->pbt_modProductConfig->setEnabled(pbtnEnabel);
		}
	}*/
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbt_setIsCheckProduct(int index)
{
	auto camera = (*m_cameraList)[index];
	if (!camera->getIsCheckProduct()) {
		(*m_setIsCheckPbtnList)[index]->setText(QString::fromStdString(m_locStrLoader->getString("31")));
		
		camera->setHardwareTriggeredAcquisition();
		camera->setIsCheckProduct(true);
	}
	else {
		(*m_setIsCheckPbtnList)[index]->setText(QString::fromStdString(m_locStrLoader->getString("30")));
		camera->setSoftwareTriggeredAcquisition();
		camera->setIsCheckProduct(false);
	}
}

void Proonnx::setCheckProduct_clicked(bool check)
{
	if (check) {
		for (auto& item : *m_cameraList) {
			set_isCheckProduct(check);
			item->setHardwareTriggeredAcquisition();
		}
	}
	else {
		for (auto& item : *m_cameraList) {
			set_isCheckProduct(check);
			item->setSoftwareTriggeredAcquisition();
		}
	}

}

void Proonnx::pbtn_clearCount_clicked()
{
	LOGRECORDER->info("----------------------------------------------");
	auto isCheckList = get_isCheckProductList();
	set_isCheckProduct(false);
	int cameraCount = m_cameraList->size();
	if (cameraCount == 1) {
		(*m_cameraList)[0]->setProductCount(0, 0, 0);
		LOGRECORDER->info("Zero product count at camera index :" + std::to_string(0));
	}
	else {
		DlgClearCount dlg;
		dlg.setWindowSize(this->width() * 0.75, this->height() * 0.75);
		dlg.iniGBox_cameraList(m_cameraList->size());
		auto dlgResult = dlg.exec();

		if (dlgResult == QDialog::Accepted) {
			auto isCheckList = dlg.getIsCheckList();
			for (int i = 0; i < isCheckList.size(); i++) {
				if (isCheckList[i]) {
					(*m_cameraList)[i]->setProductCount(0, 0, 0);
					LOGRECORDER->info("Zero product count at camera index :" + std::to_string(i));
				}
			}
		}
	}
	set_isCheckProductByList(isCheckList);
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbtn_clearCount(int index)
{
	LOGRECORDER->info("----------------------------------------------");
	auto isCheckList = get_isCheckProductList();
	set_isCheckProduct(false);
	(*m_cameraList)[index]->setProductCount(0, 0, 0);
	LOGRECORDER->info("Zero product count at camera index :" + std::to_string(0));
	set_isCheckProductByList(isCheckList);
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbtn_quit_clicked()
{
	LOGRECORDER->info("----------------------------------------------");
	auto result = QMessageBox::question(this, QString::fromStdString(m_locStrLoader->getString("40")), QString::fromStdString(m_locStrLoader->getString("41")));
	if (result == QMessageBox::Yes) {
		LOGRECORDER->info("Exit the main program........");
		des_com();
		this->close();
	}
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::clicked_label_clicked(int index)
{
	DlgSetCamera setCameraDkg;
	auto selectResult = setCameraDkg.exec();
	if (selectResult == QDialog::Accepted) {
		switch (setCameraDkg.m_select)
		{
		case 0:
			pbt_addProductCongfig(index);
			break;
		case 1:
			pbt_modProductConfig(index);
			break;
		case 2:
			select_config(index);
			break;
		case 3:
			pbtn_clearCount(index);
			break;
		default:
			break;
		}

	}


}

void Proonnx::pbtn_testDlg_clicked()
{
	DlgForTest dlg;
	dlg.exec();
}

void Proonnx::pbt_addProductCongfig_clicked()
{
	LOGRECORDER->info("----------------------------------------------");
	LOGRECORDER->info("Open the Add Product Configuration dialog box");
	auto isCheckList = get_isCheckProductList();
	set_isCheckProduct(false);
	int cameraCount = m_cameraList->size();
	if (cameraCount == 1) {
		DlgAddProductConfig dlgAddProductConfig(this);
		dlgAddProductConfig.setWindowSize(this->width() * 0.75, this->height() * 0.75);

		dlgAddProductConfig.setCameraIndex(1);
		dlgAddProductConfig.setCamera(m_cameraList->at(0));

		dlgAddProductConfig.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);

		auto dlgResult = dlgAddProductConfig.exec();
		if (dlgResult == QDialog::Accepted) {
			auto productName = dlgAddProductConfig.getProductName();
			(*m_disaplayProductNameList)[0]->setText(productName);
		}
	}
	else {
		LOGRECORDER->info("If the number of cameras is greater than 1, enter the selection dialog box");
		DlgSelectCameraIndex dlgSelectCameraIndex(this, cameraCount);
		dlgSelectCameraIndex.setWindowSize(this->width() * 0.75, this->height() * 0.75);
		dlgSelectCameraIndex.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);

		auto selectCareraIndexResult = dlgSelectCameraIndex.exec();

		if (selectCareraIndexResult == QDialog::Accepted) {
			auto cameraIndex = dlgSelectCameraIndex.m_indexIndex;

			LOGRECORDER->info("Selected camera at:" + std::to_string(cameraIndex));

			DlgAddProductConfig dlgAddProductConfig(this);
			dlgAddProductConfig.setWindowSize(this->width() * 0.75, this->height() * 0.75);

			dlgAddProductConfig.setCameraIndex(cameraIndex);
			dlgAddProductConfig.setCamera(m_cameraList->at(cameraIndex - 1));

			dlgAddProductConfig.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);

			auto dlgResult = dlgAddProductConfig.exec();
			if (dlgResult == QDialog::Accepted) {
				auto productName = dlgAddProductConfig.getProductName();
				(*m_disaplayProductNameList)[cameraIndex - 1]->setText(productName);
			}
		}
	}
	set_isCheckProductByList(isCheckList);
	LOGRECORDER->info("----------------------------------------------");
}




