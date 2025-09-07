/**
 * @file            kim_stm32_hal_button.h
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           Kim Library to offer a template for button [STM32 HAL]
 * 
 * @version         0.1.9 ( 0015L )
 *                  (match with stm32fxxx_hal.h or stm32hxxx_hal.h)
 * 
 * @date            2025-08-26
 * 
 * @attention       Copyright (c) 2025 Kim-J-Smith.
 *                  All rights reserved.
 * 
 * @copyright       This project complies with the MIT License.
 *                  Refer to the LICENCE in root for more details.
 */
# include <stdint.h>

#ifndef     KIM_STM32_HAL_BUTTON_H
#define     KIM_STM32_HAL_BUTTON_H      0015L

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
#define KIM_BUTTON_SAFE_PUSH_MAX_TIME               KIM_BUTTON_TIME_MS(60000)       /* 1 min */

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
#define KIM_BUTTON_ONLY_USE_DEFAULT_TIME            0

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
#define KIM_BUTTON_DEBUG_ERROR_HOOK()               do { while(1) {} } while(0U)

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



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus enum & struct */

/* The enum is for State Machine. */
enum Kim_Button_State {
    Kim_Button_State_Wait_For_Interrupt = 0,

    Kim_Button_State_Push_Delay,

    Kim_Button_State_Wait_For_End,

    Kim_Button_State_Wait_For_Repeat,

    Kim_Button_State_Single_Push,

    Kim_Button_State_Repeat_Push,

    Kim_Button_State_Release_Delay,

    Kim_Button_State_Cool_Down,

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

    Kim_Button_State_Combination_Push,

    Kim_Button_State_Combination_WaitForEnd,

    Kim_Button_State_Combination_Release,

#endif /* KIM_BUTTON_ENABLE_BUTTON_COMBINATION */
};

#ifndef ENUM_BITFIELD
    /* Used in struct bit field */
    #define ENUM_BITFIELD(type)     unsigned int 
#endif /* ENUM_BITFIELD */

typedef void (*Kim_Button_ShortPushCallBack_t)(void);

#if KIM_BUTTON_ENABLE_DIFFERENT_TIME_LONG_PUSH == 0
    typedef void (*Kim_Button_LongPushCallBack_t)(void);
#else
    typedef void (*Kim_Button_LongPushCallBack_t)(uint32_t long_push_tick);
#endif /* different long push time */

#if KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT == 0
    typedef void (*Kim_Button_RepeatPushCallBack_t)(void);
#else
    typedef void (*Kim_Button_RepeatPushCallBack_t)(uint8_t repeat_time);
#endif /* repeat more */

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0
    typedef void (*Kim_Button_CombinationCallBack_t)(void);
#endif /* button combination */

/* This struct is for status and behaviour of each button. */
struct Kim_Button_TypeDef {
    /** @p [private] This member variable will be changed only in interrupt service routine. */
    volatile uint32_t                               private_time_stamp_interrupt;

    /** @p [private] This member variable will be changed only in while loop. */
    uint32_t                                        private_time_stamp_loop;

    /** @b [public] Method for asynchronous handler. Use this method in while loop. */
    void (* method_asynchronous_handler) (
        Kim_Button_ShortPushCallBack_t short_push_callback,
        Kim_Button_LongPushCallBack_t long_push_callback,
        Kim_Button_RepeatPushCallBack_t repeat_push_callback
    );

    /** @b [public] Method for handler in interrupt (EXTI). Use this method in IT service routine. */
    void (* method_interrupt_handler) (void);

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

    /** @b [public] Record the combination [before] button pointer. */
    volatile struct Kim_Button_TypeDef*             public_comb_before_button;

    /** @b [public] Record the combination button callback function.
     * (This callback function will be called when button-[This] is released 
     * and button-[before] is in the pressed state.) */
    volatile Kim_Button_CombinationCallBack_t       public_comb_callback;

#endif /* button combination */

#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0

    /** @b [public] This member variable is used to record the long-push time. */
    uint16_t                                        public_long_push_min_time;

    /** @b [public] This member variable is used to record the cool-down time. */
    uint16_t                                        public_cool_down_time;

    /** @b [public] This member variable is used to record the double push max time. */
    uint16_t                                        public_repeat_push_max_time;

#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */

    /** @p [private] This member variable is used to record the state of each button. */
    volatile ENUM_BITFIELD (enum Kim_Button_State)  private_state : 4;

    /** @p [private] This member variable is used to record how many times you push the button(0 ~ 7). */
    uint8_t                                         private_push_time : 3;

    /** @p [private] This member variable is used to record the state of initialization. */
    uint8_t                                         private_is_init : 1;
};

#ifdef __cplusplus
}
#endif /* __cplusplus enum & struct */


/* ========= [private use] Some private-use macros [static] ======== */

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

/* Macro for using functin with security */
#define KIM_BUTTON_SAFE_CALLBACK(_callback)         \
    do { if((_callback) != ((void*)0)) { _callback(); } } while(0U)

/* Macro for C API */
#ifdef __cplusplus
    #define KIM_BUTTON_C_API    extern "C" 
#else 
    #define KIM_BUTTON_C_API
#endif /* __cplusplus */

/* Macro for Force Inline of private-use functions */
#if defined(__GNUC__)
    #define KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE    static inline __attribute__((always_inline)) 
#elif defined(__CC_ARM)
    #define KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE    static __forceinline 
#else
    #define KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE    static inline
#endif /* FORCE_INLINE */

/* Macro for debug */
#if KIM_BUTTON_USE_DEBUG_MODE != 0
    #define KIM_BUTTON_DEBUG
#endif

/* Macro for suggest inline of private-use functions */
#if !defined(KIM_BUTTON_DEBUG)
 #if KIM_BUTTON_NO_INLINE_STATE_MACHINE == 0
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static inline
 #else
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static
 #endif /* KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE */
#else
 #if defined(__GNUC__)
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static __attribute__((noinline)) \
                                                        __attribute__((optimize("O1"))) __attribute__((unused))
 #elif defined(__CC_ARM)
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static __attribute__((noinline))
 #else
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static 
 #endif /* KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE */
#endif /* KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE */

/* Macro to connect macro */
#define KIM_BUTTON_CONNECT(_a, _b)          KIM_BUTTON_CONNECT_1(_a, _b)
#define KIM_BUTTON_CONNECT_1(_a, _b)        KIM_BUTTON_CONNECT_2(_a, _b)
#define KIM_BUTTON_CONNECT_2(_a, _b)        _a ## _b

#define KIM_BUTTON_CONNECT3(_a, _b, _c)     KIM_BUTTON_CONNECT3_1(_a, _b, _c)
#define KIM_BUTTON_CONNECT3_1(_a, _b, _c)   KIM_BUTTON_CONNECT3_2(_a, _b, _c)
#define KIM_BUTTON_CONNECT3_2(_a, _b, _c)   _a ## _b ## _c

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) \
    || (defined(__cplusplus) && __cplusplus >= 201103L)

 /* count macro */
 #define KIM_BUTTON_COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, \
    _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N 

 /* count macro */
 #define KIM_BUTTON_COUNT_ARGS(...)         \
    KIM_BUTTON_COUNT_ARGS_IMPL(__VA_ARGS__, 16, 15, 14, 13, 12, \
    11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#endif /* greater than C99 or C++11 */

/* ================== Private-use Macros END ======================= */



/* ======= [private use] Some private-use functions [static] ======= */

/**
 * @p               [private-use]
 * @brief           Initialize the struct self.
 * @param[inout]    self - pointer to self struct.
 * @param[in]       method_asynchronous_handler - function pointer to the method named 
 *                  `Kim_Button_Asynchronous_Handler_ ## __name` [static].
 * @param[in]       method_interrupt_handler - function pointer to the method named 
 *                  `Kim_Button_Handler_During_IT_ ## __name` [static].
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitStructSelf(
    struct Kim_Button_TypeDef* const self,
    void (* method_asynchronous_handler) (
        Kim_Button_ShortPushCallBack_t short_push_callback,
        Kim_Button_LongPushCallBack_t long_push_callback,
        Kim_Button_RepeatPushCallBack_t repeat_push_callback
    ),
    void (* method_interrupt_handler) (void)
)
{
    /* Initialize only once */
    self->private_is_init = 1;

    /* Initialize the member variables */
    self->private_push_time = 0;
    self->private_time_stamp_loop = 0;

    /* volatile variables */
    KIM_BUTTON_CRITICAL_ZONE_BEGIN();
    self->private_state = Kim_Button_State_Wait_For_Interrupt;
    self->private_time_stamp_interrupt = 0;
    KIM_BUTTON_CRITICAL_ZONE_END();

    /* public variables */
#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0
    self->public_long_push_min_time = KIM_BUTTON_LONG_PUSH_MIN_TIME;
    self->public_cool_down_time = KIM_BUTTON_COOL_DOWN_TIME;
    self->public_repeat_push_max_time = KIM_BUTTON_REPEAT_PUSH_MAX_TIME;
#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */

    /* Initialize the public method */
    self->method_asynchronous_handler = method_asynchronous_handler;
    self->method_interrupt_handler = method_interrupt_handler;

    /* combination button */
#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

    self->public_comb_before_button = 0;
    self->public_comb_callback = 0;

#endif /* combination button */

}

/**
 * @p               [private-use]
 * @brief           Initialize the SysTick.
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitSysTick(void)
{

    /* SysTick configure */
#if (KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK == 0)
    uint32_t check_error = 0U;

    check_error = HAL_SYSTICK_Config(KIM_BUTTON_SYSTICK_ONE_TICK);
    HAL_NVIC_SetPriority(SysTick_IRQn,
        KIM_BUTTON_NVIC_SYSTICK_PreemptionPriority,
        0UL /* Make sure it can promptly preempt other low-priority interrupts */
    ); 
    /* SysTick is within the core, don't need to be enabled(NVIC_Enable). */
    uwTickPrio = KIM_BUTTON_NVIC_SYSTICK_PreemptionPriority;

    /* error handler */
 #if defined(KIM_BUTTON_DEBUG)
    if(check_error != 0) {
        KIM_BUTTON_DEBUG_ERROR_HOOK();
    }
 #else
    (void)check_error;
 #endif /* Debug Mode */
#endif /*KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK*/

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
#if (KIM_BUTTON_STM32CUBEMX_GENERATE_EXTI == 0)

    /* Initialize the AFIO Clock(F1xx) or SYSCFG Clock */
#if defined(__HAL_RCC_AFIO_CLK_ENABLE)
    __HAL_RCC_AFIO_CLK_ENABLE();
#elif defined(__HAL_RCC_SYSCFG_CLK_ENABLE)
    __HAL_RCC_SYSCFG_CLK_ENABLE();
#else
 #warning Cannot find macro for AFIO or SYSCFG !
#endif /* AFIO or SYSCFG */

    /* Initialize the GPIOx Clock */
    switch (gpiox_base) {
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
    default:
        /* ... error handler ... */
#if defined(KIM_BUTTON_DEBUG)
        KIM_BUTTON_DEBUG_ERROR_HOOK();
#else
        break;
#endif /* Debug Mode */
    }

    /* Configure the GPIOx */
    GPIO_InitTypeDef gpio_config;
    gpio_config.Mode = (exti_trigger_x == EXTI_TRIGGER_RISING)
        ? (GPIO_MODE_IT_RISING) : (GPIO_MODE_IT_FALLING);
    gpio_config.Pin = (uint32_t) gpio_pin_x;
    gpio_config.Pull = (exti_trigger_x == EXTI_TRIGGER_RISING)
        ? (GPIO_PULLDOWN) : (GPIO_PULLUP);
    gpio_config.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init((GPIO_TypeDef*)gpiox_base, &gpio_config);
#else
    (void)gpiox_base;
    (void)exti_trigger_x;
#endif /* KIM_BUTTON_STM32CUBEMX_GENERATE_EXTI */


#if (KIM_BUTTON_STM32CUBEMX_GENERATE_NVIC == 0)
    /* Initialize the EXTI */
    IRQn_Type the_exti_IRQ;
    switch (gpio_pin_x) {
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
        /* ... error handler ... */
        the_exti_IRQ = EXTI0_IRQn;
#if defined(KIM_BUTTON_DEBUG)
        KIM_BUTTON_DEBUG_ERROR_HOOK();
#else
        break;
#endif /* Debug Mode */
    }
    HAL_NVIC_SetPriority(
        the_exti_IRQ, 
        KIM_BUTTON_NVIC_EXTI_PreemptionPriority,
        KIM_BUTTON_NVIC_EXTI_SubPriority
    );
    HAL_NVIC_EnableIRQ(the_exti_IRQ);
#else
    (void)gpio_pin_x;
#endif /* KIM_BUTTON_STM32CUBEMX_GENERATE_NVIC */

}

/** 
 * @p               [private-use]
 * @brief           Init the Button, including the GPIO, EXTI and NVIC.
 * @param[inout]    self - pointer to self struct.
 * @param[in]       gpiox_base - can be GPIOA_BASE / GPIOB_BASE / GPIOC_BASE / ...
 * @param[in]       gpio_pin_x - can be GPIO_PIN_0 / GPIO_PIN_1 / ... / GPIO_PIN_15.
 * @param[in]       exti_trigger_x - can be EXTI_TRIGGER_RISING or EXTI_TRIGGER_FALLING.
 * @param[in]       method_asynchronous_handler - function pointer to the method named 
 *                  `Kim_Button_Asynchronous_Handler_ ## __name` [static].
 * @param[in]       method_interrupt_handler - function pointer to the method named 
 *                  `Kim_Button_Handler_During_IT_ ## __name` [static].
 * @return          None
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitButton(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint32_t exti_trigger_x,
    void (* method_asynchronous_handler) (
        Kim_Button_ShortPushCallBack_t short_push_callback,
        Kim_Button_LongPushCallBack_t long_push_callback,
        Kim_Button_RepeatPushCallBack_t repeat_push_callback
    ),
    void (* method_interrupt_handler) (void)
)
{
    /* Check whether it is first time to init */
    if(self->private_is_init != 0) {
#if defined(KIM_BUTTON_DEBUG)
        KIM_BUTTON_DEBUG_ERROR_HOOK();
#else
        return;
#endif /* Debug Mode */
    }

    /* Initialize the struct self */
    Kim_Button_PrivateUse_InitStructSelf(
        self,
        method_asynchronous_handler,
        method_interrupt_handler
    );

    /* Initialize the SysTick (include SysTick's NVIC) */
    Kim_Button_PrivateUse_InitSysTick();

    /* Initialize the EXTI (include EXTI's NVIC) */
    Kim_Button_PrivateUse_InitEXTI(gpiox_base, gpio_pin_x, exti_trigger_x);
}

/**
 * @p               [private-use]
 * @brief           Change the status of each button when during the EXTI interrupt
 *                  service routine.
 * @param[inout]    self - pointer to self struct.
 * @return          None
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_ITHandler(
    struct Kim_Button_TypeDef* const self
)
{
    if( ((enum Kim_Button_State)self->private_state) == Kim_Button_State_Wait_For_Repeat
        || ((enum Kim_Button_State)self->private_state) == Kim_Button_State_Wait_For_Interrupt) 
    {
        self->private_time_stamp_interrupt = KIM_BUTTON_GET_TICK();
        self->private_state = Kim_Button_State_Push_Delay;
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateWFIHandler(void)
{
    /* do nothing */
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StatePushDelayHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_interrupt 
        > KIM_BUTTON_PUSH_DELAY_TIME)
    {
        /* Check the GPIO Pin again */
        if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
        {
            if(self->private_push_time == 0) {
                self->private_state = Kim_Button_State_Wait_For_Interrupt;
            } else {
                self->private_state = Kim_Button_State_Wait_For_Repeat;
            }
        } else {
            self->private_state = Kim_Button_State_Wait_For_End;
        }
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateWFEHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
    {
        self->private_state = Kim_Button_State_Release_Delay;
    }
    else if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_interrupt 
        > KIM_BUTTON_SAFE_PUSH_MAX_TIME)
    {
#if defined(KIM_BUTTON_DEBUG)
        KIM_BUTTON_DEBUG_ERROR_HOOK();
#else
        self->private_push_time = 0;
        self->private_state = Kim_Button_State_Wait_For_Interrupt;
#endif /* Debug Mode */
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateRepeatPushHandler(
    struct Kim_Button_TypeDef* const self,
    Kim_Button_RepeatPushCallBack_t repeat_push_callback
)
{
    KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */

#if KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT == 0
    KIM_BUTTON_SAFE_CALLBACK(repeat_push_callback);
#else
    if(repeat_push_callback != 0) {
        repeat_push_callback(self->private_push_time);
    }
#endif /* more repeat */
    
    KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
    self->private_push_time = 0;
    self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();
    self->private_state = Kim_Button_State_Cool_Down;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateWFRHandler(
    struct Kim_Button_TypeDef* const self
)
{
    KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */

    KIM_BUTTON_ALWAYS_CRITICAL_ZONE_BEGIN(); /* ALWAYS critical zone begin */
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_loop
#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0
        > (uint32_t)self->public_repeat_push_max_time
#else
        > KIM_BUTTON_REPEAT_PUSH_MAX_TIME
#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */
    )
    {
#if KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT == 0
        self->private_state = Kim_Button_State_Single_Push;
#else
        if(self->private_push_time == 1) {
            self->private_state = Kim_Button_State_Single_Push;
        } else {
            self->private_state = Kim_Button_State_Repeat_Push;
        }
#endif /* more repeat */
    }
    KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END(); /* ALWAYS critical zone end */

    KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateSiglePushHandler(
    struct Kim_Button_TypeDef* const self,
    Kim_Button_ShortPushCallBack_t short_push_callback,
    Kim_Button_LongPushCallBack_t long_push_callback
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_interrupt
#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0
        > (uint32_t)self->public_long_push_min_time
#else
        > KIM_BUTTON_LONG_PUSH_MIN_TIME
#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */
    )
    { 
#if KIM_BUTTON_ENABLE_DIFFERENT_TIME_LONG_PUSH == 0
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        KIM_BUTTON_SAFE_CALLBACK(long_push_callback);
#else
        uint32_t long_push_tick = KIM_BUTTON_GET_TICK() - self->private_time_stamp_interrupt;
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        if(long_push_callback != ((void*)0)) {
            long_push_callback(long_push_tick); 
        }
#endif /* different long push time */
    } else {
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        KIM_BUTTON_SAFE_CALLBACK(short_push_callback);
    }
    
    KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
    self->private_push_time = 0;
    self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();
    self->private_state = Kim_Button_State_Cool_Down;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateReleaseDelayHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_loop 
        > KIM_BUTTON_RELEASE_DELAY_TIME)
    {
        /* Delay time is over */
        if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
        {
            self->private_push_time ++;
            self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();

#if KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT == 0 /* more repeat */
            self->private_state = (self->private_push_time == 1)
                ? Kim_Button_State_Wait_For_Repeat : Kim_Button_State_Repeat_Push;
#else
            self->private_state = (self->private_push_time < 7)
                ? Kim_Button_State_Wait_For_Repeat : Kim_Button_State_Repeat_Push;
#endif /* more repeat */

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0 /* combination button */

            if(self->public_comb_before_button == 0 || self->public_comb_callback == 0)
            {
                return;
            }
            /* check button-[before] */
            if(self->public_comb_before_button->private_state != Kim_Button_State_Combination_WaitForEnd)
            {
                if(self->public_comb_before_button->private_state != Kim_Button_State_Wait_For_End) {
                    return;
                } else {
                    self->public_comb_before_button->private_state = Kim_Button_State_Combination_WaitForEnd;
                }
            }
            self->private_state = Kim_Button_State_Combination_Push;
#endif /* combination button */

        } else {
            self->private_state = Kim_Button_State_Wait_For_End;
        }
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateCoolDownHandler(
    struct Kim_Button_TypeDef* const self
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_loop 
#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0
        > (uint32_t)self->public_cool_down_time
#else
        > KIM_BUTTON_COOL_DOWN_TIME
#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */
    )
    {
        self->private_state = Kim_Button_State_Wait_For_Interrupt;
    }
}



#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateCombinationPushHandler(
    struct Kim_Button_TypeDef* const self
)
{
    Kim_Button_CombinationCallBack_t tmp_comb_callback;
    tmp_comb_callback = (Kim_Button_CombinationCallBack_t)self->public_comb_callback;
    KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */

    KIM_BUTTON_SAFE_CALLBACK(tmp_comb_callback);

    KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
    self->private_push_time = 0;
    self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();
    self->private_state = Kim_Button_State_Cool_Down;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateCombinationWFEHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
    {
        self->private_state = Kim_Button_State_Combination_Release;
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateCombinationReleaseHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_loop 
        > KIM_BUTTON_RELEASE_DELAY_TIME)
    {
        /* check again */
        if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
        {
            self->private_push_time = 0;
            self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();
            self->private_state = Kim_Button_State_Cool_Down;
        } else {
            self->private_state = Kim_Button_State_Combination_WaitForEnd;
        }         
    }
}

#endif /* KIM_BUTTON_ENABLE_BUTTON_COMBINATION */



KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateDefaultHandler(void)
{

#if defined(KIM_BUTTON_DEBUG)
    KIM_BUTTON_DEBUG_ERROR_HOOK();
#endif /* Debug Mode */

}

/**
 * @p               [private-use]
 * @brief           Asynchronously call the callback func by the status of each button.
 * @param[inout]    self - pointer to self struct.
 * @param[in]       gpiox_base - can be GPIOA_BASE / GPIOB_BASE / GPIOC_BASE / ...
 * @param[in]       gpio_pin_x - can be GPIO_PIN_0 / GPIO_PIN_1 / ... / GPIO_PIN_15.
 * @param[in]       exti_trigger_x - can be EXTI_TRIGGER_RISING or EXTI_TRIGGER_FALLING.
 * @param[in]       short_push_callback - callback function for short push.
 * @param[in]       long_push_callback - callback function for long push.
 * @param[in]       repeat_push_callback - callback function for double push.
 * @return          None
 */
KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE void Kim_Button_PrivateUse_AsynchronousHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val,
    Kim_Button_ShortPushCallBack_t short_push_callback,
    Kim_Button_LongPushCallBack_t long_push_callback,
    Kim_Button_RepeatPushCallBack_t repeat_push_callback
)
{
    /* Critical Zone Begin */
    KIM_BUTTON_CRITICAL_ZONE_BEGIN();

    switch ((enum Kim_Button_State)self->private_state) 
    {

    case Kim_Button_State_Wait_For_Interrupt:
        Kim_Button_PrivateUse_StateWFIHandler();
        break;

    case Kim_Button_State_Push_Delay:
        Kim_Button_PrivateUse_StatePushDelayHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

    case Kim_Button_State_Wait_For_End:
        Kim_Button_PrivateUse_StateWFEHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

    case Kim_Button_State_Repeat_Push:
        Kim_Button_PrivateUse_StateRepeatPushHandler(self, repeat_push_callback);
        break;

    case Kim_Button_State_Wait_For_Repeat:
        Kim_Button_PrivateUse_StateWFRHandler(self);
        break;

    case Kim_Button_State_Single_Push:
        Kim_Button_PrivateUse_StateSiglePushHandler(self, short_push_callback, long_push_callback);
        break;

    case Kim_Button_State_Release_Delay:
        Kim_Button_PrivateUse_StateReleaseDelayHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

    case Kim_Button_State_Cool_Down:
        Kim_Button_PrivateUse_StateCoolDownHandler(self);
        break;

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

    case Kim_Button_State_Combination_Push:
        Kim_Button_PrivateUse_StateCombinationPushHandler(self);
        break;

    case Kim_Button_State_Combination_WaitForEnd:
        Kim_Button_PrivateUse_StateCombinationWFEHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

    case Kim_Button_State_Combination_Release:
        Kim_Button_PrivateUse_StateCombinationReleaseHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

#endif /* button combination */

    default:
        Kim_Button_PrivateUse_StateDefaultHandler();
        break;

    }

    /* Critical Zone End */
    KIM_BUTTON_CRITICAL_ZONE_END(); 
}

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) \
    || (defined(__cplusplus) && __cplusplus >= 201103L)

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_1(
    struct Kim_Button_TypeDef _button_1
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_2(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_3(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_4(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_5(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_6(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_7(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_8(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_9(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8,
    struct Kim_Button_TypeDef _button_9
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_9.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_10(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8,
    struct Kim_Button_TypeDef _button_9,
    struct Kim_Button_TypeDef _button_10
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_9.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_10.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_11(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8,
    struct Kim_Button_TypeDef _button_9,
    struct Kim_Button_TypeDef _button_10,
    struct Kim_Button_TypeDef _button_11
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_9.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_10.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_11.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_12(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8,
    struct Kim_Button_TypeDef _button_9,
    struct Kim_Button_TypeDef _button_10,
    struct Kim_Button_TypeDef _button_11,
    struct Kim_Button_TypeDef _button_12
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_9.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_10.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_11.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_12.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_13(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8,
    struct Kim_Button_TypeDef _button_9,
    struct Kim_Button_TypeDef _button_10,
    struct Kim_Button_TypeDef _button_11,
    struct Kim_Button_TypeDef _button_12,
    struct Kim_Button_TypeDef _button_13
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_9.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_10.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_11.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_12.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_13.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_14(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8,
    struct Kim_Button_TypeDef _button_9,
    struct Kim_Button_TypeDef _button_10,
    struct Kim_Button_TypeDef _button_11,
    struct Kim_Button_TypeDef _button_12,
    struct Kim_Button_TypeDef _button_13,
    struct Kim_Button_TypeDef _button_14
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_9.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_10.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_11.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_12.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_13.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_14.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_15(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8,
    struct Kim_Button_TypeDef _button_9,
    struct Kim_Button_TypeDef _button_10,
    struct Kim_Button_TypeDef _button_11,
    struct Kim_Button_TypeDef _button_12,
    struct Kim_Button_TypeDef _button_13,
    struct Kim_Button_TypeDef _button_14,
    struct Kim_Button_TypeDef _button_15
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_9.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_10.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_11.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_12.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_13.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_14.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_15.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE 
uint32_t Kim_Button_PrivateUse_AllIsWFI_16(
    struct Kim_Button_TypeDef _button_1,
    struct Kim_Button_TypeDef _button_2,
    struct Kim_Button_TypeDef _button_3,
    struct Kim_Button_TypeDef _button_4,
    struct Kim_Button_TypeDef _button_5,
    struct Kim_Button_TypeDef _button_6,
    struct Kim_Button_TypeDef _button_7,
    struct Kim_Button_TypeDef _button_8,
    struct Kim_Button_TypeDef _button_9,
    struct Kim_Button_TypeDef _button_10,
    struct Kim_Button_TypeDef _button_11,
    struct Kim_Button_TypeDef _button_12,
    struct Kim_Button_TypeDef _button_13,
    struct Kim_Button_TypeDef _button_14,
    struct Kim_Button_TypeDef _button_15,
    struct Kim_Button_TypeDef _button_16
)
{
    uint32_t ret_val = 1;
    ret_val &= (_button_1.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_2.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_3.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_4.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_5.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_6.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_7.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_8.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_9.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_10.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_11.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_12.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_13.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_14.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_15.private_state == Kim_Button_State_Wait_For_Interrupt);
    ret_val &= (_button_16.private_state == Kim_Button_State_Wait_For_Interrupt);
    return ret_val;
}

#endif /* greater than C99 or C++11 */

/* ================ Private-use functions END ====================== */



/* ========= [public use] Some private-use macros [static] ========= */

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) \
    || (defined(__cplusplus) && __cplusplus >= 201103L)

 #define KIM_BUTTON_ALL_IS_WFI(...) \
    KIM_BUTTON_CONNECT3(Kim_Button_PrivateUse_AllIsWFI, _, KIM_BUTTON_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

 /**
  * @brief      User can use this macro to begin low-power mode.
  *             the parameter must be all button status struct.
  * @param[in]  _ All button-status struct.
  * @example    KIM_BUTTON__LOW_POWER(Kim_Button_myButton1, Kim_Button_myButton2)
  */
 #define KIM_BUTTON__LOW_POWER(...)                 \
    do {                                            \
        KIM_BUTTON_ALWAYS_CRITICAL_ZONE_BEGIN();    \
        if(KIM_BUTTON_ALL_IS_WFI(__VA_ARGS__))      \
        {                                           \
            KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END();  \
            KIM_BUTTON_START_LOW_POWER();           \
        } else {                                    \
            KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END();  \
        }                                           \
    } while(0U)


#endif /* greater than C99 or C++11 */

/* =================== Public-use Macros END ======================= */


/**
 * @def         KIM_BUTTON__REGISTER
 * @brief       This is a template for generating code of button.
 *              It will generate 3 functions, including:
 *      @arg `Kim_Button_Init_ ## __name` [extern "C" public-use].
 *      @arg `Kim_Button_Asynchronous_Handler_ ## __name` [static private-use].
 *      @arg `Kim_Button_Handler_During_IT_ ## __name` [static private-use].
 * @param       GPIOx_BASE - can be GPIOA_BASE / GPIOB_BASE / GPIOC_BASE / ...
 * @param       GPIO_PIN_X - can be GPIO_PIN_0 / GPIO_PIN_1 / ... / GPIO_PIN_15.
 * @param       EXTI_TRIGGER_X - can be EXTI_TRIGGER_RISING or EXTI_TRIGGER_FALLING.
 * @param       __name - the name of your button. can be any string.
 * @attention   @p Error(array[-1]) will occur:
 *              1. If EXTI_TRIGGER_X is set as EXTI_TRIGGER_RISING_FALLING or EXTI_TRIGGER_NONE.
 *              2. If GPIOx_BASE is invalid.
 *              3. If GPIO_PIN_X is invalid.
 */
#define KIM_BUTTON__REGISTER(GPIOx_BASE, GPIO_PIN_X, EXTI_TRIGGER_X, __name)    \
    struct Kim_Button_TypeDef                                                   \
        KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX,__name) = {0};                \
                                                                                \
    static void                                                                 \
    KIM_BUTTON_CONNECT(Kim_Button_Asynchronous_Handler_, __name)(               \
        Kim_Button_ShortPushCallBack_t short_push_callback,                     \
        Kim_Button_LongPushCallBack_t long_push_callback,                       \
        Kim_Button_RepeatPushCallBack_t repeat_push_callback                    \
    )                                                                           \
    {                                                                           \
        static const uint8_t Normal_Bit_Val =                                   \
            (EXTI_TRIGGER_X == EXTI_TRIGGER_RISING) ? 0 : 1;                    \
                                                                                \
        Kim_Button_PrivateUse_AsynchronousHandler(                              \
            &(KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name)),              \
            GPIOx_BASE,                                                         \
            GPIO_PIN_X,                                                         \
            Normal_Bit_Val,                                                     \
            short_push_callback,                                                \
            long_push_callback,                                                 \
            repeat_push_callback                                                \
        );                                                                      \
    }                                                                           \
                                                                                \
    static void                                                                 \
    KIM_BUTTON_CONNECT(Kim_Button_Handler_During_IT_, __name)(void)             \
    {                                                                           \
        Kim_Button_PrivateUse_ITHandler(                                        \
            &(KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name))               \
        );                                                                      \
    }                                                                           \
                                                                                \
    KIM_BUTTON_C_API void                                                       \
    KIM_BUTTON_CONNECT3(KIM_BUTTON_NAME_PREFIX, Init_, __name) (void)           \
    {                                                                           \
        /* Assert the EXTI Trigger (use array[-1] to raise error) */            \
        static const char                                                       \
            assert_exti_trg[KIM_BUTTON_JUDGE_TRIGGER(EXTI_TRIGGER_X)] = {0};    \
        (void)assert_exti_trg;                                                  \
                                                                                \
        /* Assert the GPIO_BASE and PIN (use array[-1] to raise error) */       \
        static const char                                                       \
            assert_gpio_pin[                                                    \
                KIM_BUTTON_JUDGE_GPIO_PIN(GPIOx_BASE, GPIO_PIN_X)               \
            ] = {0};                                                            \
        (void)assert_gpio_pin;                                                  \
                                                                                \
        Kim_Button_PrivateUse_InitButton(                                       \
            &(KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name)),              \
            GPIOx_BASE,                                                         \
            GPIO_PIN_X,                                                         \
            EXTI_TRIGGER_X,                                                     \
            &(Kim_Button_Asynchronous_Handler_ ## __name),                      \
            &(Kim_Button_Handler_During_IT_ ## __name)                          \
        );                                                                      \
    }
    
/**
 * @def         KIM_BUTTON__DECLARE
 * @brief       You can use `KIM_BUTTON__REGISTER` to generate code in one file.
 *              And then, you need to use `KIM_BUTTON__DECLARE` to declare them in
 *              another file before you want to use them.
 * @param       __name - the name of your button. remain the same as it in `KIM_BUTTON__REGISTER`.
 */
#define KIM_BUTTON__DECLARE(__name)                                                         \
    extern struct Kim_Button_TypeDef KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name);    \
    KIM_BUTTON_C_API void KIM_BUTTON_CONNECT3(KIM_BUTTON_NAME_PREFIX, Init_, __name)(void);


#endif /* KIM_STM32_HAL_BUTTON_H */
