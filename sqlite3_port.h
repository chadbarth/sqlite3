//--------------------------------------------------------------------------------------------------
//
// Copyright (c) 2008 Vorne Industries
//
// $Id: sqlite3_port.h 1.2 2009/10/28 20:28:32Z phowell Exp $
//
// VERSION HISTORY
// ---------------
// $Log: sqlite3_port.h $
// Revision 1.2  2009/10/28 20:28:32Z  phowell
// Changes from 3.5.3 to 3.6.19 required port changes.
// Revision 1.1  2008/04/10 18:14:03Z  phowell
// Initial revision
//
//--------------------------------------------------------------------------------------------------

#ifndef sqlite3_port_H
#define sqlite3_port_H

#ifdef __cplusplus
extern "C"
{
#endif

///-------------------------------------------------------------------------------------------------
///
/// OVERVIEW
///
/// This file contains declarations for all items necessary for porting SQLite 3 to the ThreadX
/// RTOS:
///
///   * A virtual file system that doesn't do anything (sqlite3_noop_vfs())
///   * 
///
/// These are used in the context of the sqlite3_os_init() function, which is called by SQLite.
///
///-------------------------------------------------------------------------------------------------

#define SQLITE_MEMORY_SIZE (6 * 1024 * 1024)

sqlite3_vfs* get_sqlite3_noop_vfs(void);
sqlite3_mutex_methods* get_sqlite3_threadx_mutex_methods_object(void);
void vorne_xl_sqlite3_port_init(void);

#ifdef __cplusplus
}
#endif
#endif
