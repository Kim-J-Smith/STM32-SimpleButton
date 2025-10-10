/**
 * @file            simple_button_config.h
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           The configuration file of the Simple_Button file.
 * 
 * @version         0.7.0 ( SIMPLEBUTTON_CONFIG_H__ == 0011L )
 * 
 * @date            2025-10-03
 * 
 * @attention       Copyright (c) 2025 Kim-J-Smith.
 *                  All rights reserved.
 * 
 * @copyright       This project complies with the MIT License.
 *                  Refer to the LICENCE file in root for more details.
 *                  <https://github.com/Kim-J-Smith/Simple-Button>
 */
#ifndef     SIMPLEBUTTON_CONFIG_H__
#define     SIMPLEBUTTON_CONFIG_H__     0011L
#include <stdint.h>

/** @p ================================================================ **/
/* ====================== CUSTOMIZATION BEGIN ========================== */


/*************************************************************************
 * @b Contents
 * 
 * 1. @e Head-File (Your head file depending on your chip)
 * 
 * 2. @e Simple-Button-Type (The Type of Simple-Button)
 * 
 * 3. @e Other-Functions (Functions for read-pin, debug, low-power, ...)
 * 
 * 4. @e Time-Set (To set the adjustable time, such as repeat-push window. 
 *                  All these times will be set as default times, 
 *                  which can still be changed for each button.)
 * 
 * 5. @e Mode-Set (The mode of button, such as combination / timer-long-push / ...)
 * 
 * 6. @e Namespace (The namespace of Simple-Button public functions and variables)
 * 
 * 7. @e Initialization-Function (To init the GPIO, EXTI, NVIC/PFIC, etc)
 * 
 * 8. @e CubeMX (If you use CubeMX to generate code, define these macro to decrease code)
 * 
 *************************************************************************
 */

/** @b ================================================================ **/
/** @b Head-File */
#include "main.h"
// # include "stm32f1xx_hal.h"
// # include "stm32f2xx_hal.h"
// # include "stm32f3xx_hal.h"
// # include "stm32f4xx_hal.h"
// # include "stm32h4xx_hal.h"
// # include "stm32h7xx_hal.h"

/** @b ================================================================ **/
/** @b Simple-Button-Type */

// The type of GPIOX_BASE. (X can be A / B / C ...)
typedef uint32_t            simpleButton_Type_GPIOBase_t;
// The type of GPIO_PIN_X. (X can be 0 / 1 / 2 / 3 / ...)
typedef uint16_t            simpleButton_Type_GPIOPin_t;
// The type of Value of Pin. (The value usually be 0 or 1)
typedef uint8_t             simpleButton_Type_GPIOPinVal_t;
// The type of EXTI_TRIGGER_X. (X can be FALLING / RAISING / ...)
typedef uint32_t            simpleButton_Type_EXTITrigger_t;

#define SIMPLEBTN_EXTI_TRIGGER_FALLING      EXTI_TRIGGER_FALLING

/** @b ================================================================ **/
/** @b Other-Functions */

#define SIMPLEBTN_FUNC_READ_PIN(GPIOX_Base, Pin) HAL_GPIO_ReadPin((GPIO_TypeDef*)(GPIOX_Base), Pin)

#define SIMPLEBTN_FUNC_GET_TICK()  HAL_GetTick()

/* only used in DEBUG mode */
#define SIMPLEBTN_FUNC_PANIC(Cause, ErrorNum, etc)  do { Error_Handler(); } while (1)

#define SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN()  __disable_irq()

#define SIMPLEBTN_FUNC_CRITICAL_SECTION_END()  __enable_irq()

#define SIMPLEBTN_FUNC_START_LOW_POWER()  __WFI()

/** @b ================================================================ **/
/** @b Time-Set */

    // The delay time after you push the button.
#define SIMPLEBTN_TIME_PUSH_DELAY                       30
    // The delay time after you release the button.
#define SIMPLEBTN_TIME_RELEASE_DELAY                    30
    // The minimum time to push the button for long-push.
#define SIMPLEBTN_TIME_LONG_PUSH_MIN                    1000
    // The window time for repeat-push.
#define SIMPLEBTN_TIME_REPEAT_WINDOW                    300
    // The cool-down time after you release the button.
#define SIMPLEBTN_TIME_COOL_DOWN                        0
    // The minimum time to push the button for long-push-hold.
#define SIMPLEBTN_TIME_HOLD_PUSH_MIN                    SIMPLEBTN_TIME_LONG_PUSH_MIN
    // The interval time to call callback function in hold-long-push mode.
#define SIMPLEBTN_TIME_HOLD_INTERVAL                    200
    // The timeout time for normal long button.
#define SIMPLEBTN_TIME__TIMEOUT_NORMAL                  (1000 * 120)
    // The timeout time for combination long button.
#define SIMPLEBTN_TIME__TIMEOUT_COMBINATION             (1000 * 240)

/** @b ================================================================ **/
/** @b Mode-Set */

    // Enable debug mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_DEBUG                     1
    // Enable combination mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_COMBINATION               0
    // Enable timer long-push mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH           0
    // Enable counter repeat-push mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH       0
    // Enable only-use-default-time mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_ONLY_DEFAULT_TIME         0
    // Enable multi-threads mode(enable this only when you do use multi-thread) if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_MULTI_THREADS             0
    // Enable long-push-hold mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD             0

/** @b ================================================================ **/
/** @b Namespace */

/**
 * @brief   The namespace of Simple-Button public functions and variables.
 * 
 * @note    This namespace can be changed to avoid name conflicts.
 *          This macro can be empty.
 *          Default: SimpleButton_
 */
#define SIMPLEBTN_NAMESPACE                             SimpleButton_

/** @b ================================================================ **/
/** @b Initialization-Function */

/* This macro just forward the parameter to another function */
#define SIMPLEBTN_FUNC_INIT_EXTI(GPIOX_Base, GPIO_Pin_X, EXTI_Trigger_X) \
    simpleButton_Private_InitEXTI(GPIOX_Base, GPIO_Pin_X, EXTI_Trigger_X)

/* ====================== CUSTOMIZATION END ============================ */
/** @p ================================================================ **/

/* ================ OTHER LOCAL-PLATFORM CUSTOMIZATION ================= */

/** @b ================================================================ **/
/** @b CubeMX */

// if you use CubeMX to generate GPIO and EXTI code, define this macro as 1.
#define SIMPLEBTN_CUBEMX_G_EXTI                         0
// if you use CubeMX to generate NVIC code, define this macro as 1.
#define SIMPLEBTN_CUBEMX_G_NVIC                         0

// EXTI's IRQ preemption priority (used only when SIMPLEBTN_CUBEMX_G_NVIC == 0)
#define SIMPLEBTN_CUBEMX_NVIC_EXTI_PreemptionPriority   0
// EXTI's IRQ sub priority (used only when SIMPLEBTN_CUBEMX_G_NVIC == 0)
#define SIMPLEBTN_CUBEMX_NVIC_EXTI_SubPriority          0

/** @b ================================================================ **/

// Macro for force-inline
#if defined(__GNUC__) || defined(__clang__)
 #define SIMPLEBTN_FORCE_INLINE static inline __attribute__((always_inline))
#elif defined(_MSC_VER) || defined(__CC_ARM)
 #define SIMPLEBTN_FORCE_INLINE static __forceinline
#else
 #define SIMPLEBTN_FORCE_INLINE static inline
#endif

SIMPLEBTN_FORCE_INLINE
void simpleButton_Private_InitEXTI(
    simpleButton_Type_GPIOBase_t    GPIOX_Base,
    simpleButton_Type_GPIOPin_t     GPIO_Pin_X,
    simpleButton_Type_EXTITrigger_t EXTI_Trigger_X
) {
    /* Initialize the AFIO Clock(F1xx) or SYSCFG Clock */
#if defined(__HAL_RCC_AFIO_CLK_ENABLE)
    __HAL_RCC_AFIO_CLK_ENABLE();
#elif defined(__HAL_RCC_SYSCFG_CLK_ENABLE)
    __HAL_RCC_SYSCFG_CLK_ENABLE();
#else
 #warning Cannot find macro for AFIO or SYSCFG !
#endif /* AFIO or SYSCFG */

    /* Initialize the GPIOx Clock */
#if SIMPLEBTN_CUBEMX_G_EXTI == 0
    switch (GPIOX_Base) {
    case GPIOA_BASE:
        __HAL_RCC_GPIOA_CLK_ENABLE();
        break;
    case GPIOB_BASE:
        __HAL_RCC_GPIOB_CLK_ENABLE();
        break;
    case GPIOC_BASE:
        __HAL_RCC_GPIOC_CLK_ENABLE();
        break;
    case GPIOD_BASE:
        __HAL_RCC_GPIOD_CLK_ENABLE();
        break;
    case GPIOE_BASE:
        __HAL_RCC_GPIOE_CLK_ENABLE();
        break;
 #if defined(GPIOF_BASE)
        __HAL_RCC_GPIOF_CLK_ENABLE();
        break;
 #endif /* GPIOF_BASE */
 #if defined(GPIOG_BASE)
        __HAL_RCC_GPIOG_CLK_ENABLE();
        break;
 #endif /* GPIOG_BASE */
 #if defined(GPIOH_BASE)
        __HAL_RCC_GPIOH_CLK_ENABLE();
        break;
 #endif /* GPIOH_BASE */
 #if defined(GPIOI_BASE)
        __HAL_RCC_GPIOI_CLK_ENABLE();
        break;
 #endif /* GPIOI_BASE */
 #if defined(GPIOJ_BASE)
        __HAL_RCC_GPIOJ_CLK_ENABLE();
        break;
 #endif /* GPIOJ_BASE */
 #if defined(GPIOK_BASE)
        __HAL_RCC_GPIOK_CLK_ENABLE();
        break;
 #endif /* GPIOK_BASE */
 #if defined(GPIOL_BASE)
        __HAL_RCC_GPIOL_CLK_ENABLE();
        break;
 #endif /* GPIOL_BASE */
 #if defined(GPIOM_BASE)
        __HAL_RCC_GPIOM_CLK_ENABLE();
        break;
 #endif /* GPIOM_BASE */
 #if defined(GPION_BASE)
        __HAL_RCC_GPION_CLK_ENABLE();
        break;
 #endif /* GPION_BASE */
    default:
        SIMPLEBTN_FUNC_PANIC("unexpected GPIO BASE", , );
    }

    /* Configure the GPIOx */
    GPIO_InitTypeDef gpio_config;
    gpio_config.Mode = (EXTI_Trigger_X == EXTI_TRIGGER_RISING)
        ? (GPIO_MODE_IT_RISING) : (GPIO_MODE_IT_FALLING);
    gpio_config.Pin = (uint32_t) GPIO_Pin_X;
    gpio_config.Pull = (EXTI_Trigger_X == EXTI_TRIGGER_RISING)
        ? (GPIO_PULLDOWN) : (GPIO_PULLUP);
    gpio_config.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init((GPIO_TypeDef*)GPIOX_Base, &gpio_config);

#else

    (void)GPIOX_Base;
    (void)EXTI_Trigger_X;

#endif /* SIMPLEBTN_CUBEMX_G_EXTI == 0 */


    /* Initialize the EXTI */
#if SIMPLEBTN_CUBEMX_G_NVIC == 0
    IRQn_Type the_exti_IRQ;
    switch (GPIO_Pin_X) {
    case GPIO_PIN_0:
        the_exti_IRQ = EXTI0_IRQn;
        break;
    case GPIO_PIN_1:
        the_exti_IRQ = EXTI1_IRQn;
        break;
    case GPIO_PIN_2:
        the_exti_IRQ = EXTI2_IRQn;
        break;
    case GPIO_PIN_3:
        the_exti_IRQ = EXTI3_IRQn;
        break;
    case GPIO_PIN_4:
        the_exti_IRQ = EXTI4_IRQn;
        break;
    case GPIO_PIN_5:
        the_exti_IRQ = EXTI9_5_IRQn;
        break;
    case GPIO_PIN_6:
        the_exti_IRQ = EXTI9_5_IRQn;
        break;
    case GPIO_PIN_7:
        the_exti_IRQ = EXTI9_5_IRQn;
        break;
    case GPIO_PIN_8:
        the_exti_IRQ = EXTI9_5_IRQn;
        break;
    case GPIO_PIN_9:
        the_exti_IRQ = EXTI9_5_IRQn;
        break;
    case GPIO_PIN_10:
        the_exti_IRQ = EXTI15_10_IRQn;
        break;
    case GPIO_PIN_11:
        the_exti_IRQ = EXTI15_10_IRQn;
        break;
    case GPIO_PIN_12:
        the_exti_IRQ = EXTI15_10_IRQn;
        break;
    case GPIO_PIN_13:
        the_exti_IRQ = EXTI15_10_IRQn;
        break;
    case GPIO_PIN_14:
        the_exti_IRQ = EXTI15_10_IRQn;
        break;
    case GPIO_PIN_15:
        the_exti_IRQ = EXTI15_10_IRQn;
        break;
    default:
        SIMPLEBTN_FUNC_PANIC("unexpected GPIO Pin", , );
    }
    HAL_NVIC_SetPriority(
        the_exti_IRQ, 
        SIMPLEBTN_CUBEMX_NVIC_EXTI_PreemptionPriority,
        SIMPLEBTN_CUBEMX_NVIC_EXTI_SubPriority
    );
    HAL_NVIC_EnableIRQ(the_exti_IRQ);

#else

    (void)GPIO_Pin_X;

#endif /* SIMPLEBTN_CUBEMX_G_NVIC == 0 */
}

/* ================================ END ================================ */

#endif /* SIMPLEBUTTON_CONFIG_H__ */
