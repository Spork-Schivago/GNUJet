#
# 'make'        build executable file 'GNUJet'
# 'make clean'  removes all .o and executable files
#

# the directories we want to run 'make' in
DIRS = src

# the build directory
BUILDDIRS = $(DIRS:%=build-%)
# the directories to run 'make clean' in
CLEANDIRS = $(DIRS:%=clean-%)

all: clean $(BUILDDIRS)
$(DIRS): $(BUILDDIRS)
$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)
	@echo Copying GNUJet to $(PWD)
	cp $(PWD)/src/GNUJet $(PWD)
	@echo GNUJet has been successfully compiled

# if the src directory depends on the buil-dev
# being built first, define it like this:
#  build-src: build-dev

clean: $(CLEANDIRS)
$(CLEANDIRS):
	$(MAKE) -C $(@:clean-%=%) clean
	rm -f $(PWD)/GNUJet

.PHONY: subdirs $(DIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: all clean
