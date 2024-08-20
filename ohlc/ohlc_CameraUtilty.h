#ifndef OHLC_CAMERAUTILTY_H_
#define OHLC_CAMERAUTILTY_H_

#include<vector>
#include<string>

typedef struct _MV_FRAME_OUT_INFO_EX_ MV_FRAME_OUT_INFO_EX;
typedef struct _MV_FRAME_OUT_ MV_FRAME_OUT;

namespace cv {
    class Mat;
}

namespace rw {
    namespace ohlc {
        struct CameraUtilty
        {
            static std::vector<std::string> checkAllConnectCamera();

            static cv::Mat ConvertMat(MV_FRAME_OUT_INFO_EX* pFrameInfo, unsigned char* pData);
        };
    }

}


#endif // !OHLC_CAMERAUTILTY_H_
