################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UserFile/UserCode/algorithm.c \
../UserFile/UserCode/can_if.c \
../UserFile/UserCode/decode.c \
../UserFile/UserCode/page_manager.c \
../UserFile/UserCode/ringbuf.c 

OBJS += \
./UserFile/UserCode/algorithm.o \
./UserFile/UserCode/can_if.o \
./UserFile/UserCode/decode.o \
./UserFile/UserCode/page_manager.o \
./UserFile/UserCode/ringbuf.o 

C_DEPS += \
./UserFile/UserCode/algorithm.d \
./UserFile/UserCode/can_if.d \
./UserFile/UserCode/decode.d \
./UserFile/UserCode/page_manager.d \
./UserFile/UserCode/ringbuf.d 


# Each subdirectory must supply rules for building sources it contributes
UserFile/UserCode/algorithm.o: ../UserFile/UserCode/algorithm.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F765xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Driver" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/GuiPort" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lvgl" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lv_examples" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/Thread" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/UserCode" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"UserFile/UserCode/algorithm.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
UserFile/UserCode/can_if.o: ../UserFile/UserCode/can_if.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F765xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Driver" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/GuiPort" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lvgl" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lv_examples" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/Thread" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/UserCode" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"UserFile/UserCode/can_if.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
UserFile/UserCode/decode.o: ../UserFile/UserCode/decode.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F765xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Driver" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/GuiPort" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lvgl" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lv_examples" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/Thread" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/UserCode" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"UserFile/UserCode/decode.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
UserFile/UserCode/page_manager.o: ../UserFile/UserCode/page_manager.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F765xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Driver" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/GuiPort" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lvgl" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lv_examples" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/Thread" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/UserCode" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"UserFile/UserCode/page_manager.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
UserFile/UserCode/ringbuf.o: ../UserFile/UserCode/ringbuf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DSTM32F765xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Driver" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/GuiPort" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lvgl" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/LIb/Gui/lv_examples" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/Thread" -I"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/UserFile/UserCode" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"UserFile/UserCode/ringbuf.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

