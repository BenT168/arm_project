################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
<<<<<<< HEAD
../assemble.c \
../emulate.c 

OBJS += \
./assemble.o \
./emulate.o 

C_DEPS += \
./assemble.d \
=======
../emulate.c 

OBJS += \
./emulate.o 

C_DEPS += \
>>>>>>> d92123bcbc824b444712f5cdd0ff06e09b9d2171
./emulate.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
<<<<<<< HEAD
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
=======
	@echo 'Invoking: Cygwin C Compiler'
	gcc -std=c99 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
>>>>>>> d92123bcbc824b444712f5cdd0ff06e09b9d2171
	@echo 'Finished building: $<'
	@echo ' '


