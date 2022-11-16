#ifndef _APP_H_
#define _APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"
#include "spi.h"
#include "adc.h"
#include "gpio.h"
#include "usart.h"

#include "Lcd.h"
#include "MQ_2.h"
#include "Bh750.h"
#include "tos_k.h"
#include "cmsis_os.h"
#include "sensor_protocol.h"

#include "Model.h"
#include "View.h"
#include "Controller.h"

void RunApp(void);

#ifdef __cplusplus
}
#endif
#endif  //ifndef _APP_H_
