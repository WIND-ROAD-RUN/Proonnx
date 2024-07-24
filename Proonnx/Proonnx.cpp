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
//#include"ocrwork.h"
//#include"opencv/include/opencv2/opencv.hpp"
//#include <opencv/include/opencv2/core/core.hpp>
//#include <opencv/include/opencv2/highgui/highgui.hpp>

#include"DlgAddProductConfig.h"
#include"DlgChangeProductConfig.h"
#include"DlgSetProonnx.h"
#include"DlgSelectCameraIndex.h"
#include"LocalizationStringLoader-XML.h"
#include"ConfigBeforeRuntimeLoader.h"
#include"ProductConfigLoader.h"
#include"DlgSetIsCheckProduct.h"

void Proonnx::ini_ui()
{
	ini_configBeforeRuntimeLoader();
	ini_localizationStringLoader();
	ini_localizationStringLoaderUI();
	ini_gBox_monitoringDisplay();

	ui->ledit_currentDate->setText(QDate::currentDate().toString("yyyy/MM/dd"));
	auto font = ui->ledit_currentDate->font();
	font.setPointSize(20);
	ui->ledit_currentDate->setFont(font);
	ui->ledit_currentDate->setEnabled(false);

	/*   myCamera1 = new MyCamera();
	   o = new ocrwork();
	   InitCamera(1);
	   o->initial();*/
	   /* cv::Mat srcMat = cv::imread(R"(C:\Users\61795\Desktop\Project\Proonnx\Proonnx\image\test4.jpg)");
		 std::vector<OCRResult> ocrResult;
		 o.testOcr(srcMat, ocrResult);
		 o.drawView(srcMat, ocrResult);

		 cv::namedWindow("SrcView", cv::WINDOW_NORMAL);
		 cv::imshow("SrcView", srcMat);
		 cv::waitKey(0);*/
	ini_configBeforeRuntime();
}

void Proonnx::ini_localizationStringLoader()
{
	m_locStrLoader = LocalizationStringLoaderXML::getInstance();
	m_locStrLoader->setFilePath("C:\\Users\\61795\\Desktop\\Project\\Proonnx\\Proonnx\\languageString.xml");
	m_locStrLoader->setLanguage(m_configBeforeRuntimeLoader->readLanguage());
	auto loadStrDataResult = m_locStrLoader->loadData();
	if (!loadStrDataResult) {
		QMessageBox::warning(this, "ERROR", "Failed to load data file languageString.xml");
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

	auto loadResult = m_configBeforeRuntimeLoader->loadFile(filePath.toStdString());
	if (!loadResult) {
		m_configBeforeRuntimeLoader->setNewFile(filePath.toStdString());
	}
}

void Proonnx::ini_configBeforeRuntime()
{
	ini_cameraList();

	if (m_cameraList->size()==1){
		ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("30")));
	}
	else {
		ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("32")));
	}
}

void Proonnx::ini_gBox_monitoringDisplay()
{
	auto cameraCount = m_configBeforeRuntimeLoader->readCameraCount();
	m_disaplayCameraList = new QVector<QLabel*>;
	m_disaplayCheckInfoList= new QVector<QLabel*>;
	m_disaplayProductNameList=new QVector<QLabel*>;
	m_productCountList = new QVector<QLabel*>;
	m_productPassCountList = new QVector<QLabel*>;
	m_productOutCountList = new QVector<QLabel*>;
	m_productCountList = new QVector<QLabel*>;
	QGridLayout* gBox_monitoringDisplayLayout = new QGridLayout(this);

	// Calculate the number of rows and columns to make them as close as possible
	int rows = static_cast<int>(std::sqrt(cameraCount));
	int cols = (cameraCount + rows - 1) / rows; // Round Up

	for (int i = 0; i < cameraCount; i++) {
		QLabel* label = new QLabel;
		label->setText(QString::fromStdString(m_locStrLoader->getString("26")) + QString::number(i + 1));
		label->setAlignment(Qt::AlignCenter); // Center the label text
		label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expand tags to fill the layout
		label->setScaledContents(true); // Fill the image with labels
		m_disaplayCameraList->append(label);
		int row = i / cols;
		int col = i % cols;

		QHBoxLayout* hBoxLayout1=new QHBoxLayout(this);
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

		QVBoxLayout * vBoxLayout = new QVBoxLayout(this);
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
			auto imageIdentify = new ImageIdentify(item, devList[i]);
			imageIdentify->setDisaplayCheckInfo((*m_disaplayCheckInfoList)[i]);
			imageIdentify->m_labelForProductCount = (*m_productCountList)[i];
			imageIdentify->m_labelForProductOutCount = (*m_productOutCountList)[i];
			imageIdentify->m_labelForProductPassCount = (*m_productPassCountList)[i];
			imageIdentify->setStandDate(ui->ledit_currentDate->text());
			imageIdentify->IniOcr();
			auto connectResult = imageIdentify->connectCamera();
			if (connectResult) {
				std::string cameraConfigFilePath;
				auto readResult = m_configBeforeRuntimeLoader->readCameraConfig(devList[i], cameraConfigFilePath);
				if (readResult) {
					imageIdentify->m_productConfigFilePath = cameraConfigFilePath;
				}
				else {
					ProductConfigLoader productConfigLoader;
					ProductConfig newConfig;
					newConfig.productName = "UNDEFINED"+ (devList[i]);
					newConfig.ExposureTime = 10000;
					newConfig.gain = 1;
					QString filePath = "/ProductConfig/";
					auto  currentFilePath = QDir::currentPath();
					filePath = currentFilePath + filePath+ QString::fromStdString(newConfig.productName)+".xml";
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
				imageIdentify->setIsCheckProduct(false);
			}
			else {
				qDebug() << "Disconnected by ip :" + QString::fromStdString(devList[i]);
			}
			m_cameraList->append(imageIdentify);
		}
		else {
			auto imageIdentify = new ImageIdentify(item, "disconnecd");
			m_cameraList->append(imageIdentify);


			item->setText(QString::fromStdString(m_locStrLoader->getString("21")));

		}


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
}

void Proonnx::des_com()
{
	delete m_locStrLoader;
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

	for (auto& item : *m_productOutCountList) {
		delete item;
	}
	delete m_productOutCountList;

	for (auto& item : *m_productPassCountList) {
		delete item;
	}
	delete m_productPassCountList;
}

void Proonnx::set_isCheckProduct(bool is)
{
	for (auto & item:*m_cameraList) {
		item->setIsCheckProduct(is);
	}
}

Proonnx::Proonnx(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::ProonnxClass())
{
	ui->setupUi(this);
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
	for (const auto & item:*m_cameraList) {
		result.append(item->getIsCheckProduct());
	}
	return result;
}

void Proonnx::set_isCheckProductByList(const QVector<bool>& list)
{
	for (int i = 0;i<list.size();++i) {
		(*m_cameraList)[i]->setIsCheckProduct(list[i]);
	}
}

void Proonnx::set_allDoNotCheck()
{
	for (auto & item:(*m_cameraList)) {
		item->setIsCheckProduct(false);
	}
}

void Proonnx::pbt_modProductConfig_clicked()
{
	set_isCheckProduct(false);
	int cameraCount = m_cameraList->size();
	if (cameraCount==1) {
		DlgChangeProductConfig dlg;
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
		DlgSelectCameraIndex dlgSelectCameraIndex(this, cameraCount);
		dlgSelectCameraIndex.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);
		auto selectCareraIndexResult = dlgSelectCameraIndex.exec();

		if (selectCareraIndexResult == QDialog::Accepted) {
			auto cameraIndex = dlgSelectCameraIndex.m_indexIndex;


			DlgChangeProductConfig dlg;
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
	set_isCheckProduct(true);
}

void Proonnx::pbtn_setProonnx_clicked()
{
	DlgSetProonnx dlg;
	dlg.setFilePath(m_configBeforeRuntimeLoaderFilePath);
	dlg.iniComponet();
	dlg.exec();
}

void Proonnx::pbt_setIsCheckProduct_clicked()
{
	if (m_cameraList->size() == 1) {
		if (!m_isSingleCheckProduct) {
			ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("31")));
			set_isCheckProduct(true);
			m_isSingleCheckProduct = true;
		}
		else {
			ui->pbt_setIsCheckProduct->setText(QString::fromStdString(m_locStrLoader->getString("30")));
			set_isCheckProduct(false);
			m_isSingleCheckProduct = false;
		}
	}
	else {
		DlgSetIsCheckProduct dlg;
		auto checkList=get_isCheckProductList();
		set_allDoNotCheck();
		dlg.iniGBox_cameraList(checkList);
		auto dlgResult=dlg.exec();

		if (dlgResult==QDialog::Accepted) {
			auto currentCheckList = dlg.getCurrentIsCheckList();
			set_isCheckProductByList(currentCheckList);
		}
		else {
			auto nativeCheckList = dlg.getNativeIsCheckList();
			set_isCheckProductByList(nativeCheckList);
		}
	}
}

void Proonnx::pbt_addProductCongfig_clicked()
{
	set_isCheckProduct(false);
	int cameraCount = m_cameraList->size();
	if (cameraCount==1) {
		DlgAddProductConfig dlgAddProductConfig(this);

		dlgAddProductConfig.setCameraIndex(1);
		dlgAddProductConfig.setCamera(m_cameraList->at(0));

		dlgAddProductConfig.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);

		auto dlgResult=dlgAddProductConfig.exec();
		if (dlgResult==QDialog::Accepted) {
			auto productName=dlgAddProductConfig.getProductName();
			(*m_disaplayProductNameList)[0]->setText(productName);
		}
	}
	else {
		DlgSelectCameraIndex dlgSelectCameraIndex(this, cameraCount);
		dlgSelectCameraIndex.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);

		auto selectCareraIndexResult = dlgSelectCameraIndex.exec();

		if (selectCareraIndexResult == QDialog::Accepted) {
			auto cameraIndex = dlgSelectCameraIndex.m_indexIndex;

			DlgAddProductConfig dlgAddProductConfig(this);

			dlgAddProductConfig.setCameraIndex(cameraIndex);
			dlgAddProductConfig.setCamera(m_cameraList->at(cameraIndex - 1));

			dlgAddProductConfig.setConfigBeforeRuntime(m_configBeforeRuntimeLoaderFilePath);

			auto dlgResult = dlgAddProductConfig.exec();
			if (dlgResult == QDialog::Accepted) {
				auto productName = dlgAddProductConfig.getProductName();
				(*m_disaplayProductNameList)[cameraIndex-1]->setText(productName);
			}
		}
	}
	set_isCheckProduct(true);
	
}




