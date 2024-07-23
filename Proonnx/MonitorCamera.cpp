#include"MonitorCamera.h"

std::vector<std::string> MonitorCameraUtility::checkAllConnectCamera()
{
    std::vector<std::string> cameraIPs;
    MV_CC_DEVICE_INFO_LIST stDeviceList;
    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ö���豸
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
    if (MV_OK != nRet) {
        return cameraIPs;
    }

    // ����Ƿ����豸
    if (stDeviceList.nDeviceNum == 0) {
        return cameraIPs;
    }

    // �����豸�б���ȡ IP ��ַ
    for (unsigned int i = 0; i < stDeviceList.nDeviceNum; ++i) {
        MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
        if (nullptr == pDeviceInfo) {
            continue;
        }

        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE) {
            unsigned int ip = pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp;
            std::string ipAddress = std::to_string((ip >> 24) & 0xFF) + "." +
                std::to_string((ip >> 16) & 0xFF) + "." +
                std::to_string((ip >> 8) & 0xFF) + "." +
                std::to_string(ip & 0xFF);
            cameraIPs.push_back(ipAddress);
        }
    }

    return cameraIPs;
}

MonitorCamera::MonitorCamera() {
    // ���캯��
}

MonitorCamera::~MonitorCamera() {
    // ����������ȷ���ͷ������Դ
    if (m_handle) {
        MV_CC_CloseDevice(m_handle);
        MV_CC_DestroyHandle(m_handle);
    }
}

bool MonitorCamera::connectCamera() {
    MV_CC_DEVICE_INFO_LIST stDeviceList;
    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ö���豸
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &stDeviceList);
    if (MV_OK != nRet) {
        std::cerr << "Enum Devices failed! nRet = " << nRet << std::endl;
        return false;
    }

    // �����豸�б����ӵ�ָ�� IP �����
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
                // ����������
                nRet = MV_CC_CreateHandle(&m_handle, pDeviceInfo);
                if (MV_OK != nRet) {
                    std::cerr << "Create Handle failed! nRet = " << nRet << std::endl;
                    return false;
                }

                // ���豸
                nRet = MV_CC_OpenDevice(m_handle);
                if (MV_OK != nRet) {
                    std::cerr << "Open Device failed! nRet = " << nRet << std::endl;
                    MV_CC_DestroyHandle(m_handle);
                    m_handle = nullptr;
                    return false;
                }

                // ע��ͼ��ص�����
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

void MonitorCamera::startAcquisition() {
    if (m_handle) {
        int nRet = MV_CC_StartGrabbing(m_handle);
        if (MV_OK != nRet) {
            std::cerr << "Start Grabbing failed! nRet = " << nRet << std::endl;
        }
    }
}

void MonitorCamera::stopAcquisition() {
    if (m_handle) {
        int nRet = MV_CC_StopGrabbing(m_handle);
        if (MV_OK != nRet) {
            std::cerr << "Stop Grabbing failed! nRet = " << nRet << std::endl;
        }
    }
}

void __stdcall MonitorCamera::ImageCallback(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser) {
    MonitorCamera* pThis = static_cast<MonitorCamera*>(pUser);
    if (pThis) {
        emit pThis->ImgCallBackSignal(pData, pFrameInfo);
    }
}