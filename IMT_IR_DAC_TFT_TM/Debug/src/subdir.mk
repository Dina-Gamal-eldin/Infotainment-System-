################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Delay_Program.c \
../src/GPTIM\ _program.c \
../src/HTFT_Program.c \
../src/IR_Prog.c \
../src/MEXTI_prog.c \
../src/MGPIO_program.c \
../src/MNVIC_Program.c \
../src/MRCC_prog.c \
../src/MSPI_Program.c \
../src/MSYSTICK_Program.c \
../src/MUSART_Program.c \
../src/S2P_program.c \
../src/main.c 

OBJS += \
./src/Delay_Program.o \
./src/GPTIM\ _program.o \
./src/HTFT_Program.o \
./src/IR_Prog.o \
./src/MEXTI_prog.o \
./src/MGPIO_program.o \
./src/MNVIC_Program.o \
./src/MRCC_prog.o \
./src/MSPI_Program.o \
./src/MSYSTICK_Program.o \
./src/MUSART_Program.o \
./src/S2P_program.o \
./src/main.o 

C_DEPS += \
./src/Delay_Program.d \
./src/GPTIM\ _program.d \
./src/HTFT_Program.d \
./src/IR_Prog.d \
./src/MEXTI_prog.d \
./src/MGPIO_program.d \
./src/MNVIC_Program.d \
./src/MRCC_prog.d \
./src/MSPI_Program.d \
./src/MSYSTICK_Program.d \
./src/MUSART_Program.d \
./src/S2P_program.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/GPTIM\ _program.o: ../src/GPTIM\ _program.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"src/GPTIM _program.d" -MT"src/GPTIM\ _program.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


