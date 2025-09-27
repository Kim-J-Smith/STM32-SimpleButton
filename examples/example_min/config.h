/* ============ Users can customize these by themselves ============ */

/** @p ------------------------------------------------------------- */
/** @b HEADER-FILES */

/***** @headerfile Select one of the header files given below as needed *****/
# include "stm32f1xx_hal.h"
// # include "stm32f2xx_hal.h"
// # include "stm32f3xx_hal.h"
// # include "stm32f4xx_hal.h"
// # include "stm32h4xx_hal.h"
// # include "stm32h7xx_hal.h"

/** @p ------------------------------------------------------------- */
/** @b TIMES */

/***** time config *****/
/* one tick(one interrupt = 1ms) */
#define KIM_BUTTON_SYSTICK_ONE_TICK                 (SystemCoreClock / (1000UL / HAL_GetTickFreq()))
/* calculate the tick with the time */
#define KIM_BUTTON_TIME_MS(_xx_ms)                  (1 * (uint32_t)(_xx_ms))

#define KIM_BUTTON_PUSH_DELAY_TIME                  KIM_BUTTON_TIME_MS(40)          /* 40 ms */
#define KIM_BUTTON_REPEAT_PUSH_MAX_TIME             KIM_BUTTON_TIME_MS(300)         /* 300 ms */
#define KIM_BUTTON_LONG_PUSH_MIN_TIME               KIM_BUTTON_TIME_MS(1000)        /* 1000 ms */
#define KIM_BUTTON_RELEASE_DELAY_TIME               KIM_BUTTON_TIME_MS(40)          /* 40 ms */
#define KIM_BUTTON_COOL_DOWN_TIME                   KIM_BUTTON_TIME_MS(0)           /* 0 ms */
#define KIM_BUTTON_SAFE_PUSH_MAX_TIME               KIM_BUTTON_TIME_MS(60000)       /* 1 min for WFE */
#define KIM_BUTTON_SAFE_PUSH_CMB_MAX_TIME           KIM_BUTTON_TIME_MS(180000)      /* 3 min for Combination_WFE */

/** @p ------------------------------------------------------------- */
/** @b NVIC-PRIORITY */

/***** NVIC Priority config *****/
#define KIM_BUTTON_NVIC_SYSTICK_PreemptionPriority  TICK_INT_PRIORITY
#define KIM_BUTTON_NVIC_SYSTICK_SubPriority         0   /* this macro is not in use */

#define KIM_BUTTON_NVIC_EXTI_PreemptionPriority     0
#define KIM_BUTTON_NVIC_EXTI_SubPriority            0

/** @p ------------------------------------------------------------- */
/** @b ENABLE-DISABLE-OPTIONS */

/* If this macro is 1, then the TIME above cannot be configured separately for each button */
#define KIM_BUTTON_ONLY_USE_DEFAULT_TIME            1

/***** If you use STM32CubeMX to generate code, define follow macro as @c 1 ,   *****
 ***** otherwise define follow macro as @c 0 .                                  *****/
#define KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK     0
#define KIM_BUTTON_STM32CUBEMX_GENERATE_EXTI        0
#define KIM_BUTTON_STM32CUBEMX_GENERATE_NVIC        0

/***** Macro for use debug mode *****/
#define KIM_BUTTON_USE_DEBUG_MODE                   0   /* 1 --> use debug mode */

/***** Macro for noinline state machine(Kim_Button_PrivateUse_AsynchronousHandler) function *****/
#define KIM_BUTTON_NO_INLINE_STATE_MACHINE          0

/***** Macro to enable different long push time *****/
#define KIM_BUTTON_ENABLE_DIFFERENT_TIME_LONG_PUSH  0

/***** Macro to enable button combination *****/
#define KIM_BUTTON_ENABLE_BUTTON_COMBINATION        0

/***** Macro to enable button repeat(2 ~ 7) *****/
#define KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT        0

/** @p ------------------------------------------------------------- */
/** @b FUNCTIONS-HOOKS */

/***** Critical Zone *****/
/* define follow macro when multi-thread */
#define KIM_BUTTON_CRITICAL_ZONE_BEGIN()            do { __disable_irq(); } while(0U)
#define KIM_BUTTON_CRITICAL_ZONE_END()              do { __enable_irq(); } while(0U)

/* define follow macro any time */
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_BEGIN()     do { __disable_irq(); } while(0U)
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END()       do { __enable_irq(); } while(0U)

/***** Macro for debug hook *****/
/* ... can be your function ... */
#define KIM_BUTTON_DEBUG_ERROR_HOOK()               do { while(1) {} /* panic */ } while(0U)

/***** Macro for get tick *****/
#define KIM_BUTTON_GET_TICK()                       HAL_GetTick()

/***** Macro for GPIO read pin *****/
#define KIM_BUTTON_READ_PIN(GPIOx_BASE, PIN)        HAL_GPIO_ReadPin((GPIO_TypeDef*)(GPIOx_BASE), PIN)

/***** Macro to stat low power mode *****/
#define KIM_BUTTON_START_LOW_POWER()                do { __WFI(); } while(0U)

/** @p ------------------------------------------------------------- */
/** @b NAMESPACE-NAME-PREFIX */

/***** @namespace Name Prefix *****/
/** If you change this macro, you need to use `new_prefix + Init_ + button_name()`      **
 ** to initialize the button, and use `new_prefix + button_name` struct to use method.  **
 **                                                                                     **
 ** @example                                                                            **
 **     #define KIM_BUTTON_NAME_PREFIX         KEY_                                     **
 **     KIM_BUTTON__REGISTER(..., ..., ..., THE_NAME)                                   **
 **                                                                                     **
 **     Then I need to use `KEY_Init_THE_NAME()` to initialize the button, and use      **
 **     `KEY_THE_NAME.method_asynchronous_handler(..., ..., ...)` and                   **
 **     `KEY_THE_NAME.method_interrupt_handler()`                                       **/
#define KIM_BUTTON_NAME_PREFIX                      Kim_Button_

/* ====================== Customization END ======================== */

