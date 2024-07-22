#include"ImageIdentify.h"

ImageIdentify::ImageIdentify()
{
	m_camera = new MyCamera();
	m_indentModel = new ocrwork();
	m_labelForImage = new QLabel();
}

ImageIdentify::~ImageIdentify()
{
	delete m_camera;
	delete m_indentModel;
	delete m_labelForImage;
}

bool ImageIdentify::InitCamera(int index)
{
	MV_CC_DEVICE_INFO_LIST m_stDevList;//�豸�б�
	//�������ӵ����
	int neRt = m_camera->EnumDevices(&m_stDevList);

	//�����ip��ַ
	QString nIp;


	bool isok = false;



	//��ȡ�����IP��ַ
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
			qDebug() << "û���������";
			nIp = "";
		}


		//����ҵ�������������
		//            if(nIp!="")
		//            {

		QStringList   qlist = nIp.split(".");

		//һ�����
		if (qlist[2] == "1" && index == 1)
		{
			string cameraName; //�������
			cameraName = (char*)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber;



			int linkCamera;
			//�������
			linkCamera = m_camera->connectCamera(cameraName);



			qDebug() << linkCamera;

			if (linkCamera == 0) {

				//qDebug() << "��������ɹ�";

			}
			else {

				//qDebug() << "�������ʧ��";

			}
			int satrtCamera;

			//����ץͼ
			satrtCamera = m_camera->startCamera();
			//GlobelParam::myCamera1.HardCap();



			if (satrtCamera != 0) {
				//qDebug() << "��������ɼ�ʧ��";

				m_camera->isConnet = false;





			}
			else
			{

				m_camera->isConnet = true;
				m_camera->serialName = QString::fromStdString(cameraName);
				/* ui->lb_CameraState1->setText("���ӳɹ�");
				 ui->lb_CameraState1->setStyleSheet(QString("QLabel{color:rgb(105, 217,181);} "));*/
				connect(m_camera, &MyCamera::ImgCallBackSignal, this, &ImageIdentify::DispImgBuff);
				//ʵʱ�ɼ�
				m_camera->RunCapture();
				// GlobelParam::myCamera1.setExposureTime(lightNumber[0]);
				isok = true;
			}
		}

	}
	return isok;

}

bool ImageIdentify::iniCom(int index)
{
	auto iniCameraResult=InitCamera(index);
	if (!iniCameraResult) {
		return false;
	}
	m_indentModel->initial();
}

void ImageIdentify::iniProductCheck(int m_kConsecutiveErrorThreshold, ProductCheck<std::vector<OCRResult>, QString>::Compare compare)
{
	m_productCheck = new ProductCheck<std::vector<OCRResult>, QString>(m_kConsecutiveErrorThreshold, compare);
}

void ImageIdentify::DispImgBuff(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInf)
{
	cv::Mat dstImga1;
	bool convertisok = false;
	//��byte���͵�����ת����mat��ʽ
	dstImga1 = ImageIdentifyUtilty::Convert2Mat(pFrameInf, pData, convertisok);

	cv::Mat dstImga2;
	//���һ������
	dstImga1.copyTo(dstImga2);

	//����ͼ����
	//������Ȥ������ָ��������ת
	//��ת����
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


	//�ָ����
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
	//    cv::Rect rect1(x1, y1, (x2 - x1), (y2 - y1));//����һ��Rect����

	//    cv::Mat rectmat;
	//    rectmat = dstImga1(rect1);




	//    rectmat.copyTo(dstImga1);

	//}




	//������ͼƬ�����ַ�ʶ��
	std::vector<OCRResult> ocrResult;
	m_indentModel->testOcr(dstImga2, ocrResult);
	m_indentModel->drawView(dstImga2, ocrResult);

	//�㷨ִ��{}
	auto result = m_productCheck->check(ocrResult, m_standDate);

	bool checkResult;
	if (result == ProductCheckUtilty::ProductCheckInfo::WITHIN_THRESHOLD) {
		checkResult = true;
	}
	else {
		checkResult = false;
	}




	 //��opencvת����qimage
	QImage im = ImageIdentifyUtilty::cvMat2QImage(dstImga2);


	QPixmap pixmap = QPixmap::fromImage(im);
	pixmap.scaled(m_labelForImage->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	m_labelForImage->setPixmap(pixmap);
	m_labelForImage->setScaledContents(true);    //��ͼƬ�����QLabel
}

int 
ImageIdentifyUtilty::RGB2BGR
(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight)
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

cv::Mat 
ImageIdentifyUtilty::Convert2Mat
(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData, bool& isok)
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

		//QMessageBox::warning(this,"����","��֧�ִ�����ת��");
		QString str = QString("��֧�ִ�����ת��");
		qDebug() << str;

		isok = false;


		return srcImage;
	}

	if (NULL == srcImage.data)
	{
		// QMessageBox::warning(this,"����","ת��ͼ��Ϊ��");
		QString str = QString("ת��ͼ��Ϊ��");
		qDebug() << str;
		isok = false;

		return srcImage;
	}
}

QImage ImageIdentifyUtilty::cvMat2QImage(cv::Mat& mat)
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
