#include "Model.h"

extern k_msg_q_t Model2View;
extern k_msg_q_t Controller2Model;

void ModelSensorHandlerTask(void *pdata);
void ModelHandlerControllerTask(void *pdata);

osThreadDef(ModelHandlerControllerTask, osPriorityNormal, 1, MODEL_STK_SIZE);
osThreadDef(ModelSensorHandlerTask, osPriorityRealtime, 1, MODEL_STK_SIZE);

SensorStateItem_t SensorStateTable[] =
{
    {IDLE,   START, CALI},      //IDLE->START->CALI
    {CALI,   NEXT,  DETECT},    //CALI->NEXT->DETECT
    {CALI,   STOP,  IDLE},      //CALI->STOP->IDLE
    {DETECT, NEXT,  RESULT},    //DETECT->NEXT->RESULT
    {DETECT, PREV,  CALI},      //DETECT->PREV->CALI
    {DETECT, STOP,  IDLE},      //DETECT->STOP->IDLE
    {RESULT, PREV,  DETECT},    //RESULT->PREV->DETECT
    {RESULT, STOP,  IDLE},      //RESULT->STOP->IDLE
};

__IO SensorState_t CurState = IDLE;
__IO SensorState_t PrevState = IDLE;

SensorStateItem_t *FindCurStateAndEvent(SensorState_t CurState, SensorEvent_t Event)
{
    int Index = 0;

    for(Index = 0; Index < NR_MODEL(SensorStateTable); Index++)
    {
        if(SensorStateTable[Index].CurState == CurState \
                && SensorStateTable[Index].Event == Event)  \
            return &SensorStateTable[Index];
    }

    return NULL;
}

int DoSensorStatus(SensorEvent_t Event)
{
    int Ret = 1;
    SensorStateItem_t *Item;
    Item = FindCurStateAndEvent(CurState, Event);

    if (!Item)
        return 0;

    PrevState = CurState;
    CurState = Item->NextSate;
    return Ret;
};


//处理Controller的请求
void ModelHandlerControllerTask(void *pdata)
{
    k_err_t err;
    void *MsgRecv;
    SensorEvent_t Event;
    struct Msg_t RecvFromCtrMsg;

    while(1)
    {
        //1.接收来自Controller的请求
        err = tos_msg_q_pend(&Controller2Model, &MsgRecv, TOS_TIME_FOREVER);

        if(err != K_ERR_NONE)
        {
            printf("tos_msg_q_pend failed: %d!\n", err);
            goto ErrorHandler;
        }

        //2.开始处理Controller请求，获取控制类型
        memset(&RecvFromCtrMsg, 0, sizeof(struct Msg_t));
        memcpy(&RecvFromCtrMsg, MsgRecv, sizeof(struct Msg_t));
        printf("MsgRecv->Type:%d\n", RecvFromCtrMsg.Type);

        //3.通过控制类型让传感器状态改变
        Event = (SensorEvent_t)RecvFromCtrMsg.Type;
        DoSensorStatus(Event);
        
        osDelay(10);
    }

ErrorHandler:
    return ;
}

//处理Model内部的传感器状态变换
void ModelSensorHandlerTask(void *pdata)
{
    int SmokeValue;
    int CaliCnt = 0;
    uint8_t Result = 0;
    int DetectingCnt = 0;
    struct Msg_t Send2ViewMsg;

    while(1)
    {
        memset(&Send2ViewMsg, 0, sizeof(struct Msg_t));
        SmokeValue = GetSmokeValue() ;

        switch(CurState)
        {
            //设备空闲
            case IDLE:
                Result = 0;
                CaliCnt = 0;
                DetectingCnt = 0;
                Send2ViewMsg.Type = GET_MQ2_SENSOR;
                snprintf(Send2ViewMsg.Data, sizeof(Send2ViewMsg.Data),
                         "Status:%d;Value:%d", IDLE, SmokeValue);
                tos_msg_q_post(&Model2View, (void *) &Send2ViewMsg);
                break;

            //设备校准
            case CALI:

                CaliCnt++;
                DetectingCnt = 0;

                if(SmokeValue > 200)
                {
                    CaliCnt = 0;
                    break;
                }

                if(CaliCnt > 5)
                {
                    CaliCnt = 0;
                    DoSensorStatus(NEXT);
                    break;
                }

                Send2ViewMsg.Type = GET_MQ2_SENSOR;
                snprintf(Send2ViewMsg.Data, sizeof(Send2ViewMsg.Data),
                         "Status:%d;Value:%d", CALI, SmokeValue);
                tos_msg_q_post(&Model2View, (void *) &Send2ViewMsg);
                break;

            //设备检测中
            case DETECT:
                DetectingCnt++;

                if(DetectingCnt > 2 && SmokeValue < SENSOR_MAX)
                {
                    Result = 0;
                    DetectingCnt = 0;
                    DoSensorStatus(NEXT);
                    break;
                }
                else if(DetectingCnt < 2 && SmokeValue > SENSOR_MAX)
                {
                    Result = 1;
                    DetectingCnt = 0;
                    DoSensorStatus(NEXT);
                    break;
                }

                Send2ViewMsg.Type = GET_MQ2_SENSOR;
                snprintf(Send2ViewMsg.Data, sizeof(Send2ViewMsg.Data),
                         "Status:%d;Value:%d", DETECT, SmokeValue);
                tos_msg_q_post(&Model2View, (void *) &Send2ViewMsg);
                break;

            //检测结果
            case RESULT:
                if(Result)
                {
                    Send2ViewMsg.Type = GET_MQ2_SENSOR;
                    snprintf(Send2ViewMsg.Data, sizeof(Send2ViewMsg.Data),
                             "Status:%d;Value:%d", DETECT_DANGER, SmokeValue);
                    tos_msg_q_post(&Model2View, (void *) &Send2ViewMsg);
                }
                else
                {
                    Send2ViewMsg.Type = GET_MQ2_SENSOR;
                    snprintf(Send2ViewMsg.Data, sizeof(Send2ViewMsg.Data),
                             "Status:%d;Value:%d", DETECT_SAFETY, SmokeValue);
                    tos_msg_q_post(&Model2View, (void *) &Send2ViewMsg);
                }

                break;

            default:
                break;
        }

        osDelay(2000);
    }
}

void ModelTaskInit(void)
{
    osThreadCreate(osThread(ModelSensorHandlerTask), NULL);
    osThreadCreate(osThread(ModelHandlerControllerTask), NULL);
}
