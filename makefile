
BUILD_DIR := build

PHONY = clean
TARGET_NAME = $(notdir $(shell pwd))

include make/macros.mk
include make/env.mk

PWD := $(shell pwd)

DIRS := $(GIPCYINC) $(BARDYDIR)/BRDINC $(BARDYDIR)/BRDINC/ctrladmpro $(BARDYDIR)/IcrInc \
$(BARDYDIR)/BRDLIBS $(GIPCYINC)/pip
INC := $(addprefix  -I, $(DIRS))
LIBS += -lpip -lpip_io_utils

CFLAGS += $(INC) -std=gnu99

SRC := $(wildcard src/*.c)
SOURCE += $(SRC)
OBJ_FILES := $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCE:.c=.o)))

all: info $(TARGET_NAME)

info:
	$(CC) --version


include make/rules.mk
include make/clean.mk
include make/app.mk