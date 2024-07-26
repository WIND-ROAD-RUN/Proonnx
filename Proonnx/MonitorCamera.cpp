#include"MonitorCamera.h"

#include<qDebug>

std::vector<std::string> MonitorCameraUtility::checkAllConnectCamera()
{
	std::vector<std::string> cameraIPs;
	MV_CC_DEVICE_INFO_LIST stDeviceList;
	memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

	// 枚举设备
	int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
	if (MV_OK != nRet) {
		return cameraIPs;
	}

	// 检查是否有设备
	if (stDeviceList.nDeviceNum == 0) {
		return cameraIPs;
	}

	// 遍历设备列表并获取 IP 地址
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
	// 构造函数
}

MonitorCamera::~MonitorCamera() {
	// 析构函数，确保释放相机资源
	if (m_handle) {
		MV_CC_CloseDevice(m_handle);
		MV_CC_DestroyHandle(m_handle);
	}
}

bool MonitorCamera::connectCamera() {
	MV_CC_DEVICE_INFO_LIST stDeviceList;
	memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

	// 枚举设备
	int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &stDeviceList);
	if (MV_OK != nRet) {
		std::cerr << "Enum Devices failed! nRet = " << nRet << std::endl;
		return false;
	}

	// 遍历设备列表并连接到指定 IP 的相机
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
				// 创建相机句柄
				nRet = MV_CC_CreateHandle(&m_handle, pDeviceInfo);
				if (MV_OK != nRet) {
					std::cerr << "Create Handle failed! nRet = " << nRet << std::endl;
					return false;
				}

				// 打开设备
				nRet = MV_CC_OpenDevice(m_handle);
				if (MV_OK != nRet) {
					std::cerr << "Open Device failed! nRet = " << nRet << std::endl;
					MV_CC_DestroyHandle(m_handle);
					m_handle = nullptr;
					return false;
				}

				// 注册图像回调函数
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
			qDebug() << "Start Grabbing failed! nRet = " << nRet;
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

bool MonitorCamera::setExposureTime(int exposureTime)
{
	if (m_handle == nullptr) {
		std::cerr << "Camera is not connected." << std::endl;
		return false; // 相机未连接
	}

	// 设置曝光时间
	MVCC_FLOATVALUE exposureValue;
	exposureValue.fCurValue = exposureTime; // 设置曝光时间
	exposureValue.fMax = 100000; // 最大曝光时间（根据相机规格调整）
	exposureValue.fMin = 1; // 最小曝光时间（根据相机规格调整）

	// 调用 SDK 函数设置曝光时间
	int ret = MV_CC_SetFloatValue(m_handle, "ExposureTime", exposureValue.fCurValue);
	if (ret != MV_OK) {
		qDebug() << "Failed to set exposure time. Error code: " << ret;
		return false; // 设置失败
	}

	return true; // 设置成功
}

bool MonitorCamera::setGain(int gain)
{
	if (m_handle == nullptr) {
		std::cerr << "Camera is not connected." << std::endl;
		return false; // 相机未连接
	}

	// 设置增益
	MVCC_FLOATVALUE gainValue;
	gainValue.fCurValue = gain; // 设置增益值
	gainValue.fMax = 30; // 最大增益（根据相机规格调整）
	gainValue.fMin = 0; // 最小增益（根据相机规格调整）

	// 调用 SDK 函数设置增益
	int ret = MV_CC_SetFloatValue(m_handle, "Gain", gainValue.fCurValue);
	if (ret != MV_OK) {
		std::cerr << "Failed to set gain. Error code: " << ret << std::endl;
		return false; // 设置失败
	}

	return true; // 设置成功
}

bool MonitorCamera::setHardwareTriggeredAcquisition()
{
	int nRet = MV_CC_SetEnumValue(m_handle, "LineSelector", 2);
	//0:Line0 1:Line1 2:Line2
	nRet = MV_CC_SetEnumValue(m_handle, "LineMode", 8);//仅line2需要设置



	MV_CC_SetTriggerMode(m_handle, 1);
	int enumValue = MV_CC_SetEnumValue(m_handle, "TriggerSource", 0);

	return 0;



}

bool MonitorCamera::setSoftwareTriggeredAcquisition()
{
	int nRet = MV_CC_SetTriggerMode(m_handle, 0);
	if (MV_OK != nRet)
	{
		return -1;
	}
	return nRet;
}

int MonitorCamera::setIOStart(int time)
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


void __stdcall MonitorCamera::ImageCallback(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser) {
	MonitorCamera* pThis = static_cast<MonitorCamera*>(pUser);
	if (pThis) {
		emit pThis->ImgCallBackSignal(pData, pFrameInfo);
	}
}