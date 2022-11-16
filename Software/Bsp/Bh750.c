#include "Bh750.h"

Bh750_t Bh750Sensor;
extern I2C_HandleTypeDef hi2c1;

void Bh750SensorInit(uint8_t Mode)
{
    Bh750Sensor.Lux = 0;
    Bh750Sensor.Mode = Mode;
}

static void SendGetBh750LuxCmd(void)
{
    HAL_I2C_Master_Transmit(&hi2c1, WRITE_ADDRESS, \
    (uint8_t *)&Bh750Sensor.Mode, 1, 0xff);
}

static int32_t GetBh750Lux(void)
{
		float Lux = 0;
		uint8_t ReadData[2] = {0};
		
    if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, READ_ADDRESS, ReadData, 2, 0xff))
    {
        Lux = (float)((ReadData[0] << 8) | ReadData[1]);
        Lux = (double)Lux / 1.2;
        Bh750Sensor.Lux = (uint16_t)Lux;
				return Bh750Sensor.Lux;
    }
		
		return -1;
}

void ReadBh750Lux(int32_t *Lux, uint32_t Ms, void(*Delay)(uint32_t))
{
    static uint8_t Status = 0 ;

    switch(Status)
    {
        case 0:
						MX_I2C1_Init();
            SendGetBh750LuxCmd();
            Status = 1;
            
            break;

        case 1:
						Delay(Ms);
						Status = 0;
						*Lux = GetBh750Lux();
            break;

        default:
            break;
    }
}
