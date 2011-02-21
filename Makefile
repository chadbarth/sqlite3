####################################################################################################
#
# Make SQLite library
#
####################################################################################################

# Include a list of projects we depend on, and the rules to build that file.
# Makefile.projects.mk will define BUILD_DIR
#
-include ../build-internal/Makefile.projectlist.mk
-include ./Makefile.projects.mk

# We, and our dependent projects, expect BUILD_DIR, not BUILD-INTERNAL_DIR.  So re-map it.
#
# [pkh] This needs to be re-thought -- there are variable evalution problems that wind
# up creating circular dependencies.  For now, hard-map to ../build-internal
#
#BUILD_DIR ?= $(BUILD-INTERNAL_DIR)
BUILD_DIR ?= ../build-internal
export BUILD_DIR

# Include global helper rules and definitions
#
-include $(BUILD_DIR)/Makefile.functions.mk

# Include version numbers; this defines XL_VERSION and XL_VERSION_SHORT
#
-include ./Makefile.version

# Only use implicit rules created in this file.
#
.SUFFIXES:

# We begin by specifying the default make target:  this target will be made if make
# is run without any parameters.
#
.DEFAULT_GOAL := all


######################################################################################
### Local information

ifeq ($(THREADX_DIR), )
THREADX_DIR = ../threadx
$(warning No ThreadX directory specified, using $(THREADX_DIR))
endif

PROJECT_BASE_NAME = sqlite3

PROJECTFLAGS =  -DSQLITE_OS_OTHER=1
PROJECTFLAGS += -DSQLITE_ENABLE_MEMSYS5
PROJECTFLAGS += -I$(THREADX_DIR)

# Defining THREADSAFE and MUTEX_NOOP lets us define our own mutexes.
#
PROJECTFLAGS += -DSQLITE_THREADSAFE=1
PROJECTFLAGS += -DSQLITE_MUTEX_NOOP=1
PROJECTFLAGS += -DSQLITE_OMIT_LOOKASIDE=1
PROJECTFLAGS += -O0

PROJ_DIR = .
SOURCE_DIR = $(PROJ_DIR)/.
INCLUDE_DIR = $(PROJ_DIR)/.

-include $(BUILD_DIR)/Makefile.library.mk
