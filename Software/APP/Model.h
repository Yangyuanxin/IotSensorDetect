#ifndef _MODEL_H_
#define _MODEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tos_k.h"
#include "cmsis_os.h"
#include "MQ_2.h"
#include "Common.h"

#define MODEL_STK_SIZE 1024
#define SENSOR_MAX 1000
#define NR_MODEL(x) (sizeof(x)/sizeof(x[0]))

//检测结果
typedef enum DetectResult_t
{
    DETECT_SAFETY = 49,
    DETECT_DANGER,
} DetectResult_t;

//传感器状态
typedef enum SensorState_t
{
    IDLE = 0,
    CALI,
    DETECT,
    RESULT
} SensorState_t;

//传感器事件
typedef enum SensorEvent_t
{
    START = 0,
    STOP,
    NEXT,
    PREV
} SensorEvent_t;

//传感器状态控制结构体
typedef struct SensorStateItem_t
{
    enum SensorState_t CurState;
    enum SensorEvent_t Event;
    enum SensorState_t NextSate;
} SensorStateItem_t;


void ModelTaskInit(void);

#endif //_MODEL_H_
