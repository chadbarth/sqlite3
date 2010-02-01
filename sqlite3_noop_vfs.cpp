//--------------------------------------------------------------------------------------------------
//
// Copyright (c) 2008 Vorne Industries
//
// $Id: sqlite3_noop_vfs.cpp 1.3 2009/10/28 20:28:30Z phowell Exp $
//
// VERSION HISTORY
// ---------------
// $Log: sqlite3_noop_vfs.cpp $
// Revision 1.3  2009/10/28 20:28:30Z  phowell
// Changes from 3.5.3 to 3.6.19 required port changes.
// Revision 1.2  2008/04/10 18:44:27Z  phowell
// sqlite3_noop_vfs wi.
// Revision 1.1  2008/04/10 18:14:03Z  phowell
// Initial revision
//
//--------------------------------------------------------------------------------------------------

///-------------------------------------------------------------------------------------------------
///
/// OVERVIEW
///
/// This file contains a template VFS implementation for SQLite that is a no-op:
///     * All file open attempts fail.
///     * The current time is always 2000-01-01.
///     * There is no randomness.
///
/// The original contents of this file were provided to Vorne Industries by Hipp, Wyrick & Company,
/// who disclaimed copyright and offered this blessing in place of a legal notice:
///
///    May you do good and not evil.
///    May you find forgiveness for yourself and forgive others.
///    May you share freely, never taking more than you give.
///
///-------------------------------------------------------------------------------------------------

#include "sqlite3.h"
#include <string.h>

//
// Do not allow the opening of any files.
//
static int noop_file_open(sqlite3_vfs *pVfs, const char *zName, sqlite3_file *pFile, int flags,
                          int *pOutFlags)
{
  return SQLITE_CANTOPEN;
}

//
// Pretend that we delete any files.  But do not really
// do anything.
//
static int noop_file_delete(sqlite3_vfs *pVfs, const char *zName, int syncDir)
{
  return SQLITE_OK;
}

//
// No files exist.
//
static int noop_file_access(sqlite3_vfs*, const char *zName, int flags, int *pResOut)
{
  return 0;
}

//
// The full pathname of any file is an empty string.
//
static int noop_file_full_pathname(sqlite3_vfs *pVfs, const char *zPath, int nBuf, char *zBuf)
{
  zBuf[0] = 0;
  return SQLITE_OK;
}


//
// No randomness is supplied.  Always fill with zeros.
//
static int noop_file_randomness(sqlite3_vfs *pVfs, int nBuf, char *zBuf)
{
  memset(zBuf, 0, nBuf);
  return SQLITE_OK;
}


//
// Sleep for no time at all.
//
static int noop_file_sleep(sqlite3_vfs *pVfs, int microseconds)
{
  return 0;
}

//
// The current time is hard coded to be the stroke of midnight,
// 2000-01-01, GMT.
//
static int noop_file_current_time(sqlite3_vfs *pVfs, double *prNow)
{
  *prNow = 2451544.5;
  return 0;
}


static int noop_file_get_last_error(sqlite3_vfs*, int, char *)
{
  return 0;
}

//
// Install this no-op VFS as the default VFS within SQLite.
//
extern "C" sqlite3_vfs* get_sqlite3_noop_vfs(void)
{
  static sqlite3_vfs noop_file_system = {
    1,                      // iVersion 
    sizeof(sqlite3_file),   // szOsFile 
    512,                    // mxPathname 
    0,                      // pNext 
    "noop",                 // zName 
    0,                      // pAppData 
  
    noop_file_open,           // xOpen 
    noop_file_delete,         // xDelete 
    noop_file_access,         // xAccess 
    noop_file_full_pathname,  // xFullPathname 
    0,                  // xDlOpen 
    0,                  // xDlError 
    0,                  // xDlSym 
    0,                  // xDlClose 
    noop_file_randomness,     // xRandomness 
    noop_file_sleep,          // xSleep 
    noop_file_current_time,   // xCurrentTime 
    noop_file_get_last_error  // xGetLastError
  };
  return &noop_file_system;
}
