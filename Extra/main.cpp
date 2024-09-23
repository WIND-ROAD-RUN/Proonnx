#include"oso/oso_core.h"
#include"oso/osos_FileSave.h"
#include"oso/osop_OcrDateProductConfig.h"

#include<iostream>

using namespace rw::oso;

int main()
{
    OcrDataProductConfig config;
    config.productName = "Config1";
	FileSave<FileSaveStrategyType::XML> fileSave;
    fileSave.save(R"(C:\Users\WINDROAD\Desktop\test\test.xml)", makeObjectStoreAssemblySharedPtr(OcrDataProductConfig::toObjectStoreAssembly(config)));

	return 0;
}