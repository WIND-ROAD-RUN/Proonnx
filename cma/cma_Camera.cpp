#include"cma_Camera.h"

#include"cma_CameraUtilty.h"

#include<qDebug>
#include<iostream>
#include"MVS/Includes/MvCameraControl.h"
#include"opencv2/opencv.hpp"

namespace rw {
    namespace cma {
        Camera_MVS::Camera_MVS() {
        }

        Camera_MVS::~Camera_MVS() {
            if (m_handle) {
                MV_CC_CloseDevice(m_handle);
                MV_CC_DestroyHandle(m_handle);
            }
        }

        bool Camera_MVS::connectCamera() {
            MV_CC_DEVICE_INFO_LIST stDeviceList;
            memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

            //List devices
            int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &stDeviceList);
            if (MV_OK != nRet) {
                std::cerr << "Enum Devices failed! nRet = " << nRet << std::endl;
                return false;
            }

            //Traverse the device list and connect to the camera with the specified IP address
            for (unsigned int i = 0; i < stDeviceList.nDeviceNum; ++i) {
                MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
                if (nullptr == pDeviceInfo) {
                    continue;
                }

                if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE) {
                    unsigned int ip = pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp;
                    std::string currentIpAddress = std::to_string((ip >> 24) & 0xFF) + "." +
                        std::to_string((ip >> 16) & 0xFF) + "." +
                        std::to_string((ip >> 8) & 0xFF) + "." +
                        std::to_string(ip & 0xFF);

                    if (currentIpAddress == m_ip) {
                        //Create camera handle
                        nRet = MV_CC_CreateHandle(&m_handle, pDeviceInfo);
                        if (MV_OK != nRet) {
                            std::cerr << "Create Handle failed! nRet = " << nRet << std::endl;
                            return false;
                        }

                        //Open the device
                        nRet = MV_CC_OpenDevice(m_handle);
                        if (MV_OK != nRet) {
                            std::cerr << "Open Device failed! nRet = " << nRet << std::endl;
                            MV_CC_DestroyHandle(m_handle);
                            m_handle = nullptr;
                            return false;
                        }

                        //Register image callback function
                        nRet = MV_CC_RegisterImageCallBackEx(m_handle, ImageCallback, this);
                        if (MV_OK != nRet) {
                            std::cerr << "Register Image Callback failed! nRet = " << nRet << std::endl;
                            MV_CC_CloseDevice(m_handle);
                            MV_CC_DestroyHandle(m_handle);
                            m_handle = nullptr;
                            return false;
                        }

                        return true;
                    }
                }
            }

            std::cerr << "Camera with IP " << m_ip << " not found." << std::endl;
            return false;
        }

        void Camera_MVS::startAcquisition() {
            if (m_handle) {
                int nRet = MV_CC_StartGrabbing(m_handle);
                if (MV_OK != nRet) {
                    qDebug() << "Start Grabbing failed! nRet = " << nRet;
                }
            }
        }

        void Camera_MVS::stopAcquisition() {
            if (m_handle) {
                int nRet = MV_CC_StopGrabbing(m_handle);
                if (MV_OK != nRet) {
                    std::cerr << "Stop Grabbing failed! nRet = " << nRet << std::endl;
                }
            }
        }

        bool Camera_MVS::setExposureTime(int exposureTime)
        {
            if (m_handle == nullptr) {
                std::cerr << "Camera is not connected." << std::endl;
                return false; //Camera not connected
            }

            //Set exposure time
            MVCC_FLOATVALUE exposureValue;
            exposureValue.fCurValue = exposureTime; //Set exposure time
            exposureValue.fMax = 100000; //Maximum exposure time (adjusted according to camera specifications)
            exposureValue.fMin = 1; //Minimum exposure time (adjusted according to camera specifications)

            //Call SDK function to set exposure time
            int ret = MV_CC_SetFloatValue(m_handle, "ExposureTime", exposureValue.fCurValue);
            if (ret != MV_OK) {
                qDebug() << "Failed to set exposure time. Error code: " << ret;
                return false;//Setting failed
            }

            return true; //Setting successful
        }

        bool Camera_MVS::setGain(int gain)
        {
            if (m_handle == nullptr) {
                std::cerr << "Camera is not connected." << std::endl;
                return false;
            }


            MVCC_FLOATVALUE gainValue;
            gainValue.fCurValue = gain;
            gainValue.fMax = 30;
            gainValue.fMin = 0;


            int ret = MV_CC_SetFloatValue(m_handle, "Gain", gainValue.fCurValue);
            if (ret != MV_OK) {
                std::cerr << "Failed to set gain. Error code: " << ret << std::endl;
                return false;
            }

            return true;
        }

        bool Camera_MVS::setHardwareTriggeredAcquisition()
        {
            int nRet = MV_CC_SetEnumValue(m_handle, "LineSelector", 2);
            //0:Line0 1:Line1 2:Line2
            nRet = MV_CC_SetEnumValue(m_handle, "LineMode", 8);//Only line2 needs to be set



            MV_CC_SetTriggerMode(m_handle, 1);
            int enumValue = MV_CC_SetEnumValue(m_handle, "TriggerSource", 0);

            return 0;



        }

        bool Camera_MVS::setSoftwareTriggeredAcquisition()
        {
            int nRet = MV_CC_SetTriggerMode(m_handle, 0);
            if (MV_OK != nRet)
            {
                return -1;
            }
            return nRet;
        }

        int Camera_MVS::setIOStart(int time)
        {
            MV_CC_SetEnumValue(m_handle, "LineSelector", 1);


            //IO output for NG signal
            //Parameter initialization
            //0:Line0 1:Line1 2:Line2
            uint nRet = MV_CC_SetEnumValue(m_handle, "LineMode", 8);//Only line2 needs to be set
            //0:Input 1:Output 8:Strobe
            nRet = MV_CC_SetEnumValue(m_handle, "LineSource", 5);
            //0:ExposureStartActive 5:SoftTriggerActive  6:HardTriggerActive
            int DurationValue = time * 1000, DelayValue = 0, PreDelayValue = 0;//us
            nRet = MV_CC_SetIntValue(m_handle, "StrobeLineDuration", DurationValue);
            nRet = MV_CC_SetIntValue(m_handle, "StrobeLineDelay", DelayValue);//Strobe delay, starting from exposure, how long is the delay output
            nRet = MV_CC_SetIntValue(m_handle, "StrobeLinePreDelay", PreDelayValue);//Strobe outputs in advance, exposure starts later
            nRet = MV_CC_SetBoolValue(m_handle, "StrobeEnable", true);
            //Strobe output enable, after enabling, the above configuration parameters take effect, IO output is synchronized with LineSource
            //-------------------------------------------------------------------------------------
            //Output IO signal command

            nRet = MV_CC_SetCommandValue(m_handle, "LineTriggerSoftware");
            return 1;
        }


        void __stdcall Camera_MVS::ImageCallback(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser) {
            Camera_MVS* pThis = static_cast<Camera_MVS*>(pUser);
            if (pThis) {
                emit pThis->ImgCallBackSignal(pData, pFrameInfo);
            }
        }

        Camera::Camera() :m_cameraMVS(std::make_shared<Camera_MVS>())
        {
            QObject::connect(this->m_cameraMVS.get(), SIGNAL(ImgCallBackSignal(unsigned char*, MV_FRAME_OUT_INFO_EX*)),
                this, SLOT(monitorImage(unsigned char*, MV_FRAME_OUT_INFO_EX*)));
        }

        Camera::~Camera()
        {

        }
        void Camera::setIp(const std::string& ip)
        {
            m_cameraMVS->setIp(ip);
        }

        std::string Camera::getIp()
        {
            return m_cameraMVS->getIp();
        }

        bool Camera::connectCamera()
        {
            return m_cameraMVS->connectCamera();
        }

        void Camera::startAcquisition()
        {
            m_cameraMVS->startAcquisition();
        }

        void Camera::stopAcquisition()
        {
            m_cameraMVS->stopAcquisition();
        }

        bool Camera::setExposureTime(int exposureTime)
        {
            return m_cameraMVS->setExposureTime(exposureTime);
        }

        bool Camera::setGain(int gain)
        {
            return m_cameraMVS->setGain(gain);
        }

        bool Camera::setHardwareTriggeredAcquisition()
        {
            return m_cameraMVS->setHardwareTriggeredAcquisition();
        }

        bool Camera::setSoftwareTriggeredAcquisition()
        {
            return m_cameraMVS->setSoftwareTriggeredAcquisition();
        }

        int Camera::setIOStart(int time)
        {
            return m_cameraMVS->setIOStart(time);
        }

        void Camera::monitorImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo)
        {
            auto mat = std::make_shared<cv::Mat>(CameraUtilty::ConvertMat(pFrameInfo, pData));
            emit this->ImgCallBackSignal(mat);
        }


    }

}