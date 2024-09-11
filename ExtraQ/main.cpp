//#include <QtCore/QCoreApplication>
//
//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);
//
//    return a.exec();
//}

#include<iostream>

#include"ohlc/ohlc_CameraUtilty.h"
#include"ohlc/ohlc_Camera.h"
#include<QVector>
#include<thread>

using namespace rw::ohlc;
using namespace std;

int main() {
    auto cameraIpList = CameraUtilty::checkAllConnectCamera();
    cout << "CameraSize:" << cameraIpList.size() << endl;
    cout << "Camera Ip List:" << endl;
    for (const auto& item : cameraIpList) {
        cout << "               |____ " << item << endl;
    }

    QVector<Camera_MVS *> cameraList;
    cout << "Connect camera:" << endl;
    for (const auto& item : cameraIpList) {
        cout << "      |___Connect camera:" << item << endl;
        Camera_MVS *camera=new Camera_MVS; 
        camera->setIp(item);
        cout << "      |    |__result: " << camera->connectCamera() << endl;
        cameraList.push_back(camera);
    }

    auto camera = cameraList.at(0);
    cout << "setHardtwareTriggered Result: " << camera->setSoftwareTriggeredAcquisition();
    camera->startAcquisition();
    
    int wait;
    cin >> wait;

    camera->stopAcquisition();

    return 0;
}