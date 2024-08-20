#ifndef OGLC_CAMERA_H_
#define OGLC_CAMERA_H_

#include <vector>
#include <string>
#include <QObject> 
#include <memory>

typedef struct _MV_FRAME_OUT_INFO_EX_ MV_FRAME_OUT_INFO_EX;
typedef struct _MV_FRAME_OUT_ MV_FRAME_OUT;

namespace cv {
    class Mat;
}

namespace rw {
    namespace ohlc {
        /// <summary>
            /// 
            /// </summary>
        class Camera_MVS;
        struct CameraAttribute;
        class Camera
            : public QObject {
            Q_OBJECT

        private:
            std::shared_ptr<Camera_MVS> m_cameraMVS{};

        public:
            Camera();

            ~Camera();

        public:
            void setCameraAttribute(const CameraAttribute& cameraAttribute);

            CameraAttribute getCameraAttribute();

        public:
            bool connectCamera();

        public:
            bool setExposureTime(int exposureTime);

            bool setGain(int gain);

            int setIOStart(int time);

        public:
            bool setHardwareTriggeredAcquisition();

            bool setSoftwareTriggeredAcquisition();

        signals:
            void ImgCallBackSignal(std::shared_ptr<cv::Mat>  mat);

        private slots:
            void monitorImage(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo);

        };

        /// <summary>
        /// 
        /// </summary>
        class Camera_MVS
            : public QObject {
            Q_OBJECT

        private:
            std::string m_ip{};
            void* m_handle{ nullptr }; // Camera handle

        public:
            Camera_MVS();

            ~Camera_MVS();
        public:
            void setIp(const std::string& ip) { m_ip = ip; }

            std::string getIp() { return m_ip; }
        public:
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
    }

}

#endif // !OGLC_CAMERA_H_
