ALL_SUBDIR = $(patsubst %,all-%,$(SUBDIR))
CLEAN_SUBDIR = $(patsubst %,clean-%,$(SUBDIR))

all: $(ALL_SUBDIR)

clean: $(CLEAN_SUBDIR)

$(ALL_SUBDIR):
	make all -C $(patsubst all-%,%,$@)

$(CLEAN_SUBDIR):
	make clean -C $(patsubst clean-%,%,$@)

