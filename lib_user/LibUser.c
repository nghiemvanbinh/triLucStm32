#include "LibUser.h"
#include "stdint.h"
#include "gpio.h"
#include "cJson.h"
//---------cjson---------------//
void json_esp32(char* data)
{
	  cJSON *root2 = cJSON_Parse(data);
    if (cJSON_GetObjectItem(root2, "luc"))
    {
        char *version = cJSON_GetObjectItem(root2, "luc")->valuestring;
      //  printf("luc=%d\n", version);
    }
//    if (cJSON_GetObjectItem(root2, "position"))
//    {
//        int version = cJSON_GetObjectItem(root2, "position")->valueint;
//        printf("possition=%d", version);
//    }
    
    cJSON_free(root2);
}
//---------maybom--------------//
void airConditioner(uint16_t adc, uint16_t upperLimit)
{
	static int count = 0,countRising=0;
	if(adc < upperLimit){
		count++;
		countRising=0;
		if(count >= 800)
		{
			HAL_GPIO_WritePin(air_compressor_GPIO_Port,air_compressor_Pin,GPIO_PIN_SET);
		}
	}
	else{
		count = 0;
		countRising++;
		if(countRising> 800)
		{
		HAL_GPIO_WritePin(air_compressor_GPIO_Port,air_compressor_Pin,GPIO_PIN_RESET);
		}
	}
}
//-----------sangled------------//
void ledSign(int i){
	if(i ==0)
		{
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_00_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_01_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_02_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_03_Pin,GPIO_PIN_RESET);
		}
		else if(i==1)
		{
		  HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_00_Pin,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_01_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_02_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_03_Pin,GPIO_PIN_RESET);
		}
		else if(i==2)
		{
		  HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_00_Pin,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_01_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_02_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_03_Pin,GPIO_PIN_RESET);
		}
		else if(i==3)
		{
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_00_Pin,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_01_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_02_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_03_Pin,GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_00_Pin,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_01_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_02_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CTL_00_GPIO_Port,LED_CTL_03_Pin,GPIO_PIN_RESET);
			
		}

}
//----------tra-ve-tiem-can--------//
uint8_t position()// trai, phai, giua
{
	uint8_t a,  b,  c;
	a =   HAL_GPIO_ReadPin(tiem_can1_GPIO_Port,tiem_can1_Pin);// tiem can trai
  b =  HAL_GPIO_ReadPin(tiem_can2_GPIO_Port,tiem_can2_Pin);// tiem can phai
  c =  HAL_GPIO_ReadPin(tiem_can3_GPIO_Port,tiem_can3_Pin);// tiem can giua
	  uint8_t dich_trai, gia_tri;
    dich_trai = 0;
	  gia_tri=0;
		gia_tri   = a;
	  dich_trai = dich_trai |(gia_tri<< 2);
	  gia_tri   = b;
	  dich_trai = dich_trai |(gia_tri<<1);
	  gia_tri   = c;
	  dich_trai = dich_trai | gia_tri;
	  switch(dich_trai)
		{
			case 1:
				dich_trai = 2;// dám thang
			break;
			case 2:
			 dich_trai = 1;// punch left
			break;
			case 3:
			 dich_trai = 1; // punch left
			break;
			case 4:
				dich_trai = 3;// punch right
			case 5:
				dich_trai = 3;// punch right
			break;
			case 6:
				dich_trai = 4;// pucnh tall
			break;
			case 7:
			dich_trai = 4;// moc len
			break;
			default:
			 dich_trai = 0;
				break;
		}
		return dich_trai;
}

