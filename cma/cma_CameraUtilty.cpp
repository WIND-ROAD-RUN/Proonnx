#include "cma_CameraUtilty.h"

#include"MVS/Includes/MvCameraControl.h"
namespace rw {
    namespace cma {

        std::vector<std::string> CameraUtilty::checkAllConnectCamera()
        {
			std::vector<std::string> cameraIPs;
			MV_CC_DEVICE_INFO_LIST stDeviceList;
			memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

			// List devices
			int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
			if (MV_OK != nRet) {
				return cameraIPs;
			}

			// Check if there is any equipment
			if (stDeviceList.nDeviceNum == 0) {
				return cameraIPs;
			}

			// Traverse the device list and obtain IP addresses
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

    }
}
