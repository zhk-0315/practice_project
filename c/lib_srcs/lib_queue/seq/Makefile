FILE_PATH := $(abspath $(firstword $(MAKEFILE_LIST)))
FILE_DIR := $(patsubst %/,%,$(dir $(FILE_PATH)))
JOB_DIR ?= $(FILE_DIR)/..
include $(JOB_DIR)/Makefile.in


STATIC_LIB = $(LIBDIR)/libSeqQueue.a
SHARED_LIB = $(LIBDIR)/libSeqQueue.so
TARGET = $(STATIC_LIB) $(SHARED_LIB)

CUR_SUBDIR_LIST := $(patsubst %/,%,$(wildcard $(FILE_DIR)/*/))
SRCDIR_LIST += \
	$(FILE_DIR) \
	$(CUR_SUBDIR_LIST)

SRC += $(foreach SRCDIR,$(SRCDIR_LIST),$(wildcard $(SRCDIR)/*.c))
INC += $(foreach SRCDIR,$(SRCDIR_LIST),$(wildcard $(SRCDIR)/*.h))
OPEN_INC += $(INC)

OBJ += $(patsubst %.c,%.c.o,$(SRC))

INCFLAGS += $(foreach SRCDIR,$(SRCDIR_LIST),-I$(SRCDIR))



%.c.o: %.c
	$(CC) -o $@ -c $^ $(INCFLAGS) $(CFLAGS)

.PHONY: all install clean

all: PRE_DIR $(TARGET)

$(STATIC_LIB): $(OBJ)
	$(AR) -rsc $@ $^

$(SHARED_LIB): $(OBJ)
	$(CC) --shared -fPIC -o $@ $^ $(INCFLAGS) $(CFLAGS) $(LDFLAGS) $(LIBS)

install: all
	@ install -vD -t $(INCDIR) $(OPEN_INC)

clean: 
	rm -rf $(OBJ) $(TARGET)
