#include "DlgForTest.h"

#include"oulq/oulq_DialogCustom.h"

#include<thread>
#include<future>
#include<chrono>

#include<QFileDialog>
#include<QMessageBox>
#include"ocrwork.h"
#include<thread>
#include"opencv2/opencv.hpp"

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
}

void DlgForTest::pbtn_recognize_clicked()
{
    ui->pte_testResult->clear();
    auto pixmap = ui->label_displayImage->pixmap(Qt::ReturnByValue); // 使用返回值的重载
    if (!pixmap.isNull()) {
        QImage image = pixmap.toImage();
        cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
        cv::Mat matBGR;
        cv::cvtColor(mat, matBGR, cv::COLOR_RGBA2BGR);

        std::vector<OCRResult> ocrResult;
        m_ocrwork->testOcr(matBGR, ocrResult);

        ui->pte_testResult->appendPlainText("ocr size:"+QString::number(ocrResult.size()));
        for (const auto & item : ocrResult) {
            ui->pte_testResult->appendPlainText("text:"+QString(item.text));
            ui->pte_testResult->appendPlainText("Score"+QString::number(item.score));
            ui->pte_testResult->appendPlainText("-----------");
        }

        QImage image1;
        if (matBGR.type() == CV_8UC1) {
            // 灰度图像
            image1 = QImage(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_Grayscale8);
        }
        else if (matBGR.type() == CV_8UC3) {
            // 彩色图像 (BGR)
            cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB); // 转换为RGB格式
            image1 = QImage(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_RGB888);
        }
        else if (matBGR.type() == CV_8UC4) {
            // 带透明通道的图像 (BGRA)
            image1 = QImage(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
        }

        // 将QImage转换为QPixmap
        QPixmap pixmap = QPixmap::fromImage(image1);

        // 现在可以将pixmap设置到QLabel或其他控件
        ui->label_displayImage->setPixmap(pixmap);
    }
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
