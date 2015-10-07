################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../content.c \
../main.c \
../myQueue.c \
../server.c \
../simple_http.c \
../util.c 

OBJS += \
./content.o \
./main.o \
./myQueue.o \
./server.o \
./simple_http.o \
./util.o 

C_DEPS += \
./content.d \
./main.d \
./myQueue.d \
./server.d \
./simple_http.d \
./util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


