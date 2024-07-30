#ifndef MONITORCAMERA_H_
#define MONITORCAMERA_H_

#include "MVS/Includes/MvCameraControl.h"
#include <iostream>
#include <vector>
#include <string>
#include <QObject> // 添加此行以使用 Qt 信号和槽

struct MonitorCameraUtility
{
    static std::vector<std::string> checkAllConnectCamera();
};

class MonitorCamera : public QObject { // 继承自 QObject 以使用信号和槽
    Q_OBJECT // 添加此宏以启用信号和槽机制

private:
    std::string m_ip{};
    void* m_handle{ nullptr }; // 相机句柄

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

#endif // !MONITORCAMERA_H_