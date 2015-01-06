####################################################################################################
#
# Make SQLite library
#
####################################################################################################

BUILD_DIR ?= ../build-internal
export BUILD_DIR
-include $(BUILD_DIR)/Makefile.base.mk

######################################################################################
### Local information

PROJECT_BASE_NAME = sqlite3

#  177-D: variable "ii" was declared but never referenced
#  222-D: floating-point operation result is out of range
#  550-D: variable "nPageHeader" was set but never used
# 1293-D: assignment in condition
# C3017W: XXXX may be used before being set
#
DIAGFLAGS += --diag_remark=177,222,550,1293,3017

PROJECTFLAGS += -DSQLITE_OS_OTHER=1
PROJECTFLAGS += -DSQLITE_ENABLE_MEMSYS3
PROJECTFLAGS += -DSQLITE_DEFAULT_TEMP_CACHE_SIZE=2000
PROJECTFLAGS += -I$(THREADX_DIR)

# Defining THREADSAFE and MUTEX_NOOP lets us define our own mutexes.
#
PROJECTFLAGS += -DSQLITE_THREADSAFE=1
PROJECTFLAGS += -DSQLITE_MUTEX_NOOP=1

-include $(BUILD_DIR)/Makefile.library.mk
