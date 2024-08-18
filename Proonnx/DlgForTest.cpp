#include "DlgForTest.h"

#include"oulq/oulq_DialogCustom.h"

#include<thread>
#include<future>
#include<chrono>

#include<QFileDialog>
#include<QMessageBox>
#include"ocrwork.h"
#include<thread>

#include<QDebug>

using namespace rw::oulq;

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

void DlgForTest::ini_ocrwork()
{
    DialogInitialization dlgIni;
    dlgIni.show();
    QApplication::processEvents(); 
    std::promise<ocrwork *> iniPromise;
    std::future<ocrwork*> iniFuture = iniPromise.get_future();

    auto ini_ocrwork = [](std::promise<ocrwork *> && iniPromise) {
        ocrwork* result = new ocrwork();
        result->initial();
        iniPromise.set_value(result);
        };

    std::thread iniOcr(ini_ocrwork,std::move(iniPromise));

    int num = 0;
    while (iniFuture.wait_for(std::chrono::milliseconds(100))!=std::future_status::ready){
        QString text("Initializing");
        if (num>3) {
            num = 0;
        }
        for (int i = 0;i<num;i++ ) {
            text.push_back(".");
        }
        num++;
        dlgIni.setInitializeInfo(text);
        QApplication::processEvents();
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
    m_ocrwork = iniFuture.get();
    iniOcr.join();

    dlgIni.accept();
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
    ini_ocrwork();
    /*m_ocrwork = new ocrwork();
    m_ocrwork->initial();*/
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
