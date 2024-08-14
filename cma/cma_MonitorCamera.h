#ifndef CMA_MONITORCAMERA_H_
#define CMA_MONITORCAMERA_H_

#include"MVS/Includes/MvCameraControl.h"
#include <iostream>
#include <vector>
#include <string>
#include <QObject> 

struct MonitorCameraUtility
{
    static std::vector<std::string> checkAllConnectCamera();
};

class MonitorCamera : public QObject {
    Q_OBJECT

private:
    std::string m_ip{};
    void* m_handle{ nullptr }; // Camera handle

public:
    MonitorCamera();

    ~MonitorCamera();

    void setIp(const std::string& ip) { m_ip = ip; }

    std::string getIp() { return m_ip; }

    bool connectCamera();

    void startAcquisition();

    void stopAcquisition();

    bool setExposureTime(int exposureTime);

    bool setGain(int gain);

    bool setHardwareTriggeredAcquisition();

    bool setSoftwareTriggeredAcquisition();

    int setIOStart(int time);


signals:
    void ImgCallBackSignal(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo);

private:
    static void __stdcall ImageCallback(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);

};

#endif // !CMA_MONITORCAMERA_H_
