################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Crc32.c \
../source/Server_Client_Com.c \
../source/aes.c \
../source/lwip_tcpecho_freertos.c \
../source/semihost_hardfault.c 

OBJS += \
./source/Crc32.o \
./source/Server_Client_Com.o \
./source/aes.o \
./source/lwip_tcpecho_freertos.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/Crc32.d \
./source/Server_Client_Com.d \
./source/aes.d \
./source/lwip_tcpecho_freertos.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VDC12_cm4 -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VLL12 -DFRDM_K64F -DFREEDOM -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -I../lwip/contrib/apps/tcpecho -I../lwip/port/arch -I../lwip/port -I../lwip/src/include/lwip/priv -I../lwip/src/include/lwip/prot -I../lwip/src/include/lwip -I../lwip/src/include/netif/ppp/polarssl -I../lwip/src/include/netif/ppp -I../lwip/src/include/netif -I../lwip/src/include/posix/sys -I../lwip/src/include/posix -I../amazon-freertos/FreeRTOS/portable -I../amazon-freertos/portable -I../amazon-freertos/include -I../board/src -I../lwip/src -I../lwip/src/include -I../lwip/contrib/apps -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


