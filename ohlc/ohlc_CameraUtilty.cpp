#include"ohlc_CameraUtilty.h"

#include"MVS/Includes/MvCameraControl.h"
#include"opencv2/opencv.hpp"

#include<qdebug>

namespace rw {
    namespace ohlc {
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

		cv::Mat CameraUtilty::ConvertMat(MV_FRAME_OUT_INFO_EX* pFrameInfo, unsigned char* pData)
		{
			cv::Mat mat;
			switch (pFrameInfo->enPixelType) {
			case PixelType_Gvsp_RGB8_Packed:
				mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
				cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB); // convert color format
				break;
			case PixelType_Gvsp_Mono8:
				mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
				break;
			case PixelType_Gvsp_YUV422_Packed:
				mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC2, pData);
				cv::cvtColor(mat, mat, cv::COLOR_YUV2RGB_Y422);
				break;
			case PixelType_Gvsp_YUV422_YUYV_Packed:
				mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC2, pData);
				cv::cvtColor(mat, mat, cv::COLOR_YUV2RGB_YUYV);
				break;
			case PixelType_Gvsp_BayerGB8:
				mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
				cv::cvtColor(mat, mat, cv::COLOR_BayerGB2RGB);
				break;
			case PixelType_Gvsp_BayerGR8:
				mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
				cv::cvtColor(mat, mat, cv::COLOR_BayerGR2RGB);
				break;
			case PixelType_Gvsp_BayerRG8:
				mat = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
				cv::cvtColor(mat, mat, cv::COLOR_BayerBG2RGB);
				break;
			default:
				qDebug() << "Unsupported pixel type";
				qDebug() << pFrameInfo->enPixelType;
				return mat;
			}
			return mat;
		}
    }

}
