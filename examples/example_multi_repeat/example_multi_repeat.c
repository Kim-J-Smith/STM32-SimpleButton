# include "kim_stm32_hal_button.h"

/* 解释非默认自定义宏选项 Explain the custom macro options */

/**
 * 这个选项默认为 0
 * 默认只提供双击事件回调，而不提供多击事件回调。
 * 此处需要多击事件回调，所以设置为 1 即可。
 * @note    多击事件回调启动后，用户提供的多击回调函数必须有且只有一个参数，
 *          参数类型为 uint8_t，此参数变量将接收按键的点击次数。
 *          (如果没有启动多击事件回调，用户提供的多击(双击)回调函数默认无参无返回值)
 * 
 * This option is 0 by default.
 * By default, only double-click event callbacks are provided, and no multi-click event callbacks are provided.
 * Here we need multi-click event callbacks, so set it to 1.
 * @note    When the multi-click event callback is started, the user-provided multi-click (double-click) callback function must have and only have one parameter,
 *          the parameter type is uint8_t, this parameter variable will receive the number of clicks of the button.
 *          (If the multi-click event callback is not started, the user-provided multi-click (double-click) callback function defaults to no parameter and no return value)
 */
#define KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT        1


// 进行按键注册 Do button registration
KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_0, EXTI_TRIGGER_FALLING, myBtn0)


