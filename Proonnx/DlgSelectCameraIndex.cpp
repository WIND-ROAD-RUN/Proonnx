#include "DlgSelectCameraIndex.h"


#include<qdebug>
DlgSelectCameraIndex::DlgSelectCameraIndex(QWidget *parent, int cameraCount)
	: QDialog(parent),m_cameraCount(cameraCount)
	, ui(new Ui::DlgSelectCameraIndexClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_connect();
}

DlgSelectCameraIndex::~DlgSelectCameraIndex()
{
	delete ui;
}

void DlgSelectCameraIndex::ini_ui()
{
	qDebug() << m_cameraCount;
	ui->spBox_cameraIndex->setMaximum(m_cameraCount);
	
}

void DlgSelectCameraIndex::ini_connect()
{
	QObject::connect(ui->pbtn_ok, SIGNAL(clicked()),
		this, SLOT(pbtn_ok_clicked()));
	QObject::connect(ui->pbtn_cancel, SIGNAL(clicked()),
		this, SLOT(pbtn_cancel()));
}

void DlgSelectCameraIndex::pbtn_ok_clicked()
{
	m_indexIndex = ui->spBox_cameraIndex->value();
	this->accept();
}

void DlgSelectCameraIndex::pbtn_cancel()
{
	this->reject();
}
