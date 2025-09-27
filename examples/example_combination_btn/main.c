# include "example_combination_btn.h"

void Combination_Push_0__Then_Push_1(void)
{
    // 先按下按键0，再按下按键1，触发该事件
    // Push button 0 first, then push button 1, trigger this event
}

void Combination_Push_0__Then_Push_2(void)
{
    // 先按下按键0，再按下按键2，触发该事件 
    // Push button 0 first, then push button 2, trigger this event
}

void Combination_Push_1__Then_Push_0(void)
{
    // 先按下按键1，再按下按键0，触发该事件
    // Push button 1 first, then push button 0, trigger this event
}


int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // 初始化按键 Initialize the button
    Btn_Init_button_0();
    Btn_Init_button_1();
    Btn_Init_button_2();
    Btn_Init_button_3();

/* 设置组合键 0 --> 1 的触发事件 */
    /* Set the trigger event of the combination key 0 --> 1 */

    // 设置前驱按键 Set the predecessor button
    Btn_button_1.public_comb_before_button = &Btn_button_0; 
    // 设置触发事件 Set the trigger event
    Btn_button_1.public_comb_callback = Combination_Push_0__Then_Push_1; 

/* 设置组合键 0 --> 2 的触发事件 */
    /* Set the trigger event of the combination key 0 --> 2 */

    // 设置前驱按键 Set the predecessor button
    Btn_button_2.public_comb_before_button = &Btn_button_0; 
    // 设置触发事件 Set the trigger event
    Btn_button_2.public_comb_callback = Combination_Push_0__Then_Push_2; 

/* 设置组合键 1 --> 0 的触发事件 */
    /* Set the trigger event of the combination key 1 --> 0 */

    // 设置前驱按键 Set the predecessor button
    Btn_button_0.public_comb_before_button = &Btn_button_1; 
    // 设置触发事件 Set the trigger event
    Btn_button_0.public_comb_callback = Combination_Push_1__Then_Push_0; 

    /* ... */

    while(1)
    {
        // 调用按键的异步处理函数 call the asynchronous handler of the button
        Btn_button_0.method_asynchronous_handler(NULL, NULL, NULL);
        Btn_button_1.method_asynchronous_handler(NULL, NULL, NULL);
        Btn_button_2.method_asynchronous_handler(NULL, NULL, NULL);
        Btn_button_3.method_asynchronous_handler(NULL, NULL, NULL);

        // 当所有按键空闲时进入低功耗模式 Enter low power mode when all buttons are idle
        KIM_BUTTON__LOW_POWER(Btn_button_0, Btn_button_1, Btn_button_2, Btn_button_3);
    }
}

// 如果使用了CubeMX设置NVIC，就可以这样写，HAL_GPIO_EXTI_Callback会被正真的中断处理函数调用
// If you use CubeMX to set NVIC, you can write it like this, HAL_GPIO_EXTI_Callback will be called by the real interrupt handler
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_0)
    {
        // 调用按键的中断处理函数 call the interrupt handler of the button
        Btn_button_0.method_interrupt_handler();
        Btn_button_1.method_interrupt_handler();
        Btn_button_2.method_interrupt_handler();
        Btn_button_3.method_interrupt_handler();
    }
}

// 如果没有使用CubeMX设置NVIC，可以这样写
// If you don't use CubeMX to set NVIC, you can write it like this
void EXTI0_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != 0)
    {
        // 调用按键的中断处理函数
        Btn_button_0.method_interrupt_handler();
        Btn_button_1.method_interrupt_handler();
        Btn_button_2.method_interrupt_handler();
        Btn_button_3.method_interrupt_handler();
        __HAL_GPIO_EXTI_CLEAR_IT();
    }
}
