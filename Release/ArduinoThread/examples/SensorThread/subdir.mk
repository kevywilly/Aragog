################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
/Users/kevywilly/Documents/Arduino/libraries/ArduinoThread/examples/SensorThread/SensorThread.ino 

INO_DEPS += \
./ArduinoThread/examples/SensorThread/SensorThread.ino.d 


# Each subdirectory must supply rules for building sources it contributes
ArduinoThread/examples/SensorThread/SensorThread.o: /Users/kevywilly/Documents/Arduino/libraries/ArduinoThread/examples/SensorThread/SensorThread.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Applications/Eclipse.app/Contents/Eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_MICRO -DARDUINO_ARCH_AVR -DUSB_VID=0x2341 -DUSB_PID=0x8037 '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Micro"'  -I"/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/cores/arduino" -I"/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/variants/micro" -I"/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/libraries/Wire" -I"/Applications/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/libraries/Wire/src" -I"/Users/kevywilly/Documents/Arduino/libraries/Adafruit_PWMServoDriver" -I"/Users/kevywilly/Documents/Arduino/libraries/NewPing" -I"/Users/kevywilly/Documents/Arduino/libraries/ArduinoThread" -I"/Users/kevywilly/Documents/Robotics/workspace/RoboTools" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


