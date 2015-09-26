##############################################################################
# GNU Makefile for Arduino hexTest application.
#
# Designed for use by the WiTAQ build environment.
#
# The hex file is named hexTest_<CPU>_<VARIANT> where CPU is the target
# processor, and VARIANT is the Arduino hardware variant.
#
# Targets:
#    all : Builds the hexTest_<CPU>_<VARIANT>.hex file.
#    clean : Deletes intermediate files created during the make process: *.o, *.d, *.elf, etc.
#    clobber : In addition to clean, deletes generated targets, list files, map files, etc.
#
#######################
# Modification History:
#
# 09/22/2015 - Tom Kerr
# Created.
##############################################################################

# Project name.
PROJECT = hexTest

# The target architecture, CPU, and clock frequency.
ARCH       = ARDUINO_ARCH_AVR
CPU_TARGET = atmega328p
F_CPU      = 16000000L

# The Arduino board and hardware variant, if any.
BOARD   = ARDUINO_AVR_UNO
VARIANT = standard

# Arduino IDE/core revision; 10605 = 1.06.05
ARDUINO_REV = 10605

# The full name of the target being built.
TARGET = $(PROJECT)_$(CPU_TARGET)_$(VARIANT)

# Absolute paths to base directories.
TOOL_BASE  = C:/tools/arduino-1.6.5-r2/hardware
WITAQ_BASE = C:/dev/WiTAQ/Arduino

# Toolset definition.
BIN = $(TOOL_BASE)/tools/avr/bin
AR = $(BIN)/avr-ar.exe
AS = $(BIN)/avr-as.exe
CC = $(BIN)/avr-gcc.exe
LD = $(BIN)/avr-gcc.exe
OC = $(BIN)/avr-objcopy.exe
SZ = $(BIN)/avr-size.exe

# Source file paths.
VPATH = ../../util ../../Arduino ../../Arduino/aunit

# Include and file paths.
INC = -I. -I../../util -I../../Arduino -I../../Arduino/aunit \
	-I$(WITAQ_BASE)/lib_witaq -I$(WITAQ_BASE)/alib/include -I$(WITAQ_BASE)/core \
	-I$(TOOL_BASE)/arduino/avr/variants/$(VARIANT) -I$(TOOL_BASE)/tools/avr/avr/include

# Arduino core library.
LD_CORE_LIB = $(WITAQ_BASE)/core/core_arduino_$(CPU_TARGET)_$(VARIANT).a

# Third-party add-on library.
LD_ALIB = $(WITAQ_BASE)/alib/lib_arduino_$(CPU_TARGET)_$(VARIANT).a

# Custom libraries
LD_LIB_WITAQ = $(WITAQ_BASE)/lib_witaq/lib_witaq_$(CPU_TARGET)_$(VARIANT).a

# Toolset flags.
CFLAGS   = -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=$(CPU_TARGET) \
	-DF_CPU=$(F_CPU) -DARDUINO=$(ARDUINO_REV) -D$(CPU_TARGET) -D$(BOARD) -D$(ARCH)
CCFLAGS  = -fno-threadsafe-statics
INOFLAGS = -x c++
LDFLAGS  = -Os -Wl,--gc-sections -mmcu=$(CPU_TARGET)
ARFLAGS  = rcs
OCFLAGS  = --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0

# The set of object files to make.
OBJS = \
   hexTest.o \
   Hex.o \
   aunit.o 

# The primary target is the hex file.
all : echotarget $(TARGET).hex

echotarget:
	printf "\nBuilding $(TARGET).hex \n"

$(TARGET).hex : $(TARGET).elf
	$(SZ) --mcu=$(CPU_TARGET) -C $(TARGET).elf
	$(OC) -O ihex -j .eeprom $(OCFLAGS) $(TARGET).elf $(TARGET).eep
	$(OC) -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex 
	
$(TARGET).elf : $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET).elf $(OBJS) $(LD_LIB_WITAQ) $(LD_ALIB) $(LD_CORE_LIB) -lm
	
%.o : %.c
	$(CC) $(CFLAGS) $(INC) -o $@ $<
	
%.o : %.cpp
	$(CC) $(CFLAGS) $(CCFLAGS) $(INC) -o $@ $<
	
%.o : %.ino
	$(CC) $(CFLAGS) $(CCFLAGS) $(INOFLAGS) $(INC) -Wa,-ahls=$(basename $@).lst -o $@ $<
	
clobber : clean
	rm -rf *.hex *.map *.lst
	
clean:
	rm -rf *.o *.d *.elf *.eep
	
