export CC := /usr/lib/llvm-16/bin/clang
#export AR := /usr/bin/ar
#export MAKE := /usr/bin/make

export COMMON_DIR := $(JOB_DIR)/common
export BUILD_DIR := $(JOB_DIR)/build

export BINDIR := $(BUILD_DIR)/bin
export LIBDIR := $(BUILD_DIR)/lib
export OBJDIR := $(BUILD_DIR)/obj
export INCDIR := $(BUILD_DIR)/include

CFLAGS += \
	-DLOCAL_DEBUG \
	-g \
	-Wall \
	#-Werror

LDFLAGS += \
	-L$(MY_LD_PATH)

LIBS += \
	-lDbgout \
	-lLinkQueue \
	-lSeqQueue \
	-lncurses

INCFLAGS += \
	-I$(MY_INC_PATH) \

SRCDIR_LIST += \
	$(COMMON_DIR)/pre_process \
	$(COMMON_DIR)/local_queue \
	$(COMMON_DIR)/safe_api

GENDIR_LIST += \
	$(INCDIR) \
	$(BINDIR) \
	$(LIBDIR) \
	$(OBJDIR)


define cteate_dir
test -d $(1) || mkdir -p $(1);
endef
PRE_DIR:
	@ $(foreach GENDIR,$(GENDIR_LIST),$(call cteate_dir,$(GENDIR)))
