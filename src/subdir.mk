################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../decrypt.c \
../encrypt.c \
../escribir.c \
../estega.c \
../leer.c \
../main.c \
../rijndael.c \
../zcomp.c 

C_DEPS += \
./decrypt.d \
./encrypt.d \
./escribir.d \
./estega.d \
./leer.d \
./main.d \
./rijndael.d \
./zcomp.d 

OBJS += \
./decrypt.o \
./encrypt.o \
./escribir.o \
./estega.o \
./leer.o \
./main.o \
./rijndael.o \
./zcomp.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -ggdb -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


