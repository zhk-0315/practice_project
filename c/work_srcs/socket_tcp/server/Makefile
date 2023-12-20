FILE_PATH = $(abspath $(firstword $(MAKEFILE_LIST)))
FILE_DIR = $(patsubst %/,%,$(dir $(FILE_PATH)))
JOB_DIR ?= $(FILE_DIR)/..
include $(JOB_DIR)/include.mk

TARGET = $(BINDIR)/TcpServer

CUR_SUBDIR_LIST := $(patsubst %/,%,$(wildcard $(FILE_DIR)/*/))
SRCDIR_LIST += \
	$(FILE_DIR) \
	$(CUR_SUBDIR_LIST)

SRC += $(foreach SRCDIR,$(SRCDIR_LIST),$(wildcard $(SRCDIR)/*.c))

OBJ = $(patsubst %.c,%.o,$(SRC))
CMDB := $(patsubst %.c,%.o.json,$(SRC))

INCFLAGS += $(foreach SRCDIR,$(SRCDIR_LIST),-I$(SRCDIR))



%.o: %.c
	$(CC) -MJ $@.json -o $@ -c $^ $(INCFLAGS) $(CFLAGS)

.PHONY: all install clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(INCFLAGS) $(CFLAGS) $(LDFLAGS) $(LIBS)

install: all

clean:
	$(RM) $(TARGET) $(OBJ) $(CMDB)
