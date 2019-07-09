#
# Makefile for snake.gba
#
# --- Project details -------------------------------------------------

PROJ    := snake
TARGET  := $(PROJ)

OBJS    := $(PROJ).o

# --- Build defines ---------------------------------------------------

PREFIX  := arm-none-eabi-
CXX     := $(PREFIX)g++
LD      := $(PREFIX)g++
OBJCOPY := $(PREFIX)objcopy

ARCH    := -mthumb-interwork -mthumb
SPECS   := -specs=gba.specs

CFLAGS  := $(ARCH) -O2 -Wall -fno-strict-aliasing
LDFLAGS := $(ARCH) $(SPECS)


.PHONY : build clean

# --- Build -----------------------------------------------------------
# Build process starts here!
build: $(TARGET).gba

# Strip and fix header (step 3,4)
$(TARGET).gba : $(TARGET).elf
	$(OBJCOPY) -v -O binary $< $@
	-@gbafix $@

# Link (step 2)
$(TARGET).elf : $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

# Compile (step 1)
$(OBJS) : %.o : %.cc
	$(CXX) -c $< $(CFLAGS) -o $@
		
# --- Clean -----------------------------------------------------------

clean : 
	@rm -fv *.elf
	@rm -fv *.gba
	@rm -fv *.o
	@rm -fv *.sav

#EOF
