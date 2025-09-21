# Developer Guide For STM32-SimpleButton

---

### This file is a guide for developer to deeply customize STM32-SimpleButton for a new chip. You can understand how this project works and the logic of it.

---

## Content

### Intruduction for STM32-SimpleButton self
  * [Do-What](#do-what)
  * [How-To-Do](#how-to-do)
  * [Structure](#structure)
### Begin a new chip development
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

* What if you don't use STM32 or HAL or even juct use a new chip? That's fine. You can deeply customize this project by following steps:

    * 1. Get the template for your new chip [template_for_new_chip](./template_newChip.h)

    * 2. Use `Ctrl` + `F` to find all the "__CUSTOMIZABLE__"

    * 3. Customize all of them depending on your chip
    
    * 4. finish

