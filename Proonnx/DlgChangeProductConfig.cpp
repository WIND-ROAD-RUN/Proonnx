#include "DlgChangeProductConfig.h"

#include<QMessageBox>

#include"spdlog/spdlog.h"
#include"ProductConfigLoader.h"
#include"FrameSelectLabel.h"
#include"ImageIdentify.h"
#include"LocalizationStringLoader-XML.h"
#include"ConfigBeforeRuntimeLoader.h"
#include"LogRecorder.h"

static LogRecorder* LOGRECORDER = LogRecorder::getInstance();

void DlgChangeProductConfig::setConfigBeforeRuntime(const QString& filePath)
{
	m_configBeforeRuntime = filePath;
}

void DlgChangeProductConfig::setCamera(ImageIdentify* camera)
{
	m_camera = camera;
	m_camera->setDlgLabelForImage(m_frameSelectLabel);
}

DlgChangeProductConfig::DlgChangeProductConfig(QWidget* parent, int cameraIndex)
	: QDialog(parent)
	, ui(new Ui::DlgChangeProductConfigClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_connect();
}

DlgChangeProductConfig::~DlgChangeProductConfig()
{
	if (m_camera) {
		m_camera->deleteDlgLabelForImage();
	}
	delete ui;
	delete m_loader;
	delete m_recognizeRange;
	delete m_frameSelectLabel;
}

void DlgChangeProductConfig::ini_ui()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	ui->lEdit_filePath->setEnabled(false);
	m_frameSelectLabel = new FrameSelectLabel();
	m_frameSelectLabel->setText(QString::fromStdString(loader->getString("21")));
	QVBoxLayout* gBox_dispalyImageLayout = new QVBoxLayout();
	gBox_dispalyImageLayout->addWidget(m_frameSelectLabel);
	ui->gBox_dispalyImage->setLayout(gBox_dispalyImageLayout);
	m_frameSelectLabel->setScaledContents(true);
	ini_localizationStringLoaderUI();
}

void DlgChangeProductConfig::ini_localizationStringLoaderUI()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	ConfigBeforeRuntimeLoader configLoader;
	configLoader.loadFile(m_configBeforeRuntime.toStdString());
	loader->setLanguage(configLoader.readLanguage());

	ui->label_productName->setText(QString::fromStdString(loader->getString("7")));
	ui->label_drawRecognitionRange->setText(QString::fromStdString(loader->getString("8")));
	ui->label_gain->setText(QString::fromStdString(loader->getString("9")));
	ui->pbtn_spinImage->setText(QString::fromStdString(loader->getString("10")));
	ui->pbtn_drawRecognitionRange->setText(QString::fromStdString(loader->getString("11")));
	ui->pbt_saveProductConfig->setText(QString::fromStdString(loader->getString("12")));
	ui->label_filePath->setText(QString::fromStdString(loader->getString("13")));
	ui->label_disposalTime->setText(QString::fromStdString(loader->getString("39")));
	ui->label_delayInRejection->setText(QString::fromStdString(loader->getString("38")));
	ui->label_numberOffsets->setText(QString::fromStdString(loader->getString("37")));
	this->setWindowTitle(QString::fromStdString(loader->getString("6")));
}

void DlgChangeProductConfig::ini_connect()
{
	QObject::connect(ui->pbt_saveProductConfig, SIGNAL(clicked()),
		this, SLOT(pbt_saveProductConfig_clicked()));
	QObject::connect(ui->pbtn_spinImage, SIGNAL(clicked()),
		this, SLOT(pbtn_spinImage_clicked()));
	QObject::connect(ui->pbtn_drawRecognitionRange, SIGNAL(clicked()),
		this, SLOT(pbtn_drawRecognitionRange_clicked()));
	QObject::connect(m_frameSelectLabel, SIGNAL(selectionMade(const QRect&)),
		this, SLOT(selectionMade_complete(const QRect&)));
	QObject::connect(ui->sBox_exposureTime, SIGNAL(valueChanged(int))
		, this, SLOT(sBox_exposureTime_value_change(int)));
	QObject::connect(ui->sBox_gain, SIGNAL(valueChanged(int))
		, this, SLOT(sBox_gain_value_change(int)));
	QObject::connect(ui->pbtn_updateCamera, SIGNAL(clicked()),
		this, SLOT(pbtn_updateCamera_clicked()));
	
	
}

void DlgChangeProductConfig::ini_configLoader()
{
	m_loader = new ProductConfigLoader();
	m_recognizeRange = new RecognizeRange();
	auto productConfig = m_loader->loadProductConfig(m_filePath.toStdString());
	auto RejectAttributeCongif = m_loader->loadRejectAttribute(m_filePath.toStdString());
	ui->lEdit_productName->setText(QString::fromStdString(productConfig.productName));
	ui->sBox_exposureTime->setValue(productConfig.ExposureTime);
	ui->sBox_gain->setValue(productConfig.gain);
	ui->sBox_delayInRejection->setValue(RejectAttributeCongif.RejectDelay);
	ui->sBox_disposalTime->setValue(RejectAttributeCongif.DisposalTime);
	ui->sBox_numberOffsets->setValue(RejectAttributeCongif.OffsetsNumber);
	m_rotateCount = productConfig.rotateCount;
}

void DlgChangeProductConfig::iniUI()
{
	ini_configLoader();
	ui->lEdit_filePath->setText(m_filePath);

	auto productConfig = m_loader->loadProductConfig(m_filePath.toStdString());
	m_recognizeRange->leftLowerCorner = productConfig.leftLowerCorner;
	m_recognizeRange->lowerRightCorner = productConfig.lowerRightCorner;
	m_recognizeRange->topLeftCorner = productConfig.topLeftCorner;
	m_recognizeRange->upperRightCorner = productConfig.upperRightCorner;

}

void DlgChangeProductConfig::setWindowSize(int wide, int height)
{
	this->setFixedSize(wide, height);
}

void DlgChangeProductConfig::closeEvent(QCloseEvent* event)
{
	auto loader = LocalizationStringLoaderXML::getInstance();

	if (QMessageBox::question(this, QString::fromStdString(loader->getString("42")), QString::fromStdString(loader->getString("43")),
		QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
		event->accept();
	}
	else {
		this->pbt_saveProductConfig_clicked();
		event->accept(); 
	}
}
void DlgChangeProductConfig::pbt_saveProductConfig_clicked()
{

	auto loader = LocalizationStringLoaderXML::getInstance();
	if (ui->lEdit_productName->text().size() == 0) {
		QMessageBox::warning(this, QString::fromStdString(loader->getString("34")), QString::fromStdString(loader->getString("35")));
		return;
	}


	LOGRECORDER->info("Add product config at path:" + m_filePath.toStdString());
	LOGRECORDER->info("And the save data is next :");

	ProductConfigLoader configLoader;
	configLoader.setNewFile(m_filePath.toStdString());

	ProductConfig config;
	RecognizeRange recognizeRange;
	config.gain = ui->sBox_gain->value();
	config.productName = ui->lEdit_productName->text().toStdString();
	config.rotateCount = m_rotateCount;
	config.ExposureTime = ui->sBox_exposureTime->value();
	config.topLeftCorner = m_recognizeRange->topLeftCorner;
	recognizeRange.topLeftCorner= m_recognizeRange->topLeftCorner;
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

	auto storeRejectAttributeResult = configLoader.storeRejectAttribute(rejectAttribute);
	auto storeConfigResult = configLoader.storeConfig(config);
	auto saveConfigResult = configLoader.saveFile(m_filePath.toStdString());
	if (storeConfigResult && saveConfigResult&& storeRejectAttributeResult) {
		LOGRECORDER->info("Save successfulls");
	}
	else {
		QMessageBox::warning(this, QString::fromStdString(loader->getString("12")), QString::fromStdString(loader->getString("25")));
		LOGRECORDER->error("Save failed!!!!");
	}

	this->accept();
}

void DlgChangeProductConfig::pbtn_spinImage_clicked()
{
	m_rotateCount++;
	m_rotateCount = m_rotateCount % 4;
	m_camera->setRotateCount(m_rotateCount);

	LOGRECORDER->info("Spin image");
}

void DlgChangeProductConfig::pbtn_drawRecognitionRange_clicked()
{
	int labelWidth = m_frameSelectLabel->width();
	int labelHeight = m_frameSelectLabel->height();

	QRect lastRange;
	lastRange.setBottomLeft
	(QPoint(
		(int)(m_recognizeRange->leftLowerCorner.first * labelWidth),
		(int)(m_recognizeRange->leftLowerCorner.second * labelHeight)));

	lastRange.setBottomRight
	(QPoint(
		(int)(m_recognizeRange->lowerRightCorner.first * labelWidth),
		(int)(m_recognizeRange->lowerRightCorner.second * labelHeight)));

	lastRange.setTopLeft
	(QPoint(
		(int)(m_recognizeRange->topLeftCorner.first * labelWidth),
		(int)(m_recognizeRange->topLeftCorner.second * labelHeight)));

	lastRange.setTopRight
	(QPoint(
		(int)(m_recognizeRange->upperRightCorner.first * labelWidth),
		(int)(m_recognizeRange->upperRightCorner.second * labelHeight)));

	m_frameSelectLabel->setLastSelectionRect(lastRange);
	m_frameSelectLabel->paintLastRange();
	m_frameSelectLabel->enableSelection(true);
}

void DlgChangeProductConfig::sBox_exposureTime_value_change(int)
{

	LOGRECORDER->info("Set exposire time is:" + std::to_string(ui->sBox_exposureTime->value()));
}

void DlgChangeProductConfig::sBox_gain_value_change(int)
{

	LOGRECORDER->info("Set gain is:" + std::to_string(ui->sBox_gain->value()));
}

void DlgChangeProductConfig::pbtn_updateCamera_clicked()
{
	m_camera->stopAcquisition();
	m_camera->setGain(ui->sBox_gain->value());
	m_camera->setExposureTime(ui->sBox_exposureTime->value());
	m_camera->startAcquisition();
}

void DlgChangeProductConfig::selectionMade_complete(const QRect& rect)
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
