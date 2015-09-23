##############################################################################
# FifoTest.mak
#
# MAKEFILE for Arduino Uno Fifo.cpp module test application.
# Uses OpenWatcom make utility to build a test application for  
# the Ardunio UNO based on the Atmel AVR ATmega328p processor.
#
# Designed for use by the WiTAQ build environment.
#
# The hex file is named FifoTest_<CPU>_<VARIANT> where CPU is the target
# processor, and VARIANT is the Arduino hardware variant.
#
# General build command: wmake [options] /f FifoTest.mak [targets]
#
# Modification History:
#
# 09/22/2015 - Tom Kerr
# Refactored .ino build rule.
#
# 08/22/2015 - Tom Kerr
# Initial creation.
##############################################################################

.HOLD                       # Don't delete on failure
.EXTENSIONS:                # Clear default extensions
.EXTENSIONS:.o .c .cpp .ino # Assign intrinsic extensions

# Macros for defining the build target.
PRJ  = FifoTes          # Project name
CPU  = atmega328p       # Processor type
ARCH = ARDUINO_ARCH_AVR # Processor architecture
BRD  = ARDUINO_AVR_UNO  # Target ardunio board
ARDUINO_REV = 10605     # Ardunio revision (1.06.05)
F_CPU = 16000000L       # Clock frequency
VARIANT = standard      # The Arduino hardware variant

TARGET = $(PRJ)_$(CPU)_$(VARIANT) # Full name of target

TOOL_BASE = C:\tools\arduino-1.6.5-r2\hardware
WITAQ_BASE = C:\dev\WiTAQ\Arduino

# Toolset definition.
BIN = $(TOOL_BASE)\tools\avr\bin        # Path to compiler and tools
AR = $(BIN)\avr-ar.exe                  # Archive processor
AS = $(BIN)\avr-as.exe                  # Assembler
CC = $(BIN)\avr-gcc.exe                 # C/C++ Compiler
LD = $(BIN)\avr-gcc.exe                 # Linker/loader
OC = $(BIN)\avr-objcopy.exe             # Create hex file
SZ = $(BIN)\avr-size.exe                # Program size

# Include and source code file paths.
INC = -I. -I$(WITAQ_BASE)\lib_witaq -I$(WITAQ_BASE)\alib\include -I$(WITAQ_BASE)\core -I$(TOOL_BASE)\arduino\avr\variants\$(VARIANT) -I$(TOOL_BASE)\tools\avr\avr\include

# Arduino core library.
LD_CORE_LIB = $(WITAQ_BASE)\core\core_arduino_$(CPU)_$(VARIANT).a

# Third-party add-on library.
LD_ALIB = $(WITAQ_BASE)\alib\lib_arduino_$(CPU)_$(VARIANT).a

# Custom libraries
LD_LIB_WITAQ = $(WITAQ_BASE)\lib_witaq\lib_witaq_$(CPU)_$(VARIANT).a

# Paths to search for source files.
SOURCEPATH = .
.c: $(SOURCEPATH);
.cpp: $(SOURCEPATH);
.ino: $(SOURCEPATH);

# Toolset flags.
CFLAGS  = -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=$(CPU) &
    -DF_CPU=$(F_CPU) -DARDUINO=$(ARDUINO_REV) -D$(CPU) -D$(BRD) -D$(ARCH)
CCFLAGS = -fno-threadsafe-statics
INOFLAGS = -x c++
LDFLAGS = -Os -Wl,--gc-sections -mmcu=$(CPU)
ARFLAGS = rcs
OCFLAGS = --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0

# List of object files that must be built for the project.
OBJS = &
   FifoTest.o &
   Fifo.o &
   aunit.o 
      
all: $(TARGET).hex
 
$(TARGET).hex : $(TARGET).elf
    $(SZ) --mcu=$(CPU) -C $(TARGET).elf
    $(OC) -O ihex -j .eeprom $(OCFLAGS) $(TARGET).elf $(TARGET).eep
    $(OC) -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex 
    
$(TARGET).elf : $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET).elf $(OBJS) $(LD_LIB_WITAQ) $(LD_ALIB) $(LD_CORE_LIB) -lm
    
.ino.o:
    $(CC) $(CFLAGS) $(CCFLAGS) $(INOFLAGS) $(INC) -Wa,-ahls="$[*.lst" "$[*.ino" -o"$[&.o"
    
.c.o:
	$(CC) $(CFLAGS) $(INC) -Wa,-ahls="$[*.lst" "$[*.c" -o"$[&.o" 
    
.cpp.o:
	$(CC) $(CFLAGS) $(CCFLAGS) $(INC) -Wa,-ahls="$[*.lst" "$[*.cpp" -o"$[&.o" 
    
clobber: clean .SYMBOLIC
    if exist $(TARGET).hex del $(TARGET).hex
    if exist $(TARGET).map del $(TARGET).map
    if exist *.lst del *.lst

clean: .SYMBOLIC
   	if exist *.o del $(OBJS)
    if exist *.d del *.d
    if exist $(TARGET).elf del $(TARGET).elf
    if exist $(TARGET).eep del $(TARGET).eep
    
.BEFORE
    @echo -------------------------------------------------------------------------------
   	@echo Building Project : $(TARGET)

.AFTER
    @echo Done.
    @echo -------------------------------------------------------------------------------
    
