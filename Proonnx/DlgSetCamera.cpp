#include "DlgSetCamera.h"

DlgSetCamera::DlgSetCamera(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgSetCameraClass())
{
	ui->setupUi(this);
	ini_connect();
}

DlgSetCamera::~DlgSetCamera()
{
	delete ui;
}

void DlgSetCamera::ini_connect()
{
	QObject::connect(ui->pbt_addProduct, SIGNAL(clicked()),
		this, SLOT(pbt_addProduct_clicked()));
	QObject::connect(ui->pbt_changeProduct, SIGNAL(clicked()),
		this, SLOT(pbt_changeProduct_clicked()));
	QObject::connect(ui->pbt_clear, SIGNAL(clicked()),
		this, SLOT(pbt_clear_clicked()));
	QObject::connect(ui->pbt_selectProduct, SIGNAL(clicked()),
		this, SLOT(pbt_selectProduct_clicked()));
}

void DlgSetCamera::pbt_addProduct_clicked()
{
	m_select = 0;
	this->accept();
}

void DlgSetCamera::pbt_changeProduct_clicked()
{
	m_select = 1;
	this->accept();
}

void DlgSetCamera::pbt_selectProduct_clicked()
{
	m_select = 2;
	this->accept();
}

void DlgSetCamera::pbt_clear_clicked()
{
	m_select = 3;
	this->accept();
}
