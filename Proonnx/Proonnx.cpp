#include "Proonnx.h"

#include<QMessageBox>
#include<QComboBox>
#include<QFileDialog>
#include<QDate>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QVBoxLayout>

#include"oulq/oulq_LabelCustom.h"
//#include"LocalizationStringLoader-XML.h"
#include"cfgl/cfgl_LocalizationStringLoader.h"
#include"cfgr/cfgr_ConfigBeforeRuntimeLoader.h"
#include"cfgr/cfgr_CatalogueInitializer.h"
#include"cfgr/cfgr_RuntimeConfigLoader.h"

#include"MonitorCamera.h"
#include"DlgAddProductConfig.h"
#include"DlgChangeProductConfig.h"
#include"DlgSetProonnx.h"
#include"DlgSelectCameraIndex.h"
#include"DlgSetCamera.h"
#include"DlgForTest.h"
#include"DlgSetIsCheckProduct.h"
#include"DlgClearCount.h"
#include"IndexButton.h"
#include"ProductConfigLoader.h"
#include"ConfigForImageSave.h"
#include"DateTransform.h"
#include"LogRecorder.h"
#include"ImageIdentify.h"
#include"DlgSelectProductConfig.h"

static LogRecorder* LOGRECORDER = LogRecorder::getInstance();

void Proonnx::ini_ui()
{

#ifdef NDEBUG_RW
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->showFullScreen();

	this->setWindowFlags(this->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
#endif // NDEBUG_RW


	ini_configBeforeRuntimeLoader();
	ini_localizationStringLoader();
	ini_localizationStringUI();
	ini_gBox_monitoringDisplay();

	ui->ledit_currentDate->setText(QDate::currentDate().toString("yyyy/MM/dd"));
	auto font = ui->ledit_currentDate->font();
	font.setPointSize(20);
	ui->ledit_currentDate->setFont(font);
	ui->ledit_currentDate->setEnabled(false);

	ini_configForImageSave();
	ini_configBeforeRuntime();


	//////////////////////
#ifdef DEBUG_RW
	ui->pbtn_testDlg->setVisible(true);
	ui->pbtn_setProonnx->setVisible(true);
#endif // DEBUG_RW

#ifdef NDEBUG_RW
	ui->pbtn_testDlg->setVisible(false);
	ui->pbtn_setProonnx->setVisible(false);
#endif // NDEBUG_RW

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

void Proonnx::ini_localizationStringUI()
{
	ui->gBox_monitoringDisplay->setTitle(QString::fromStdString(m_locStrLoader->getString("1")));
	ui->gBox_setPutton->setTitle(QString::fromStdString(m_locStrLoader->getString("2")));
	ui->pbtn_setProonnx->setText(QString::fromStdString(m_locStrLoader->getString("20")));
	ui->gBox_currenctDate->setTitle(QString::fromStdString(m_locStrLoader->getString("19")));
}

void Proonnx::ini_configBeforeRuntimeLoader()
{
	m_configBeforeRuntimeLoader = new ConfigBeforeRuntimeLoader();

	///---------------
	auto configPath=rw::cfgr::CatalogueInitializer::findWorkPath("Config");
	configPath=rw::cfgr::CatalogueInitializer::pathAppend(configPath,"ConfigBeforeRuntimeLoader.xml");

	m_filePathConfigBeforeRuntimeLoader = QString::fromStdString(configPath);

	LOGRECORDER->info("Loading config of runtime at:" + configPath);

	auto loadResult = m_configBeforeRuntimeLoader->loadFile(configPath);
	if (!loadResult) {
		LOGRECORDER->warn("Cannot find config file!");
		LOGRECORDER->info("Create new config file at:" + configPath);
		m_configBeforeRuntimeLoader->setNewFile(configPath);
	}

    //----------------

    auto runtimePath = rw::cfgr::CatalogueInitializer::findWorkPath("Config");
    runtimePath = rw::cfgr::CatalogueInitializer::pathAppend(runtimePath, "runtimeCfg.xml");

    m_filePathRuntimeCfg = runtimePath;

	bool loaderResult{false};
    auto runtimeLoader = RuntimeConfigLoader::load(runtimePath,loaderResult);
	if (!loaderResult) {
		RutimeConfig config;
        config.cameraCount = 4;
        config.language = "CHN";
        RuntimeConfigLoader::save(runtimePath, config);
	}

}

void Proonnx::ini_configBeforeRuntime()
{
	LOGRECORDER->info("Initialize camera...................");
	ini_cameraList();

	ui->pbt_startImageIdentify->setText(QString::fromStdString(m_locStrLoader->getString("30")));
}

void Proonnx::ini_configForImageSave()
{
	LOGRECORDER->info("Create a new historical image folder and clean up old ones");
	m_configForImageSaveLoader = ConfigForImageSave::getInstance();
	m_configForImageSaveLoader->setToday(QString::fromStdString(DateTransFormUtilty::replaceSlashWithDash(ui->ledit_currentDate->text().toStdString())));
	m_configForImageSaveLoader->setSaveDays(7);
	auto histoyImagesPath=rw::cfgr::CatalogueInitializer::findWorkPath("HistoryImage");

	m_configForImageSaveLoader->setCurrentFilePath(QString::fromStdString(histoyImagesPath));
	m_configForImageSaveLoader->iniConfig();//////////path中的'//
}

void Proonnx::ini_gBox_monitoringDisplay()
{
	auto cameraCount = m_configBeforeRuntimeLoader->readCameraCount();

	LOGRECORDER->info("Set the window layout to display the camera window as " + std::to_string(cameraCount));

	m_labelDisaplayCameraList = new QVector<LabelClickable*>;
	m_labelDisaplayCheckInfoList = new QVector<QLabel*>;
	m_labelDisaplayProductNameList = new QVector<QLabel*>;
	m_labelProductCountList = new QVector<QLabel*>;
	m_labelProductPassCountList = new QVector<QLabel*>;
	m_labelProductOutCountList = new QVector<QLabel*>;
	m_labelProductCountList = new QVector<QLabel*>;
	m_pbtnSetIsCheckList = new QVector<IndexButton*>;
	QGridLayout* gBox_monitoringDisplayLayout = new QGridLayout(this);

	// Calculate the number of rows and columns to make them as close as possible
	int rows = static_cast<int>(std::sqrt(cameraCount));
	int cols = (cameraCount + rows - 1) / rows; // Round Up

	for (int i = 0; i < cameraCount; i++) {
		LabelClickable* label = new LabelClickable;
		label->m_index = i;
		QObject::connect(label, SIGNAL(clicked(int)), this, SLOT(monitorImageDisplaylabel_clicked(int)));
		label->setText(QString::fromStdString(m_locStrLoader->getString("26")) + QString::number(i + 1));
		label->setAlignment(Qt::AlignCenter); // Center the label text
		label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expand tags to fill the layout
		label->setScaledContents(true); // Fill the image with labels
		m_labelDisaplayCameraList->append(label);
		m_labelDisaplayCameraListHasCamera.append(false);
		int row = i / cols;
		int col = i % cols;

		QHBoxLayout* hBoxLayout1 = new QHBoxLayout(this);
		QLabel* checkInfo = new QLabel();
		checkInfo->setText("OK");
		m_labelDisaplayCheckInfoList->append(checkInfo);
		hBoxLayout1->addWidget(checkInfo);

		QLabel* ProductName = new QLabel();
		m_labelDisaplayProductNameList->append(ProductName);
		ProductName->setText(QString::fromStdString(m_locStrLoader->getString("7")));
		hBoxLayout1->addWidget(ProductName);

		QHBoxLayout* hBoxLayoutProductCheckInfo = new QHBoxLayout(this);
		QLabel* productCount = new QLabel();
		productCount->setText(QString::fromStdString(m_locStrLoader->getString("27")));
		m_labelProductCountList->append(productCount);
		hBoxLayoutProductCheckInfo->addWidget(productCount);

		QLabel* productPassCount = new QLabel();
		m_labelProductPassCountList->append(productPassCount);
		productPassCount->setText(QString::fromStdString(m_locStrLoader->getString("28")));
		hBoxLayoutProductCheckInfo->addWidget(productPassCount);

		QLabel* productOutCount = new QLabel();
		m_labelProductOutCountList->append(productOutCount);
		productOutCount->setText(QString::fromStdString(m_locStrLoader->getString("29")));
		hBoxLayoutProductCheckInfo->addWidget(productOutCount);

		IndexButton* indexButton = new IndexButton();
		indexButton->m_index = i;
		indexButton->setText(QString::fromStdString(m_locStrLoader->getString("30")));
		m_pbtnSetIsCheckList->append(indexButton);
		hBoxLayoutProductCheckInfo->addWidget(indexButton);
		QObject::connect(indexButton,SIGNAL(clickedWithIndex(int)),this,SLOT(pbt_setIsImageIdentify(int)));

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
	m_imageIdentifyList = new QVector<ImageIdentify*>;
	auto devList = MonitorCameraUtility::checkAllConnectCamera();

	for (int i = 0; i < m_labelDisaplayCameraList->size(); i++) {
		auto item = (*m_labelDisaplayCameraList)[i];
		if (i < devList.size()) {
			LOGRECORDER->info("Camera: " + devList[i]);
			auto imageIdentify = new ImageIdentify(item, devList[i]);
			imageIdentify->setDisaplayCheckInfo((*m_labelDisaplayCheckInfoList)[i]);
			imageIdentify->m_labelForProductCount = (*m_labelProductCountList)[i];
			imageIdentify->m_labelForProductOutCount = (*m_labelProductOutCountList)[i];
			imageIdentify->m_labelForProductPassCount = (*m_labelProductPassCountList)[i];
			imageIdentify->m_labelForNg = ui->label_ng;
			imageIdentify->setStandDate(ui->ledit_currentDate->text());
			imageIdentify->IniOcr();
			auto connectResult = imageIdentify->connectCamera();
			if (connectResult) {
				(*m_labelDisaplayCameraList)[i]->m_enbaleClicked = true;
				m_labelDisaplayCameraListHasCamera[i] = true;
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
					std::string path=rw::cfgr::CatalogueInitializer::findWorkPath("ProductConfig");
					path = rw::cfgr::CatalogueInitializer::pathAppend(path, newConfig.productName+".xml");

					LOGRECORDER->warn("Camera last run configuration not found, default configuration will be generated at:" + path);

					imageIdentify->m_productConfigFilePath = path;
					productConfigLoader.setNewFile(path);
					productConfigLoader.storeConfig(newConfig);
					productConfigLoader.saveFile(path);
					m_configBeforeRuntimeLoader->storeCameraConfig(devList[i], path);
					m_configBeforeRuntimeLoader->saveFile(m_filePathConfigBeforeRuntimeLoader.toStdString());
				}
				imageIdentify->m_labelForProductName = (*m_labelDisaplayProductNameList)[i];
				imageIdentify->iniCamera();
				imageIdentify->startMonitor();
				imageIdentify->setSoftwareTriggeredAcquisition();
				imageIdentify->m_setIsCheck = false;
				imageIdentify->setIsCheckProduct(false);
			}
			else {
				LOGRECORDER->warn("Disconnected by ip : " + devList[i]);
			}
			m_imageIdentifyList->append(imageIdentify);
		}
		else {
			auto imageIdentify = new ImageIdentify(item, "disconnecd");
			m_imageIdentifyList->append(imageIdentify);


			item->setText(QString::fromStdString(m_locStrLoader->getString("21")));

			LOGRECORDER->warn("No connected cameras detected at the location" + std::to_string(i));

		}
	}
}

void Proonnx::ini_connect()
{
	QObject::connect(ui->pbtn_setProonnx, SIGNAL(clicked()),
		this, SLOT(pbtn_setProonnx_clicked()));
	QObject::connect(ui->pbt_startImageIdentify, SIGNAL(clicked()),
		this, SLOT(pbt_startImageIdentify_clicked()));
	QObject::connect(ui->pbtn_quit, SIGNAL(clicked()),
		this, SLOT(pbtn_quit_clicked()));
	QObject::connect(ui->pbtn_testDlg, SIGNAL(clicked()),
		this, SLOT(pbtn_testDlg_clicked()));
}

void Proonnx::des_com()
{
	delete m_configBeforeRuntimeLoader;
	for (auto& item : *m_labelDisaplayCameraList) {
		delete item;
	}
	delete m_labelDisaplayCameraList;


	for (auto& item : *m_imageIdentifyList) {
		delete item;
	}
	delete m_imageIdentifyList;

	for (auto& item : *m_labelDisaplayCheckInfoList) {
		delete item;
	}
	delete m_labelDisaplayCheckInfoList;

	for (auto& item : *m_labelDisaplayProductNameList) {
		delete item;
	}
	delete m_labelDisaplayProductNameList;

	for (auto& item : *m_labelProductCountList) {
		delete item;
	}
	delete m_labelProductCountList;

	for (auto& item : *m_labelProductPassCountList) {
		delete item;
	}
	delete m_labelProductPassCountList;

	for (auto& item : *m_labelProductOutCountList) {
		delete item;
	}
	delete m_labelProductOutCountList;
	delete m_configForImageSaveLoader;

	for (auto& item : *m_pbtnSetIsCheckList) {
		delete item;
	}
	delete m_pbtnSetIsCheckList;
}

void Proonnx::set_isCheckProduct(bool is)
{
	for (auto& item : *m_imageIdentifyList) {
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

#ifdef NDEBUG_RW
	pbt_startImageIdentify_clicked();
#endif // NDEBUG_RW

#ifdef DEBUG_RW
	(*m_labelDisaplayCameraList)[0]->m_enbaleClicked = true;
#endif // DEBUG_RW


}

Proonnx::~Proonnx()
{
	delete ui;
}

QVector<bool> Proonnx::get_isCheckProductList()
{
	QVector<bool> result;
	for (const auto& item : *m_imageIdentifyList) {
		result.append(item->getIsCheckProduct());
	}
	return result;
}

void Proonnx::set_isCheckProductByList(const QVector<bool>& list)
{
	for (int i = 0; i < list.size(); ++i) {
		(*m_imageIdentifyList)[i]->setIsCheckProduct(list[i]);
		if (list[i]) {
			(*m_imageIdentifyList)[i]->setHardwareTriggeredAcquisition();
			LOGRECORDER->info("Camera  start recognizing at" + std::to_string(i));
		}
		else {
			(*m_imageIdentifyList)[i]->setSoftwareTriggeredAcquisition();
			LOGRECORDER->info("Camera  stops recognizing at" + std::to_string(i));
		}
	}
}

void Proonnx::set_allDoNotCheck()
{
	for (auto& item : (*m_imageIdentifyList)) {
		item->setIsCheckProduct(false);
	}
}

void Proonnx::select_config(int index)
{
	DlgSelectProductConfig dlg;
	dlg.exec();
	/*auto loader = LocalizationStringLoaderXML::getInstance();
	QFileDialog
		fileDlg(this, QString::fromStdString(loader->getString("22")),
			"", QString::fromStdString(loader->getString("23")));
	if (fileDlg.exec() == QFileDialog::Accepted) {
		auto fileName = fileDlg.selectedFiles().first();
		ProductConfigLoader loader;
		auto productConfig = loader.loadProductConfig(fileName.toStdString());
		(*m_imageIdentifyList)[index]->setExposureTime(productConfig.ExposureTime);
		(*m_imageIdentifyList)[index]->setGain(productConfig.gain);
		(*m_imageIdentifyList)[index]->setRotateCount(productConfig.rotateCount);
		auto recognizeRange = loader.loadRejectAttribute(fileName.toStdString());
		(*m_imageIdentifyList)[index]->setRejectAttribute(recognizeRange);

		RecognizeRange range;
		range.leftLowerCorner = productConfig.leftLowerCorner;
		range.lowerRightCorner = productConfig.lowerRightCorner;
		range.topLeftCorner = productConfig.topLeftCorner;
		range.upperRightCorner = productConfig.upperRightCorner;
		(*m_imageIdentifyList)[index]->setRecognizeRange(range);

		
		(*m_labelDisaplayProductNameList)[index]->setText(QString::fromStdString(productConfig.productName));
		
		auto countConfig = loader.loadProductCountInfo(fileName.toStdString());
		(*m_imageIdentifyList)[index]->setProductCount(countConfig.totalCount, countConfig.passCount, countConfig.outCount);
		m_configBeforeRuntimeLoader->storeCameraConfig((*m_imageIdentifyList)[index]->m_Ip, fileName.toStdString());
	}*/
}

void Proonnx::pbt_addProductCongfig(int index)
{
	LOGRECORDER->info("----------------------------------------------");
	LOGRECORDER->info("Open the Add Product Configuration dialog box");
	auto isCheckList = get_isCheckProductList();
	set_isCheckProduct(false);
	DlgAddProductConfig dlgAddProductConfig(this);
	dlgAddProductConfig.setWindowSize(this->width() , this->height() );

	dlgAddProductConfig.setCameraIndex(index + 1);
	dlgAddProductConfig.setCamera(m_imageIdentifyList->at(index));

	dlgAddProductConfig.setConfigBeforeRuntime(m_filePathConfigBeforeRuntimeLoader);

	dlgAddProductConfig.setWindowModality(Qt::WindowModal);
	auto dlgResult = dlgAddProductConfig.exec();
	if (dlgResult == QDialog::Accepted) {
		auto productName = dlgAddProductConfig.getProductName();
		(*m_labelDisaplayProductNameList)[index]->setText(productName);
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
	dlg.setWindowSize(this->width(), this->height());
	std::string path;
	auto readResult = m_configBeforeRuntimeLoader->readCameraConfig(m_imageIdentifyList->at(index)->m_Ip, path);
	if (readResult) {
		dlg.setFilePath(QString::fromStdString(path));
		dlg.setCameraIndex(index + 1);
		dlg.setCamera(m_imageIdentifyList->at(index));
		dlg.setConfigBeforeRuntime(m_filePathConfigBeforeRuntimeLoader);
		dlg.iniUI();
		dlg.setWindowModality(Qt::WindowModal);
		auto dlgResult = dlg.exec();
		if (dlgResult == QDialog::Accepted) {
			auto productName = dlg.getProductName();
			(*m_labelDisaplayProductNameList)[index]->setText(productName);
		}
	}
	else {
		LOGRECORDER->warn("No corresponding camera configuration detected, possibly due to data file corruption. Now jump to the configuration file selection window");
		QFileDialog fileDlg(this, QString::fromStdString(m_locStrLoader->getString("22")), "", QString::fromStdString(m_locStrLoader->getString("23")));
		if (fileDlg.exec() == QFileDialog::Accepted) {
			auto filePath = fileDlg.selectedFiles().first();
			dlg.setFilePath(filePath);
			dlg.setCameraIndex(index + 1);
			dlg.setCamera(m_imageIdentifyList->at(index));
			dlg.setConfigBeforeRuntime(m_filePathConfigBeforeRuntimeLoader);
			dlg.iniUI();
			auto dlgResult = dlg.exec();
			if (dlgResult == QDialog::Accepted) {
				auto productName = dlg.getProductName();
				(*m_labelDisaplayProductNameList)[index]->setText(productName);
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
	dlg.setFilePath(m_filePathConfigBeforeRuntimeLoader);
	dlg.iniComponet();
	dlg.exec();
	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbt_startImageIdentify_clicked()
{
	LOGRECORDER->info("----------------------------------------------");
	if (!m_isSingleCheckProduct) {
		ui->pbt_startImageIdentify->setText(QString::fromStdString(m_locStrLoader->getString("31")));
		set_isCheckProduct(true);
		m_isSingleCheckProduct = true;
		setCheckProduct_clicked(true);
		LOGRECORDER->info("Camera all begins to recognize ");
		for (auto & item:*m_pbtnSetIsCheckList) {
			item->setText(QString::fromStdString(m_locStrLoader->getString("31")));
		}

		for (int i = 0;i< m_imageIdentifyList->size();i++) {
			(*m_labelDisaplayCameraList)[i]->m_enbaleClicked = false&& m_labelDisaplayCameraListHasCamera[i];
		}

	}
	else {
		ui->pbt_startImageIdentify->setText(QString::fromStdString(m_locStrLoader->getString("30")));
		set_isCheckProduct(false);
		m_isSingleCheckProduct = false;
		setCheckProduct_clicked(false);
		LOGRECORDER->info("Camera  all stops recognizing ");
		for (auto& item : *m_pbtnSetIsCheckList) {
			item->setText(QString::fromStdString(m_locStrLoader->getString("30")));
		}
		for (int i = 0; i < m_imageIdentifyList->size(); i++) {
			(*m_labelDisaplayCameraList)[i]->m_enbaleClicked = true && m_labelDisaplayCameraListHasCamera[i];
		}
	}

	LOGRECORDER->info("----------------------------------------------");
}

void Proonnx::pbt_setIsImageIdentify(int index)
{
	auto camera = (*m_imageIdentifyList)[index];
	if (!camera->getIsCheckProduct()) {
		(*m_pbtnSetIsCheckList)[index]->setText(QString::fromStdString(m_locStrLoader->getString("31")));
		(*m_labelDisaplayCameraList)[index]->m_enbaleClicked = false && m_labelDisaplayCameraListHasCamera[index];
		camera->setHardwareTriggeredAcquisition();
		camera->setIsCheckProduct(true);
	}
	else {
		(*m_pbtnSetIsCheckList)[index]->setText(QString::fromStdString(m_locStrLoader->getString("30")));
		camera->setSoftwareTriggeredAcquisition();
		(*m_labelDisaplayCameraList)[index]->m_enbaleClicked = true && m_labelDisaplayCameraListHasCamera[index];
		camera->setIsCheckProduct(false);
	}
}

void Proonnx::setCheckProduct_clicked(bool check)
{
	if (check) {
		for (auto& item : *m_imageIdentifyList) {
			set_isCheckProduct(check);

			item->setHardwareTriggeredAcquisition();


		}
	}
	else {
		for (auto& item : *m_imageIdentifyList) {
			set_isCheckProduct(check);

			item->setSoftwareTriggeredAcquisition();

		}
	}

}

void Proonnx::pbtn_clearCount(int index)
{
	LOGRECORDER->info("----------------------------------------------");
	auto isCheckList = get_isCheckProductList();
	set_isCheckProduct(false);
	(*m_imageIdentifyList)[index]->setProductCount(0, 0, 0);
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

void Proonnx::monitorImageDisplaylabel_clicked(int index)
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
	dlg.setWindowModality(Qt::WindowModal);
	dlg.exec();
}





