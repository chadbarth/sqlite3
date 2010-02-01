//--------------------------------------------------------------------------------------------------
//
// Copyright (c) 2009 Vorne industries
//
// $Id: sqlite3_port.c 1.1 2009/10/28 20:27:16Z phowell Exp $
//
// VERSION HISTORY
// ---------------
// $Log: sqlite3_port.c $
// Revision 1.1  2009/10/28 20:27:16Z  phowell
// Initial revision
//
//--------------------------------------------------------------------------------------------------


#include <stdint.h>
#include <sqlite3.h>
#include <sqlite3_port.h>

// SQLite requires that its memory be 8-byte aligned.
//
static uint64_t sqlite_memory_buffer[SQLITE_MEMORY_SIZE / sizeof(uint64_t)];

// We have to handle any configuration work before sqlite3_initialize() is called, but we cannot
// register a VFS until after sqlite3_initialize() is called.  sqlite3_initialize() will call us
// back (via sqlite3_os_init()) to do the VFS registration.
//
void vorne_xl_sqlite3_port_init(void)
{
	static int initialized = 0;
	if (!initialized)
	{
		initialized = 1;
		// Configure SQLite's internal heap. The smallest allocation allowed is 4 bytes.
		//
		sqlite3_config(SQLITE_CONFIG_HEAP, sqlite_memory_buffer, sizeof(sqlite_memory_buffer), 4);
	
		// Set up the mutex implementation
		//
		sqlite3_config(SQLITE_CONFIG_MUTEX, get_sqlite3_threadx_mutex_methods_object());
	
		// Provide the strongest possible threadsafety.
		//	
		sqlite3_config(SQLITE_CONFIG_SERIALIZED);
		
		sqlite3_initialize();
	}	
}

// We need 
int sqlite3_os_init(void)
{
	static int initialized = 0;
	if (!initialized)
	{
		initialized = 1;
		// Set up the no-op VFS.
		//
		sqlite3_vfs_register(get_sqlite3_noop_vfs(), 1);
	}
	return SQLITE_OK;
}
int sqlite3_os_end(void)
{
    return SQLITE_OK;
}
