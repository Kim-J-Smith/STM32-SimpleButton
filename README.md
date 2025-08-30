# STM32-SimpleButton

[Chinese] :  一个非常精简的STM32按键框架，适配STM32 HAL库，支持每个按键独立的短按/长按/双击，非阻塞。

[English] : A very tiny STM32 key(button) frame, compatible with the STM32 HAL library, which offer short-press/long-press/double-press for each button, non-blocking.

---

### 新增功能特性(v0.1.1)：

+ ✅ **更加精简**：新增设置选项可以降低大量使用按键时的RAM占用

### 已有功能特性：

+ ✅ **按键事件完善**：支持短按、长按、双击 （长按判定时间可以每个按键单独配置）

+ ✅ **静态参数检查**：静态检查宏函数参数，确保生成代码准确、可靠

+ ✅ **软件消抖**：采用状态机，非阻塞消抖

+ ✅ **二次确认**：状态机内部对引脚状态二次确认，屏蔽抖动与意外触发中断的影响

+ ✅ **回调支持**：每个按键短按、长按、双击均支持独立的回调函数注册，回调函数允许为空

+ ✅ **内存精简**：数据结构紧凑，内存占用少

+ ✅ **配置便捷**：配置相关宏定义集中在文件开头，注释详尽

+ ✅ **立刻开始**：项目只有一个文件，仅需使用一个宏定义即可生成所需代码

+ ✅ **跨平台友好**：支持GCC与ArmCC等编译器

+ ✅ **临界区保护**：多线程数据安全、不冲突

+ ✅ **调试模式**：增加调试期生效死循环(需定义宏DEBUG)，精准锁定异常

+ ✅ **临界区保护优化**：单线程危险临界区单独默认保护，多线程临界区可选保护

+ ✅ **智能内联**：修改内联方式，智能内联函数，大幅减少ROM占用

+ ✅ **异步处理**：在重循环负载情况下，外部中断触发保证按键请求不会被忽略

+ ✅ **按键定制**：支持每个按键单独设置各个判定时间

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



#### 动态设置：

* 可以在代码中为每个按键设置独立的长按判定时间，示例如下：

```c
// 先初始化
Kim_Button_Init_myButton();

// 将 myButton 按键的长按判定时间延长至3000ms(改设置必须在 Init 函数之后)
Kim_Button_myButton.public_long_push_min_time = 3000;
```

+ 可以在代码中为每个按键设置独立的冷却时间，示例如下：

```c
// 先初始化
Kim_Button_Init_myButton();

Kim_Button_myButton.public_cool_down_time = 5000; // 每5s才能触发一次
```

+ 可以在代码中为每个按键设置独立的双击判定时间，示例如下：

```c
// 先初始化
Kim_Button_Init_myButton();

Kim_Button_myButton.public_double_push_max_time = 0; // 不等待双击判定（减少短按响应延迟，放弃双击功能）
```



#### 注意事项：

* ~~使用了SysTick，可能会与HAL_Delay冲突。【默认设置下不冲突】~~ （v0.0.5后完全不冲突）

* 每一个EXTI端口号只能有一个按钮，也就是说PA3与PB3不能同时作为按钮引脚。
  
  

#### 自定义选项（宏）：

* 在`kim_stm32f1xx_hal_button.h`文件的一开头，有一些可以修改的宏定义，也可以称之为自定义选项。可以根据项目需要更改这些宏定义的值。

```c
/* ============ Users can customize these by themselves(自定义选项开始) ============ */

/***** time config(配置各种时间) *****/
/* one tick(one interrupt = 1ms) (默认SysTick中断间隔为1ms) */
#define KIM_BUTTON_SYSTICK_ONE_TICK                 (SystemCoreClock / (1000UL / HAL_TICK_FREQ_DEFAULT))
/* calculate the tick with the time(计算宏，由于一次中断计数是1ms，此处tick == time) */
#define KIM_BUTTON_TIME_MS(_xx_ms)                  (1 * (_xx_ms))

// 按下按键后，延时（非阻塞）用于消抖的时间
#define KIM_BUTTON_PUSH_DELAY_TIME                  KIM_BUTTON_TIME_MS(40)          /* 40 ms */

// 松开按键后，判定双击的窗口时间。在此期间再次按下，判定为双击。
#define KIM_BUTTON_DOUBLE_PUSH_MAX_TIME             KIM_BUTTON_TIME_MS(300)         /* 300 ms */

// 长按判定的最小时间，超过这个时间就判定为长按
#define KIM_BUTTON_LONG_PUSH_MIN_TIME               KIM_BUTTON_TIME_MS(1000)        /* 1000 ms */

// 松开按键后，延时（非阻塞）用于消抖的时间
#define KIM_BUTTON_RELEASE_DELAY_TIME               KIM_BUTTON_TIME_MS(40)          /* 40 ms */

// 按键功能执行完毕后的冷却时间
#define KIM_BUTTON_COOL_DOWN_TIME                   KIM_BUTTON_TIME_MS(0)           /* 0 ms */

/* If this macro is 1, then the TIME above cannot be configured separately for each button */
// 如果这个宏是1，那么上面的TIME不能为每个按钮单独配置（但更节省RAM）
#define KIM_BUTTON_ONLY_USE_DEFAULT_TIME            0

/***** NVIC Priority config(NVIC 中断优先级配置) *****/
#define KIM_BUTTON_NVIC_SYSTICK_PreemptionPriority  0 // SysTick 抢占优先级
#define KIM_BUTTON_NVIC_SYSTICK_SubPriority         0 // SysTick 响应优先级

#define KIM_BUTTON_NVIC_EXTI_PreemptionPriority     0 // EXTI 抢占优先级
#define KIM_BUTTON_NVIC_EXTI_SubPriority            0 // EXTI 响应优先级

/***** If you use STM32CubeMX to generate code, define follow macro as @c 1 ,   *****
 ***** otherwise define follow macro as @c 0 .                                  *****/
/* 如果你使用STM32CubeMX生成了相关代码，请将下方对应的宏定义值改为1，可以减少重复代码 */
/* 如果不确定，可以先保持宏定义为0，测试通过后，再改为1，查看效果是否改变 */
#define KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK     0 // 如果 CubeMX生成了SysTick相关代码，宏改为1
#define KIM_BUTTON_STM32CUBEMX_GENERATE_EXTI        0 // 如果 CubeMX生成了EXTI相关代码，宏改为1
#define KIM_BUTTON_STM32CUBEMX_GENERATE_NVIC        0 // 如果 CubeMX生成了NVIC相关代码，宏改为1

/***** Name Prefix(自定义前缀名) *****/
/** If you change this macro, you need to use `new_prefix + Init_ + button_name()`      **
 ** to initialize the button, and use `new_prefix + button_name` struct to use method.  **
 **                                                                                     **
 ** @example #define KIM_BUTTON_NAME_PREFIX         KEY_                                **
 ** KIM_BUTTON__REGISTER(..., ..., ..., THE_NAME)                                       **
 ** Then I need to use `KEY_Init_THE_NAME()` to initialize the button, and use          **
 ** `KEY_THE_NAME.method_asynchronous_handler(..., ..., ...)` and                       **
 ** `KEY_THE_NAME.method_interrupt_handler()`                                           **/
// 这个宏定义是用来自定义前缀的，默认为Kim_Button_。如果修改为KEY_，那么在main.c使用的时候
// 就要使用 `KEY_Init_##__name()` 函数初始化，而非使用默认的 `Kim_Button_Init_##__name()`
// 相应的，也应该使用 KEY_##__name.method_asynchronous_handler(..., ..., ...) 以及
// KEY_##__name.method_interrupt_handler()
#define KIM_BUTTON_NAME_PREFIX                      Kim_Button_

/***** Critical Zone(临界区保护，多线程时必须使用) *****/
/* define follow macro when multi-thread */
// 以下两个宏定义在多线程场景下需要取消do...while内部注释
#define KIM_BUTTON_CRITICAL_ZONE_BEGIN()            do {/* __disable_irq(); */} while(0U)
#define KIM_BUTTON_CRITICAL_ZONE_END()              do {/* __enable_irq(); */} while(0U)

/* define follow macro any time */
// 以下两个宏定义即使在单线程下也需要定义
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_BEGIN()     do { __disable_irq(); } while(0U)
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END()       do { __enable_irq(); } while(0U)

/***** Macro for debug mode *****/
// 将宏的值设置为1可以启动调试模式
#define KIM_BUTTON_USE_DEBUG_MODE                   0   /* 1 --> use debug mode */

// DEBUG模式下，发生异常会调用的内容，需用户自行填写
#define KIM_BUTTON_DEBUG_ERROR_HOOK()                     

/* ====================== Customization END(自定义选项结束) ======================== */
```



### [English]:

#### Brief introduction:

* This project contains only one file, namely `kim_stm32f1xx_hal_button.h` . All the required code can be generated simply by using one macro definition.
  
  

#### How to use：

* First, suppose we have three files (`main.c `, `my_button.c`, `my_button.h`). Among them, the `my_button.c` file stores the key codes, the `my_button.h` file stores the necessary declarations, and the `main.c` call code.

* Then, in `my_button.c`, first import the header file `kim_stm32f1xx_hal_button.h`, and use the **KIM_BUTTON__REGISTER** macro to generate the required code. (Example: When my button is triggered, it will produce a falling edge signal at **PA7**. I want to name the button **myButton**. The code is as follows: )
  
  ```c
  /* The following is the content of my_button.c */ 
  #include "kim_stm32f1xx_hal_button.h" // Include header files
  
  // The sequence is port base address, pin number, trigger edge selection, and key name(up to you)
  KIM_BUTTON__REGISTER(GPIOA_BASE, GPIO_PIN_7, EXTI_TRIGGER_FALLING, myButton) // Note: No need to add ;
  ```
  
  

* Next, in `my_button.h`, first import the header file `kim_stm32f1xx_hal_button.h`, and use the **KIM_BUTTON__DECLARE** macro to generate the necessary declaration information. (Note: The declared button name must be defined by the **KIM_BUTTON__REGISTER** macro)
  
  ```c
  /* The following is the content of my_button.h */ 
  #pragma once 
  #include "kim_stm32f1xx_hal_button.h" // Include header files
  
  // The button name must be consistent with that in my_button.c
  KIM_BUTTON__DECLARE(myButton) // Note: No need to add ;
  ```

* Finally, in `main.c`, import the header file `my_button.h`, prepare the callback function, and then call the three functions in three places. Examples and detailed explanations are as follows:
  
  

```c
/* The following is the content of main.c */ 
/* ... */
#include "my_button.h" 

// Callback function of short press. After shor pressing, it will be executed (the function name is arbitrary)
void short_push_callback(void) { ... } 

// Callback function of long press. After long pressing, it will be executed (the function name is arbitrary).
void long_push_callback(void) { ... } 

// Callback function of double press. After double pressing, it will be executed (the function name is arbitrary).
void double_push_callback(void) { ... } 

int main(void) 
{
  /* ... Other irrelevant code ... */

  // 【 First Place 】 : Call the initialization function before the while loop. The name of the initialization function is Kim_Button_Init_ plus the key(button) name you defined.
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
  if(GPIO_Pin == GPIO_PIN_7) // Suppose my button is linked to PA7
  {
      // Call the interrupt handling function of the key
      Kim_Button_myButton.method_interrupt_handler();
  }
   /* ... Other irrelevant code ... */
} 

// Writing Method Two: (You can try to find this callback function in the stm32_xxxx_it.c file. If it's not available, write it yourself.
void EXTI7_IRQHandler(void) // Suppose my button is linked to PA7
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



#### Dynamic set:

* You can set an independent long-press determination time for each key in the code. An example is as follows:

```c
// Initialize first
Kim_Button_Init_myButton();

// Extend the long-press determination time of the myButton key to 3000ms(this setting must be changed after the Init function).
Kim_Button_myButton.public_long_push_min_time = 3000;
```

+ An independent cooldown time can be set for each key in the code. An example is as follows:

```c
// Initialize first
Kim_Button_Init_myButton();

Kim_Button_myButton.public_cool_down_time = 5000; // It can only be triggered once every 5 seconds
```

+ You can set an independent double-click determination time for each key in the code. An example is as follows:

```c
// Initialize first
Kim_Button_Init_myButton();

Kim_Button_myButton.public_double_push_max_time = 0; // Do not wait for double-click determination (reduce the response delay of short presses and abandon the double-click function)
```

#### Note：

* ~~SysTick is used, which may conflict with HAL Delay(). [There is no conflict under the default Settings]~~ (After v0.0.5, there is no conflict)

* Each EXTI port number can only have one button, which means that PA3 and PB3 cannot be used as button pins simultaneously.
  
  

#### Customizable options (Macro):

* At the beginning of the `kim_stm32f1xx_hal_button.h` file, there are some macro definitions that can be modified, which can also be called custom options. The values defined by these macros can be changed according to the project requirements.

```c
/* ============ Users can customize these by themselves ============ */

/***** time config *****/
/* one tick(one interrupt = 1ms) */
#define KIM_BUTTON_SYSTICK_ONE_TICK                 (SystemCoreClock / (1000UL / HAL_TICK_FREQ_DEFAULT))
/* calculate the tick with the time */
#define KIM_BUTTON_TIME_MS(_xx_ms)                  (1 * (_xx_ms))

// The delay (non-blocking) used for debouncing after pressing the key
#define KIM_BUTTON_PUSH_DELAY_TIME                  KIM_BUTTON_TIME_MS(40)          /* 40 ms */

// After releasing the key, determine the window time for double-clicking. If you press again during this period, it will be judged as a double-click.
#define KIM_BUTTON_DOUBLE_PUSH_MAX_TIME             KIM_BUTTON_TIME_MS(300)         /* 300 ms */

// The minimum duration for long press determination. If it exceeds this time, it will be determined as a long press
#define KIM_BUTTON_LONG_PUSH_MIN_TIME               KIM_BUTTON_TIME_MS(1000)        /* 1000 ms */

// The delay (non-blocking) used for debouncing after releasing the key
#define KIM_BUTTON_RELEASE_DELAY_TIME               KIM_BUTTON_TIME_MS(40)          /* 40 ms */

// CD time for button
#define KIM_BUTTON_COOL_DOWN_TIME                   KIM_BUTTON_TIME_MS(0)           /* 0 ms */

/* If this macro is 1, then the TIME above cannot be configured separately for each button */
#define KIM_BUTTON_ONLY_USE_DEFAULT_TIME            0

/***** NVIC Priority config *****/
#define KIM_BUTTON_NVIC_SYSTICK_PreemptionPriority  0 // SysTick PreemptionPriority
#define KIM_BUTTON_NVIC_SYSTICK_SubPriority         0 // SysTick SubPriority

#define KIM_BUTTON_NVIC_EXTI_PreemptionPriority     0 // EXTI PreemptionPriority
#define KIM_BUTTON_NVIC_EXTI_SubPriority            0 // EXTI SubPriority

/***** If you use STM32CubeMX to generate code, define follow macro as @c 1 ,   *****
 ***** otherwise define follow macro as @c 0 .                                  *****/
#define KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK     0//If CubeMX generates Systick code, change the macro to 1
#define KIM_BUTTON_STM32CUBEMX_GENERATE_EXTI        0//If CubeMX generates EXTI code, change the macro to 1
#define KIM_BUTTON_STM32CUBEMX_GENERATE_NVIC        0//If CubeMX generates NVIC code, change the macro to 1

/***** Name Prefix *****/
/** If you change this macro, you need to use `new_prefix + Init_ + button_name()`      **
 ** to initialize the button, and use `new_prefix + button_name` struct to use method.  **
 **                                                                                     **
 ** @example #define KIM_BUTTON_NAME_PREFIX         KEY_                                **
 ** KIM_BUTTON__REGISTER(..., ..., ..., THE_NAME)                                       **
 ** Then I need to use `KEY_Init_THE_NAME()` to initialize the button, and use          **
 ** `KEY_THE_NAME.method_asynchronous_handler(..., ..., ...)` and                       **
 ** `KEY_THE_NAME.method_interrupt_handler()`                                           **/
#define KIM_BUTTON_NAME_PREFIX                      Kim_Button_

/***** Critical Zone *****/
/* define follow macro when multi-thread */
#define KIM_BUTTON_CRITICAL_ZONE_BEGIN()            do {/* __disable_irq(); */} while(0U)
#define KIM_BUTTON_CRITICAL_ZONE_END()              do {/* __enable_irq(); */} while(0U)

/* define follow macro any time */
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_BEGIN()     do { __disable_irq(); } while(0U)
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END()       do { __enable_irq(); } while(0U)

/***** Macro for debug mode *****/
#define KIM_BUTTON_USE_DEBUG_MODE                   0   /* 1 --> use debug mode */

// In DEBUG mode, the content that will be called in case of an exception needs to be filled in by the user themselves
#define KIM_BUTTON_DEBUG_ERROR_HOOK()                     

/* ====================== Customization END ======================== */
```
