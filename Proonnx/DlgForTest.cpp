#include "DlgForTest.h"

#include<QFileDialog>
#include<QMessageBox>
#include"ocrwork.h"

DlgForTest::DlgForTest(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgForTestClass())
{
    ui->setupUi(this);
    ini_ui();
    ini_connect();
}

DlgForTest::~DlgForTest()
{
    delete ui;
    delete m_ocrwork;
}

void DlgForTest::ini_connect()
{
    QObject::connect(ui->pbtn_openFile, SIGNAL(clicked()),
        this, SLOT(pbtn_openFile_clicked()));
    QObject::connect(ui->pbtn_recognize, SIGNAL(clicked()),
        this, SLOT(pbtn_recognize_clicked()));
}

void DlgForTest::ini_ui()
{
    m_ocrwork = new ocrwork();
    m_ocrwork->initial();
}

void DlgForTest::pbtn_recognize_clicked()
{
    QMessageBox::information(this,"","");
}

void DlgForTest::pbtn_openFile_clicked()
{
    auto filePath=QFileDialog::getOpenFileName(this,"","","*.jpg");
    if (filePath.size()==0) {
        return;
    }
    ui->lEdit_filePath->setText(filePath);

    QPixmap imagePixmap(filePath);
    imagePixmap = imagePixmap.scaled(ui->label_displayImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_displayImage->setPixmap(imagePixmap);
}
