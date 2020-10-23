#include "lowpower.h"
#include "main.h"

void LPM_EnterStopMode(void)
{
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPower();
  
  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableFastWakeUp();
	
  /* Select HSI as system clock source after Wake Up from Stop mode */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);
  
	__HAL_RCC_PWR_CLK_ENABLE();
	
  HAL_SuspendTick();
	/* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	HAL_ResumeTick();
	
}

void LPM_DisableStopMode(void)
{
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	
}
