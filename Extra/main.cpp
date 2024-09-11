// filesystem_path_example.cpp
// compile by using: /EHsc /W4 /permissive- /std:c++17 (or later)
#include <string>
#include <iostream>
#include <sstream>
#include"cfgr/cfgr_CatalogueInitializer.h"
#include"oso/oso_core.h"
#include"oso/osoc_MVSCameraConfig.h"
#include"oso/osop_OcrDateProductConfig.h"
#include"oso/osos_FileSave.h"

using namespace std;
using namespace rw::cfgr;
using namespace rw::oso;

int main()
{

    OcrDataProductConfig a;
    auto b=OcrDataProductConfig::toObjectStoreAssembly(a);
    b.print(std::cout);

    FileSave xmlSave;
    xmlSave.save(R"(C:\Users\WINDROAD\Desktop\test\test\test.xml)", std::make_shared<ObjectStoreAssembly>(b));
    auto resutl = xmlSave.load(R"(C:\Users\WINDROAD\Desktop\test\test\test.xml)");
    resutl->print(std::cout);
    auto i = OcrDataProductConfig::toOcrDataProductConfig(*(resutl.get()));

    return 0;
}
