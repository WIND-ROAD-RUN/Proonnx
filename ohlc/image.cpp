#include "image.h"
#include<iostream>
#include"opencv2/opencv.hpp"

using namespace std;

void imageIdentify::getImage(std::shared_ptr<cv::Mat>  mat) {
    cout << mat->size() << endl;
}

imageIdentify::imageIdentify(Camera* camera)
    : m_camera(camera) {
    QObject::connect(this->m_camera, SIGNAL(ImgCallBackSignal(std::shared_ptr<cv::Mat>)), this, SLOT(getImage(std::shared_ptr<cv::Mat>)));
}
