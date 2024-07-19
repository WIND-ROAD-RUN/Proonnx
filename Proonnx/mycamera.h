#ifndef MYCAMERA_H
#define MYCAMERA_H

#include"MVS/Includes/MvCameraControl.h"
#include"iostream"
#include"opencv/include/opencv2/opencv.hpp"
#include <opencv/include/opencv2/core/core.hpp>
#include <opencv/include/opencv2/highgui/highgui.hpp>
#include<functional>
#include <QObject>
using namespace std;
using namespace cv;

class MyCamera;
/*回调函数原型声明*/
class MyCamera: public QObject
{
    Q_OBJECT
public:
    MyCamera();
    ~MyCamera();
      //声明相关变量及函数等
      static int EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList);



public:
      // ch:连接相机
      int connectCamera(string id);

      //设置是否为触发模式
      int setTriggerMode(unsigned int TriggerModeNum);

      //设置为连续采集模式
       int RunCapture();
      //开启相机采集
      int startCamera();

      //发送软触发
      int softTrigger();

      //读取buffer
      int ReadBuffer(Mat &image);

      //开启抓图
      int StartGrabbing();
;
      //注册回调
      int RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser), void* pUser);

      //设置心跳时间
      int setHeartBeatTime(unsigned int time);
      //设置心跳时间
      int setIOStart( int time);
      //设置增益
      int SetGain(float number);
      //设置行高
      int setHeight(unsigned int number);

      //停止采集
      int StopGrabbing();

      //设置曝光时间
      int setExposureTime(float ExposureTimeNum);
      //获取曝光时间
      float getExposureTime();
      //关闭相机
        int closeCamera();

        //获得图片
        int GetImage(Mat &image);

        //获取曝光时间
        int GetExposureTime(float &ExposureTimeNum);

        //获取连接状态
        bool GetCameraState();

        //设置相机电平
        int setinvert(bool state);

        //设置相机电平
        int setReverseX(bool state);
        //设置相机电平
        int setReverseY(bool state);
        //设置分频三个参数
        int SetPreDivider(int number);
        int SetMultiplier(int number);
        int SetPostDivider(int number);
//设置上下延触发
         int SetUpCapture(int number);
         int SetFrameRate(int value);

//硬触发
        int HardCap();
        void SetRoi(int x1,int y1,int x2,int y2);

          private:
          unsigned char*  m_pBufForSaveImage;         // 用于保存图像的缓存
          unsigned int    m_nBufSizeForSaveImage;

          unsigned char*  m_pBufForDriver;            // 用于从驱动获取图像的缓存
          unsigned int    m_nBufSizeForDriver;
           void*   m_hDevHandle;
public:

           bool isConnet;
         QString serialName;

private://私有函数
           void static __stdcall ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
           void ImageCallBackInner(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInf);

     signals:
         void ImgCallBackSignal(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInf);

          };



#endif // MYCAMERA_H
