export JOB_DIR = $(CURDIR)
include $(JOB_DIR)/Makefile.in

SUBDIR_LIST += $(JOB_DIR)/src

.PHONY: all install clean

define subdir_make
$(MAKE) $(1) -C $(2);
endef
all:
	@ $(foreach SUBDIR,$(SUBDIR_LIST),$(call subdir_make,$@,$(SUBDIR)))

install:
	@ $(foreach SUBDIR,$(SUBDIR_LIST),$(call subdir_make,$@,$(SUBDIR)))
	@ cp -rf $(GENDIR_LIST) $(MY_LOCAL_PATH)

clean:
	@ $(foreach SUBDIR,$(SUBDIR_LIST),$(call subdir_make,$@,$(SUBDIR)))
