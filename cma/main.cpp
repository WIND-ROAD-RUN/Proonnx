#include<iostream>
#include"cma_Camera.h"
#include"cma_CameraUtilty.h"
#include <QObject> 
#include"opencv2/opencv.hpp"
#include"image.h"
#include <QtCore/QCoreApplication>

using namespace std;
using namespace rw::cma;




int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    auto cameraList = CameraUtilty::checkAllConnectCamera();
    Camera* camera = new Camera;
    cout << cameraList.size()<<endl;
    if (cameraList.size()) {
        camera->setIp(cameraList.at(0));
        cout << cameraList.at(0)<<endl;
        cout << "connectResult:";
        cout <<camera->connectCamera();
        imageIdentify identify(camera);
        camera->startAcquisition();
        camera->setHardwareTriggeredAcquisition();
        int i;
        cin >> i;
        camera->stopAcquisition();

    }
    delete camera;
    return a.exec();
}




