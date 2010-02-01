//--------------------------------------------------------------------------------------------------
//
// Copyright (c) 2008 Vorne Industries
//
// $Id: sqlite3_port.h 1.1 2008/04/10 18:14:03Z phowell Exp $
//
// VERSION HISTORY
// ---------------
// $Log: sqlite3_port.h $
// Revision 1.1  2008/04/10 18:14:03Z  phowell
// Initial revision
//
//--------------------------------------------------------------------------------------------------

#ifndef sqlite3_port_H
#define sqlite3_port_H

///-------------------------------------------------------------------------------------------------
///
/// OVERVIEW
///
/// This file contains declarations for all items necessary for porting SQLite 3 to the ThreadX
/// RTOS:
///
///   * A virtual file system that doesn't do anything (sqlite3_noop_vfs())
///
///-------------------------------------------------------------------------------------------------

extern "C"
{
    void sqlite3_noop_vfs(void);
    void sqlite3_threadx_mutex_initialize(void);
}

#endif
