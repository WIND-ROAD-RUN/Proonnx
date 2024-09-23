#ifndef OSO_CAMERACONFIG_H_
#define OSO_CAMERACONFIG_H_

#include<string>

namespace rw {
    namespace oso {
        class ObjectStoreAssembly;
    }
}

namespace rw {
    namespace oso
    {
        struct CameraAttributesBasic
        {
        public:
            long exposureTime{ 10000 };
            int gain{ 1 };
        public:
            static ObjectStoreAssembly toObjectStoreAssembly(const CameraAttributesBasic & cameraAttributesBasic);
            static CameraAttributesBasic toCameraAttributesBasic(const ObjectStoreAssembly& assembly);
        };

        enum class CameraTriggerMode
        {
            Software,
            Hardware,
            Undefined
        };

        struct CameraAttributesAdvanced
        {
        public:
            CameraTriggerMode mode{ CameraTriggerMode::Undefined };
        public:
            static ObjectStoreAssembly toObjectStoreAssembly(const CameraAttributesAdvanced& cameraAttributesBasic);
            static CameraAttributesAdvanced toCameraAttributesAdvanced(const ObjectStoreAssembly& assembly);
        };

    }

    
}


#endif // !oso_cameraConfig_H_
