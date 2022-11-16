#ifndef _SENSOR_PROTOCOL_H_
#define _SENSOR_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
	
	
typedef struct SensorProtocol
{
	uint8_t  FrameHeader;	    //֡ͷ Byte
	uint8_t  Length;					//���ݳ���
	uint8_t  Type;						//����������
	uint8_t  ValueHBit;				//���ݸ�λ
	uint8_t  ValueLBit;				//���ݵ�λ
	uint8_t  ChecksumHBit;		//У��͸�λ
	uint8_t  ChecksumLBit;		//У��͵�λ
}SensorProtocol;

extern SensorProtocol Protocol;

uint8_t SensorGetType(void);
void SetSensorType(uint8_t Type);
void SensorProcolPacket(uint8_t Type, uint16_t val);
void FunCal(uint16_t arr[240], int n, uint16_t *max, uint16_t *min);
	
#ifdef __cplusplus
}
#endif
#endif  //ifndef _SENSOR_PROTOCOL_H_
