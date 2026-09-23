.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error DEVKITARM is not set)
endif

include $(DEVKITARM)/gba_rules

TARGET := rex_region
BUILD := build

SOURCES := .
INCLUDES := .

ARCH := -mthumb -mthumb-interwork
CFLAGS := -g -Wall -O2 -mcpu=arm7tdmi -mtune=arm7tdmi $(ARCH)
CFLAGS += $(INCLUDE)

LDFLAGS := -g $(ARCH)

LIBS := -lgba
LIBDIRS := $(LIBGBA)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(CURDIR)
export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(notdir $(wildcard *.c))
OFILES := $(CFILES:.c=.o)

export LD := $(CC)

export INCLUDE := \
$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
-I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: all clean

all: $(OUTPUT).gba

$(OUTPUT).gba: $(OUTPUT).elf

$(OUTPUT).elf: $(OFILES)

$(BUILD):
	mkdir -p $@
	$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).gba

else

DEPENDS := $(OFILES:.o=.d)

$(OUTPUT).gba: $(OUTPUT).elf

$(OUTPUT).elf: $(OFILES)

-include $(DEPENDS)

endif
