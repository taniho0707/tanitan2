/**
 * @file main.cpp
 */
#include "main.h"


static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

static void Delay(__IO uint32_t nTime);


int main(void){
	SystemInit();
	SysTick_Config(SystemCoreClock / 1000);

	Led::init();
	Switch::init();
	Led::on(LedNumbers::FRONT);
	Delay(500);
	Led::off(LedNumbers::FRONT);

	while(true){
		// if(Switch::isPushing(SwitchNumbers::LEFT))
		// 	Led::on(LedNumbers::FRONT);
		// else
		// 	Led::off(LedNumbers::FRONT);
	}

}


void Delay(__IO uint32_t nTime){ 
	uwTimingDelay = nTime;
	while(uwTimingDelay != 0);
}

void TimingDelay_Decrement(void){
	if (uwTimingDelay != 0x00){ 
		uwTimingDelay--;
	}
}




#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line){ 
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* Infinite loop */
	while (1){
	}
}
#endif
