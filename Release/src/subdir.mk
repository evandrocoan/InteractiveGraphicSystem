################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ChooseFileWindow.cpp \
../src/RwObjectService.cpp \
../src/addobject.cpp \
../src/addtransformation.cpp \
../src/coordinate.cpp \
../src/debugger.cpp \
../src/displayfile.cpp \
../src/drawableobject.cpp \
../src/line.cpp \
../src/main.cpp \
../src/mainwindow.cpp \
../src/point.cpp \
../src/polygon.cpp \
../src/transformation.cpp \
../src/viewport.cpp \
../src/viewwindow.cpp 

OBJS += \
./src/ChooseFileWindow.o \
./src/RwObjectService.o \
./src/addobject.o \
./src/addtransformation.o \
./src/coordinate.o \
./src/debugger.o \
./src/displayfile.o \
./src/drawableobject.o \
./src/line.o \
./src/main.o \
./src/mainwindow.o \
./src/point.o \
./src/polygon.o \
./src/transformation.o \
./src/viewport.o \
./src/viewwindow.o 

CPP_DEPS += \
./src/ChooseFileWindow.d \
./src/RwObjectService.d \
./src/addobject.d \
./src/addtransformation.d \
./src/coordinate.d \
./src/debugger.d \
./src/displayfile.d \
./src/drawableobject.d \
./src/line.d \
./src/main.d \
./src/mainwindow.d \
./src/point.d \
./src/polygon.d \
./src/transformation.d \
./src/viewport.d \
./src/viewwindow.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


