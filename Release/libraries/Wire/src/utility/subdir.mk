################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/libraries/Wire/src/utility/twi.c 

C_DEPS += \
./libraries/Wire/src/utility/twi.c.d 

LINK_OBJ += \
./libraries/Wire/src/utility/twi.c.o 


# Each subdirectory must supply rules for building sources it contributes
libraries/Wire/src/utility/twi.c.o: /Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/libraries/Wire/src/utility/twi.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"/Applications/Eclipse.app/Contents/Eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc" -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -flto -fno-fat-lto-objects -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_MICRO -DARDUINO_ARCH_AVR -DUSB_VID=0x2341 -DUSB_PID=0x8037 '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Micro"'  -I"/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/cores/arduino" -I"/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/variants/micro" -I"/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/libraries/Wire" -I"/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/libraries/Wire/src" -I"/Users/kevywilly/Documents/Arduino/libraries/Adafruit_PWMServoDriver" -I"/Users/kevywilly/Documents/Arduino/libraries/NewPing" -I"/Users/kevywilly/Documents/robotics/workspace/RoboTools" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


