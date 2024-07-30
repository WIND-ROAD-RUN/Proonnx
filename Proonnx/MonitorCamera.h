#ifndef MONITORCAMERA_H_
#define MONITORCAMERA_H_

#include "MVS/Includes/MvCameraControl.h"
#include <iostream>
#include <vector>
#include <string>
#include <QObject> // ��Ӵ�����ʹ�� Qt �źźͲ�

struct MonitorCameraUtility
{
    static std::vector<std::string> checkAllConnectCamera();
};

class MonitorCamera : public QObject { // �̳��� QObject ��ʹ���źźͲ�
    Q_OBJECT // ��Ӵ˺��������źźͲۻ���

private:
    std::string m_ip{};
    void* m_handle{ nullptr }; // ������

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