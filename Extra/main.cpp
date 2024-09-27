#include"oso/oso_core.h"
#include"oso/osos_FileSave.h"
#include"oso/osop_OcrDateProductConfig.h"
#include"cfgr/cfgr_RuntimeConfig.h"
#include"oso/osos_FileSave.h"
#include"oso/osop_OcrDateProductConfig.h"

#include<iostream>

using namespace rw::oso;
using namespace rw::cfgr;

int main()
{
    ObjectStoreAssembly assembly;
    assembly.setName("root");
    
    ObjectStoreItem item;
    item.setName("item1");
    item.setValueFromInt(12);

    ObjectStoreItem item2;
    item2.setName("item2");
    item2.setValueFromInt(13);

    assembly.addItem(std::move(item));
    assembly.addItem(std::move(item2));

    ObjectStoreAssembly assembly2;
    assembly2.setName("root2");

    ObjectStoreItem item3;
    item3.setName("item3");
    item3.setValueFromInt(14);

    ObjectStoreItem item4;
    item4.setName("item4");
    item4.setValueFromInt(15);

    assembly2.addItem(std::move(item3));
    assembly2.addItem(std::move(item4));

    assembly.addItem(std::move(assembly2));

    assembly.print(std::cout);
    OcrDataProductConfig config;
    config.productName = "12313123";
    

    FileSave_XML fileSave;
    fileSave.save(R"(C:\Users\WINDROAD\Desktop\test\test.xml)", makeObjectStoreAssemblySharedPtr(OcrDataProductConfig::toObjectStoreAssembly(OcrDataProductConfig(config))));

    auto loadResult = fileSave.load(R"(C:\Users\WINDROAD\Desktop\test\test.xml)");
    loadResult->print(std::cout);
	return 0;
}