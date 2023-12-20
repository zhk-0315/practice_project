export JOB_DIR = $(CURDIR)
include $(JOB_DIR)/Makefile.in


SUBJOB_DIR_LIST += $(JOB_DIR)/link
SUBJOB_DIR_LIST += $(JOB_DIR)/seq


.PHONY: all install clean

define subjob_dir_make
$(MAKE) $(1) -C $(2);
endef
all: 
	@ $(foreach SUBJOB_DIR,$(SUBJOB_DIR_LIST),$(call subjob_dir_make,$@,$(SUBJOB_DIR)))

install:
	@ $(foreach SUBJOB_DIR,$(SUBJOB_DIR_LIST),$(call subjob_dir_make,$@,$(SUBJOB_DIR)))
	@ cp -rf $(GENDIR_LIST) $(MY_LOCAL_PATH)

clean:
	@ $(foreach SUBJOB_DIR,$(SUBJOB_DIR_LIST),$(call subjob_dir_make,$@,$(SUBJOB_DIR)))
