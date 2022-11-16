#include "View.h"
#include "cJSON.h"


uint8_t cur_status = 0;
uint8_t connect_flag = 0;
char payload[256] = {0};
char token_cache[128] = {0};
extern mqtt_client_t *client;


extern k_msg_q_t Model2View;
void IotRecvTask(void *pdata);
void IotCtrlTask(void *pdata);
osThreadDef(IotRecvTask, osPriorityNormal, 1, IOT_STK_SIZE);  //模拟视图->接收事件

int GetKeyValueOfInt(char *Key, char *Value)
{
    int Num = 0;
    char *P1 = NULL, *P2 = NULL;

    P1 = Key;

    P2 = strstr(P1, Value);

    if(NULL == P2)
        return -1;

    P1 = P2 + strlen(Value);
    Num = atoi(P1);

    return Num;
}

int MqttPublish(mqtt_client_t *Client, char *Data)
{
    mqtt_message_t Msg;

    memset(&Msg, 0, sizeof(Msg));
    Msg.payload = Data;
    Msg.payloadlen = strlen(Data);
    Msg.qos = (mqtt_qos_t)0;

    return mqtt_publish(Client, "$thing/up/property/TM9854KHIS/DetectMachine", &Msg);
}

void ViewTaskInit(void)
{
    osThreadCreate(osThread(IotRecvTask), NULL);
}

void Msg2Json(char *SensorState, int SensorData)
{
    cJSON *params = NULL;
    cJSON *root = NULL;
    static unsigned int Cnt = 0;
    char *Str = NULL;
    char Buf[10] = {0};                                       
    root = cJSON_CreateObject();
    if(NULL == root)
    {
        printf("cJSON_CreateObject failed!\n");
        return;
    }
    
    ++Cnt;
    if(Cnt > 65535)
        Cnt = 1;
    snprintf(Buf, sizeof(Buf), "%05d", Cnt);
    cJSON_AddStringToObject(root, "method", "report");
    cJSON_AddStringToObject(root, "clientToken", Buf);
    params = cJSON_CreateObject();
    if(NULL == params)
    {
        printf("cJSON_CreateObject failed!\n");
        cJSON_Delete(root);
        return;
    }
    cJSON_AddStringToObject(params, "DeviceStatus", SensorState);
    cJSON_AddNumberToObject(params, "SmokeValue", SensorData);
    cJSON_AddItemToObject(root, "params", params);
    Str = cJSON_Print(root);
    if(NULL == Str)
    {
        printf("cJSON_Print Str failed!\n");
        cJSON_Delete(root);
        return;
    }
    printf("%s\n", Str);
    MqttPublish(client, Str);
    free(Str);
    Str = NULL;
    cJSON_Delete(root);
    root = NULL;
}


void IotRecvTask(void *pdata)
{
    k_err_t err;
    void *MsgRecv;
    struct Msg_t Msg;
    int SensorValue = 0;
    int CurSensorValue = 0;
    uint8_t DetectState = 0;
    memset(&Msg, 0, sizeof(struct Msg_t));

    while(1)
    {
        err = tos_msg_q_pend(&Model2View, &MsgRecv, TOS_TIME_FOREVER);

        if(err != K_ERR_NONE)
        {
            printf("tos_msg_q_pend failed: %d!\n", err);
            goto ErrorHandler;
        }

        memset(payload, 0, sizeof(payload));
        memset(&Msg, 0, sizeof(struct Msg_t));
        memcpy(&Msg, MsgRecv, sizeof(struct Msg_t));
        DetectState = GetKeyValueOfInt(Msg.Data, "Status:");
        SensorValue = GetKeyValueOfInt(Msg.Data, "Value:");

        if(CurSensorValue != SensorValue)
        {
            CurSensorValue = SensorValue;

            switch(DetectState)
            {
                case 0:

                    if(NULL != client)
                    {
                        if(CLIENT_STATE_CONNECTED == client->mqtt_client_state)
                        {
                            Msg2Json("IDLE", CurSensorValue);
                        }
                    }

                    break;

                case 1:
                    if(NULL != client)
                    {
                        if(CLIENT_STATE_CONNECTED == client->mqtt_client_state)
                        {
                            Msg2Json("CALI", CurSensorValue);
                        }
                    }

                    break;

                case 2:
                    if(NULL != client)
                    {
                        if(CLIENT_STATE_CONNECTED == client->mqtt_client_state)
                        {
                            Msg2Json("DETECT", CurSensorValue);
                        }
                    }

                    break;

                case 49:
                    if(NULL != client)
                    {
                        if(CLIENT_STATE_CONNECTED == client->mqtt_client_state)
                        {
                            Msg2Json("DETECT_SAFETY", CurSensorValue);
                        }
                    }

                    break;

                case 50:
                    if(NULL != client)
                    {
                        if(CLIENT_STATE_CONNECTED == client->mqtt_client_state)
                        {
                            Msg2Json("DETECT_DANGER", CurSensorValue);
                        }
                    }

                    break;
            }
        }
    }

ErrorHandler:
    printf("IotRecvTask Error!!!\n");
    return;
}

