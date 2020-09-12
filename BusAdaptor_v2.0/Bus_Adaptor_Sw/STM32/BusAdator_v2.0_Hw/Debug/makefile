################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include UserFile/UserCode/page/subdir.mk
-include UserFile/UserCode/subdir.mk
-include UserFile/Thread/subdir.mk
-include UserFile/Resources/subdir.mk
-include UserFile/LIb/Gui/lvgl/src/lv_widgets/subdir.mk
-include UserFile/LIb/Gui/lvgl/src/lv_themes/subdir.mk
-include UserFile/LIb/Gui/lvgl/src/lv_misc/subdir.mk
-include UserFile/LIb/Gui/lvgl/src/lv_hal/subdir.mk
-include UserFile/LIb/Gui/lvgl/src/lv_gpu/subdir.mk
-include UserFile/LIb/Gui/lvgl/src/lv_font/subdir.mk
-include UserFile/LIb/Gui/lvgl/src/lv_draw/subdir.mk
-include UserFile/LIb/Gui/lvgl/src/lv_core/subdir.mk
-include UserFile/LIb/Driver/subdir.mk
-include UserFile/GuiPort/subdir.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
BusAdator_Hw.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
BusAdator_Hw.list \

OBJCOPY_BIN += \
BusAdator_Hw.bin \


# All Target
all: BusAdator_Hw.elf secondary-outputs

# Tool invocations
BusAdator_Hw.elf: $(OBJS) $(USER_OBJS) /home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/STM32F765VGHX_FLASH.ld
	arm-none-eabi-gcc -o "BusAdator_Hw.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"/home/ou/workspace/stm32_ws/BusAdaptor/BusAdaptor_v2.0/BusAdator_v2.0_Hw/STM32F765VGHX_FLASH.ld" --specs=nosys.specs -Wl,-Map="BusAdator_Hw.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

BusAdator_Hw.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "BusAdator_Hw.list"
	@echo 'Finished building: $@'
	@echo ' '

BusAdator_Hw.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "BusAdator_Hw.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
