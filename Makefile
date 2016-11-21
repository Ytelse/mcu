##############################################################
# Attempt at creating a generic Makefile for IDE-less        #
# developement for EFM32GG990F1024 devices.                  #
##############################################################

##############################################################
#                        DEFINITIONS                         #
##############################################################

# Name of device we are developing for
DEVICE:=EFM32GG990F1024
# This will be the name of the binary after all compilation is done
TARGET:=PACMAN_MCU

# Directories
OBJDIR:= build
BINDIR:= bin
LSTDIR:= lst

# ANSI color codes
# Only used for fancy output
NO_COLOR=\x1b[0m
BLUE_COLOR=\x1b[34;01m

# Overwrite this to point to the EFM32 SDK on the machine it will build on
SDK_PATH:= /Users/frodeja/Documents/Skole/TDT4295/efm32_sdk/v4.4.1

##############################################################
#                          TOOLCHAIN                         #
##############################################################

# Path to toolchain folders
TOOLCHAIN_PATH:=/usr/local/arm-none-eabi/gcc-arm-none-eabi-5_4-2016q3/bin
JLINK_PATH:=/Applications/SEGGER/JLink

# Toolchain aliases
CC:=$(TOOLCHAIN_PATH)/arm-none-eabi-gcc
LD:=$(TOOLCHAIN_PATH)/arm-none-eabi-gcc
OBJCOPY:=$(TOOLCHAIN_PATH)/arm-none-eabi-objcopy
OBJDUMP:=$(TOOLCHAIN_PATH)/arm-none-eabi-objdump
JLINK:=$(JLINK_PATH)/JLinkExe

##############################################################
#                            FLAGS                           #
##############################################################

# Generate dependency list, not working properly ATM
# Currently not working:
# Only generates a dependency list for the last compiled file
# and puts it in lst/efm32gg_usb_device_dependencies.d
# -MMD : Don't generate dependencies on system header files.
# -MP  : Add phony targets, useful when a h-file is removed from a project.
# -MF  : Specify a file to write the dependencies to.
DEPFLAGS:=-MMD -MP -MF $(LSTDIR)/$(TARGET)_dependencies.d

#C compiler flags
CFLAGS:=-D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd \
-ffunction-sections -fdata-sections -fomit-frame-pointer -fwide-exec-charset=UTF-16LE \
-fshort-wchar -O3 -DNDEBUG $(DEPFLAGS)

#Assembler flags
ASFLAGS:=-x assembler-with-cpp -D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb \
-fwide-exec-charset=UTF-16LE -fshort-wchar

#Linker flags
LINKERSCRIPT:=$(SDK_PATH)/Device/SiliconLabs/EFM32GG/Source/GCC/efm32gg.ld

LDFLAGS:= -Xlinker -Map=$(LSTDIR)/$(TARGET).map -mcpu=cortex-m3 \
-mthumb -T$(LINKERSCRIPT) \
-Wl,--gc-sections,--no-wchar-size-warning

LIBS = -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group

# Include the entire EFM32 SDK
INCLUDEPATHS += $(addprefix -I$(SDK_PATH), \
/CMSIS/Include \
/Device/SiliconLabs/EFM32GG/Include \
/emlib/inc \
/usb/inc \
)
# Include local headers
# Assuming header files are found in the src directory, edit this as necessary
INCLUDEPATHS += -Isrc -Iinclude

##############################################################
#                            FILES                           #
##############################################################

# emlib sources

CSRC := $(addprefix $(SDK_PATH), \
/Device/SiliconLabs/EFM32GG/Source/system_efm32gg.c \
/emlib/src/em_system.c \
/emlib/src/em_gpio.c \
/emlib/src/em_emu.c \
/emlib/src/em_cmu.c \
/emlib/src/em_int.c \
/emlib/src/em_timer.c \
/emlib/src/em_rtc.c \
/usb/src/em_usbd.c \
/usb/src/em_usbdch9.c \
/usb/src/em_usbdep.c \
/usb/src/em_usbdint.c \
/usb/src/em_usbhal.c \
/usb/src/em_usbtimer.c \
)

# Local C sources

CSRC += \
src/main.c \
src/usbcallbacks.c \
src/leds.c \
src/dbus.c \

#src/mstate.c

# Assembly sources

sSRC +=

SSRC += $(SDK_PATH)/Device/SiliconLabs/EFM32GG/Source/GCC/startup_efm32gg.S

C_FILES := $(notdir $(CSRC) )
S_FILES := $(notdir $(SSRC) $(sSRC) )

C_PATHS := $(dir $(CSRC))
S_PATHS := $(dir $(SSRC))
S_PATHS += $(dir $(sSRC))

C_OBJS := $(addprefix $(OBJDIR)/, $(C_FILES:.c=.o))
S_OBJS := $(if $(SSRC), $(addprefix $(OBJDIR)/, $(S_FILES:.S=.o)))
s_OBJS := $(if $(sSRC), $(addprefix $(OBJDIR)/, $(S_FILES:.s=.o)))
C_DEPS := $(addprefix $(OBJDIR)/, $(C_FILES:.c=.d))
OBJS := $(C_OBJS) $(S_OBJS) $(s_OBJS)

vpath %.c $(C_PATHS)
vpath %.s $(S_PATHS)
vpath %.S $(S_PATHS)

##############################################################
#                        BUILD RULES                         #
##############################################################

.PHONY : all clean upload

all : $(BINDIR)/$(TARGET).bin

# Compile C sources into objects
$(OBJDIR)/%.o : %.c | $(OBJDIR) $(LSTDIR)
	@echo "$(BLUE_COLOR)Building file: $<$(NO_COLOR)"
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Assemble assembly sources
$(OBJDIR)/%.o : %.s | $(OBJDIR) $(LSTDIR)
	@echo "$(BLUE_COLOR)Assembling $<$(NO_COLOR)"
	$(CC) $(ASFLAGS) $(INCLUDEPATHS) -c -o $@ $<

$(OBJDIR)/%.o : %.S | $(OBJDIR) $(LSTDIR)
	@echo "$(BLUE_COLOR)Assembling $<$(NO_COLOR)"
	$(CC) $(ASFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Link objects
$(BINDIR)/$(TARGET).out : $(OBJS) | $(BINDIR)
	@echo "$(BLUE_COLOR)Linking target: $@$(NO_COLOR)"
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(BINDIR)/$(TARGET).out

# Create binary
$(BINDIR)/$(TARGET).bin : $(BINDIR)/$(TARGET).out
	@echo "$(BLUE_COLOR)Creating binary file$(NO_COLOR)"
	$(OBJCOPY) -O binary $(BINDIR)/$(TARGET).out $(BINDIR)/$(TARGET).bin
	@echo "$(BLUE_COLOR)Creating assembly listing of entire program$(NO_COLOR)"
	$(OBJDUMP) -h -S -C $(BINDIR)/$(TARGET).out>$(LSTDIR)/$(TARGET)_out.lst

$(OBJDIR) :
	mkdir $@

$(BINDIR) : 
	mkdir $@

$(LSTDIR) :
	mkdir $@

# Upload the binary to the MCU (if it is connected) using JLink
# Remove the commanderscript after use to remove clutter and
# to force updates if anything is changed in the Makefile
upload : upload_bin.jlink
	$(JLINK) -commanderscript $<
	rm -f upload_bin.jlink

# Create commanderscript for JLinkExe
# si 1 									-- Use SWD for debugging
# speed 4000 							-- default speed setting
# device EFM32GG990F1024				-- target device
# erase 								-- erase any flash currently on the device
# loadbin $(BINDIR)/$(TARGET).bin, 0x0 	-- load the project binary into address 0x0
# r 									-- reset device
# g 									-- run (go) device
# q 									-- quit JLinkExe
upload_bin.jlink :
	@echo "si 1\n\
		   speed 4000\n\
		   device EFM32GG990F1024\n\
		   erase\n\
		   loadbin $(BINDIR)/$(TARGET).bin, 0x0\n\
		   r\n\
		   g\n\
		   q\n" > $@	

clean : $(OBJDIR) $(LSTDIR) $(BINDIR)
	rm -rf $(OBJDIR) $(LSTDIR) $(BINDIR) upload_bin.jlink
