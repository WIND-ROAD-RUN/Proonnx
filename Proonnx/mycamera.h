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
/*Callback function prototype declaration*/
class MyCamera: public QObject
{
    Q_OBJECT
public:
    MyCamera();
    ~MyCamera();
        //Declare related variables and functions, etc
      static int EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList);
public:
        //Ch: Connect the camera
      int connectCamera(string id);

      //Set whether to trigger mode
      int setTriggerMode(unsigned int TriggerModeNum);

      //Set to continuous acquisition mode
       int RunCapture();
       //Enable camera capture
      int startCamera();

      //Send soft trigger
      int softTrigger();

      //Read buffer
      int ReadBuffer(Mat &image);

      //Enable snapshot capture
      int StartGrabbing();
      //Registration callback
      int RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser), void* pUser);
      //Set heartbeat time
      int setHeartBeatTime(unsigned int time);
      //Set heartbeat time
      int setIOStart(int time);
      //Set gain
      int SetGain(float number);
      //Set row height
      int setHeight(unsigned int number);
      //Stop collecting
      int StopGrabbing();
      //Set exposure time
      int setExposureTime(float ExposureTimeNum);
      //Obtain exposure time
      float getExposureTime();
      //Turn off the camera
      int closeCamera();
      //Obtain images
      int GetImage(Mat& image);
      //Obtain exposure time
      int GetExposureTime(float& ExposureTimeNum);
      //Get connection status
      bool GetCameraState();
      //Set camera voltage level
      int setinvert(bool state);
      //Set camera voltage level
      int setReverseX(bool state);
      //Set camera voltage level
      int setReverseY(bool state);
      //Set three parameters for frequency division
      int SetPreDivider(int number);
      int SetMultiplier(int number);
      int SetPostDivider(int number);
      //Set up up and down extension trigger
      int SetUpCapture(int number);
      int SetFrameRate(int value);
      //Hard trigger
        int HardCap();
        void SetRoi(int x1,int y1,int x2,int y2);

          private:
          unsigned char*  m_pBufForSaveImage;         //Cache for saving images
          unsigned int    m_nBufSizeForSaveImage;

          unsigned char*  m_pBufForDriver;            // Cache used to retrieve images from drivers
          unsigned int    m_nBufSizeForDriver;
           void*   m_hDevHandle;
public:

           bool isConnet;
         QString serialName;

private:
           void static __stdcall ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
           void ImageCallBackInner(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInf);

     signals:
         void ImgCallBackSignal(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInf);

          };



#endif // MYCAMERA_H
