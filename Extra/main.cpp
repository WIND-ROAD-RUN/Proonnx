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
    OcrDataProductConfig ocrDataProductConfig;
    ocrDataProductConfig.cameraAttributesBasic.exposureTime = 10000;
    ocrDataProductConfig.cameraAttributesBasic.gain = 1;
    ocrDataProductConfig.productName = "test";
    ocrDataProductConfig.productCheckCount.outCount = 1;
    ocrDataProductConfig.productCheckCount.passCount = 1;
    ocrDataProductConfig.productCheckCount.totalCount = 1;
    ocrDataProductConfig.recognizeRange.leftLowerCorner.first = 1;
    ocrDataProductConfig.recognizeRange.leftLowerCorner.second = 1;

    ocrDataProductConfig.rejectAttribute.DisposalTime = 1000;
    ocrDataProductConfig.rejectAttribute.OffsetsNumber = 1;
    ocrDataProductConfig.rejectAttribute.RejectDelay = 1;





    FileSave<FileSaveStrategyType::XML> fileSave;
    auto a = OcrDataProductConfig::toObjectStoreAssembly(ocrDataProductConfig);
    fileSave.save(R"(C:\Users\34615\Desktop\test\test.xml)", a);

    auto assembly = fileSave.load(R"(C:\Users\34615\Desktop\test\test.xml)");
    assembly->print(std::cout);


    return 0;
}