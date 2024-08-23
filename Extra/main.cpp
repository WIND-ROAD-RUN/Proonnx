#include<iostream>

#include"ohlc/ohlc_CameraUtilty.h"
#include"ohlc/ohlc_Camera.h"

using namespace rw::ohlc;
using namespace std;

int main() {
    auto cameraIpList=CameraUtilty::checkAllConnectCamera();
    cout << "CameraSize:" << cameraIpList.size() << endl;
    cout << "Camera Ip List:" << endl;
    for (const auto & item: cameraIpList) {
        cout <<"               |____ " << item << endl;
    }

    std::vector<Camera_MVS> cameraList;
    cout << "Connect camera:" << endl;
    for (const auto& item : cameraIpList) {
        cout << " Connect camera:"<<item << endl;
        Camera_MVS camera;
        camera.setIp(item);
        cout << "Connect result: " << camera.connectCamera() << endl;
        cameraList.push_back(std::move(camera));
    }
  

    return 0;
}