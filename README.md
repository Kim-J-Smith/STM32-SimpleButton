# STM32-SimpleButton
[Chinese]: 一个非常精简的STM32按键框架，适配STM32 HAL库。[English]: A very tiny STM32 key(button) frame, compatible with the STM32 HAL library.

---

### [Chinese]:

#### 简介：

* 本项目仅含**一个**文件，即 `kim_stm32f1xx_hal_button.h` 。只需要使用一个宏定义即可生成全部所需代码。

#### 使用方法：

* 首先，假设我们有三个文件（`main.c` , `my_button.c` , `my_button.h` ）。其中，my_button.c 文件存放按键代码，my_button.h 文件存放必要的声明，main.c 调用代码。

* 然后，在 `my_button.c` 中，先引入头文件`kim_stm32f1xx_hal_button.h`，使用 **KIM_BUTTON__REGISTER** 宏 来生成所需要的代码。（示例如下： 我的按钮触发时会在 **PA7** 产升**下降沿** 信号， 我想给按钮取名为**myButton**  ）

```c
/* 以下是 my_button.c 内容 */
#include "kim_stm32f1xx_hal_button.h" // 包含头文件

// 依次为            端口基地址    引脚编号       触发边沿选择       按键名称 
KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_7, EXTI_TRIGGER_FALLING, myButton) // 注意不用加 ;

```

* 接着，在 `my_button.h` 中，先引入头文件`kim_stm32f1xx_hal_button.h`，使用 **KIM_BUTTON__DECLARE** 宏 来生成必要的声明信息。（注意： 声明的按钮名称必须是 **KIM_BUTTON__REGISTER** 宏 定义过的）

```c
/* 以下是 my_button.h 内容 */
#pragma once
#include "kim_stm32f1xx_hal_button.h" // 包含头文件

// 按钮名称必须与 my_button.c 中保持一致
KIM_BUTTON__DECLARE(myButton) // 注意不用加 ;

```

* 最后，在 `main.c` 中，引入头文件 `my_button.h` ，准备好回调函数，然后在三处调用三个函数即可。示例与详细说明如下：

```c
/* 以下是 main.c 内容 */
/* ... */
#include "my_button.h"

// 短按回调函数，按键短按后会执行它 (函数名随意)
void short_push_callback(void) { ... }

// 长按回调函数，长按后会执行它 (函数名随意)
void long_push_callback(void) { ... }

// 双击回调函数，双击后会执行它 (函数名随意)
void double_push_callback(void) { ... }

int main(void)
{
    /* ... 其他无关代码 ... */

    // 【第一处】：在 while 循环之前，调用初始化函数。初始化函数名称为 Kim_Button_Init_ 加上你自定义的按键名。
    //     例如：我在my_button.c和my_button.h中还定义了一个按键，名叫 ABC，
    //     那么此处我应该再调用一个函数 Kim_Button_Init_ABC()
    Kim_Button_Init_myButton();

    while(1)
    {
        /* ... 其他无关代码 ... */

        // 【第二处】：在while循环当中调用异步处理函数，调用的方法类似成员函数，依次传入 短/长/双 按回调函数。
        //     补充： （Kim_Button_ 加上 按钮名称） 这个结构体是全局变量，我们通过它调用函数。
        //     如果不需要某个回调函数，可以传入NULL。
        Kim_Button_myButton.method_asynchronous_handler(
            short_push_callback， // 如果不需要，可以填NULL
            long_push_callback，  // 如果不需要，可以填NULL
            double_push_callback  // 如果不需要，可以填NULL
        );

        /* ... 其他无关代码 ... */
    }
}

// 【第三处】：在相应的中断回调函数中使用按键的中断处理函数
//        如果你使用了 STM32CubeMX 生成了 NVIC 对应代码，可以在 HAL_GPIO_EXTI_Callback()函数中调用
//        如果没有使用 STM32CubeMX 生成对应中断代码，则需要在 EXTI?_IRQHandler()函数中调用(? 为0~4，9_5或15_10)
//        下面演示两种写法

// 写法一：
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
     /* ... 其他无关代码 ... */
    if(GPIO_Pin == GPIO_PIN_7) // 假设我的按钮链接的是 PA7
    {
        // 调用按键的中断处理函数
        Kim_Button_myButton.method_interrupt_handler();
    }
     /* ... 其他无关代码 ... */
}

// 写法二：（可以尝试在stm32_xxxx_it.c文件中找一下这个回调函数，如果没有就自己写）
void EXTI7_IRQHandler(void) // 假设我的按钮链接的是 PA7
{
     /* ... 其他无关代码 ... */
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != 0)
    {
        // 调用按键的中断处理函数
        Kim_Button_myButton.method_interrupt_handler();
        __HAL_GPIO_EXTI_CLEAR_IT();
    }
     /* ... 其他无关代码 ... */
}
```

#### 注意事项：

* 使用了SysTick，可能会与HAL_Delay冲突。

* 每一个EXTI端口号只能有一个按钮，也就是说PA3与PB3不能同时作为按钮引脚。
  
  

### [English]:

#### Brief introduction:

* This project contains only one file, namely `kim_stm32f1xx_hal_button.h` . All the required code can be generated simply by using one macro definition.

#### How to use：

* First, suppose we have three files (' main.c ', 'my_button.c', 'my_button.h'). Among them, the my_button.c file stores the key codes, the my_button.h file stores the necessary declarations, and the main.c call code.

* Then, in 'my_button.c', first import the header file 'kim_stm32f1xx_hal_button.h', and use the **KIM_BUTTON__REGISTER** macro to generate the required code. When my button is triggered, it will produce a rising and falling edge signal at **PA7**. I want to name the button **myButton**
  
  ```c
  /* The following is the content of my_button.c */ 
  #include "kim_stm32f1xx_hal_button.h" // Include header files
  
  // The sequence is port base address, pin number, trigger edge selection, and key name
  KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_7, EXTI_TRIGGER_FALLING, myButton) // Note: No need to add ;
  ```
  
  

* Next, in 'my_button.h', first import the header file 'kim_stm32f1xx_hal_button.h', and use the **KIM_BUTTON__DECLARE** macro to generate the necessary declaration information. (Note: The declared button name must be defined by the **KIM_BUTTON__REGISTER** macro)
  
  ```c
  /* The following is the content of my_button.h */ 
  #pragma once 
  #include "kim_stm32f1xx_hal_button.h" // Include header files
  
  // The button name must be consistent with that in my_button.c
  KIM_BUTTON__DECLARE(myButton) // Note: No need to add ;
  ```

* Finally, in main.c, import the header file my_button.h, prepare the callback function, and then call the three functions in three places. Examples and detailed explanations are as follows:
  
  

```c
/* 以下是 main.c 内容 */ 
/* ... */
#include "my_button.h" 

// Press the callback function briefly. After pressing the key briefly, it will be executed (the function name is arbitrary)
void short_push_callback(void) { ... } 

// 长按回调函数，长按后会执行它 (函数名随意) 
void long_push_callback(void) { ... } 

// Long press the callback function. After long pressing, it will be executed (the function name is arbitrary).
void double_push_callback(void) { ... } 

int main(void) 
{
  /* ... Other irrelevant code ... */

  // 【 First Place 】 : Call the initialization function before the while loop. The name of the initialization function is Kim_Button_Init_ plus the key name you defined.
  //     For example: I also defined a key named ABC in my_button.c and my_button.h.
  //     Then here I should call another function, Kim_Button_Init_ABC()
  Kim_Button_Init_myButton();

  while(1)
  {
      /* ... Other irrelevant code ... */

      // 【 Second Point 】 : In a while loop, an asynchronous processing function is called. The method of the call is similar to a member function, and short/long/double-press callback functions are passed in sequence.
      //     Supplementary: (Kim_Button_ plus the button name) This structure is a global variable, and we call functions through it.
      //     If a certain callback function is not needed, NULL can be passed in.
      Kim_Button_myButton.method_asynchronous_handler(
          short_push_callback， // If not needed, NULL can be filled in
          long_push_callback，  // If not needed, NULL can be filled in
          double_push_callback  // If not needed, NULL can be filled in
      );

      /* ... Other irrelevant code ... */
  }

} 
// 【 Third Point 】 : Use the interrupt handling function of the key in the corresponding interrupt callback function
// If you have generated the corresponding code for NVIC using STM32CubeMX, you can call it in the HAL_GPIO_EXTI_Callback() function
// If the corresponding interrupt code is not generated using STM32CubeMX, it needs to be called in the EXTIx_IRQHandler() function (x is 0~ 4,9 _5 or 15_10).
// The following demonstrates two writing methods

// Writing Method One:
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
   /* ... Other irrelevant code ... */
  if(GPIO_Pin == GPIO_PIN_7) // 假设我的按钮链接的是 PA7
  {
      // Call the interrupt handling function of the key
      Kim_Button_myButton.method_interrupt_handler();
  }
   /* ... Other irrelevant code ... */
} 

// Writing Method Two: (You can try to find this callback function in the stm32_xxxx_it.c file. If it's not available, write it yourself.
void EXTI7_IRQHandler(void) // 假设我的按钮链接的是 PA7 
{
   /* ... Other irrelevant code ... */
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != 0)
  {
      // Call the interrupt handling function of the key
      Kim_Button_myButton.method_interrupt_handler();
      __HAL_GPIO_EXTI_CLEAR_IT();
  }
   /* ... Other irrelevant code ... */
}
```

#### Note：

* SysTick is used, which may conflict with HAL Delay()

* Each EXTI port number can only have one button, which means that PA3 and PB3 cannot be used as button pins simultaneously.
