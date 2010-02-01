####################################################################################################
#
# Make SQLite library
#
####################################################################################################

-include Makefile.version

BUILD_DIR ?= ../build-internal

ifeq ($(THREADX_DIR), )
THREADX_DIR = ../ThreadX
$(warning No ThreadX directory specified, using $(THREADX_DIR))
endif

PROJECT_BASE_NAME = sqlite3

PROJECTFLAGS =  -DSQLITE_OS_OTHER=1
PROJECTFLAGS += -DSQLITE_ENABLE_MEMSYS3
PROJECTFLAGS += -I$(THREADX_DIR)

# Defining THREADSAFE and MUTEX_NOOP lets us define our own mutexes.
#
PROJECTFLAGS += -DSQLITE_THREADSAFE=1
PROJECTFLAGS += -DSQLITE_MUTEX_NOOP=1

PROJ_DIR = .
SOURCE_DIR = $(PROJ_DIR)/.
INCLUDE_DIR = $(PROJ_DIR)/.

-include $(BUILD_DIR)/Makefile.library.mk
