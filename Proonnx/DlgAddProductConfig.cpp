#include "DlgAddProductConfig.h"

#include<QMessageBox>
#include<QFileDialog>
#include<QGroupBox>

#include"FrameSelectLabel.h"
#include"ProductConfigLoader.h"
#include"ImageIdentify.h"
#include"ConfigBeforeRuntimeLoader.h"
#include"LocalizationStringLoader-XML.h"
#include"LogRecorder.h"

static LogRecorder* LOGRECORDER = LogRecorder::getInstance();

DlgAddProductConfig::DlgAddProductConfig(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgAddProductConfigClass())
{
	ui->setupUi(this);
	m_recognizeRange = new RecognizeRange();
	ini_ui();
	ini_connect();
}

DlgAddProductConfig::~DlgAddProductConfig()
{
	m_camera->deleteDlgLabelForImage();
	delete ui;
	delete m_frameSelectLabel;
	delete m_recognizeRange;
}

void DlgAddProductConfig::setCamera(ImageIdentify* camera)
{
	m_camera = camera;
	m_camera->setDlgLabelForImage(m_frameSelectLabel);
	ui->sBox_exposureTime->setValue(m_camera->getExposureTime())  ;
	ui->sBox_gain->setValue(m_camera->getGain());
	m_rotateCount = m_camera->getRotateCont();
}

void DlgAddProductConfig::setConfigBeforeRuntime(const QString& filePath)
{
	m_configBeforeRuntime = filePath;
}

void DlgAddProductConfig::ini_ui()
{
	m_frameSelectLabel = new FrameSelectLabel;
	auto loader = LocalizationStringLoaderXML::getInstance();
	m_frameSelectLabel->setText(QString::fromStdString(loader->getString("21")));
	QVBoxLayout* gBox_dispalyImageLayout = new QVBoxLayout();
	gBox_dispalyImageLayout->addWidget(m_frameSelectLabel);
	ui->gBox_dispalyImage->setLayout(gBox_dispalyImageLayout);
	m_frameSelectLabel->setScaledContents(true);
	ini_localizationStringLoaderUI();
}

void DlgAddProductConfig::ini_localizationStringLoaderUI()
{
	auto loader=LocalizationStringLoaderXML::getInstance();
	ConfigBeforeRuntimeLoader configLoader;
	configLoader.loadFile(m_configBeforeRuntime.toStdString());
	loader->setLanguage(configLoader.readLanguage());

	ui->label_productName->setText(QString::fromStdString(loader->getString("7")));
	ui->label_drawRecognitionRange->setText(QString::fromStdString(loader->getString("8")));
	ui->label_gain->setText(QString::fromStdString(loader->getString("9")));
	ui->pbtn_spinImage->setText(QString::fromStdString(loader->getString("10")));
	ui->pbtn_drawRecognitionRange->setText(QString::fromStdString(loader->getString("11")));
	ui->pbt_saveProductConfig->setText(QString::fromStdString(loader->getString("12")));
	ui->label_disposalTime->setText(QString::fromStdString(loader->getString("39")));
	ui->label_delayInRejection->setText(QString::fromStdString(loader->getString("38")));
	ui->label_numberOffsets->setText(QString::fromStdString(loader->getString("37")));
	this->setWindowTitle(QString::fromStdString(loader->getString("5")));
	
}

void DlgAddProductConfig::ini_connect()
{
	QObject::connect(ui->pbt_saveProductConfig, SIGNAL(clicked()),
		this, SLOT(pbt_saveProductConfig_clicked()));
	QObject::connect(ui->pbtn_spinImage, SIGNAL(clicked()),
		this, SLOT(pbtn_spinImage_clicked()));
	QObject::connect(ui->pbtn_drawRecognitionRange, SIGNAL(clicked()),
		this, SLOT(pbtn_drawRecognitionRange_clicked()));
	QObject::connect(m_frameSelectLabel, SIGNAL(selectionMade(const QRect & )),
		this, SLOT(selectionMade_complete(const QRect & )));
	QObject::connect(ui->sBox_exposureTime, SIGNAL(valueChanged(int))
		, this, SLOT(sBox_exposureTime_value_change(int)));
	QObject::connect(ui->sBox_gain, SIGNAL(valueChanged(int))
		, this, SLOT(sBox_gain_value_change(int)));
}

void DlgAddProductConfig::setWindowSize(int wide, int height)
{
	this->setFixedSize(wide, height);
}

void DlgAddProductConfig::selectionMade_complete(const QRect& rect)
{
	int labelWidth = m_frameSelectLabel->width();
	int labelHeight = m_frameSelectLabel->height();

	{
		auto x = (double)(rect.bottomLeft().x()) / labelWidth;
		if (x > 1) {
			x = 0.99;
		}
		auto y = (double)(rect.bottomLeft().y()) / (double)(labelHeight);
		if (y > 1) {
			y = 0.99;
		}
		m_recognizeRange->leftLowerCorner = { x,y };
	}

	{
		auto x = (double)(rect.topLeft().x()) / labelWidth;
		if (x > 1) {
			x = 0.99;
		}
		auto y = (double)(rect.topLeft().y()) / (double)(labelHeight);
		if (y > 1) {
			y = 0.99;
		}
		m_recognizeRange->topLeftCorner = { x,y };
	}

	{
		auto x = (double)(rect.bottomRight().x()) / labelWidth;
		if (x > 1) {
			x = 0.99;
		}
		auto y = (double)(rect.bottomRight().y()) / (double)(labelHeight);
		if (y > 1) {
			y = 0.99;
		}
		m_recognizeRange->lowerRightCorner = { x,y };
	}

	{
		auto x = (double)(rect.topRight().x()) / labelWidth;
		if (x > 1) {
			x = 0.99;
		}
		auto y = (double)(rect.topRight().y()) / (double)(labelHeight);
		if (y > 1) {
			y = 0.99;
		}
		m_recognizeRange->upperRightCorner = { x,y };
	}
	
	LOGRECORDER->info("Draw recognize range :");
	LOGRECORDER->info(*m_recognizeRange);

	m_frameSelectLabel->enableSelection(false);
}

void DlgAddProductConfig::sBox_exposureTime_value_change(int)
{
	m_camera->stopAcquisition();
	m_camera->setExposureTime(ui->sBox_exposureTime->value());
	m_camera->startAcquisition();

	LOGRECORDER->info("Set exposire time is:"+std::to_string(ui->sBox_exposureTime->value()));
}

void DlgAddProductConfig::sBox_gain_value_change(int)
{
	m_camera->stopAcquisition();
	m_camera->setGain(ui->sBox_gain->value());
	m_camera->startAcquisition();

	LOGRECORDER->info("Set gain is:" + std::to_string(ui->sBox_gain->value()));
}

void DlgAddProductConfig::pbtn_spinImage_clicked()
{
	m_rotateCount++;
	m_rotateCount = m_rotateCount % 4;
	m_camera->setRotateCount(m_rotateCount);

	LOGRECORDER->info("Spin image");
}

void DlgAddProductConfig::pbtn_drawRecognitionRange_clicked()
{
	m_frameSelectLabel->enableSelection(true);
}

void DlgAddProductConfig::pbt_saveProductConfig_clicked()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	if (ui->lEdit_productName->text().size()==0) {
		QMessageBox::warning(this, QString::fromStdString(loader->getString("34")),QString::fromStdString(loader->getString("35")));
		return;
	}

	
	QFileDialog 
		fileDlg(this, QString::fromStdString(loader->getString("22")),
			"", QString::fromStdString(loader->getString("23")));
	if (fileDlg.exec() == QFileDialog::Accepted) {
		auto fileName = fileDlg.selectedFiles().first();

		LOGRECORDER->info("Add product config at path:" + fileName.toStdString());
		LOGRECORDER->info("And the save data is next :");

		ProductConfigLoader configLoader;
		configLoader.setNewFile(fileName.toStdString());

		ProductConfig config;
		RecognizeRange recognizeRange;
		config.gain = ui->sBox_gain->value();
		config.productName = ui->lEdit_productName->text().toStdString();
		config.rotateCount = m_rotateCount;
		config.ExposureTime = ui->sBox_exposureTime->value();
		config.topLeftCorner = m_recognizeRange->topLeftCorner;
		recognizeRange.topLeftCorner = m_recognizeRange->topLeftCorner;
		config.leftLowerCorner = m_recognizeRange->leftLowerCorner;
		recognizeRange.leftLowerCorner = m_recognizeRange->leftLowerCorner;
		config.upperRightCorner = m_recognizeRange->upperRightCorner;
		recognizeRange.upperRightCorner = m_recognizeRange->upperRightCorner;
		config.lowerRightCorner = m_recognizeRange->lowerRightCorner;
		recognizeRange.lowerRightCorner = m_recognizeRange->lowerRightCorner;

		LOGRECORDER->info(config);
		LOGRECORDER->info(recognizeRange);

		m_camera->setRecognizeRange(recognizeRange);

		RejectAttribute rejectAttribute;
		rejectAttribute.DisposalTime = ui->sBox_disposalTime->value();
		rejectAttribute.OffsetsNumber = ui->sBox_numberOffsets->value();
		rejectAttribute.RejectDelay = ui->sBox_delayInRejection->value();
		m_camera->setRejectAttribute(rejectAttribute);
		LOGRECORDER->info(rejectAttribute);

		ConfigBeforeRuntimeLoader configBeforeRuntimeLoader;
		configBeforeRuntimeLoader.loadFile(m_configBeforeRuntime.toStdString());
		configBeforeRuntimeLoader.storeCameraConfig(m_camera->m_Ip, fileName.toStdString());
		configBeforeRuntimeLoader.saveFile(m_configBeforeRuntime.toStdString());

		auto storeRejectAttributeResult = configLoader.storeRejectAttribute(rejectAttribute);
		auto storeConfigResult = configLoader.storeConfig(config);
		auto saveConfigResult = configLoader.saveFile(fileName.toStdString());

		if (storeConfigResult&& saveConfigResult&& storeRejectAttributeResult) {
			QMessageBox::information(this, QString::fromStdString(loader->getString("12")), QString::fromStdString(loader->getString("24")));
			LOGRECORDER->info("Save successfulls");
			this->accept();
		}
		else {
			QMessageBox::warning(this, QString::fromStdString(loader->getString("12")), QString::fromStdString(loader->getString("25")));
			LOGRECORDER->error("Save failed!!!!");
		}


	}
}
