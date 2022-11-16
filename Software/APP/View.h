#ifndef _VIEW_H_
#define _VIEW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tos_k.h"
#include "Common.h"
#include "cmsis_os.h"
#include "mqttclient.h"
#include "esp8266_tencent_firmware.h"

#define IOT_STK_SIZE 1024
#define PRODUCT_ID   "TM9854KHIS"
#define DEVICE_NAME  "DetectMachine"
#define DEVICE_KEY   "vGcKPCu22EK1bBxkJ5Pz2g=="


void ViewTaskInit(void);

#endif //_VIEW_H_
