#pragma once

#include <QObject> 
#include"ohlc_Camera.h"

using namespace rw::ohlc;

class imageIdentify : public QObject {
    Q_OBJECT // 确保这个宏存在
private:
    Camera* m_camera{};
public:
    imageIdentify(Camera* camera);

    ~imageIdentify() {}
private slots:
    void getImage(std::shared_ptr<cv::Mat>  mat);
};