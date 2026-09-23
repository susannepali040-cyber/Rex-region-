export DEVKITPRO ?= /opt/devkitpro
export DEVKITARM ?= $(DEVKITPRO)/devkitARM
include $(DEVKITARM)/gba_rules

TARGET := rex_region
SOURCES := source
INCLUDES := include
CFILES := $(notdir $(wildcard $(SOURCES)/*.c))
OFILES := $(CFILES:.c=.o)

CFLAGS := -g -Wall -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer
CFLAGS += -I$(INCLUDES)
LIBS := -lgba
LIBDIRS := $(LIBGBA)

.PHONY: all clean
all: $(TARGET).gba

%.o: $(SOURCES)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OFILES)
	$(LD) $^ $(LIBDIRS) $(LIBS) $(LDFLAGS) -o $@

$(TARGET).gba: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	$(GBAFIX) $@

clean:
	rm -f $(OFILES) $(TARGET).elf $(TARGET).gba
