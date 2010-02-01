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

PROJECTFLAGS =  -DOS_OTHER=1
PROJECTFLAGS += -DSQLITE_MEMORY_SIZE=1000000
PROJECTFLAGS += -DSQLITE_THREADSAFE=1
PROJECTFLAGS += -DSQLITE_MUTEX_APPDEF=1
PROJECTFLAGS += -I$(THREADX_DIR)

PROJ_DIR = .
SOURCE_DIR = $(PROJ_DIR)/.
INCLUDE_DIR = $(PROJ_DIR)/.

-include $(BUILD_DIR)/Makefile.library.mk
