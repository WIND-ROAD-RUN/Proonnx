#ifndef OCRWORK_H
#define OCRWORK_H

#include"OCRApi_GPU/include/OcrApi.h"
#include "opencv/include/opencv2/opencv.hpp"
#include <cmath>  
#include <algorithm>
#include <numeric>
#include"qdebug.h"
class ocrwork
{
public:
    ocrwork();

    OcrApi* ocrApi;
   void  drawView(cv::Mat &m_matSrc, std::vector<OCRResult>) ;

    void testOcr(cv::Mat srcMat,std::vector<OCRResult> &ocrResult );

    void initial();


};

#endif // OCRWORK_H
