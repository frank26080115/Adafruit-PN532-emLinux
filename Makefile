PROJECT = Adafruit_PN532
PATHTO_CHIP_IO = ./CHIP_IO/source
PATHTO_ARDUINO = ./Arduino
FINALNAME = lib$(PROJECT).a

CC = gcc
AR = ar

CFLAGS = -I$(PATHTO_CHIP_IO) -I$(PATHTO_ARDUINO)

FILESOF_CHIP_IO = common constants event_gpio
SOURCESOF_CHIP_IO = $(addprefix $(PATHTO_CHIP_IO)/, $(addsuffix .c, $(FILESOF_CHIP_IO)))

FILESOF_ARDUINO = SPI Wire
SOURCESOF_ARDUINO = $(addprefix $(PATHTO_CHIP_IO)/, $(addsuffix .cpp, $(FILESOF_ARDUINO)))

SOURCES = $(PROJECT).cpp emlinux_hal.c
SOURCES += $(SOURCESOF_CHIP_IO)
SOURCES += $(SOURCESOF_ARDUINO)

OBJECTS = $(addsuffix .o, $(SOURCES))

$(FINALNAME): $(OBJECTS)
    $(AR) cr $@ $^

$(OBJECTS): $(SOURCES)
    $(CC) $(CFLAGS) -c $(SOURCES)

$(PATHTO_CHIP_IO):
    echo "CHIP_IO by xtacocorex is required, cloning from github now"
    git clone https://github.com/xtacocorex/CHIP_IO.git

.PHONY clean
clean:
    rm -f $(OBJECTS) $(FINALNAME)

.PHONY all
all: $(FINALNAME)