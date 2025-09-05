# Developer Guide For STM32-SimpleButton

---

This file is a guide for developer to deeply customize STM32-SimpleButton. You can understand how this project works and the logic of it.

---

## Content

* [Do-What](#do-what)

* [How-To-Do](#how-to-do)

* [Structure](#structure)

* [A-New-Chip](#a-new-chip)

---

## Do What

* Whenever we need to add button to our project, we have to write a whole complex State Machine for debounce and several push event. 

* So, why don't we create a project, which can generate all State Machine code we need.
  As a result, I choose macro of C language to create a template.

* The template, I think, can generate 3 functions we need: 
  
  * **First**, the intialization function. 
  * **Secondly**, the main handler(who really call the callback function) function in while loop. 
  * **Thirdly**, the helper handler(who just record the time stamp) function in interrupt service routine.

* User can just use one macro to generate 3 functions, and then use them in different place.

## How To Do

* Well, let's begin. We use **KIM_BUTTON__REGISTER** to generate 3 function and 1 status struct.

```c

#define KIM_BUTTON__REGISTER(GPIOx_BASE, GPIO_PIN_X, EXTI_TRIGGER_X, __name)    \
                                                                                \
    struct Kim_Button_TypeDef Kim_Button_ ## __name;                            \
                                                                                \
    void Init_Function(void) { ... }                                            \
                                                                                \
    void Main_Handler_Function(...) { ... }                                     \
                                                                                \
    void Helper_Handler_Function(...) { ... }                                   \


```

* But I don't want to write all my code in the macro. Because '\' is needed every line and you cannot see the warning information because of the IDE. As a result, I choose to write only a "shell function" in the macro. The "true function" is outside the macro, and they are called by "shell function", while the "true functions" are declared as "static inline".

```c

static inline true_Init_Func(void) { ... }
static inline true_Main_Handler(...) { ... }
static inline true_Helper_Handler(void) { ... }

#define KIM_BUTTON__REGISTER(GPIOx_BASE, GPIO_PIN_X, EXTI_TRIGGER_X, __name)    \
                                                                                \
    struct Kim_Button_TypeDef Kim_Button_ ## __name;                            \
    void Init_Function(void) { true_Init_Fuc(); }                               \
    void Main_Handler_Function(...) { true_Main_Handler(...); }                 \
    void Helper_Handler_Function(void) { true_Helper_Handler(); }               \

```

* What's more, the main State Machine in main handler(named Kim_Button_Asynchronous_Handler_xxx) has a complex `switch...case...` module, so I choose to write an inline function for each `case:`.

## Structure

```
KIM_BUTTON__REGISTER
|
+-- Status Structure (named KIM_BUTTON_NAME_PREFIX + __name)
|
+-- Main Handler (named Kim_Button_Asynchronous_Handler_ + __name)
|       |
|       +-- Kim_Button_PrivateUse_AsynchronousHandler
|               |
|               +-- Kim_Button_PrivateUse_StateWFIHandler
|               |
|               +-- Kim_Button_PrivateUse_StatePushDelayHandler
|               |
|               +-- Kim_Button_PrivateUse_StateWFEHandler
|               |
|               +-- Kim_Button_PrivateUse_StateRepeatPushHandler
|               |
|               +-- Kim_Button_PrivateUse_StateWFRHandler
|               |
|               +-- Kim_Button_PrivateUse_StateSiglePushHandler
|               |
|               +-- Kim_Button_PrivateUse_StateReleaseDelayHandler
|               |
|               +-- Kim_Button_PrivateUse_StateCoolDownHandler
|               |
|               +-- Kim_Button_PrivateUse_StateCombinationPushHandler
|               |
|               +-- Kim_Button_PrivateUse_StateCombinationWFEHandler
|               |
|               +-- Kim_Button_PrivateUse_StateCombinationReleaseHandler
|               |
|               +-- Kim_Button_PrivateUse_StateDefaultHandler
|
+-- Helper Handler(named Kim_Button_Handler_During_IT_ + __name)
|       |
|       +-- Kim_Button_PrivateUse_ITHandler
|
+-- Initialization Function(named KIM_BUTTON_NAME_PREFIX + Init_ + __name)
        |
        +-- Kim_Button_PrivateUse_InitButton
                |
                +-- Kim_Button_PrivateUse_InitStructSelf
                |
                +-- Kim_Button_PrivateUse_InitSysTick
                |
                +-- Kim_Button_PrivateUse_InitEXTI
```

* The Main/Helper Handler is static, they are load into Status Struct as two method for OOP.
  
  
## A New Chip

* What if you don't use STM32 or HAL ? That's fine. You can deeply customize this project by following steps:

  * 1. ignore these macros (delete them because they are only used in STM32-HAL)

```c

/* one tick(one interrupt = 1ms) */
#define KIM_BUTTON_SYSTICK_ONE_TICK                 (SystemCoreClock / (1000UL / HAL_GetTickFreq()))

/***** NVIC Priority config *****/
#define KIM_BUTTON_NVIC_SYSTICK_PreemptionPriority  TICK_INT_PRIORITY
#define KIM_BUTTON_NVIC_SYSTICK_SubPriority         0   /* this macro is not in use */

#define KIM_BUTTON_NVIC_EXTI_PreemptionPriority     0
#define KIM_BUTTON_NVIC_EXTI_SubPriority            0

/***** If you use STM32CubeMX to generate code, define follow macro as @c 1 ,   *****
 ***** otherwise define follow macro as @c 0 .                                  *****/
#define KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK     0
#define KIM_BUTTON_STM32CUBEMX_GENERATE_EXTI        0
#define KIM_BUTTON_STM32CUBEMX_GENERATE_NVIC        0

```

  * 2. customize these macros according to your chip.

```c

/* define follow macro when multi-thread */
#define KIM_BUTTON_CRITICAL_ZONE_BEGIN()            do { __disable_irq(); } while(0U)
#define KIM_BUTTON_CRITICAL_ZONE_END()              do { __enable_irq(); } while(0U)

/* define follow macro any time */
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_BEGIN()     do { __disable_irq(); } while(0U)
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END()       do { __enable_irq(); } while(0U)

/***** Macro for get tick *****/
#define KIM_BUTTON_GET_TICK()                       HAL_GetTick() // need 32bit tick

/***** Macro for GPIO read pin *****/
#define KIM_BUTTON_READ_PIN(GPIOx_BASE, PIN)        HAL_GPIO_ReadPin((GPIO_TypeDef*)(GPIOx_BASE), PIN)

/* Macro for judge the EXTI Trigger */
#define KIM_BUTTON_JUDGE_TRIGGER(EXTI_TRIGGER_X)        \
    (( (EXTI_TRIGGER_X == EXTI_TRIGGER_RISING_FALLING)  \
    || (EXTI_TRIGGER_X == EXTI_TRIGGER_NONE) ) ? -1 : 1)

/* Macro for judge GPIOx_BASE and GPIO_PIN_X */
#if defined(GPIOF_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_F(GPIOx_BASE)    ((GPIOx_BASE) == GPIOF_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_F(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_F */

#if defined(GPIOG_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_G(GPIOx_BASE)    ((GPIOx_BASE) == GPIOG_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_G(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_G */

#if defined(GPIOH_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_H(GPIOx_BASE)    ((GPIOx_BASE) == GPIOH_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_H(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_H */

#if defined(GPIOI_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_I(GPIOx_BASE)    ((GPIOx_BASE) == GPIOI_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_I(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_I */

#if defined(GPIOJ_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_J(GPIOx_BASE)    ((GPIOx_BASE) == GPIOJ_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_J(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_J */

#if defined(GPIOK_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_K(GPIOx_BASE)    ((GPIOx_BASE) == GPIOK_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_K(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_K */

#define KIM_BUTTON_JUDGE_GPIO_PIN(GPIOx_BASE, PIN)      \
    ( ((                                                \
        (GPIOx_BASE) == GPIOA_BASE ||                   \
        (GPIOx_BASE) == GPIOB_BASE ||                   \
        (GPIOx_BASE) == GPIOC_BASE ||                   \
        (GPIOx_BASE) == GPIOD_BASE ||                   \
        (GPIOx_BASE) == GPIOE_BASE ||                   \
        KIM_BUTTON_JUDGE_GPIO_PIN_F(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_G(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_H(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_I(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_J(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_K(GPIOx_BASE)         \
    ) && (                                              \
        (PIN) == GPIO_PIN_0 || (PIN) == GPIO_PIN_1 ||   \
        (PIN) == GPIO_PIN_2 || (PIN) == GPIO_PIN_3 ||   \
        (PIN) == GPIO_PIN_4 || (PIN) == GPIO_PIN_5 ||   \
        (PIN) == GPIO_PIN_6 || (PIN) == GPIO_PIN_7 ||   \
        (PIN) == GPIO_PIN_8 || (PIN) == GPIO_PIN_9 ||   \
        (PIN) == GPIO_PIN_10 || (PIN) == GPIO_PIN_11 || \
        (PIN) == GPIO_PIN_12 || (PIN) == GPIO_PIN_13 || \
        (PIN) == GPIO_PIN_14 || (PIN) == GPIO_PIN_15    \
    )) ? 1 : -1 )

```

  * 3. customize these functions according to your chip.

```c

/**
 * @p               [private-use]
 * @brief           Initialize the SysTick.
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitSysTick(void)
{
  /* ... your code to initialize the SysTick or TIM in your chip */
}

/**
 * @p               [private-use]
 * @brief           Initialize the EXTI.
 * @param[in]       gpiox_base - can be GPIOA_BASE / GPIOB_BASE / GPIOC_BASE / ...
 * @param[in]       gpio_pin_x - can be GPIO_PIN_0 / GPIO_PIN_1 / ... / GPIO_PIN_15.
 * @param[in]       exti_trigger_x - can be EXTI_TRIGGER_RISING or EXTI_TRIGGER_FALLING.
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitEXTI(
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint32_t exti_trigger_x
)
{
  /* ... your code to initialize the GPIO EXTI AFIO/SYSCFG in your chip */
}

```

  * 4. delete origin header file, add your header file.

```c

/***** @headerfile Select one of the header files given below as needed *****/
# include "stm32f1xx_hal.h"
// # include "stm32f2xx_hal.h"
// # include "stm32f3xx_hal.h"
// # include "stm32f4xx_hal.h"
// # include "stm32h4xx_hal.h"
// # include "stm32h7xx_hal.h"

```
