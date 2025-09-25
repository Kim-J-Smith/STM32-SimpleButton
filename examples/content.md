
```
.
|
+-- example_min // 最小按键配置(仅使用基础功能，最小RAM和ROM占用) 示例
|
+-- example_multi_repeat // 【计数多击】按键配置及使用 示例
|
+-- example_timer_long_push // 【计时长按】按键配置及使用 示例
|
+-- example_combination_btn // 【组合键】按键配置及使用 示例
|
+-- example_lower_power // 【低功耗】使用 示例
```

提示：每个*示例*当中隐式包含 `kim_stm32_hal_button.h` 文件（否则仓库大小可能会太大），而不同的*示例*会在 `kim_stm32_hal_button.h` 文件开头的**宏定义选项**进行一定的自定义。所有配置信息会在 `config.h` 文件中列出，并在 `example_xxx.c` 文件开头用注释说明**非默认配置**的原因。

```
.
|
+-- example_min // minimum button configuration (using only basic functions, minimum RAM and ROM usage) example
|
+-- example_multi_repeat // [Count Multiple Hits] button configuration and usage example
|
+-- example_timer_long_push // [Timer long Press] button configuration and usage example
|
+-- example_combination_btn // [Combination button] button configuration and usage example
|
+-- example_lower_power // [Low Power] usage example
```

Note: The `kim_stm32_hal_button.h` file is implicitly included in each *example* (otherwise the repository size might be too large), and different *examples* will have certain customizations in the **macro definition options** at the beginning of the `kim_stm32_hal_button.h` file. All configuration information will be listed in the `config.h` file and the reasons for **non-default configuration** will be explained with comments at the beginning of the `example_xxx.c` file.


