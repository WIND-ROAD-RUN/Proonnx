#include"osop_OcrDateProductConfig.h"

namespace rw
{
    namespace oso {

        ObjectStoreAssembly 
            RecognizeRange::toObjectStoreAssembly
            (const RecognizeRange& recognizeRange)
        {
            ObjectStoreAssembly assembly;
            assembly.setName("RecognizeRange");

            {
                ObjectStoreItem leftTopCorner_x;
                leftTopCorner_x.setName("leftTopCorner_x");
                leftTopCorner_x.setValueFromDouble(recognizeRange.leftTopCorner.first);
                assembly.addItem(makeObjectStoreItemSharedPtr(leftTopCorner_x));

                ObjectStoreItem leftTopCorner_y;
                leftTopCorner_y.setName("leftTopCorner_y");
                leftTopCorner_y.setValueFromDouble(recognizeRange.leftTopCorner.second);
                assembly.addItem(makeObjectStoreItemSharedPtr(leftTopCorner_y));
            }

            {
                ObjectStoreItem leftLowerCorner_x;
                leftLowerCorner_x.setName("leftLowerCorner_x");
                leftLowerCorner_x.setValueFromDouble(recognizeRange.leftLowerCorner.first);
                assembly.addItem(makeObjectStoreItemSharedPtr(leftLowerCorner_x));

                ObjectStoreItem leftLowerCorner_y;
                leftLowerCorner_y.setName("leftLowerCorner_y");
                leftLowerCorner_y.setValueFromDouble(recognizeRange.leftLowerCorner.second);
                assembly.addItem(makeObjectStoreItemSharedPtr(leftLowerCorner_y));
            }

            {
                ObjectStoreItem rightTopCorner_x;
                rightTopCorner_x.setName("rightTopCorner_x");
                rightTopCorner_x.setValueFromDouble(recognizeRange.rightTopCorner.first);
                assembly.addItem(makeObjectStoreItemSharedPtr(rightTopCorner_x));

                ObjectStoreItem rightTopCorner_y;
                rightTopCorner_y.setName("rightTopCorner_y");
                rightTopCorner_y.setValueFromDouble(recognizeRange.rightTopCorner.second);
                assembly.addItem(makeObjectStoreItemSharedPtr(rightTopCorner_y));
            }

            {
                ObjectStoreItem rightLowerCorner_x;
                rightLowerCorner_x.setName("rightLowerCorner_x");
                rightLowerCorner_x.setValueFromDouble(recognizeRange.rightLowerCorner.first);
                assembly.addItem(makeObjectStoreItemSharedPtr(rightLowerCorner_x));

                ObjectStoreItem rightLowerCorner_y;
                rightLowerCorner_y.setName("rightLowerCorner_y");
                rightLowerCorner_y.setValueFromDouble(recognizeRange.rightLowerCorner.second);
                assembly.addItem(makeObjectStoreItemSharedPtr(rightLowerCorner_y));
            }
           
            return assembly;
        }

        RecognizeRange 
            RecognizeRange::toRecognizeRange
            (const ObjectStoreAssembly& assembly)
        {
            RecognizeRange recognizeRange;
            for (auto& item : assembly.getItems())
            {
                auto nItem = ObjectStoreCoreToItem(item);
                if (nItem->getName() == "leftTopCorner_x")
                {
                    recognizeRange.leftTopCorner.first = nItem->getValueAsDouble();
                }
                else if (nItem->getName() == "leftTopCorner_y")
                {
                    recognizeRange.leftTopCorner.second = nItem->getValueAsDouble();
                }
                else if (nItem->getName() == "leftLowerCorner_x")
                {
                    recognizeRange.leftLowerCorner.first = nItem->getValueAsDouble();
                }
                else if (nItem->getName() == "leftLowerCorner_y")
                {
                    recognizeRange.leftLowerCorner.second = nItem->getValueAsDouble();
                }
                else if (nItem->getName() == "rightTopCorner_x")
                {
                    recognizeRange.rightTopCorner.first = nItem->getValueAsDouble();
                }
                else if (nItem->getName() == "rightTopCorner_y")
                {
                    recognizeRange.rightTopCorner.second = nItem->getValueAsDouble();
                }
                else if (nItem->getName() == "rightLowerCorner_x")
                {
                    recognizeRange.rightLowerCorner.first = nItem->getValueAsDouble();
                }
                else if (nItem->getName() == "rightLowerCorner_y")
                {
                    recognizeRange.rightLowerCorner.second = nItem->getValueAsDouble();
                }
            }

            return recognizeRange;
        }

        ObjectStoreAssembly 
            ProductCheckCount::toObjectStoreAssembly
            (const ProductCheckCount& productCheckCount)
        {
            ObjectStoreAssembly assembly;
            assembly.setName("ProductCheckCount");

            {
                ObjectStoreItem totalCount;
                totalCount.setName("totalCount");
                totalCount.setValueFromInt(productCheckCount.totalCount);
                assembly.addItem(makeObjectStoreItemSharedPtr(totalCount));
            }

            {
                ObjectStoreItem passCount;
                passCount.setName("passCount");
                passCount.setValueFromInt(productCheckCount.passCount);
                assembly.addItem(makeObjectStoreItemSharedPtr(passCount));
            }

            {
                ObjectStoreItem outCount;
                outCount.setName("outCount");
                outCount.setValueFromInt(productCheckCount.outCount);
                assembly.addItem(makeObjectStoreItemSharedPtr(outCount));
            }

            return assembly;
        }

        ProductCheckCount 
            ProductCheckCount::toProductCheckCount
            (const ObjectStoreAssembly& assembly)
        {
            ProductCheckCount productCheckCount;
            for (auto& item : assembly.getItems())
            {
                auto nItem = ObjectStoreCoreToItem(item);
                if (nItem->getName() == "totalCount")
                {
                    productCheckCount.totalCount = nItem->getValueAsInt();
                }
                else if (nItem->getName() == "passCount")
                {
                    productCheckCount.passCount = nItem->getValueAsInt();
                }
                else if (nItem->getName() == "outCount")
                {
                    productCheckCount.outCount = nItem->getValueAsInt();
                }
            }

            return productCheckCount;
        }

        ObjectStoreAssembly 
            RejectAttribute::toObjectStoreAssembly
            (const RejectAttribute& rejectAttribute)
        {
            ObjectStoreAssembly assembly;
            assembly.setName("RejectAttribute");

            {
                ObjectStoreItem RejectDelay;
                RejectDelay.setName("RejectDelay");
                RejectDelay.setValueFromInt(rejectAttribute.RejectDelay);
                assembly.addItem(makeObjectStoreItemSharedPtr(RejectDelay));
            }

            {
                ObjectStoreItem OffsetsNumber;
                OffsetsNumber.setName("OffsetsNumber");
                OffsetsNumber.setValueFromInt(rejectAttribute.OffsetsNumber);
                assembly.addItem(makeObjectStoreItemSharedPtr(OffsetsNumber));
            }

            {
                ObjectStoreItem DisposalTime;
                DisposalTime.setName("DisposalTime");
                DisposalTime.setValueFromInt(rejectAttribute.DisposalTime);
                assembly.addItem(makeObjectStoreItemSharedPtr(DisposalTime));
            }

            return assembly;
        }

        RejectAttribute 
            RejectAttribute::toRejectAttribute
            (const ObjectStoreAssembly& assembly)
        {
            RejectAttribute rejectAttribute;

            for (auto& item : assembly.getItems())
            {
                auto nItem = ObjectStoreCoreToItem(item);
                if (nItem->getName() == "RejectDelay")
                {
                    rejectAttribute.RejectDelay = nItem->getValueAsInt();
                }
                else if (nItem->getName() == "OffsetsNumber")
                {
                    rejectAttribute.OffsetsNumber = nItem->getValueAsInt();
                }
                else if (nItem->getName() == "DisposalTime")
                {
                    rejectAttribute.DisposalTime = nItem->getValueAsInt();
                }
            }

            return rejectAttribute;
        }

        ObjectStoreAssembly 
            OcrDataProductConfig::toObjectStoreAssembly
            (const OcrDataProductConfig& ocrDataProductConfig)
        {
            ObjectStoreAssembly assembly;

            assembly.setName("OcrDataProductConfig");

            {
                ObjectStoreItem productName;
                productName.setName("productName");
                productName.setValueFromString(ocrDataProductConfig.productName);
                assembly.addItem(makeObjectStoreItemSharedPtr(productName));
            }

            {
                ObjectStoreAssembly cameraAttributesBasic = CameraAttributesBasic::toObjectStoreAssembly(ocrDataProductConfig.cameraAttributesBasic);
                cameraAttributesBasic.setName("cameraAttributesBasic");
                assembly.addItem(makeObjectStoreAssemblySharedPtr(cameraAttributesBasic));
            }

            {
                ObjectStoreAssembly recognizeRange = RecognizeRange::toObjectStoreAssembly(ocrDataProductConfig.recognizeRange);
                recognizeRange.setName("recognizeRange");
                assembly.addItem(makeObjectStoreAssemblySharedPtr(recognizeRange));
            }

            {
                ObjectStoreAssembly productCheckCount = ProductCheckCount::toObjectStoreAssembly(ocrDataProductConfig.productCheckCount);
                productCheckCount.setName("productCheckCount");
                assembly.addItem(makeObjectStoreAssemblySharedPtr(productCheckCount));
            }

            {
                ObjectStoreAssembly rejectAttribute = RejectAttribute::toObjectStoreAssembly(ocrDataProductConfig.rejectAttribute);
                rejectAttribute.setName("rejectAttribute");
                assembly.addItem(makeObjectStoreAssemblySharedPtr(rejectAttribute));
            }

            return assembly;
        }

        OcrDataProductConfig 
            OcrDataProductConfig::toOcrDataProductConfig
            (const ObjectStoreAssembly& assembly)
        {
            OcrDataProductConfig ocrDataProductConfig;

            for (auto& item : assembly.getItems())
            {
                auto itemType = item->getStoreType();
                if (itemType ==std::string("assembly")) {
                    auto nItem = ObjectStoreCoreToAssembly(item);
                    if (nItem->getName() == "cameraAttributesBasic")
                    {
                        ocrDataProductConfig.cameraAttributesBasic = CameraAttributesBasic::toCameraAttributesBasic(*nItem);
                    }
                    else if (nItem->getName() == "recognizeRange")
                    {
                        ocrDataProductConfig.recognizeRange = RecognizeRange::toRecognizeRange(*nItem);
                    }
                    else if (nItem->getName() == "productCheckCount")
                    {
                        ocrDataProductConfig.productCheckCount = ProductCheckCount::toProductCheckCount(*nItem);
                    }
                    else if (nItem->getName() == "rejectAttribute")
                    {
                        ocrDataProductConfig.rejectAttribute = RejectAttribute::toRejectAttribute(*nItem);
                    }
                    else
                    {
                        static_assert(true, "Unknown item type");
                    }

                }
                else if (itemType == std::string("item")) {
                    auto nItem = ObjectStoreCoreToItem(item);
                    if (nItem->getName() == "productName")
                    {
                        ocrDataProductConfig.productName = nItem->getValueAsString();
                    }
                    else {
                        static_assert(true, "Unknown item type");
                    }
                }
                
               
            }

            return ocrDataProductConfig;
        }

}

}
