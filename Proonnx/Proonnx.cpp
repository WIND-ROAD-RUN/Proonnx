#include "Proonnx.h"

#include<QMessageBox>
#include<QComboBox>
#include<QFileDialog>
#include<QDate>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QVBoxLayout>

#include"pch.h"

using namespace rw::oso;
void Proonnx::ini_readConfig()
{
    m_runtimeInfo.ini();
}

void Proonnx::ini_ui()
{

#ifdef NDEBUG_RW
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->showFullScreen();

	this->setWindowFlags(this->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
#endif // NDEBUG_RW


	//ini_configBeforeRuntimeLoader();

	ini_localizationStringLoader();
	ini_localizationStringUI();

	//ini_gBox_monitoringDisplay();
	ini_gBox_monitoringDisplay_refactor();

	ui->ledit_currentDate->setText(QDate::currentDate().toString("yyyy/MM/dd"));
	auto font = ui->ledit_currentDate->font();
	font.setPointSize(20);
	ui->ledit_currentDate->setFont(font);
	ui->ledit_currentDate->setEnabled(false);
	
	ini_configForImageSave();
    ini_cameraList_refactor();
	//ini_cameraList();


	//////////////////////
#ifdef DEBUG_RW
	ui->pbtn_testDlg->setVisible(true);
	ui->pbtn_setProonnx->setVisible(true);
#endif // DEBUG_RW

#ifdef NDEBUG_RW
	ui->pbtn_testDlg->setVisible(false);
	ui->pbtn_setProonnx->setVisible(false);
#endif // NDEBUG_RW


//#ifdef NDEBUG_RW
//	pbt_startImageIdentify_clicked();
//#endif // NDEBUG_RW
//
//#ifdef DEBUG_RW
//	(*m_labelDisaplayCameraList)[0]->m_enbaleClicked = true;
//#endif // DEBUG_RW

}

void Proonnx::ini_localizationStringLoader()
{
	m_locStrLoader = LocalizationStringLoaderXML::getInstance();

	QString filePath = "/languageString.xml";
	auto  currentFilePath = QDir::currentPath();
	filePath = currentFilePath + filePath;
	m_locStrLoader->setFilePath(filePath.toStdString());
	m_locStrLoader->setLanguage(m_runtimeInfo.m_runtimeConfigPtr->language);

	auto loadStrDataResult = m_locStrLoader->loadData();
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
}

void Proonnx::ini_configBeforeRuntime()
{
	LOGRECORDER->info("Initialize camera...................");

	ui->pbt_startImageIdentify->setText(QString::fromStdString(m_locStrLoader->getString("30")));
}

void Proonnx::ini_configForImageSave()
{
	m_configForImageSaveLoader = ConfigForImageSave::getInstance();
	m_configForImageSaveLoader->setToday(QString::fromStdString(DateTransFormUtilty::replaceSlashWithDash(ui->ledit_currentDate->text().toStdString())));
	m_configForImageSaveLoader->setSaveDays(7);
	auto histoyImagesPath=rw::cfgr::CatalogueInitializer::findWorkPath("HistoryImage");

	m_configForImageSaveLoader->setCurrentFilePath(QString::fromStdString(histoyImagesPath));
	m_configForImageSaveLoader->iniConfig();//////////path中的'//
}

void Proonnx::ini_gBox_monitoringDisplay()
{	
   auto cameraCount = m_runtimeInfo.m_runtimeConfigPtr->cameraCount;

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

void Proonnx::ini_gBox_monitoringDisplay_refactor()
{
    auto cameraCount = m_runtimeInfo.m_runtimeConfigPtr->cameraCount;
    m_displayCameraGroupBox.ini(cameraCount);
	QGridLayout* gBox_monitoringDisplayLayout = new QGridLayout(this);

	// Calculate the number of rows and columns to make them as close as possible
	int rows = static_cast<int>(std::sqrt(cameraCount));
	int cols = (cameraCount + rows - 1) / rows; // Round Up

	for (int i = 0;i<cameraCount;i++) {
		auto imageDisplayLabel = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i]->m_labelDisplayCamera;
        imageDisplayLabel->m_index = i;
        QObject::connect(imageDisplayLabel, SIGNAL(clicked(int)), this, SLOT(monitorImageDisplaylabel_clicked(int)));
        imageDisplayLabel->setText("disconnected");
        imageDisplayLabel->setAlignment(Qt::AlignCenter); // Center the label text
        imageDisplayLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expand tags to fill the layout
        imageDisplayLabel->setScaledContents(true); // Fill the image with labels

        int row = i / cols;
        int col = i % cols;

        QHBoxLayout* hBoxLayout1 = new QHBoxLayout(this);
        auto checkInfo = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i]->m_labelCheckInfo;
        checkInfo->setText("OK");
        hBoxLayout1->addWidget(checkInfo);

        auto ProductName = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i]->m_labelProductName;
        ProductName->setText("ProductName");
        hBoxLayout1->addWidget(ProductName);

        QHBoxLayout* hBoxLayoutProductCheckInfo = new QHBoxLayout(this);
        auto productCount = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i]->m_labelProductCount;
        productCount->setText("ProductCount");
        hBoxLayoutProductCheckInfo->addWidget(productCount);


        auto productPassCount = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i]->m_labelProductPassCount;
        productPassCount->setText("ProductPassCount");
        hBoxLayoutProductCheckInfo->addWidget(productPassCount);

        auto productOutCount = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i]->m_labelProductOutCount;
        productOutCount->setText("ProductOutCount");
        hBoxLayoutProductCheckInfo->addWidget(productOutCount);

        auto indexButton = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i]->m_pbtnSetIsCheck;
        indexButton->m_index = i;
        indexButton->setText("Start");
        hBoxLayoutProductCheckInfo->addWidget(indexButton);
        QObject::connect(indexButton, SIGNAL(clickedWithIndex(int)), this, SLOT(pbt_setIsImageIdentify(int)));

        QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
        vBoxLayout->addLayout(hBoxLayout1);
        vBoxLayout->addWidget(imageDisplayLabel);
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
			auto imageIdentify = new ImageIdentify(item, devList[i]);
			imageIdentify->setDisaplayCheckInfo((*m_labelDisaplayCheckInfoList)[i]);
			imageIdentify->m_labelForProductCount = (*m_labelProductCountList)[i];
			imageIdentify->m_labelForProductOutCount = (*m_labelProductOutCountList)[i];
			imageIdentify->m_labelForProductPassCount = (*m_labelProductPassCountList)[i];
			imageIdentify->m_labelForNg = ui->label_ng;
			imageIdentify->setStandDate(ui->ledit_currentDate->text());
			auto connectResult = imageIdentify->connectCamera();
			if (connectResult) {
				(*m_labelDisaplayCameraList)[i]->m_enbaleClicked = true;
				m_labelDisaplayCameraListHasCamera[i] = true;
				std::string cameraConfigFilePath;
                auto readResult = m_runtimeInfo.m_runtimeConfigPtr->readCameraLastRunTimeConfig(devList[i], cameraConfigFilePath);
				if (readResult) {
					imageIdentify->m_productConfigFilePath = cameraConfigFilePath;
                    (*m_labelDisaplayProductNameList)[i]->setText(QString::fromStdString("-------"));
				}
				else {
					std::string path=rw::cfgr::CatalogueInitializer::findWorkPath("ProductConfig");

					rw::oso::OcrDataProductConfig newCameraCfg;
					newCameraCfg.productName = "UNDEFINED" + (devList[i]);
                    (*m_labelDisaplayProductNameList)[i]->setText(QString::fromStdString(newCameraCfg.productName));

					path = rw::cfgr::CatalogueInitializer::pathAppend(path, newCameraCfg.productName+".xml");

					imageIdentify->m_productConfigFilePath = path;
					//
                    rw::oso::FileSave fileSave;
					fileSave.save(path, newCameraCfg);
                    m_runtimeInfo.m_runtimeConfigPtr->addCameraLastRunTimeConfig(devList[i], path);
					m_runtimeInfo.saveRuntimeConfigFile();
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
			imageIdentify->IniOcr();
		}
		else {
			auto imageIdentify = new ImageIdentify(item, "disconnecd");
			m_imageIdentifyList->append(imageIdentify);


			item->setText(QString::fromStdString(m_locStrLoader->getString("21")));

			LOGRECORDER->warn("No connected cameras detected at the location" + std::to_string(i));

		}
	}
}

void Proonnx::ini_cameraList_refactor()
{
	const auto & devList = m_runtimeInfo.m_cameraIpList;

    //创建相机接管类
	for (int i = 0;i< devList.size();i++) {

        auto label = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i]->m_labelDisplayCamera;
        label->_ip = devList[i];
        auto imageIdentify = 
			std::make_shared<ImageIdentify>(label, devList[i]);
        m_imageIdentifies.append(imageIdentify);
        
		
        //相机接管显示信息label
        auto& item = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i];
        imageIdentify->setDisaplayCheckInfo(item->m_labelCheckInfo);
		imageIdentify->m_labelForProductCount = item->m_labelProductCount;
        imageIdentify->m_labelForProductOutCount = item->m_labelProductOutCount;
        imageIdentify->m_labelForProductPassCount = item->m_labelProductPassCount;
        imageIdentify->m_labelForNg = ui->label_ng;
        imageIdentify->setStandDate(ui->ledit_currentDate->text());

		//连接摄像机
        auto connectResult = imageIdentify->connectCamera();
		if (connectResult) {
            item->m_labelDisplayCamera->m_enbaleClicked = true;
		}

		//显示产品名称以及相关信息到界面上
        auto &gboxDisplayCameraItem = m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[i];
		auto &ocrCfg = m_runtimeInfo.m_ocrConfigs.find(devList[i])->second;
        gboxDisplayCameraItem->m_labelProductName->setText(QString::fromStdString(ocrCfg->productName));
        gboxDisplayCameraItem->m_labelProductCount->setText(QString::fromStdString("ProductCount:")+QString::number(ocrCfg->productCheckCount.totalCount));
        gboxDisplayCameraItem->m_labelProductPassCount->setText(QString::fromStdString("ProductPassCount:" ) + QString::number(ocrCfg->productCheckCount.passCount));
        gboxDisplayCameraItem->m_labelProductOutCount->setText(QString::fromStdString("ProductOutCount:" ) + QString::number(ocrCfg->productCheckCount.outCount));


	}

	//初始化相机以及ocr
	for (auto & camera:m_imageIdentifies) {
		camera->IniOcr();

		camera->setSoftwareTriggeredAcquisition();
        camera->m_setIsCheck = false;
        camera->setIsCheckProduct(false);
	}
	for (auto& camera : m_imageIdentifies) {
		camera->startMonitor();
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
	QObject::connect(ui->act_manageProductConfig,&QAction::triggered,
		this,&Proonnx::act_manageProductConfig_triggered);
}

void Proonnx::des_com()
{
	/*delete m_configBeforeRuntimeLoader;
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
	delete m_pbtnSetIsCheckList;*/
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

	ini_readConfig();
	ini_ui();
	ini_connect();

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
    //将运行时的信息传到对话框中并且对话框可以对其做出更改
	LOGRECORDER->info("----------------------------------------------");
	LOGRECORDER->info("Opened the dialog box for modifying product configuration");
	DlgChangeProductConfig dlg;
	dlg.setWindowSize(this->width(), this->height());
	std::string path;

	auto & ip = m_imageIdentifies.at(index)->m_Ip;
    auto readResult = m_runtimeInfo.m_runtimeConfigPtr->readCameraLastRunTimeConfig(ip, path);
	if (readResult) {
		///// refactor
        dlg.setRuntimeInfo(&m_runtimeInfo);
        dlg._ip = ip;
		////
		dlg.setFilePath(QString::fromStdString(path));
		dlg.setCameraIndex(index + 1);
		dlg.setCamera(m_imageIdentifies[index].get());
		dlg.setConfigBeforeRuntime(m_filePathConfigBeforeRuntimeLoader);
		dlg.readRuntimeInfo();
		dlg.setWindowModality(Qt::WindowModal);
		auto dlgResult = dlg.exec();
		if (dlgResult == QDialog::Accepted) {
			auto productName = dlg.getProductName();
            m_displayCameraGroupBox.m_displayCameraGroupBoxItemList[index]->m_labelProductName->setText(productName);
		}
	}
	else {
        assert(false);
	}

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

void Proonnx::act_manageProductConfig_triggered()
{
    DlgManageProductConfig dlg;
    dlg.exec();
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









Proonnx_displayCameraGroupBoxItem::Proonnx_displayCameraGroupBoxItem()
{
    m_labelDisplayCamera = new LabelClickable();
    m_labelProductName = new QLabel();
    m_labelCheckInfo = new QLabel();
    m_labelProductCount = new QLabel();
    m_labelProductPassCount = new QLabel();
    m_labelProductOutCount = new QLabel();
    m_pbtnSetIsCheck = new IndexButton();
}

Proonnx_displayCameraGroupBoxItem::~Proonnx_displayCameraGroupBoxItem()
{
	/*if (m_labelCheckInfo) {
        delete m_labelCheckInfo;
	}
    if (m_labelDisplayCamera) {
        delete m_labelDisplayCamera;
    }
    if (m_labelProductName) {
        delete m_labelProductName;
    }
    if (m_labelProductCount) {
        delete m_labelProductCount;
    }
    if (m_labelProductPassCount) {
        delete m_labelProductPassCount;
    }
    if (m_labelProductOutCount) {
        delete m_labelProductOutCount;
    }
    if (m_pbtnSetIsCheck) {
        delete m_pbtnSetIsCheck;
    }*/
}

void Proonnx_disblayCameraGroupBox::ini(int itemSize)
{
    for (int i = 0; i < itemSize; i++) {
        auto item=new Proonnx_displayCameraGroupBoxItem();
		QSharedPointer<Proonnx_displayCameraGroupBoxItem> itemPtr(item);
        m_displayCameraGroupBoxItemList.append(itemPtr);
    }
}
