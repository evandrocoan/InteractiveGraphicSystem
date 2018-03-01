################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/addobjectwindow.cpp \
../src/displayfile.cpp \
../src/drawoptionsbox.cpp \
../src/line.cpp \
../src/main.cpp \
../src/mainwindow.cpp \
../src/point.cpp \
../src/polygon.cpp \
../src/viewport.cpp \
../src/viewwindow.cpp 

OBJS += \
./src/addobjectwindow.o \
./src/displayfile.o \
./src/drawoptionsbox.o \
./src/line.o \
./src/main.o \
./src/mainwindow.o \
./src/point.o \
./src/polygon.o \
./src/viewport.o \
./src/viewwindow.o 

CPP_DEPS += \
./src/addobjectwindow.d \
./src/displayfile.d \
./src/drawoptionsbox.d \
./src/line.d \
./src/main.d \
./src/mainwindow.d \
./src/point.d \
./src/polygon.d \
./src/viewport.d \
./src/viewwindow.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include/gtk-3.0 -I/usr/include/gtkmm-3.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" `pkg-config --cflags --libs gtk+-3.0 gtkmm-3.0`
	@echo 'Finished building: $<'
	@echo ' '


