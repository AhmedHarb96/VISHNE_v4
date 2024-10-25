################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/SRC/BLE.c \
../USER/SRC/Barcode.c \
../USER/SRC/Calculation.c \
../USER/SRC/Flash.c \
../USER/SRC/LCD.c \
../USER/SRC/RTC.c \
../USER/SRC/Spectrometer.c \
../USER/SRC/System.c 

OBJS += \
./USER/SRC/BLE.o \
./USER/SRC/Barcode.o \
./USER/SRC/Calculation.o \
./USER/SRC/Flash.o \
./USER/SRC/LCD.o \
./USER/SRC/RTC.o \
./USER/SRC/Spectrometer.o \
./USER/SRC/System.o 

C_DEPS += \
./USER/SRC/BLE.d \
./USER/SRC/Barcode.d \
./USER/SRC/Calculation.d \
./USER/SRC/Flash.d \
./USER/SRC/LCD.d \
./USER/SRC/RTC.d \
./USER/SRC/Spectrometer.d \
./USER/SRC/System.d 


# Each subdirectory must supply rules for building sources it contributes
USER/SRC/%.o USER/SRC/%.su USER/SRC/%.cyclo: ../USER/SRC/%.c USER/SRC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -I../BlueNRG_2/App -I../BlueNRG_2/Target -I../Drivers/BSP/custom -I../Middlewares/ST/BlueNRG-2/hci/hci_tl_patterns/Basic -I../Middlewares/ST/BlueNRG-2/utils -I../Middlewares/ST/BlueNRG-2/includes -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-USER-2f-SRC

clean-USER-2f-SRC:
	-$(RM) ./USER/SRC/BLE.cyclo ./USER/SRC/BLE.d ./USER/SRC/BLE.o ./USER/SRC/BLE.su ./USER/SRC/Barcode.cyclo ./USER/SRC/Barcode.d ./USER/SRC/Barcode.o ./USER/SRC/Barcode.su ./USER/SRC/Calculation.cyclo ./USER/SRC/Calculation.d ./USER/SRC/Calculation.o ./USER/SRC/Calculation.su ./USER/SRC/Flash.cyclo ./USER/SRC/Flash.d ./USER/SRC/Flash.o ./USER/SRC/Flash.su ./USER/SRC/LCD.cyclo ./USER/SRC/LCD.d ./USER/SRC/LCD.o ./USER/SRC/LCD.su ./USER/SRC/RTC.cyclo ./USER/SRC/RTC.d ./USER/SRC/RTC.o ./USER/SRC/RTC.su ./USER/SRC/Spectrometer.cyclo ./USER/SRC/Spectrometer.d ./USER/SRC/Spectrometer.o ./USER/SRC/Spectrometer.su ./USER/SRC/System.cyclo ./USER/SRC/System.d ./USER/SRC/System.o ./USER/SRC/System.su

.PHONY: clean-USER-2f-SRC

