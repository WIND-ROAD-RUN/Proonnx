#include<iostream>
#include"cma_Camera.h"

using namespace std;
using namespace rw::cma;

int main(int argc, char *argv[])
{
    auto cameraList = CameraMVSUtility::checkAllConnectCamera();
    cout << cameraList.size();

    return 0;
}
