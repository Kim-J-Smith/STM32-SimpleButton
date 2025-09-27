# include "example_timer_long_push.h"

void Btn_Long_Press_Handler(uint32_t long_press_time)
{
    /* 按键长按处理函数 long press handler */
    
    /* long_press_time 表示按键被按了多长时间 */
    /* long_press_time indicates how long the button is pressed */
    
    if(long_press_time < 5000) {
        /* do somthing when push less than 5s */
    } else if (long_press_time < 10000) {
        /* do somthing when push less than 10s */
    } else {
        /* do somthing when push more than 10s */
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();   

    Kim_Button_Init_myBtn0();

    /* 可以为每个按键单独设置冷却时间、最小长按时间、最大重复按判定时间 */

    Kim_Button_myBtn0.public_cool_down_time = 1000; // 冷却1秒
    Kim_Button_myBtn0.public_long_push_min_time = 2000; // 两秒以上才判定为长按
    Kim_Button_myBtn0.public_repeat_push_max_time = 0; // 不等待重复按判定
    while(1)
    {
        Kim_Button_myBtn0.method_asynchronous_handler(
            NULL,
            Btn_Long_Press_Handler,
            NULL    
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