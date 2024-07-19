#include "Proonnx.h"

#include<QMessageBox>
#include<QComboBox>
#include<QFileDialog>

#include"DlgAddProductConfig.h"
#include"DlgAddCamera.h"
#include"DlgChangeCameraConfig.h"
#include"DlgChangeProductConfig.h"

void Proonnx::ini_ui()
{
    ini_localizationStringLoader();
    ini_localizationStringLoaderUI();
    InitCamera(1);
    o.initial();
 /* cv::Mat srcMat = cv::imread(R"(C:\Users\61795\Desktop\Project\Proonnx\Proonnx\image\test4.jpg)");
   std::vector<OCRResult> ocrResult;
   o.testOcr(srcMat, ocrResult);
   o.drawView(srcMat, ocrResult);

   cv::namedWindow("SrcView", cv::WINDOW_NORMAL);
   cv::imshow("SrcView", srcMat);
   cv::waitKey(0);*/

}

void Proonnx::ini_localizationStringLoader()
{
    m_locStrLoader = LocalizationStringLoaderXML::getInstance();
    m_locStrLoader->setFilePath("C:\\Users\\61795\\Desktop\\Project\\Proonnx\\Proonnx\\languageString.xml");
    m_locStrLoader->setLanguage("CHN");
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
    ui->pbt_addCamera->setText(QString::fromStdString(m_locStrLoader->getString("4")));
    ui->pbt_addProductCongfig->setText(QString::fromStdString(m_locStrLoader->getString("5")));
    ui->pbt_modCameraConfig->setText(QString::fromStdString(m_locStrLoader->getString("6")));
}

void Proonnx::ini_connect()
{
    QObject::connect(ui->cBox_changeLanguage, SIGNAL(currentIndexChanged(int)),
        this, SLOT(cBox_changeLanguage_index_change_on(int)));
    QObject::connect(ui->pbt_addProductCongfig, SIGNAL(clicked()),
        this, SLOT(pbt_addProductCongfig_clicked()));
    QObject::connect(ui->pbt_addCamera, SIGNAL(clicked()),
        this, SLOT(pbt_addCamera_clicked()));
    QObject::connect(ui->pbt_modCameraConfig, SIGNAL(clicked()),
        this, SLOT(pbt_modCameraConfig_clicked()));
    QObject::connect(ui->pbt_modProductConfig, SIGNAL(clicked()),
        this, SLOT(pbt_modProductConfig_clicked()));
}

void Proonnx::des_com()
{
    delete m_locStrLoader;
}

Proonnx::Proonnx(QWidget *parent)
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

void Proonnx::pbt_addCamera_clicked()
{
    DlgAddCamera dlg;
    dlg.exec();
}

void Proonnx::pbt_modCameraConfig_clicked()
{
    DlgChangeCameraConfig dlg;
    dlg.exec();
}

void Proonnx::pbt_modProductConfig_clicked()
{
    QFileDialog fileDlg(nullptr, tr("打开文件"), "", tr("数据文件(*.xml);;所有文件 (*)"));
    if (fileDlg.exec() == QFileDialog::Accepted) {
        auto filePath = fileDlg.selectedFiles().first();
        DlgChangeProductConfig dlg;
        dlg.setFilePath(filePath);
        dlg.iniUI();
        dlg.exec();
    }
}

void Proonnx::pbt_addProductCongfig_clicked()
{
    DlgAddProductConfig dlg;
    dlg.exec();

}

void Proonnx::cBox_changeLanguage_index_change_on(int index)
{
    if (index==0) {
        m_locStrLoader->setLanguage("CHN");
        auto loadStrDataResult = m_locStrLoader->loadData();
    }
    else if(index == 1){
        m_locStrLoader->setLanguage("USA");
        auto loadStrDataResult = m_locStrLoader->loadData();
    }
    ini_localizationStringLoaderUI();
}


int Proonnx::RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight)//RGB转BGR
{
    if (NULL == pRgbData)
    {
        return MV_E_PARAMETER;
    }

    for (unsigned int j = 0; j < nHeight; j++)
    {
        for (unsigned int i = 0; i < nWidth; i++)
        {
            unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
            pRgbData[j * (nWidth * 3) + i * 3] = pRgbData[j * (nWidth * 3) + i * 3 + 2];
            pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
        }
    }

    return MV_OK;
}
cv::Mat  Proonnx::Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData, bool& isok)//转Mat格式
{
    cv::Mat srcImage;
    if (pstImageInfo->enPixelType == PixelType_Gvsp_Mono8)
    {
        srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1, pData);
        isok = true;
        return srcImage;
    }
    else if (pstImageInfo->enPixelType == PixelType_Gvsp_RGB8_Packed)
    {
        RGB2BGR(pData, pstImageInfo->nWidth, pstImageInfo->nHeight);
        srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC3, pData);
        //cvtColor(srcImage,srcImage,COLOR_BGR2GRAY);
        isok = true;
        return srcImage;
    }
    else
    {

        //QMessageBox::warning(this,"警告","不支持此类型转换");
        QString str = QString("不支持此类型转换");
        qDebug() << str;

        isok = false;


        return srcImage;
    }

    if (NULL == srcImage.data)
    {
        // QMessageBox::warning(this,"警告","转换图像为空");
        QString str = QString("转换图像为空");
        qDebug() << str;
        isok = false;

        return srcImage;
    }
}
QImage Proonnx::cvMat2QImage(cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1)
    {
        QImage qimage(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        qimage.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            qimage.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar* pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar* pDest = qimage.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return qimage;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if (mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}
//回调得到的图片在这个里面
void Proonnx::DispImgBuff1(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo)
{

    cv::Mat dstImga1;
    bool convertisok = false;
    //将byte类型的数据转换成mat格式
    dstImga1 = Convert2Mat(pFrameInfo, pData, convertisok);

    cv::Mat dstImga2;
    //深拷贝一份数据
    dstImga1.copyTo(dstImga2);

    //进行图像处理
    //将感兴趣的区域分割出来，旋转
    //旋转代码
    //int cishu = (GlobelParam::peifang1[3].toInt() - 1) % 4;
  /*  if (cishu >= 0)
    {
        if (cishu <= 2)
        {
            cv::rotate(dstImga1, dstImga1, cishu);
        }
        else
        {
            cv::rotate(dstImga1, dstImga1, 3);
            cv::rotate(dstImga1, dstImga1, 0);

        }

    }*/


    //分割代码
    //if (GlobelParam::peifang1[4].toDouble() > 0 && GlobelParam::peifang1[5].toDouble() > 0 && GlobelParam::peifang1[6].toDouble() > 0 && GlobelParam::peifang1[7].toDouble() > 0)
    //{

    //    double x1 = GlobelParam::peifang1[4].toDouble();
    //    double y1 = GlobelParam::peifang1[5].toDouble();
    //    double x2 = GlobelParam::peifang1[6].toDouble();
    //    double y2 = GlobelParam::peifang1[7].toDouble();
    //    double mx = 0;
    //    double my = 0;
    //    if (x1 > x2)
    //    {
    //        mx = x1;

    //        x1 = x2;
    //        x2 = mx;
    //    }
    //    if (y1 > y2)
    //    {
    //        my = y1;

    //        y1 = y2;
    //        y2 = my;
    //    }

    //    if (y2 > dstImga1.rows)
    //    {
    //        y2 = dstImga1.rows;

    //    }
    //    if (x2 > dstImga1.cols)
    //    {
    //        x2 = dstImga1.cols;

    //    }
    //    cv::Rect rect1(x1, y1, (x2 - x1), (y2 - y1));//创建一个Rect对象

    //    cv::Mat rectmat;
    //    rectmat = dstImga1(rect1);




    //    rectmat.copyTo(dstImga1);

    //}




    //对这张图片进行字符识别
    std::vector<OCRResult> ocrResult;
    o.testOcr(dstImga2, ocrResult);
    o.drawView(dstImga2, ocrResult);

   //算法执行{}





    //将opencv转换成qimage
    QImage im = cvMat2QImage(dstImga2);


    QPixmap pixmap = QPixmap::fromImage(im);
    pixmap.scaled(ui->label ->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);    //让图片填充满QLabel

    




  

}

bool Proonnx::InitCamera(int index)
{
    MV_CC_DEVICE_INFO_LIST m_stDevList;//设备列表
    //查找连接的相机
    int neRt = myCamera1.EnumDevices(&m_stDevList);
   
    //保存的ip地址
    QString nIp;
    
   
    bool isok = false;



    //获取相机的IP地址
    for (int i = 0; i < m_stDevList.nDeviceNum; i++)
    {

        if (1 == m_stDevList.pDeviceInfo[i]->nTLayerType) {
            int nIp1, nIp2, nIp3, nIp4;
            nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);
            nIp = QString("%1.%2.%3.%4").arg(nIp1).arg(nIp2).arg(nIp3).arg(nIp4);
            //             qDebug() << "nIp";
            //            qDebug() << nIp;
        }
        else
        {
            qDebug() << "没有联上相机";
            nIp = "";
        }


        //如果找到相机则进行连接
        //            if(nIp!="")
        //            {
        
        QStringList   qlist = nIp.split(".");

        //一号相机
        if (qlist[2] == "1" && index == 1)
        {
            string cameraName; //相机名称
            cameraName = (char*)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber;



            int linkCamera;
            //连接相机
            linkCamera = myCamera1.connectCamera(cameraName);



            qDebug() << linkCamera;

            if (linkCamera == 0) {

                //qDebug() << "连接相机成功";

            }
            else {

                //qDebug() << "连接相机失败";

            }
            int satrtCamera;

            //开启抓图
            satrtCamera = myCamera1.startCamera();
            //GlobelParam::myCamera1.HardCap();



            if (satrtCamera != 0) {
                //qDebug() << "启动相机采集失败";

                myCamera1.isConnet = false;





            }
            else
            {

                myCamera1.isConnet = true;
                myCamera1.serialName = QString::fromStdString(cameraName);
               /* ui->lb_CameraState1->setText("连接成功");
                ui->lb_CameraState1->setStyleSheet(QString("QLabel{color:rgb(105, 217,181);} "));*/
                connect(&myCamera1, &MyCamera::ImgCallBackSignal, this, &Proonnx::DispImgBuff1);
                //实时采集
                myCamera1.RunCapture();
                // GlobelParam::myCamera1.setExposureTime(lightNumber[0]);
                isok = true;
            }
        }
      
    }
    return isok;


    //  }






}






