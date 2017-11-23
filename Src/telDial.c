#include "telDial.h"
#include "stm32f1xx_hal.h"
#include "hal_key.h"

#define HT9200_CE_LOW		HAL_GPIO_WritePin(HT_CE_GPIO_Port,HT_CE_Pin,GPIO_PIN_RESET)
#define HT9200_CE_HIGH		HAL_GPIO_WritePin(HT_CE_GPIO_Port,HT_CE_Pin,GPIO_PIN_SET)

#define HT9200_DATA_LOW		HAL_GPIO_WritePin(HT_DATA_GPIO_Port,HT_DATA_Pin,GPIO_PIN_RESET)
#define HT9200_DATA_HIGH	HAL_GPIO_WritePin(HT_DATA_GPIO_Port,HT_DATA_Pin,GPIO_PIN_SET)

#define HT9200_CLK_LOW		HAL_GPIO_WritePin(HT_CLK_GPIO_Port,HT_CLK_Pin,GPIO_PIN_RESET)
#define HT9200_CLK_HIGH		HAL_GPIO_WritePin(HT_CLK_GPIO_Port,HT_CLK_Pin,GPIO_PIN_SET)

uint8_t keyTempFlag[12];



void sendOneBit(uint8_t dtmfData) {
	
	HT9200_CE_LOW;
	HAL_Delay(20);
	for (int i = 0; i<5; i++) {
		HT9200_CLK_HIGH;
		HAL_Delay(4);
		if (dtmfData & 0x01) {
			HT9200_DATA_HIGH;
			HAL_Delay(4);
		}
		else {
			HT9200_DATA_LOW;
			HAL_Delay(4);
		}
		HT9200_CLK_LOW;
		HAL_Delay(4);
		HT9200_CLK_HIGH;
		HAL_Delay(1);
		dtmfData >>= 1;
	}
	HAL_Delay(100);
	HT9200_CE_HIGH;
	HAL_Delay(10);
	HT9200_CLK_LOW;
}

void keyScan() {
	if (keyTempFlag[0] != keyFlag[0])
	{
		sendOneBit(0x0a);
		keyTempFlag[0] = keyFlag[0];
	}

	for (int i = 1; i <= 9; i++)
	{
		if (keyTempFlag[i] != keyFlag[i])
		{
			sendOneBit(i);
			keyTempFlag[i] = keyFlag[i];
		}
	}

	if (keyTempFlag[10] != keyFlag[10])
	{
		sendOneBit(0x0b);
		keyTempFlag[10] = keyFlag[10];
	}

	if (keyTempFlag[11] != keyFlag[11])
	{
		sendOneBit(0x0c);
		keyTempFlag[11] = keyFlag[11];
	}

}