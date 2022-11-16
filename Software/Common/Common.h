#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
    
#define MSG_LEN 50
struct Msg_t
{
	uint8_t Type;
    uint32_t SensorValue;
	char Data[MSG_LEN];
};

enum SensorType_t
{
    BEARPI_BH750_SENSOR,
    BEARPI_MQ_2_SENSOR
};

enum MsgType_t
{
    GET_MQ2_SENSOR,
    GET_BH750_SENSOR,
};

#endif //_COMMON_H_
