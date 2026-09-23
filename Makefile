.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment")
endif

include $(DEVKITARM)/gba_rules

TARGET := rex_region

BUILD := build
SOURCES := .
INCLUDES := .

CFILES := main.c
OFILES := $(CFILES:.c=.o)

LIBS := -lgba

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))
export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: all clean

all: $(TARGET).gba

$(TARGET).gba: $(TARGET).elf
	@gbafix $< -o $@

$(TARGET).elf: $(OFILES)
	$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(BUILD) *.o *.elf *.gba
