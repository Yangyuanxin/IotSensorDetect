#ifndef _BH750_H_
#define _BH750_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "i2c.h"
#include <stdint.h>
#include "stm32l4xx_hal.h"

/*���չ�ǿ��ʱֵ*/
#define TIMEOUT 200
/*��ǿֵ�����ֵ*/
#define LIGHT_SENSOR_THREHOLD 30


/*mode:ģʽѡ��*/
//�����߷ֱ���ģʽ���� 1 lux
#define LUX_1_MODE   0x10
//�����߷ֱ���ģʽ2���� 0.5 lux
#define LUX_0_5_MODE 0x11
//�ͷֱ���ģʽ
#define LUX_LOW_MODE 0x13

#define WRITE_ADDRESS 0x46		//0100 011  0
#define READ_ADDRESS  0x47		//0100 011  1

typedef struct Bh750_t
{
	uint16_t Lux;
	uint8_t Mode;

}Bh750_t;
	
void Bh750SensorInit(uint8_t Mode);
void ReadBh750Lux(int32_t *Lux, uint32_t Ms, void(*Delay)(uint32_t));
	
#ifdef __cplusplus
}
#endif
#endif  //ifndef _BH750_H_
