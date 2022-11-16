#include "App.h"

mqtt_client_t *client = NULL;

k_msg_q_t Model2View;
uint8_t Model2ViewMsgPool[50];

k_msg_q_t Controller2View;
k_msg_q_t Controller2Model;
uint8_t Controller2ViewMsgPool[50];
uint8_t Controller2ModelMsgPool[50];

#define DEFAULT_STK_SIZE	1024
void DefaultTask(void *pdata);
osThreadDef(DefaultTask, osPriorityNormal, 1, DEFAULT_STK_SIZE);

void MsgInit(void);

void DefaultTask(void *pdata)
{
    ViewTaskInit();
    ModelTaskInit();
    ControllerTaskInit();

    while(1)
    {
        osDelay(10);
    }
}

void RunApp(void)
{
    LCD_Init();
    LCD_Clear(BLACK);
    osKernelInitialize();
    MsgInit();
    osThreadCreate(osThread(DefaultTask), NULL);
    osKernelStart();
}

void MsgInit(void)
{
    tos_msg_q_create(&Model2View, Model2ViewMsgPool, 50);
    tos_msg_q_create(&Controller2View, Controller2ViewMsgPool, 50);
    tos_msg_q_create(&Controller2Model, Controller2ModelMsgPool, 50);
}
