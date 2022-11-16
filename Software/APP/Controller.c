#include "Controller.h"

extern mqtt_client_t *client;
extern k_msg_q_t Controller2Model;
k_event_t report_result_event;
k_event_flag_t CtrlDetect = 1 << 0;
k_event_flag_t CtrlStop   = 1 << 1;
k_event_flag_t CtrlBack   = 1 << 2;

osThreadDef(ControllerTask, osPriorityNormal, 1, CONTROLLER_STK_SIZE);

void ControllerTaskInit(void)
{
    osThreadCreate(osThread(ControllerTask), NULL);
}


static void tos_topic_handler(void* client, message_data_t* msg)
{
    cJSON *root = NULL;
    cJSON *token = NULL;
    cJSON *method = NULL;
    cJSON *params = NULL;
    cJSON *status = NULL;
    cJSON *ControlState = NULL;

    /* 打印日志 */
    MQTT_LOG_I("-----------------------------------------------------------------------------------");
    MQTT_LOG_I("%s:%d %s()...\ntopic: %s, qos: %d. \nmessage:\n\t%s\n", __FILE__, __LINE__, __FUNCTION__,
               msg->topic_name, msg->message->qos, (char*)msg->message->payload);
    MQTT_LOG_I("-----------------------------------------------------------------------------------\n");

    root = cJSON_Parse((char*)msg->message->payload);

    if (!root)
    {
        printf("Invalid json root\r\n");
        return;
    }

    method = cJSON_GetObjectItem(root, "method");

    if (!method)
    {
        printf("Invalid json method\r\n");
        cJSON_Delete(root);
        return;
    }

    //上报回复
    if(strstr(method->valuestring, "report_reply"))
    {
        token = cJSON_GetObjectItem(root, "clientToken");

        if (!token)
        {
            printf("Invalid json token\r\n");
            cJSON_Delete(root);
            return;
        }

        status = cJSON_GetObjectItem(root, "status");

        if (!status)
        {
            printf("Invalid json status\r\n");
            cJSON_Delete(root);
            return;
        }

        if(strstr(status->valuestring, "success"))
            printf("上报数据成功!\n");
        else
            printf("上报数据失败!\n");
    }
    //控制命令
    else if(strstr(method->valuestring, "control"))
    {
        token = cJSON_GetObjectItem(root, "clientToken");

        if (!token)
        {
            printf("Invalid json token\r\n");
            cJSON_Delete(root);
            return;
        }

        params = cJSON_GetObjectItem(root, "params");

        if (!params)
        {
            printf("Invalid json params\r\n");
            cJSON_Delete(root);
            return;
        }

        ControlState = cJSON_GetObjectItem(params, "ControlState");

        if (!ControlState)
        {
            printf("Invalid json state\r\n");
            cJSON_Delete(root);
            return;
        }

        if(strstr(ControlState->valuestring, "START"))
            tos_event_post(&report_result_event, CtrlDetect);
        else if(strstr(ControlState->valuestring, "STOP"))
            tos_event_post(&report_result_event, CtrlStop);
        else if(strstr(ControlState->valuestring, "PREV"))
            tos_event_post(&report_result_event, CtrlBack);
    }

    cJSON_Delete(root);
    root = NULL;
}

void ControllerTask(void *pdata)
{
    int error;
    char  host_ip[20];
    mqtt_message_t msg;
    struct Msg_t Send2ModelMsg;
    k_event_flag_t match_flag;

    memset(&msg, 0, sizeof(msg));
    esp8266_sal_init(HAL_UART_PORT_0);
    esp8266_join_ap("602", "13602584536");

    mqtt_log_init();
    client = mqtt_lease();

    tos_event_create(&report_result_event, (k_event_flag_t)0u);

    tos_sal_module_parse_domain("TM9854KHIS.iotcloud.tencentdevices.com", host_ip, sizeof(host_ip));

    mqtt_set_port(client, "1883");
    mqtt_set_host(client, host_ip);
    //使用qcloud-mqtt-sign工具根据规则生成，网址：https://github.com/tencentyun/qcloud_iot_mqtt_sign
    mqtt_set_client_id(client, "TM9854KHISDetectMachine");
    mqtt_set_user_name(client, "TM9854KHISDetectMachine;21010406;fYC6m;92233720368547758");
    mqtt_set_password(client, "d1dcc270b744a6e177c1143a39356c77a67a35c5;hmacsha1");
    mqtt_set_clean_session(client, 1);

    error = mqtt_connect(client);

    MQTT_LOG_D("mqtt connect error is %#0x", error);

    error = mqtt_subscribe(client, "$thing/down/property/TM9854KHIS/DetectMachine", QOS0, tos_topic_handler);

    MQTT_LOG_D("mqtt subscribe error is %#0x", error);

    while(1)
    {
        tos_event_pend(&report_result_event,
                       CtrlDetect | CtrlStop | CtrlBack,
                       &match_flag,
                       TOS_TIME_FOREVER,
                       TOS_OPT_EVENT_PEND_ANY | TOS_OPT_EVENT_PEND_CLR);
        
        memset(&Send2ModelMsg, 0, sizeof(struct Msg_t));
        if (match_flag == CtrlDetect)
        {
            Send2ModelMsg.Type = 0;
            tos_msg_q_post(&Controller2Model, (void *) &Send2ModelMsg);
        }
        else if (match_flag == CtrlStop)
        {
            Send2ModelMsg.Type = 1;
            tos_msg_q_post(&Controller2Model, (void *) &Send2ModelMsg);
        }
        else if (match_flag == CtrlBack)
        {           
            Send2ModelMsg.Type = 3;
            tos_msg_q_post(&Controller2Model, (void *) &Send2ModelMsg);
        }

        osDelay(500);
    }
}
