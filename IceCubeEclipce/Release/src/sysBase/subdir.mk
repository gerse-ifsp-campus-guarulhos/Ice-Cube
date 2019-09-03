################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sysBase/adc.cpp \
../src/sysBase/flash.cpp \
../src/sysBase/gpio.cpp \
../src/sysBase/iwdg.cpp \
../src/sysBase/pwm.cpp \
../src/sysBase/usartx.cpp \
../src/sysBase/utility_timer.cpp 

OBJS += \
./src/sysBase/adc.o \
./src/sysBase/flash.o \
./src/sysBase/gpio.o \
./src/sysBase/iwdg.o \
./src/sysBase/pwm.o \
./src/sysBase/usartx.o \
./src/sysBase/utility_timer.o 

CPP_DEPS += \
./src/sysBase/adc.d \
./src/sysBase/flash.d \
./src/sysBase/gpio.d \
./src/sysBase/iwdg.d \
./src/sysBase/pwm.d \
./src/sysBase/usartx.d \
./src/sysBase/utility_timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/sysBase/%.o: ../src/sysBase/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DNDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


