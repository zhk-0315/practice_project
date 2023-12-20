FILE_PATH = $(abspath $(firstword $(MAKEFILE_LIST)))
FILE_DIR = $(patsubst %/,%,$(dir $(FILE_PATH)))
LIB_DIR = $(FILE_DIR)/..
include $(LIB_DIR)/Makefile.in


STATIC_LIB = $(LIBDIR)/libDbgout.a
SHARED_LIB = $(LIBDIR)/libDbgout.so
TARGET = $(STATIC_LIB) $(SHARED_LIB)

CUR_SUBDIR_LIST := $(patsubst %/,%,$(wildcard $(FILE_DIR)/*/))
SRCDIR_LIST += \
	$(FILE_DIR) \
	$(CUR_SUBDIR_LIST)

SRC += $(foreach SRCDIR,$(SRCDIR_LIST),$(wildcard $(SRCDIR)/*.c))
INC += $(foreach SRCDIR,$(SRCDIR_LIST),$(wildcard $(SRCDIR)/*.h))
OPEN_INC += $(INC)

OBJ := $(patsubst %.c,%.c.o,$(SRC))



%.c.o: %.c
	$(CC) -o $@ -c $^ $(CFLAGS)

.PHONY: all install clean

all: PRE_DIR $(TARGET)

$(STATIC_LIB): $(OBJ)
	$(AR) -rsc $@ $^

$(SHARED_LIB): $(OBJ)
	$(CC) --shared -fPIC -o $@ $^ $(CFLAGS)

install: all
	@ install -vD -t $(INCDIR) $(OPEN_INC)

clean:
	rm -f $(OBJ) $(TARGET)