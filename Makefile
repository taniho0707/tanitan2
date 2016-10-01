SHELL = /bin/sh
TARGET_ARCH = -mcpu=cortex-m4 -march=armv7e-m -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -mtune=cortex-m4
INCLUDE_DIRS = -I ../STM32F4xx_DSP_StdPeriph_Lib/Libraries \
				-I ./ \
				-I ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/STM32F4xx_StdPeriph_Driver/inc \
				-I ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
				-I ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/CMSIS/Include \
				-I $(TOOLDIR)../arm-none-eabi/include \
				-I $(TOOLDIR)../arm-none-eabi/include/c++/4.9.3
STARTUP_DIR = ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc_ride7
MPU_OPTS = -DSTM32F40_41xxx
BOARD_DIR = -DHSE_VALUE=\(\(uint32_t\)8000000\) $(MPU_OPTS)
FIRMWARE_OPTS = -DUSE_STDPERIPH_DRIVER
COMPILE_OPTS = -O0 -g0 -ffunction-sections -fdata-sections -fpermissive -funsigned-char -fno-rtti -fexceptions -Wall -fmessage-length=0 -std=c++11 $(INCLUDE_DIRS) $(BOARD_OPTS) $(FIRMWARE_OPTS) $(MPU_OPTS) -fno-use-cxa-atexit

TOOLDIR = /usr/bin
CC = $(TOOLDIR)/arm-none-eabi-g++
CXX = $(CC)
AS = $(CC)
LD = $(CC)
AR = $(TOOLDIR)/arm-none-eabi-ar
OBJCOPY = $(TOOLDIR)/arm-none-eabi-objcopy
CFLAGS = $(COMPILE_OPTS) $(TARGET_ARCH)
CXXFLAGS = $(COMPILE_OPTS) $(TARGET_ARCH)
ASFLAGS = -x assembler-with-cpp -c $(TARGET_ARCH) $(COMPILE_OPTS)
LDFLAGS = -Wl,-lgcc,-lc,-lm,-lrdimon,--gc-sections,-Map=bin/main.map,-cref -T stm32_flash.ld $(INCLUDE_DIRS) -lm -lstdc++ -L $(TOOLDIR)/../arm-none-eabi/lib/thumb -L ../STM32F4xx_DSP_StdPeriph_Lib/Libraries -nostartfiles --specs=nano.specs --specs=rdimon.specs -u _printf_float $(TARGET_ARCH)

.PHONY: all
all: libstm32f4xx startup bin/main.bin

bin/main.bin: $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp)) $(STARTUP_DIR)/startup_stm32f40xx.o ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/libstm32f4xx.a
	$(LD) $(LDFLAGS) $(TARGET_ARCH) $^ -o bin/main.elf
	$(OBJCOPY) -O ihex --change-addresses 0x08000000 bin/main.elf bin/main.hex
	$(OBJCOPY) -O binary --change-addresses 0x08000000 bin/main.elf bin/main.bin
	etags *.[ch]*

LIB_OBJS = $(sort \
 $(patsubst %.c,%.o,$(wildcard ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/STM32F4xx_StdPeriph_Driver/src/*.c)))

libstm32f4xx: $(LIB_OBJS)
	$(AR) cr ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/libstm32f4xx.a $(LIB_OBJS)

startup:
	$(AS) -o $(STARTUP_DIR)/startup_stm32f40xx.o $(ASFLAGS) $(STARTUP_DIR)/startup_stm32f40xx.s

$(LIB_OBJS): \
 $(wildcard ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/STM32F4xx_StdPeriph_Driver/inc/*.h) \
 $(wildcard ../STM32F4xx_DSP_StdPeriph_Lib/Libraries/STM32F4xx_StdPeriph_Driver/src/*.c) \

.PHONY: clean
clean:
	rm -rf *.o *.s bin/* *\~

debug:

.PHONY: flash
flash:
	python2 write_main.py
<<<<<<< HEAD
=======
# stm32flash -b 115200 -w bin/main.hex -v -g 0x0 /dev/ttyUSB0
# gtkterm
>>>>>>> master

