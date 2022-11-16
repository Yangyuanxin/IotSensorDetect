#include "MQ_2.h"

extern ADC_HandleTypeDef hadc1;

uint32_t GetSmokeValue(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 50);
    return HAL_ADC_GetValue(&hadc1);
}
