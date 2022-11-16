#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cJSON.h"
#include "tos_k.h"
#include "cmsis_os.h"
#include "mqttclient.h"
#include "esp8266.h"
#include "Common.h"
#include "sal_module_wrapper.h"

#define CONTROLLER_STK_SIZE 1024

void ControllerTaskInit(void);
void ControllerTask(void *pdata);


#endif //_CONTROLLER_H_
