####################################################################################################
#
# Make SQLite library
#
####################################################################################################

# Include a list of projects we depend on, and the rules to build that file.
# Makefile.projects.mk will define BUILD_DIR
#
BUILD_DIR ?= ../build-internal
PROJECT_DIR = .
export BUILD_DIR

-include $(BUILD_DIR)/Makefile.base.mk


# Only use implicit rules created in this file.
#
.SUFFIXES:

# We begin by specifying the default make target:  this target will be made if make
# is run without any parameters.
#
.DEFAULT_GOAL := all


######################################################################################
### Local information

PROJECT_BASE_NAME = sqlite3

PROJECTFLAGS =  -DSQLITE_OS_OTHER=1
PROJECTFLAGS += -DSQLITE_ENABLE_MEMSYS3
PROJECTFLAGS += -DSQLITE_DEFAULT_TEMP_CACHE_SIZE=2000
PROJECTFLAGS += -I$(THREADX_DIR)

# Defining THREADSAFE and MUTEX_NOOP lets us define our own mutexes.
#
PROJECTFLAGS += -DSQLITE_THREADSAFE=1
PROJECTFLAGS += -DSQLITE_MUTEX_NOOP=1

PROJ_DIR = .
SOURCE_DIR = $(PROJ_DIR)/.
INCLUDE_DIR = $(PROJ_DIR)/.

-include $(BUILD_DIR)/Makefile.library.mk
