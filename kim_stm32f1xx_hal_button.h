/**
 * @file            kim_stm32f1xx_hal_button.h
 * @author          Kim-J-Smith
 * @brief           Kim Library to offer a template for button [STM32 HAL]
 * @version         0.0.4 ( 0004L )
 *          (match with stm32f1xx_hal.h version 1.0.0)
 * @date            2025-08-26
 * @copyright       Copyright (c) 2025 Kim-J-Smith under MIT License.
 *          Refer to the LICENCE in root for more details.
 */
# include <stdint.h>
# include "stm32f1xx_hal.h"

#ifndef     KIM_STM32F1XX_HAL_BUTTON_H
#define     KIM_STM32F1XX_HAL_BUTTON_H  0004L

/* ================================================================================ */
/**    @b ATTENTION Kim_Button use SysTick, which conflicts with with Delay_xxx    **/
/* ================================================================================ */


/* ============ Users can customize these by themselves ============ */

/***** time config *****/
/* one tick(one interrupt = 1ms) */
#define KIM_BUTTON_SYSTICK_ONE_TICK                 (SystemCoreClock / (1000UL / HAL_TICK_FREQ_DEFAULT))
/* calculate the tick with the time */
#define KIM_BUTTON_TIME_MS(_xx_ms)                  (1 * (_xx_ms))

#define KIM_BUTTON_PUSH_DELAY_TIME                  KIM_BUTTON_TIME_MS(40)          /* 40 ms */
#define KIM_BUTTON_DOUBLE_PUSH_MAX_TIME             KIM_BUTTON_TIME_MS(300)         /* 300 ms */
#define KIM_BUTTON_LONG_PUSH_MIN_TIME               KIM_BUTTON_TIME_MS(1000)        /* 1000 ms */
#define KIM_BUTTON_RELEASE_DELAY_TIME               KIM_BUTTON_TIME_MS(40)          /* 40 ms */

/***** NVIC Priority config *****/
#define KIM_BUTTON_NVIC_SYSTICK_PreemptionPriority  0
#define KIM_BUTTON_NVIC_SYSTICK_SubPriority         0

#define KIM_BUTTON_NVIC_EXTI_PreemptionPriority     0
#define KIM_BUTTON_NVIC_EXTI_SubPriority            0

/***** If you use STM32CubeMX to generate code, define follow macro as @c 1 ,   *****
 ***** otherwise define follow macro as @c 0 .                                  *****/
#define KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK     0
#define KIM_BUTTON_STM32CUBEMX_GENERATE_EXTI        0
#define KIM_BUTTON_STM32CUBEMX_GENERATE_NVIC        0

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
#define KIM_BUTTON_CRITICAL_ZONE_BEGIN()            /* __disable_irq() */
#define KIM_BUTTON_CRITICAL_ZONE_END()              /* __enable_irq() */

/* ====================== Customization END ======================== */



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus enum & struct */

enum Kim_Button_State {
    Kim_Button_State_Wait_For_Interrupt,

    Kim_Button_State_Push_Delay,

    Kim_Button_State_Wait_For_End,

    Kim_Button_State_Wait_For_Double,

    Kim_Button_State_Single_Push,

    Kim_Button_State_Double_Push,

    Kim_Button_State_Release_Delay,
};

#ifndef ENUM_BITFIELD
    /* Used in struct bit field */
    #define ENUM_BITFIELD(type)     unsigned int 
#endif /* ENUM_BITFIELD */

struct Kim_Button_Status {
    /** @p [private] This member variable will be changed only in interrupt service routine. */
    volatile uint32_t                               private_time_stamp_interrupt;

    /** @p [private] This member variable will be changed only in while loop. */
    uint32_t                                        private_time_stamp_loop;

    /** @b [public] Method for asynchronous handler. Use this method in while loop. */
    void (* method_asynchronous_handler) (
        void (*short_push_callback)(void),
        void (*long_push_callback)(void),
        void (*double_push_callback)(void)
    );

    /** @b [public] Method for handler in interrupt (EXTI). Use this method in IT service routine. */
    void (* method_interrupt_handler) (void);

    /** @b [public] This member variable is used to record the long-push time. */
    uint16_t                                        public_long_push_min_time;

    /** @p [private] This member variable is used to record the state of each button. */
    volatile ENUM_BITFIELD (enum Kim_Button_State)  private_state : 8;

    /** @p [private] This member variable is used to record how many times you push the button(0/1/2). */
    uint8_t                                         private_push_time;
};

#ifdef __cplusplus
}
#endif /* __cplusplus enum & struct */


/* ======= [private use] Some private-use functions [static] ======= */

/* Macro for judge the EXTI Trigger */
#define KIM_BUTTON_JUDGE_TRIGGER(EXTI_TRIGGER_X)        \
    (( (EXTI_TRIGGER_X == EXTI_TRIGGER_RISING_FALLING)  \
    || (EXTI_TRIGGER_X == EXTI_TRIGGER_NONE) ) ? -1 : 1)

/* Macro for judge GPIOx_BASE and GPIO_PIN_X */
#define KIM_BUTTON_JUDGE_GPIO_PIN(GPIOx_BASE, PIN)      \
    ( ((                                                \
        (GPIOx_BASE) == GPIOA_BASE ||                   \
        (GPIOx_BASE) == GPIOB_BASE ||                   \
        (GPIOx_BASE) == GPIOC_BASE ||                   \
        (GPIOx_BASE) == GPIOD_BASE ||                   \
        (GPIOx_BASE) == GPIOE_BASE                      \
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

/* Macro to connect macro */
#define KIM_BUTTON_CONNECT(_a, _b)          KIM_BUTTON_CONNECT_1(_a, _b)
#define KIM_BUTTON_CONNECT_1(_a, _b)        KIM_BUTTON_CONNECT_2(_a, _b)
#define KIM_BUTTON_CONNECT_2(_a, _b)        _a ## _b

#define KIM_BUTTON_CONNECT3(_a, _b, _c)     KIM_BUTTON_CONNECT3_1(_a, _b, _c)
#define KIM_BUTTON_CONNECT3_1(_a, _b, _c)   KIM_BUTTON_CONNECT3_2(_a, _b, _c)
#define KIM_BUTTON_CONNECT3_2(_a, _b, _c)   _a ## _b ## _c

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
    struct Kim_Button_Status* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint32_t exti_trigger_x,
    void (* method_asynchronous_handler) (
        void (*short_push_callback)(void),
        void (*long_push_callback)(void),
        void (*double_push_callback)(void)
    ),
    void (* method_interrupt_handler) (void)
)
{
    /* Initialize the member variables */
    self->private_push_time = 0;

    KIM_BUTTON_CRITICAL_ZONE_BEGIN();
    self->private_state = Kim_Button_State_Wait_For_Interrupt;
    self->private_time_stamp_interrupt = 0;
    KIM_BUTTON_CRITICAL_ZONE_END();

    self->private_time_stamp_loop = 0;
    self->public_long_push_min_time = KIM_BUTTON_LONG_PUSH_MIN_TIME;

    /* Initialize the public method */
    self->method_asynchronous_handler = method_asynchronous_handler;
    self->method_interrupt_handler = method_interrupt_handler;

    /* SysTick configure */
#if (KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK == 0)
    SysTick->LOAD = (uint32_t)(KIM_BUTTON_SYSTICK_ONE_TICK - 1UL);
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk 
                    | SysTick_CTRL_CLKSOURCE_Msk;
    HAL_NVIC_SetPriority(SysTick_IRQn, 
        KIM_BUTTON_NVIC_SYSTICK_PreemptionPriority,
        KIM_BUTTON_NVIC_SYSTICK_SubPriority
    ); /* SysTick is within the core, don't need to be enabled(NVIC_Enable). */
    uwTickPrio = (1UL << __NVIC_PRIO_BITS) - 1UL;
#endif /*KIM_BUTTON_STM32CUBEMX_GENERATE_SYSTICK*/


#if (KIM_BUTTON_STM32CUBEMX_GENERATE_EXTI == 0)
    /* Initialize the AFIO Clock */
    __HAL_RCC_AFIO_CLK_ENABLE();

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
#if defined(DEBUG) || defined(_DEBUG)
        while(1) {}
#endif /* DEBUG */
        break;
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
#if defined(DEBUG) || defined(_DEBUG)
        while(1) {}
#endif /* DEBUG */
        break;
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
 * @brief           Change the status of each button when during the EXTI interrupt
 *                  service routine.
 * @param[inout]    self - pointer to self struct.
 * @return          None
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_ITHandler(
    struct Kim_Button_Status* const self
)
{
    if( ((enum Kim_Button_State)self->private_state) == Kim_Button_State_Wait_For_Double
        || ((enum Kim_Button_State)self->private_state) == Kim_Button_State_Wait_For_Interrupt) 
    {
        self->private_time_stamp_interrupt = HAL_GetTick();
        self->private_state = Kim_Button_State_Push_Delay;
    }
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
 * @param[in]       double_push_callback - callback function for double push.
 * @return          None
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_AsynchronousHandler(
    struct Kim_Button_Status* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint32_t exti_trigger_x,
    void (* short_push_callback)(void),
    void (* long_push_callback)(void),
    void (* double_push_callback)(void)
)
{
    uint8_t Normal_Bit_Val = (exti_trigger_x == EXTI_TRIGGER_RISING) ? 0 : 1;

    /* Critical Zone Begin */
    KIM_BUTTON_CRITICAL_ZONE_BEGIN();

    switch ((enum Kim_Button_State)self->private_state) {
    case Kim_Button_State_Wait_For_Interrupt:
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        break;
    case Kim_Button_State_Push_Delay:
        if(HAL_GetTick() - self->private_time_stamp_interrupt 
            > KIM_BUTTON_PUSH_DELAY_TIME)
        {
            /* Check the GPIO Pin again */
            if(HAL_GPIO_ReadPin((GPIO_TypeDef*)gpiox_base, gpio_pin_x) == Normal_Bit_Val)
            {
                self->private_state = Kim_Button_State_Wait_For_Interrupt;
            } else {
                self->private_state = Kim_Button_State_Wait_For_End;
            }
        }
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        break;
    case Kim_Button_State_Wait_For_End:
        if(HAL_GPIO_ReadPin((GPIO_TypeDef*)gpiox_base, gpio_pin_x) == Normal_Bit_Val)
        {
            self->private_push_time ++;
            self->private_time_stamp_loop = HAL_GetTick();
            self->private_state = Kim_Button_State_Release_Delay;
        }
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        break;
    case Kim_Button_State_Double_Push:
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */

        KIM_BUTTON_SAFE_CALLBACK(double_push_callback);
        self->private_push_time = 0;

        KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
        self->private_state = Kim_Button_State_Wait_For_Interrupt;
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        break;
    case Kim_Button_State_Wait_For_Double:
        if(HAL_GetTick() - self->private_time_stamp_loop
            > KIM_BUTTON_DOUBLE_PUSH_MAX_TIME)
        {
            self->private_state = Kim_Button_State_Single_Push;
        }
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        break;
    case Kim_Button_State_Single_Push:
        if(HAL_GetTick() - self->private_time_stamp_interrupt
            > (uint32_t)self->public_long_push_min_time)
        {
            KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
            KIM_BUTTON_SAFE_CALLBACK(long_push_callback);
        } else {
            KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
            KIM_BUTTON_SAFE_CALLBACK(short_push_callback);
        }
        self->private_push_time = 0;

        KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
        self->private_state = Kim_Button_State_Wait_For_Interrupt;
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        break;
    case Kim_Button_State_Release_Delay:
        if(HAL_GetTick() - self->private_time_stamp_loop 
            > KIM_BUTTON_RELEASE_DELAY_TIME)
        {
            if(HAL_GPIO_ReadPin((GPIO_TypeDef*)gpiox_base, gpio_pin_x) == Normal_Bit_Val)
            {
                self->private_state = (self->private_push_time == 1)
                    ? Kim_Button_State_Wait_For_Double : Kim_Button_State_Double_Push;
            } else {
                self->private_state = Kim_Button_State_Wait_For_End;
            }
        }
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        break;
    default:
        /* ... error handler ... */
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
#if defined(DEBUG) || defined(_DEBUG)
        while(1) {}
#endif /* DEBUG */
        break;
    }
}

/* ================ Private-use functions END ====================== */



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
    struct Kim_Button_Status KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name);\
                                                                                \
    static void                                                                 \
    KIM_BUTTON_CONNECT(Kim_Button_Asynchronous_Handler_, __name)(               \
        void (*short_push_callback)(void),                                      \
        void (*long_push_callback)(void),                                       \
        void (*double_push_callback)(void)                                      \
    )                                                                           \
    {                                                                           \
        Kim_Button_PrivateUse_AsynchronousHandler(                              \
            &(KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name)),              \
            GPIOx_BASE,                                                         \
            GPIO_PIN_X,                                                         \
            EXTI_TRIGGER_X,                                                     \
            short_push_callback,                                                \
            long_push_callback,                                                 \
            double_push_callback                                                \
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
    extern struct Kim_Button_Status KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name);     \
    KIM_BUTTON_C_API void KIM_BUTTON_CONNECT3(KIM_BUTTON_NAME_PREFIX, Init_, __name)(void);


#endif /* KIM_STM32F1XX_HAL_BUTTON_H */
