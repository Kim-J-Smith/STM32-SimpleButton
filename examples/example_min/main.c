# include "stm32f1xx_hal.h"
# include "example_min.h"


// 无参，无返回值。 No parameter, no return value.
void Btn_Short_Press_Handler(void)
{
    /* 按键短按处理函数 short press handler */
}

// 无参，无返回值。 No parameter, no return value.
void Btn_Long_Press_Handler(void)
{
    /* 按键长按处理函数 long press handler */
}

// 无参，无返回值。 No parameter, no return value.
void Btn_Repeat_Press_Handler(void)
{
    /* 按键重复按处理函数 repeat press handler */
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // 初始化按键 Initialize the button
    Kim_Button_Init_myBtn0();
    while(1)
    {
        // 调用按键的异步处理函数 call the asynchronous handler of the button
        Kim_Button_myBtn0.method_asynchronous_handler(
            Btn_Short_Press_Handler,
            Btn_Long_Press_Handler,
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


