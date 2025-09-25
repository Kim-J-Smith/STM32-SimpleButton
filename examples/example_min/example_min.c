# include "kim_stm32_hal_button.h"

/* 解释非默认自定义宏选项 Explain the custom macro options */

/**
 * 这个选项默认为 0
 * 默认允许用户对多个按键的时间参数（长按判定时间、双击判定时间）进行不同的设置。
 * 此处只有一个按键，所以设置为 1 即可，可以节省RAM空间。
 * 
 * This option is 0 by default.
 * By default, users are allowed to set different time parameters for multiple buttons (long press judgment time, double click judgment time).
 * Here only one button, so set it to 1 to save RAM space.
 **/ 
#define KIM_BUTTON_ONLY_USE_DEFAULT_TIME            1


// 进行按键注册 Do button registration
KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_0, EXTI_TRIGGER_FALLING, myBtn0)


