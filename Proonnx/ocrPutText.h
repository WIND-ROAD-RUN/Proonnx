#ifndef OCRPUTTEXT_H_
#define OCRPUTTEXT_H_

#include <string>
#include <opencv/include/opencv2/opencv.hpp>
#include <Windows.h>
using namespace cv;

class putText
{

    static void GetStringSize(HDC hdc, const char* str, int* w, int* h);

public:
    static void putTextZH(Mat& dst, const char* str, Point org, Scalar color, int fontsize,
        const char* fn = "Arial", bool italic = false, bool underline = false);
};

#endif // !OCRPUTTEXT_H_
