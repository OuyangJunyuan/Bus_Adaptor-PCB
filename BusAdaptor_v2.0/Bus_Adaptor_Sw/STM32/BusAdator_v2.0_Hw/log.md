# 记录一下一些错误


* 在中断函数中使用rtos isr api的时候，中断优先级>RTOS设置的最大可调用优先级则会满足
中断卡在configASSERT( ucCurrentPriority >= ucMaxSysCallPriority );

一般rtos中断最高优先级是5，所以你要用rtos api就要中断的优先级设置小于５．