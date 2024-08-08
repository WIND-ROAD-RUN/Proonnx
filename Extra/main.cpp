#include<iostream>
#include"cfgl/cfgl_LocalizationStringLoader.h"
using namespace std;
using namespace rw::cfgl;

int main()
{
    auto loader = LocalizationStringLoaderXML::getInstance();
    loader->setFilePath("");
    loader->loadData();

    cout << "hello";

    return 0;
}