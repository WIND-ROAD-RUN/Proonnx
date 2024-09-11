// filesystem_path_example.cpp
// compile by using: /EHsc /W4 /permissive- /std:c++17 (or later)
#include <string>
#include <iostream>
#include <sstream>
#include"cfgr/cfgr_CatalogueInitializer.h"

using namespace std;
using namespace rw::cfgr;

int main()
{
    CatalogueInitializer cata;
    cata.setRootPath(R"(C:\Users\WINDROAD\Desktop\test)");
    cata.createDirectory("Config");
    cata.createDirectory("Config");
 
    cout << CatalogueInitializer::findWorkPath("Config");

    return 0;
}