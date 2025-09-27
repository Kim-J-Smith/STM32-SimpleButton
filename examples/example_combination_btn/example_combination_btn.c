# include "kim_stm32_hal_button.h"

/* 解释非默认自定义宏选项 Explain the custom macro options */

/**
 * 按键名称前缀
 * 这个宏默认为 Kim_Button_
 * 此处设置为 Btn_ ，后续调用按键初始化函数以及按键本身时
 * 需要在函数名称前加上 Btn_ 前缀(而非默认的Kim_Button_)，例如 Btn_Init_button_0() 
 * 和 Btn_button_0.method_asynchronous_handler()
 * 
 * Button name prefix
 * This macro is Kim_Button_ by default.
 * Here we set it to Btn_, and when we call the button initialization function and the button itself,
 * we need to add the Btn_ prefix (not the default Kim_Button_) before the function name, for example Btn_Init_button_0()
 * and Btn_button_0.method_asynchronous_handler()
 */
#define KIM_BUTTON_NAME_PREFIX                      Btn_

/**
 * 这个宏默认为 0
 * 此处设置为 1 ，表示支持按键组合，即同时按下多个按键时，
 * 会触发特殊事件，具体示例见 main.c 文件
 * 
 * This macro is 0 by default.
 * Here we set it to 1, which means that button combination is supported, that is, when multiple buttons are pressed at the same time,
 * a special event will be triggered, for example see the main.c file
 */
#define KIM_BUTTON_ENABLE_BUTTON_COMBINATION        1

KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_0, EXTI_TRIGGER_FALLING, button_0)
KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_1, EXTI_TRIGGER_FALLING, button_1)
KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_2, EXTI_TRIGGER_FALLING, button_2)
KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_3, EXTI_TRIGGER_FALLING, button_3)

