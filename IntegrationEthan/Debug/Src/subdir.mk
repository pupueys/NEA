################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/digital_IO.c \
../Src/main.c \
../Src/parse.c \
../Src/serial.c \
../Src/set_get_leds.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/digital_IO.o \
./Src/main.o \
./Src/parse.o \
./Src/serial.o \
./Src/set_get_leds.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/digital_IO.d \
./Src/main.d \
./Src/parse.d \
./Src/serial.d \
./Src/set_get_leds.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F303VCTx -DSTM32 -DSTM32F3 -DSTM32F3DISCOVERY -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/digital_IO.cyclo ./Src/digital_IO.d ./Src/digital_IO.o ./Src/digital_IO.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/parse.cyclo ./Src/parse.d ./Src/parse.o ./Src/parse.su ./Src/serial.cyclo ./Src/serial.d ./Src/serial.o ./Src/serial.su ./Src/set_get_leds.cyclo ./Src/set_get_leds.d ./Src/set_get_leds.o ./Src/set_get_leds.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

