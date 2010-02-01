//--------------------------------------------------------------------------------------------------
//
// Copyright (c) 2008 Vorne Industries
//
// $Id: threadx_mutex.cpp 1.2 2008/12/02 20:56:36Z phowell Exp $
//
// VERSION HISTORY
// ---------------
// $Log: threadx_mutex.cpp $
// Revision 1.2  2008/12/02 20:56:36Z  phowell
// Fix static initialization problems.
// Revision 1.1  2008/04/10 18:14:04Z  phowell
// Initial revision
//
//--------------------------------------------------------------------------------------------------

///-------------------------------------------------------------------------------------------------
///
/// OVERVIEW
///
/// This file defines all the necessary sqlite3_mutex_* functions to implement an application-
/// defined set of mutices for SQLite.  Most of the documentation in this file has been taken from
/// the sqlite3.h header and reproduced.
///
/// SQLite must be compiled with -DSQLITE_MUTEX_APPDEF=1 and -DSQLITE_THREADSAFE=1 in order for this
/// mutex implementation to be used.
///
///-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <sqlite3.h>
#include <tx_api.h>
#include <sqlite3_port.h>

///-------------------------------------------------------------------------------------------------
///
/// Mutex_Descriptor is a single array entry that describes a single sqlite mutex.  It's arrayed
/// into mutex_array.  mutex_array_mutex is used to control access to the array.
///
///-------------------------------------------------------------------------------------------------

struct Mutex_Descriptor
{
    enum { UNUSED_MUTEX = 0xFF };

    Mutex_Descriptor() : mutex(), mutex_id(UNUSED_MUTEX) {}
    TX_MUTEX mutex;
    uint8_t mutex_id;
};

static Mutex_Descriptor mutex_array[10];
enum { MUTEX_ARRAY_LENGTH = sizeof(mutex_array) / sizeof(*mutex_array) };

static TX_MUTEX mutex_array_mutex;
static bool mutex_array_prepared = false;

///-------------------------------------------------------------------------------------------------
///
/// One-time preparation for the array of mutices defined above, and that array's controlling mutex.
/// After this function is called, the array is guarateed to be ready for use, with all static
/// mutices created.
///
/// NOTES:
///     This function MUST be completed before any other functions that use mutex_array are called;
///         this suggests a call in initialization context.
///
///     This function SHOULD only be called once.  Calls after the first are a no-op.
///
///-------------------------------------------------------------------------------------------------
void sqlite3_threadx_mutex_initialize()
{
    if (mutex_array_prepared == true) return;
    
    tx_mutex_create(&mutex_array_mutex, "sqlite3 global", TX_INHERIT);

    for (int i = 0; i < sizeof(mutex_array) / sizeof(*mutex_array); ++i)
    {
        mutex_array[i] = Mutex_Descriptor();
        if (i >= SQLITE_MUTEX_STATIC_MASTER && i <= SQLITE_MUTEX_STATIC_LRU)
        {
            mutex_array[i].mutex_id = i;
            tx_mutex_create(&mutex_array[i].mutex, "", TX_INHERIT);
        }
    }

    mutex_array_prepared = true;    
}
        
        
///-------------------------------------------------------------------------------------------------
///
/// sqlite3_mutex_alloc
///
/// The sqlite3_mutex_alloc() routine allocates a new mutex and returns a pointer to it.  If it
/// returns NULL that means that a mutex could not be allocated.  SQLite will unwind its stack and
///  return an error.  The argument to sqlite3_mutex_alloc() is one of these integer constants:
///
/// * SQLITE_MUTEX_FAST           (0)
/// * SQLITE_MUTEX_RECURSIVE      (1)
/// * SQLITE_MUTEX_STATIC_MASTER  (2)
/// * SQLITE_MUTEX_STATIC_MEM     (3)
/// * SQLITE_MUTEX_STATIC_MEM2    (4)
/// * SQLITE_MUTEX_STATIC_PRNG    (5)
/// * SQLITE_MUTEX_STATIC_LRU     (6)
///
/// The first two constants cause sqlite3_mutex_alloc() to create a new mutex.  The new mutex is
/// recursive when SQLITE_MUTEX_RECURSIVE is used but not necessarily so when SQLITE_MUTEX_FAST is
/// used. The mutex implementation does not need to make a distinction between
/// SQLITE_MUTEX_RECURSIVE and SQLITE_MUTEX_FAST if it does not want to.  But SQLite will only
/// request a recursive mutex in cases where it really needs one.  If a faster non-recursive mutex
/// implementation is available on the host platform, the mutex subsystem might return such a mutex
/// in response to SQLITE_MUTEX_FAST.
///
/// The other allowed parameters to sqlite3_mutex_alloc() each return a pointer to a static
/// preexisting mutex.  Four static mutexes are used by the current version of SQLite.  Future
/// versions of SQLite may add additional static mutexes.  Static mutexes are for internal use by
/// SQLite only.  Applications that use SQLite mutexes should use only the dynamic mutexes returned
/// by SQLITE_MUTEX_FAST or SQLITE_MUTEX_RECURSIVE.
///
///-------------------------------------------------------------------------------------------------                                          
sqlite3_mutex *sqlite3_mutex_alloc(int mutex_id)
{
    // We will return 0 if we can't return or allocate the correct kind of mutex.
    //
    Mutex_Descriptor* return_mutex = 0;

    // Static mutices are indexed directly
    //
    if (mutex_id >= SQLITE_MUTEX_STATIC_MASTER && mutex_id <= SQLITE_MUTEX_STATIC_LRU)
    {
        return_mutex = &mutex_array[mutex_id];
    }
    // For dynamic mutices, we loop through the array until we find an empty slot and take it.
    //
    else for (int i = 0; i < MUTEX_ARRAY_LENGTH && return_mutex == 0; ++i)
    {
        tx_mutex_get(&mutex_array_mutex, TX_WAIT_FOREVER);
        if (mutex_array[i].mutex_id == Mutex_Descriptor::UNUSED_MUTEX)
        {
            mutex_array[i].mutex_id = mutex_id;
            return_mutex = &mutex_array[i];
        }
        tx_mutex_put(&mutex_array_mutex);
    }
    
    return reinterpret_cast<sqlite3_mutex*>(return_mutex);
}

///-------------------------------------------------------------------------------------------------
///
/// The sqlite3_mutex_free() routine deallocates a previously allocated dynamic mutex.  SQLite is
/// careful to deallocate every dynamic mutex that it allocates.  The dynamic mutexes must not be in 
/// use when they are deallocated.  Attempting to deallocate a static mutex results in undefined
/// behavior.  SQLite never deallocates a static mutex.
///
///-------------------------------------------------------------------------------------------------
void sqlite3_mutex_free(sqlite3_mutex* sqlite_mutex)
{
    Mutex_Descriptor* mutex = reinterpret_cast<Mutex_Descriptor*>(sqlite_mutex);
    
    tx_mutex_get(&mutex_array_mutex, TX_WAIT_FOREVER);
    mutex->mutex_id = Mutex_Descriptor::UNUSED_MUTEX;
    tx_mutex_delete(&mutex->mutex);
    tx_mutex_put(&mutex_array_mutex);
}

///-------------------------------------------------------------------------------------------------
///
/// The sqlite3_mutex_enter() routine attempts to enter a mutex.  If another thread is already
/// within the mutex, it will block.
///
/// NOTES:
/// Mutexes created using SQLITE_MUTEX_RECURSIVE can be entered multiple times by the same thread.
/// In such cases the, mutex must be exited an equal number of times before another thread can
/// enter.  If the same thread tries to enter any other kind of mutex more than once, the behavior
/// is undefined.   SQLite will never exhibit such behavior in its own use of mutexes.
///
///-------------------------------------------------------------------------------------------------
void sqlite3_mutex_enter(sqlite3_mutex* sqlite_mutex) 
{
    Mutex_Descriptor* mutex = reinterpret_cast<Mutex_Descriptor*>(sqlite_mutex);

    UINT status = tx_mutex_get(&mutex->mutex, TX_WAIT_FOREVER);
// [XXX]   check_assertion(status == TX_SUCCESS);
}

///-------------------------------------------------------------------------------------------------
///
/// The sqlite3_mutex_try() routine attempts to enter a mutex.  If another thread is already within
/// the mutex, sqlite3_mutex_try() will return SQLITE_BUSY.  This function returns SQLITE_OK
/// upon successful entry. 
///
/// NOTES:
/// See sqlite_mutex_enter()
///
///-------------------------------------------------------------------------------------------------
int sqlite3_mutex_try(sqlite3_mutex* sqlite_mutex)
{
    Mutex_Descriptor* mutex = reinterpret_cast<Mutex_Descriptor*>(sqlite_mutex);

    UINT status = tx_mutex_get(&mutex->mutex, TX_NO_WAIT);
// [XXX]   check_assertion(status == TX_SUCCESS || status == TX_NOT_AVAILABLE);

    return status == TX_NOT_AVAILABLE ? SQLITE_BUSY : SQLITE_OK;
}

///-------------------------------------------------------------------------------------------------
///
/// The sqlite3_mutex_leave() routine exits a mutex that was previously entered by the same thread.
/// The behavior is undefined if the mutex is not currently entered by the calling thread or is not
/// currently allocated.  SQLite will never do either.
///
///-------------------------------------------------------------------------------------------------
void sqlite3_mutex_leave(sqlite3_mutex* sqlite_mutex)
{
    Mutex_Descriptor* mutex = reinterpret_cast<Mutex_Descriptor*>(sqlite_mutex);
    
    // We don't assert here because all possible errors still mean that the mutex isn't being held.
    // Also note that we make sure the highest priority waiting thread will get the mutex after we
    // release it.
    //
    tx_mutex_prioritize(&mutex->mutex);
    tx_mutex_put(&mutex->mutex);
}
    
    
    
///-------------------------------------------------------------------------------------------------
///
/// sqlite3_mutex_held
/// sqlite3_mutex_notheld
///
/// According to API documentation, these are only ever called in the context of an assertionm, and
/// "[t]he implementation is not required to provided versions of these routines that actually
/// work," and so we don't.  However, "[i]f the implementation does not provide working versions of
/// these routines, it should at least provide stubs that always return true so that one does not
/// get spurious assertion failures", so we do.
///
///-------------------------------------------------------------------------------------------------
int sqlite3_mutex_held(sqlite3_mutex*)
{
    return 1;
}
int sqlite3_mutex_notheld(sqlite3_mutex*)
{
    return 1;
}



