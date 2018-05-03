################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include protocol/bluetooth_2.7/lib/EFR32BG1B/GCC/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/tempdrv/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/emdrv/gpiointerrupt/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG1B/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG1B/Source/subdir.mk
-include lcd/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: soc-empty_2.axf

# Tool invocations
soc-empty_2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\harsi\Desktop\project\soc-empty_2\efr32bg1b232f256gm56.ld" -Xlinker --gc-sections -Xlinker -Map="soc-empty_2.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o soc-empty_2.axf -Wl,--start-group "./adc.o" "./application_properties.o" "./em_adc.o" "./gatt_db.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./main_working_mitm.o" "./pti.o" "./hardware/kit/common/bsp/bsp_stk.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./lcd/bmp.o" "./lcd/display.o" "./lcd/displayls013b7dh03.o" "./lcd/displaypalemlib.o" "./lcd/dmd_display.o" "./lcd/glib.o" "./lcd/glib_bitmap.o" "./lcd/glib_circle.o" "./lcd/glib_font_narrow_6x8.o" "./lcd/glib_font_normal_8x8.o" "./lcd/glib_font_number_16x20.o" "./lcd/glib_line.o" "./lcd/glib_polygon.o" "./lcd/glib_rectangle.o" "./lcd/glib_string.o" "./lcd/graphics.o" "./lcd/udelay.o" "./platform/Device/SiliconLabs/EFR32BG1B/Source/GCC/startup_efr32bg1b.o" "./platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o" "./platform/emdrv/gpiointerrupt/gpiointerrupt.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emdrv/tempdrv/src/tempdrv.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_usart.o" "C:\Users\harsi\Desktop\project\soc-empty_2\protocol\bluetooth_2.7\lib\EFR32BG1B\GCC\libpsstore.a" "C:\Users\harsi\Desktop\project\soc-empty_2\platform\radio\rail_lib\autogen\librail_release\librail_efr32xg1_gcc_release.a" "C:\Users\harsi\Desktop\project\soc-empty_2\protocol\bluetooth_2.7\lib\EFR32BG1B\GCC\binapploader.o" "C:\Users\harsi\Desktop\project\soc-empty_2\protocol\bluetooth_2.7\lib\EFR32BG1B\GCC\libbluetooth.a" "C:\Users\harsi\Desktop\project\soc-empty_2\protocol\bluetooth_2.7\lib\EFR32BG1B\GCC\libmbedtls.a" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: soc-empty_2.hex'
	arm-none-eabi-objcopy -O ihex "soc-empty_2.axf" "soc-empty_2.hex"
	@echo ' '

	@echo 'Building bin file: soc-empty_2.bin'
	arm-none-eabi-objcopy -O binary "soc-empty_2.axf" "soc-empty_2.bin"
	@echo ' '

	@echo 'Building s37 file: soc-empty_2.s37'
	arm-none-eabi-objcopy -O srec "soc-empty_2.axf" "soc-empty_2.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "soc-empty_2.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) soc-empty_2.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
