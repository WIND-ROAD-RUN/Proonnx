#include "mycamera.h"
#include<QDebug>
#pragma execution_character_set("utf-8")

MV_CC_DEVICE_INFO_LIST m_stDevList;       // ch:设备信息列表结构体变量，用来存储设备列表
MV_CC_DEVICE_INFO* m_Device=NULL;         //设备对象
MyCamera::MyCamera()
{
    m_hDevHandle    = NULL;
}
MyCamera::~MyCamera()
{
    if (m_hDevHandle)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle    = NULL;
    }
}



//查询设备列表
int MyCamera::EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList)
{

    int temp= MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, pstDevList);
    if (MV_OK != temp)
    {
        return -1;
    }
    return 0;
}

//连接相机

//int  MyCamera::connectCamera(string id,QString ms)
//{
//QString mess;
//    int temp= EnumDevices(&m_stDevList);
//    if(temp!=0)
//        //设备更新成功接收命令的返回值为0，返回值不为0则为异常
//        return -1;
//    if(m_stDevList.nDeviceNum==0)
//        //未找到任何相机
//        return 2;
//    for (unsigned int i = 0; i < m_stDevList.nDeviceNum; i++)
//    {
//        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
//        if (NULL == pDeviceInfo)
//        {
//            continue;
//        }

//        if(id== (char*)pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName||id== (char*)pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber)
//        {
//           mess.append("此时相机状态设备号\r\n");
//            qDebug()<<"此时相机状态设备号";
//            m_Device= m_stDevList.pDeviceInfo[i];
//             mess.append((char*)m_Device);
//             qDebug()<<m_Device;

//            qDebug()<<"此时相机状态设备号2";
//             qDebug()<<(char*)pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber;
//              mess.append((char*)pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
//             qDebug()<<"此时相机找到编号";
// mess.append("/r/n");
//              qDebug()<<i;
//            break;
//        }else
//        {
//            continue;
//        }
//    }
//    if(m_Device==NULL)
//    {
//        //未找到指定名称的相机
//        return 3;
//    }
//    unsigned int nAccessMode;


//   bool isok=  MV_CC_IsDeviceAccessible(m_Device, nAccessMode);

//    temp  = MV_CC_CreateHandle(&m_hDevHandle, m_Device);

//    if(temp  !=0)
//    {

//     qDebug()<<"创建相机句柄失败";
//       return -1;
//    }


//    temp  = MV_CC_OpenDevice(m_hDevHandle);
//    // 探测网络最佳包大小（只对GigE相机有效）
//                   if ( m_Device->nTLayerType == MV_GIGE_DEVICE )
//                   {
//                       int nPacketSize = MV_CC_GetOptimalPacketSize(m_hDevHandle);
//                       if (nPacketSize > 0)
//                       {
//                         int  ret = MV_CC_SetIntValue(m_hDevHandle, "GevSCPSPacketSize", nPacketSize);
//                           if ( ret != MV_OK ) { qDebug() << "SetIntValue failed !"; }
//                       }
//                   }

//    if (temp  !=0)
//    {
//        MV_CC_DestroyHandle(m_hDevHandle);
//        m_hDevHandle = NULL;
//       mess.append("打开相机失败/r/n");

//         qDebug()<<"打开相机失败";
//        return -1;
//    }
//    else
//    {
//        //注册回调函数
//        int nRet = MyCamera::RegisterImageCallBack(MyCamera::ImageCallBack,this);
//        mess.append("RegisterImageCallBack\r\n");

//     qDebug()<<"RegisterImageCallBack";
//        //setTriggerMode(0);
//        return 0;
//    }
//    if (m_Device->nTLayerType == MV_GIGE_DEVICE)
//    {
//       //std::cout<<"Gige Camera"<<std::endl;
//    }



//}




int  MyCamera::connectCamera(string id)
{

    int temp= EnumDevices(&m_stDevList);
    if(temp!=0)
        //设备更新成功接收命令的返回值为0，返回值不为0则为异常
        return -1;
    if(m_stDevList.nDeviceNum==0)
        //未找到任何相机
        return 2;
    for (unsigned int i = 0; i < m_stDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (NULL == pDeviceInfo)
        {
            continue;
        }

        if(id== (char*)pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName||id== (char*)pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber)
        {

            m_Device= m_stDevList.pDeviceInfo[i];





            break;
        }else
        {
            continue;
        }
    }
    if(m_Device==NULL)
    {
        //未找到指定名称的相机
        return 3;
    }
    unsigned int nAccessMode{};


   bool isok=  MV_CC_IsDeviceAccessible(m_Device, nAccessMode);

    temp  = MV_CC_CreateHandle(&m_hDevHandle, m_Device);

    if(temp  !=0)
        return -1;

    temp  = MV_CC_OpenDevice(m_hDevHandle);
    // 探测网络最佳包大小（只对GigE相机有效）
    if ( m_Device->nTLayerType == MV_GIGE_DEVICE )
    {
        int nPacketSize = MV_CC_GetOptimalPacketSize(m_hDevHandle);
        if (nPacketSize > 0)
        {
            int  ret = MV_CC_SetIntValue(m_hDevHandle, "GevSCPSPacketSize", nPacketSize);
            if ( ret != MV_OK ) { qDebug() << "SetIntValue failed !"; }
        }
    }
    if (temp  !=0)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = NULL;
        return -1;
    }
    else
    {
        //注册回调函数
        int nRet = MyCamera::RegisterImageCallBack(MyCamera::ImageCallBack,this);

     qDebug()<<"RegisterImageCallBack";
        setTriggerMode(0);
        return 0;
    }
    if (m_Device->nTLayerType == MV_GIGE_DEVICE)
    {
       //std::cout<<"Gige Camera"<<std::endl;
    }



}
//启动相机采集
int MyCamera::startCamera()
{
    int temp=MV_CC_StartGrabbing(m_hDevHandle);
    if(temp!=0)
    {
        return -1;



    }
    else
    {

        //设置位实时采集
        //RunCapture();
        return 0;
    }









}
//发送软触发
int MyCamera::softTrigger()
{
      MV_CC_SetTriggerMode(m_hDevHandle,1);
    int enumValue = MV_CC_SetEnumValue(m_hDevHandle,"TriggerSource",MV_TRIGGER_SOURCE_SOFTWARE);

       if(enumValue != 0){
           qDebug() << "1设置软触发失败1";
           return -1;
       }else {
           qDebug() << "1设置软触发1";
       }
       int comdValue= MV_CC_SetCommandValue(m_hDevHandle, "TriggerSoftware");
       if(comdValue!=0)
       {
           qDebug() << "1软触发失败1";
           return -1;
       }else
       {
           qDebug() << "1软触发一次1";
           return 0;
       }
}
//硬触发
 int MyCamera::HardCap()
 {
     int nRet = MV_CC_SetEnumValue(m_hDevHandle, "LineSelector", 2);
      //0:Line0 1:Line1 2:Line2
      nRet = MV_CC_SetEnumValue(m_hDevHandle, "LineMode", 8);//仅line2需要设置



     MV_CC_SetTriggerMode(m_hDevHandle,1);
   int enumValue = MV_CC_SetEnumValue(m_hDevHandle,"TriggerSource",0);

return 0;



 }
// ch:注册图像数据回调 | en:Register Image Data CallBack
int MyCamera::RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser), void* pUser)
{
    return MV_CC_RegisterImageCallBackForRGB(m_hDevHandle, cbOutput, pUser);
}



//读取相机中的图像
int MyCamera::ReadBuffer(Mat &image)
{
    unsigned int nRecvBufSize = 0;
    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    int temp = MV_CC_GetIntValue(m_hDevHandle, "PayloadSize", &stParam);
    if (temp != 0)
    {
        return -1;
    }
    nRecvBufSize = stParam.nCurValue;
    m_pBufForDriver = (unsigned char *)malloc(nRecvBufSize);
    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
   int tempValue= MV_CC_GetOneFrameTimeout(m_hDevHandle, m_pBufForDriver, nRecvBufSize, &stImageInfo, 700);
    if(tempValue!=0)
    {
        return -1;
    }
    m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;
    m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);

    bool isMono;//判断是否为黑白图像
    switch (stImageInfo.enPixelType)
    {
    case PixelType_Gvsp_Mono8:
    case PixelType_Gvsp_Mono10:
    case PixelType_Gvsp_Mono10_Packed:
    case PixelType_Gvsp_Mono12:
    case PixelType_Gvsp_Mono12_Packed:
        isMono=true;
        break;
    default:
        isMono=false;
        break;
    }

    if(isMono)
    {
      Mat image1=Mat(stImageInfo.nHeight,stImageInfo.nWidth,CV_8UC1,m_pBufForDriver);
      image=image1.clone();

    }
    else
    {
        //转换图像格式为BGR8
        MV_CC_PIXEL_CONVERT_PARAM stConvertParam = {0};
        memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
        stConvertParam.nWidth = stImageInfo.nWidth;                 //ch:图像宽 | en:image width
        stConvertParam.nHeight = stImageInfo.nHeight;               //ch:图像高 | en:image height
        stConvertParam.pSrcData = m_pBufForDriver;                  //ch:输入数据缓存 | en:input data buffer
        stConvertParam.nSrcDataLen = stImageInfo.nFrameLen;         //ch:输入数据大小 | en:input data size
        stConvertParam.enSrcPixelType = stImageInfo.enPixelType;    //ch:输入像素格式 | en:input pixel format
        //stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed; //ch:输出像素格式 | en:output pixel format  适用于OPENCV的图像格式
        stConvertParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed; //ch:输出像素格式 | en:output pixel format
        stConvertParam.pDstBuffer = m_pBufForSaveImage;                    //ch:输出数据缓存 | en:output data buffer
        stConvertParam.nDstBufferSize = m_nBufSizeForSaveImage;            //ch:输出缓存大小 | en:output buffer size
        MV_CC_ConvertPixelType(m_hDevHandle, &stConvertParam);
        Mat image1=Mat(stImageInfo.nHeight,stImageInfo.nWidth,CV_8UC3,m_pBufForSaveImage);
        image=image1.clone();
    }
free(m_pBufForDriver);
free(m_pBufForSaveImage);
    return 0;
}
//设置心跳时间
int MyCamera::setHeartBeatTime(unsigned int time)
{
    //心跳时间最小为500ms
    if(time<500)
        time=500;
    int temp=MV_CC_SetIntValue(m_hDevHandle, "GevHeartbeatTimeout", time);
    if(temp!=0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
//设置曝光时间
int MyCamera::setExposureTime(float ExposureTimeNum)
{
    int temp= MV_CC_SetFloatValue(m_hDevHandle, "ExposureTime",ExposureTimeNum );
    if(temp!=0)
        return -1;
    return 0;
}
//设置相机是否开启触发模式
int MyCamera::setTriggerMode(unsigned int TriggerModeNum)
{
    int nRet = MV_CC_SetTriggerMode(m_hDevHandle,TriggerModeNum);
    if (MV_OK != nRet)
    {
        return -1;
    }

}
//设置为连续采集模式
int MyCamera::RunCapture()
{

   int nRet=setTriggerMode(0);
    return nRet;

}
int MyCamera::closeCamera()
{
    int nRet = MV_OK;
    if (NULL == m_hDevHandle)
    {
       // qDebug() << "没有句柄，不用关闭";
        return -1;
    }
    MV_CC_CloseDevice(m_hDevHandle);
    nRet = MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = NULL;
    return nRet;
}



void __stdcall MyCamera::ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{

    if(pUser)
    {
        MyCamera * cam = (MyCamera*)pUser;
        cam->ImageCallBackInner(pData, pFrameInfo);

    }
}

void MyCamera::ImageCallBackInner(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo)
{

    emit ImgCallBackSignal(pData,pFrameInfo);
}


//设置行高
int  MyCamera::setHeight(unsigned int number)
{

     int temp=MV_CC_SetIntValue(m_hDevHandle, "Height", number);
     return temp;
}

int MyCamera::StartGrabbing()
{

    MV_CC_StartGrabbing(m_hDevHandle);
 return -1;
}

//停止采集
int  MyCamera::StopGrabbing()
{

    int temp=MV_CC_StopGrabbing(m_hDevHandle);
    if(temp!=0)
    {
        return -1;



    }
    else
    {

        //设置位实时采集
        //RunCapture();
        return 0;
    }


}

int MyCamera::GetExposureTime(float &ExposureTimeNum)
{
return 0;
}

int MyCamera::SetPreDivider(int number)
{
    int temp=MV_CC_SetIntValue(m_hDevHandle, "PreDivider", number);
    return temp;
}
int MyCamera::SetMultiplier(int number)
{
    int temp=MV_CC_SetIntValue(m_hDevHandle, "Multiplier", number);
    return temp;
}
int MyCamera::SetPostDivider(int number)
{
    int temp=MV_CC_SetIntValue(m_hDevHandle, "PostDivider", number);
    return temp;

}
int MyCamera::SetUpCapture(int number)
{

  int  nRet = MV_CC_SetEnumValue( m_hDevHandle,"TriggerActivation", number);

     return nRet;
}

bool MyCamera::GetCameraState()
{

   return MV_CC_IsDeviceConnected(m_hDevHandle);
}


int MyCamera::SetGain( float number)
{

     int  nRet;



         nRet = MV_CC_SetGain(m_hDevHandle, number);


    if (MV_OK != nRet)
    {
        qDebug()<<("Set Gain fail! nRet [0x%x]\n");

  }
       return nRet;

}
int MyCamera::SetFrameRate(int value)
{
    int nRet = MV_OK;
   nRet= MV_CC_SetFrameRate(m_hDevHandle,value);

    return nRet;
}
float MyCamera::getExposureTime()
{
    MVCC_FLOATVALUE struFloatValue = {0};
   int nRet = MV_CC_GetFloatValue(m_hDevHandle, "ExposureTime", &struFloatValue);
   return struFloatValue.fCurValue;

}

int MyCamera::setinvert(bool state)
{
MV_CC_SetEnumValue(m_hDevHandle, "LineSelector", 1);
    //0:Input 1:Output 8:Strobe
    //信号输出----------------------------------------------------
   int nRet = MV_CC_SetBoolValue(m_hDevHandle, "LineInverter",state);//电平反转，也可用作IO输出,连续两次翻转可以输出一个IO信号



    return nRet;


}
int MyCamera::setReverseX(bool state)
{
    int nRet = MV_CC_SetBoolValue(m_hDevHandle, "ReverseX",state);



     return nRet;

}


int MyCamera::setReverseY(bool state)
{
    int nRet = MV_CC_SetBoolValue(m_hDevHandle, "ReverseY",state);



     return nRet;

}


void MyCamera::SetRoi(int x1,int y1,int x2,int y2)
{


    int   nRet = MV_CC_SetIntValue(m_hDevHandle,"Width", 10);
 if (MV_OK != nRet)          {
           qDebug()<<"shibai1";
          }
    nRet = MV_CC_SetIntValue(m_hDevHandle,"Height", 20);

 if (MV_OK != nRet)          {
     qDebug()<<"shibai2";
          }
    nRet = MV_CC_SetIntValue(m_hDevHandle,"OffsetX", 1);
 if (MV_OK != nRet)          {
     qDebug()<<"shibai3";
          }
    nRet = MV_CC_SetIntValue(m_hDevHandle,"OffsetY", 2);
 if (MV_OK != nRet)          {
     qDebug()<<"shibai4";
          }
}

//设置心跳时间
int MyCamera::setIOStart( int time)
{

    MV_CC_SetEnumValue(m_hDevHandle, "LineSelector", 1);


    //IO输出做NG信号用
    //参数初始化
    //0:Line0 1:Line1 2:Line2
   uint nRet = MV_CC_SetEnumValue(m_hDevHandle, "LineMode", 8);//仅line2需要设置
    //0:Input 1:Output 8:Strobe
    nRet = MV_CC_SetEnumValue(m_hDevHandle, "LineSource", 5);
    //0:ExposureStartActive 5:SoftTriggerActive  6:HardTriggerActive
    int DurationValue=time*1000,DelayValue=0,PreDelayValue=0;//us
    nRet = MV_CC_SetIntValue(m_hDevHandle, "StrobeLineDuration",DurationValue);
    nRet = MV_CC_SetIntValue(m_hDevHandle, "StrobeLineDelay",DelayValue);//strobe延时，从曝光开始，延时多久输出
    nRet = MV_CC_SetIntValue(m_hDevHandle, "StrobeLinePreDelay",PreDelayValue);//strobe提前输出，曝光延后开始
    nRet = MV_CC_SetBoolValue(m_hDevHandle, "StrobeEnable",true);
    //Strobe输出使能，使能之后，上面配置参数生效，IO输出与LineSource同步
    //-------------------------------------------------------------------------------------
    //输出IO信号命令

    nRet = MV_CC_SetCommandValue(m_hDevHandle, "LineTriggerSoftware");
    return 1;
}



