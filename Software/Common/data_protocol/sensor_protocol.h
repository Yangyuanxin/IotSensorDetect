#ifndef _SENSOR_PROTOCOL_H_
#define _SENSOR_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
	
	
typedef struct SensorProtocol
{
	uint8_t  FrameHeader;	    //帧头 Byte
	uint8_t  Length;					//数据长度
	uint8_t  Type;						//传感器类型
	uint8_t  ValueHBit;				//数据高位
	uint8_t  ValueLBit;				//数据低位
	uint8_t  ChecksumHBit;		//校验和高位
	uint8_t  ChecksumLBit;		//校验和低位
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
