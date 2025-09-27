# include "example_multi_repeat.h"

// 有参，无返回值。No return value, only one parameter.
void Btn_Repeat_Press_Handler(uint8_t repeat_times)
{
    /* 按键重复按处理函数 repeat press handler */

    /* repeat_times 表示按键被按了几次 */
    /* repeat_times indicates how many times the button is pressed */
    switch (repeat_times)
    {
    case 2:
        /* do somthing when push 2 times */
        break;
    case 3:
        /* do somthing when push 3 times */
        break;
    case 4:
        /* do somthing when push 4 times */
        break;
    case 5:
        /* do somthing when push 5 times */
        break;
    /* ... */
    default:
        break;
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();   

    Kim_Button_Init_myBtn0();
    while(1)
    {
        Kim_Button_myBtn0.method_asynchronous_handler(
            NULL,
            NULL,
            Btn_Repeat_Press_Handler    
        );
    }
}

// 如果使用了CubeMX设置NVIC，就可以这样写，HAL_GPIO_EXTI_Callback会被正真的中断处理函数调用
// If you use CubeMX to set NVIC, you can write it like this, HAL_GPIO_EXTI_Callback will be called by the real interrupt handler
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_0)
    {
        // 调用按键的中断处理函数 call the interrupt handler of the button
        Kim_Button_myBtn0.method_interrupt_handler();
    }
}

// 如果没有使用CubeMX设置NVIC，可以这样写
// If you don't use CubeMX to set NVIC, you can write it like this
void EXTI0_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != 0)
    {
        // 调用按键的中断处理函数
        Kim_Button_myBtn0.method_interrupt_handler();
        __HAL_GPIO_EXTI_CLEAR_IT();
    }
}

