#ifndef OSOP_OCRDATEPRODUCTCONFIG_H_
#define OSOP_OCRDATEPRODUCTCONFIG_H_

#include"osoc_MVSCameraConfig.h"
#include"oso_core.h"

#include<string>

namespace rw {
    namespace oso {
        class ObjectStoreAssembly;
        class ObjectStoreItem;
    }
}

namespace rw {
    namespace oso {
        struct RecognizeRange {
        public:
            std::pair<double, double> leftTopCorner{ 0,0 };
            std::pair<double, double> leftLowerCorner{ 0,0 };
            std::pair<double, double> rightTopCorner{ 0,0 };
            std::pair<double, double> rightLowerCorner{ 0,0 };
        public:
            static ObjectStoreAssembly toObjectStoreAssembly(const RecognizeRange& recognizeRange);
            static RecognizeRange toRecognizeRange(const ObjectStoreAssembly& assembly);
        };

        struct ProductCheckCount {
        public:
            int totalCount{ 0 };
            int passCount{ 0 };
            int outCount{ 0 };
        public:
            static ObjectStoreAssembly toObjectStoreAssembly(const ProductCheckCount& productCheckCount);
            static ProductCheckCount toProductCheckCount(const ObjectStoreAssembly& assembly);
        };

        struct RejectAttribute {
        public:
            int RejectDelay{ 0 };
            int OffsetsNumber{ 0 };
            int DisposalTime{ 0 };
        public:
            static ObjectStoreAssembly toObjectStoreAssembly(const RejectAttribute& rejectAttribute);
            static RejectAttribute toRejectAttribute(const ObjectStoreAssembly& assembly);
        };

        struct OcrDataProductConfig {
        public:
            std::string productName{ "XXXX" };

            CameraAttributesBasic cameraAttributesBasic;
            RecognizeRange recognizeRange;
            ProductCheckCount productCheckCount;
            RejectAttribute rejectAttribute;
        public:
            static ObjectStoreAssembly toObjectStoreAssembly(const OcrDataProductConfig& ocrDataProductConfig);
            static OcrDataProductConfig toOcrDataProductConfig(const ObjectStoreAssembly& assembly);

        };
    }
}



#endif // OSOP_OCRDATEPRODUCTCONFIG_H_
