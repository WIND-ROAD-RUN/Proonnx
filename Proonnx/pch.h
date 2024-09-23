#pragma once

#include"oulq/oulq_LabelCustom.h"
//#include"LocalizationStringLoader-XML.h"
#include"cfgl/cfgl_LocalizationStringLoader.h"

#include"cfgr/cfgr_ConfigBeforeRuntimeLoader.h"
#include"cfgr/cfgr_CatalogueInitializer.h"
#include"cfgr/cfgr_RuntimeConfigLoader.h"
#include"oso/osoc_MVSCameraConfig.h"
#include"oso/oso_core.h"
#include"oso/osop_OcrDateProductConfig.h"
#include"oso/osos_FileSave.h"

#include"MonitorCamera.h"

#include"DlgAddProductConfig.h"
#include"DlgChangeProductConfig.h"
#include"DlgSetProonnx.h"
#include"DlgSelectCameraIndex.h"
#include"DlgSetCamera.h"
#include"DlgForTest.h"
#include"DlgSetIsCheckProduct.h"
#include"DlgClearCount.h"
#include"DlgSelectProductConfig.h"
#include"DlgManageProductConfig.h"

#include"IndexButton.h"
#include"ProductConfigLoader.h"
#include"ConfigForImageSave.h"
#include"DateTransform.h"
#include"LogRecorder.h"
#include"ImageIdentify.h"


static LogRecorder* LOGRECORDER = LogRecorder::getInstance();