# include "kim_stm32_hal_button.h"

/* 解释非默认自定义宏选项 Explain the custom macro options */

/**
 * 这个选项默认为 0
 * 默认只支持普通长按事件回调，不支持不同时间长按事件回调。
 * 此处需要不同时间长按事件回调（会向回调函数传入长按时间），所以设置为 1 即可。
 * @note    不同时间长按事件回调启动后，用户提供的长按回调函数必须有且只有一个参数，
 *          参数类型为 uint32_t，此参数变量将接收长按时间。
 *          (如果没有启动不同时间长按事件回调，用户提供的长按回调函数默认无参无返回值)
 * 
 * This option is 0 by default.
 * By default, only normal long-press event callbacks are supported, and different time long-press event callbacks are not supported.
 * Here we need different time long-press event callbacks (will pass the long-press time to the callback function), so set it to 1.
 * @note    When the different time long-press event callback is started, the user-provided long-press callback function must have and only have one parameter,
 *          the parameter type is uint32_t, this parameter variable will receive the long-press time.
 *          (If the different time long-press event callback is not started, the user-provided long-press callback function defaults to no parameter and no return value)
 * 
 */
#define KIM_BUTTON_ENABLE_DIFFERENT_TIME_LONG_PUSH  1

// 进行按键注册 Do button registration
KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_0, EXTI_TRIGGER_FALLING, myBtn0)

