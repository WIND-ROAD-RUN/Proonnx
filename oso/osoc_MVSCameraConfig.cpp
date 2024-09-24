#include "osoc_MVSCameraConfig.h"

#include"oso_core.h"

#include<cassert>

namespace rw {
    namespace oso {
        ObjectStoreAssembly 
            CameraAttributesBasic::toObjectStoreAssembly
            (const CameraAttributesBasic& cameraAttributesBasic)
        {
            ObjectStoreAssembly assembly;
            assembly.setName("CameraAttributesBasic");

            ObjectStoreItem exposureTime;
            exposureTime.setName("exposureTime");
            exposureTime.setValueFromLong(cameraAttributesBasic.exposureTime);
            assembly.addItem(makeObjectStoreItemSharedPtr(exposureTime));

            ObjectStoreItem gain;
            gain.setName("gain");
            gain.setValueFromInt(cameraAttributesBasic.gain);
            assembly.addItem(makeObjectStoreItemSharedPtr(gain));

            return assembly;
        }

        CameraAttributesBasic 
            CameraAttributesBasic::toCameraAttributesBasic
            (const ObjectStoreAssembly& assembly)
        {
            CameraAttributesBasic cameraAttributesBasic;

            for (const auto& item : assembly.getItems())
            {
                std::shared_ptr<ObjectStoreItem> nItem = std::dynamic_pointer_cast<ObjectStoreItem>(item);
                if (nItem->getName() == "exposureTime")
                {
                    cameraAttributesBasic.exposureTime = nItem->getValueAsLong();
                }
                else if (nItem->getName() == "gain")
                {
                    cameraAttributesBasic.gain = nItem->getValueAsInt();
                }
            }

            return cameraAttributesBasic;
        }

        ObjectStoreAssembly 
            CameraAttributesAdvanced::toObjectStoreAssembly
            (const CameraAttributesAdvanced& cameraAttributesBasic)
        {
            assert(cameraAttributesBasic.mode!= CameraTriggerMode::Undefined);

            ObjectStoreItem mode;
            mode.setName("CameraTriggerMode");
            switch (cameraAttributesBasic.mode)
            {
            case CameraTriggerMode::Software:
                mode.setValueFromString("Software");
                break;
            case CameraTriggerMode::Hardware:
                mode.setValueFromString("Hardware");
                break;
            }

            ObjectStoreAssembly assembly;
            assembly.setName("CameraAttributesAdvanced");
            assembly.addItem(makeObjectStoreItemSharedPtr(mode));

            return assembly;
        }

        CameraAttributesAdvanced 
            CameraAttributesAdvanced::toCameraAttributesAdvanced
            (const ObjectStoreAssembly& assembly)
        {
            CameraAttributesAdvanced cameraAttributesAdvanced;

            for (const auto& item : assembly.getItems())
            {
                auto nItem = ObjectStoreCoreToItem(item);
                if (nItem->getName() == "CameraTriggerMode")
                {
                    if (nItem->getValueAsString() == "Software")
                    {
                        cameraAttributesAdvanced.mode = CameraTriggerMode::Software;
                    }
                    else if (nItem->getValueAsString() == "Hardware")
                    {
                        cameraAttributesAdvanced.mode = CameraTriggerMode::Hardware;
                    }
                }

            }

            return cameraAttributesAdvanced;

        }
    }

}


