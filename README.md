# 基于正点原子 MiniSTM 的使用 STM32CubeMX 初始化的 HAL 库例程

项目基于正点原子 MiniSTM 开发板的硬件使用 STM32CubeMX + Keil 开发。由于正点原子的 HAL 库例程是其多次封装后开发的，不方便之后的移植。所以重新由 STM32CubeMX 搭建初始化的框架，再在此基础上重写正点原子的 HAL 库例程。

## 例程目录

点击即可跳转

```mermaid
graph TD;
M01[LED]-->M02[BUTTON]-->M03[UART];
M02-->M04[EXTI]-->M11[LCD]-->M12[USMART]-->M13[RTC];
M03-->M05[IWDG];
M03-->M06[WWGD];
M02-->M07[TIMMER]-->M08[PWM]-->M09[ICAPTURE];
M12-->M14[WKUP];
M12-->M15[ADC]-->M16[ITEMP];
M12-->M17[DAC];
M12-->M18[DMA];
M12-->M19[IIC]-->M21[ITOUCH];
M12-->M20[SPI];
M12-->M22[REMOTE];
M12-->M26[EEPRROM]-->M27[MALLOC]-->M28[SD];
M20-->M28-->M29[FATFS];
click M01 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M01LED" "LED"
click M02 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M02BUTTON"
click M03 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M03UART"
click M04 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M04EXTI"
click M05 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M05IWDG"
click M06 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M06WWDG"
click M07 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M07TIMER"
click M08 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M08PWM"
click M09 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M09ICAPTURE"
click M11 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M11LCD"
click M12 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M12USMART"
click M13 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M13RTC"
click M14 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M14WKUP"
click M15 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M15ADC"
click M16 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M16ITEMP"
click M17 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M17DAC"
click M18 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M18DMA"
click M19 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M19IIC"
click M20 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M20SPI"
click M21 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M21TOUCH"
click M22 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M22REMOTE"
click M26 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M26EEPRROM"
click M27 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M27MALLOC"
click M28 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M28SD"
click M29 "https://github.com/Wingplane/MiniSTM_STM32CubeMX/tree/main/M29FATFS"
```

## 例程文件结构

	├─Core: 核心文件夹
	│  ├─Inc: 头文件文件夹
	│  └─Src: 源文件文件夹
	├─Drivers: 
	│  ├─CMSIS: 微控制器软件接口标准文件夹
	│  └─STM32F1xx_HAL_Driver: HAL 驱动文件夹
	├─MDK-ARM: 驱动文件夹
	│	├─DebugConfig: Debug设置文件夹
	│	├─MiniSTM: 编译后的烧录文件文件夹
	│	├─RTE: 运行时环境组件配置文件文件夹
	│	└─MiniSTM.uvprojx: Keil 工程文件
	├─keilkilll.bat: 删除编译过程文件
	└─MiniSTM.ioc: STM32CubeMX 工程文件
