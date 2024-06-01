#include <ch32v00x.h>
#include <debug.h>

#define BLINKY_GPIO_PIN GPIO_Pin_6

#define LEDR GPIO_Pin_4
#define LEDY GPIO_Pin_2
#define LEDG GPIO_Pin_3
#define LEDI GPIO_Pin_1

#define CLOCK_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE)

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Delay_Init(void);
void Delay_Ms(uint32_t n);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();

	GPIO_InitTypeDef GPIO_InitStructure = {0};

	CLOCK_ENABLE;
	
	GPIO_InitStructure.GPIO_Pin = BLINKY_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//pin 1 OUT PIN FOR IR SENSOR
GPIO_InitStructure.GPIO_Pin = LEDI ; // Defines which Pin to configure
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Defines Output Type
GPIO_Init(GPIOD, &GPIO_InitStructure);

//pins for LED-R,Y,G
GPIO_InitStructure.GPIO_Pin = LEDY | LEDG | LEDR ; // Defines which Pin to configure
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Defines Output Type
GPIO_Init(GPIOD, &GPIO_InitStructure);


	//uint8_t ledState = 0;
	uint8_t IR=1;

	while (1)
	{
		IR = GPIO_ReadInputDataBit(GPIOD, LEDI);
		if(IR==0){
			GPIO_WriteBit(GPIOD, LEDG, 0);
			GPIO_WriteBit(GPIOD, LEDY, 1);
			GPIO_WriteBit(GPIOD, LEDR, 0);

			Delay_Ms(2000);
			IR = GPIO_ReadInputDataBit(GPIOD, LEDI);

			while(IR==0){
				GPIO_WriteBit(GPIOD, LEDY, 0);
				GPIO_WriteBit(GPIOD, LEDR, 1);

				IR = GPIO_ReadInputDataBit(GPIOD, LEDI);
			}
		}
		else{
			GPIO_WriteBit(GPIOD, LEDG, 1);
			GPIO_WriteBit(GPIOD, LEDY, 0);
			GPIO_WriteBit(GPIOD, LEDR, 0);
		}
		// GPIO_WriteBit(GPIOD, BLINKY_GPIO_PIN, ledState);
		// ledState ^= 1; // invert for the next run
		// Delay_Ms(500);
	}
}

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
	while (1)
	{
	}
}
