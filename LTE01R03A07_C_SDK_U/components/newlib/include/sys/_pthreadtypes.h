/*
 *  Written by Joel Sherrill <joel.sherrill@OARcorp.com>.
 *
 *  COPYRIGHT (c) 1989-2013, 2015.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Permission to use, copy, modify, and distribute this software for any
 *  purpose without fee is hereby granted, provided that this entire notice
 *  is included in all copies of any software which is or includes a copy
 *  or modification of this software.
 *
 *  THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 *  WARRANTY.  IN PARTICULAR,  THE AUTHOR MAKES NO REPRESENTATION
 *  OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY OF THIS
 *  SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */

#ifndef _SYS__PTHREADTYPES_H_
#define	_SYS__PTHREADTYPES_H_

#if defined(CONFIG_QUEC_PROJECT_FEATURE_OHOS_POSIX)

#include <sys/sched.h>

#define PTHREAD_COND_INITIALIZER    {0}
#define PTHREAD_RWLOCK_INITIALIZER  {0}
#define PTHREAD_MUTEX_INITIALIZER   {0}

#ifndef PTHREAD_EXPLICIT_SCHED
#define PTHREAD_CREATE_JOINABLE     0x00
#endif

#ifndef PTHREAD_CREATE_DETACHED
#define PTHREAD_CREATE_DETACHED     0x01
#endif

#ifndef PTHREAD_EXPLICIT_SCHED
#define PTHREAD_EXPLICIT_SCHED      0
#endif

#ifndef PTHREAD_INHERIT_SCHED
#define PTHREAD_INHERIT_SCHED       1
#endif

typedef long pthread_t;
typedef long pthread_condattr_t;
typedef long pthread_rwlockattr_t;
typedef long pthread_barrierattr_t;


typedef int pthread_key_t;
typedef int pthread_once_t;

enum
{
    PTHREAD_CANCEL_ASYNCHRONOUS = 0,
    PTHREAD_CANCEL_ENABLE,
    PTHREAD_CANCEL_DEFERRED,
    PTHREAD_CANCEL_DISABLE,
    PTHREAD_CANCELED
};

enum
{
    PTHREAD_MUTEX_NORMAL = 0,
    PTHREAD_MUTEX_RECURSIVE = 1,
    PTHREAD_MUTEX_ERRORCHECK = 2,
    PTHREAD_MUTEX_ERRORCHECK_NP = PTHREAD_MUTEX_ERRORCHECK,
    PTHREAD_MUTEX_RECURSIVE_NP  = PTHREAD_MUTEX_RECURSIVE,
    PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL
};

/* init value for pthread_once_t */
#define PTHREAD_ONCE_INIT       0

enum
{
    PTHREAD_PRIO_INHERIT =0,
    PTHREAD_PRIO_NONE,
    PTHREAD_PRIO_PROTECT,
};

#define PTHREAD_PROCESS_PRIVATE  0
#define PTHREAD_PROCESS_SHARED   1

#define PTHREAD_SCOPE_PROCESS   0
#define PTHREAD_SCOPE_SYSTEM    1

#define TASK_NAME_MAX_LEN       16

struct pthread_attr
{
    void* stackaddr;        /* stack address of thread */
    int   stacksize;        /* stack size of thread */

    int   inheritsched;     /* Inherit parent prio/policy */
    int   schedpolicy;      /* scheduler policy */
    struct sched_param schedparam; /* sched parameter */

    int   detachstate;      /* detach state */
};
typedef struct pthread_attr pthread_attr_t;


typedef struct {
  int   is_initialized;
  int   type;
  //int   recursive;
} pthread_mutexattr_t;

struct pthread_mutex
{
    pthread_mutexattr_t attr;
    void* lock;
};
typedef struct pthread_mutex pthread_mutex_t;

struct pthread_cond
{
    pthread_condattr_t attr;
    void* sem;
    int waiting_task_cnt;
};
typedef struct pthread_cond pthread_cond_t;

struct pthread_rwlock
{
    pthread_rwlockattr_t attr;

    pthread_mutex_t      rw_mutex;          /* basic lock on this struct */
    pthread_cond_t       rw_condreaders;    /* for reader threads waiting */
    pthread_cond_t       rw_condwriters;    /* for writer threads waiting */

    int rw_nwaitreaders;    /* the number of reader threads waiting */
    int rw_nwaitwriters;    /* the number of writer threads waiting */
    int rw_refcount;    /* 0: unlocked, -1: locked by writer, > 0 locked by n readers */
};
typedef struct pthread_rwlock pthread_rwlock_t;

/* spinlock implementation, (ADVANCED REALTIME THREADS)*/
struct pthread_spinlock
{
    int lock;
};
typedef struct pthread_spinlock pthread_spinlock_t;

struct pthread_barrier
{
    int count;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};
typedef struct pthread_barrier pthread_barrier_t;

#else /* CONFIG_QUEC_PROJECT_FEATURE_OHOS_POSIX */


#if defined(_POSIX_THREADS) || __POSIX_VISIBLE >= 199506

#include <sys/sched.h>

/*
 *  2.5 Primitive System Data Types,  P1003.1c/D10, p. 19.
 */

#if defined(__XMK__)
typedef unsigned int pthread_t;          /* identify a thread */
#else
typedef __uint32_t pthread_t;            /* identify a thread */
#endif

/* P1003.1c/D10, p. 118-119 */
#define PTHREAD_SCOPE_PROCESS 0
#define PTHREAD_SCOPE_SYSTEM  1

/* P1003.1c/D10, p. 111 */
#define PTHREAD_INHERIT_SCHED  1      /* scheduling policy and associated */
                                      /*   attributes are inherited from */
                                      /*   the calling thread. */
#define PTHREAD_EXPLICIT_SCHED 2      /* set from provided attribute object */

/* P1003.1c/D10, p. 141 */
#define PTHREAD_CREATE_DETACHED 0
#define PTHREAD_CREATE_JOINABLE  1

#if defined(__XMK__)
typedef struct pthread_attr_s {
  int contentionscope;
  struct sched_param schedparam;
  int  detachstate;
  void *stackaddr;
  size_t stacksize;
} pthread_attr_t;

#define PTHREAD_STACK_MIN       200

#else /* !defined(__XMK__) */
typedef struct {
  int is_initialized;
  void *stackaddr;
  int stacksize;
  int contentionscope;
  int inheritsched;
  int schedpolicy;
  struct sched_param schedparam;

  /* P1003.4b/D8, p. 54 adds cputime_clock_allowed attribute.  */
#if defined(_POSIX_THREAD_CPUTIME)
  int  cputime_clock_allowed;  /* see time.h */
#endif
  int  detachstate;
} pthread_attr_t;

#endif /* !defined(__XMK__) */

#if defined(_POSIX_THREAD_PROCESS_SHARED)
/* NOTE: P1003.1c/D10, p. 81 defines following values for process_shared.  */

#define PTHREAD_PROCESS_PRIVATE 0 /* visible within only the creating process */
#define PTHREAD_PROCESS_SHARED  1 /* visible too all processes with access to */
                                  /*   the memory where the resource is */
                                  /*   located */
#endif

#if defined(_POSIX_THREAD_PRIO_PROTECT)
/* Mutexes */

/* Values for blocking protocol. */

#define PTHREAD_PRIO_NONE    0
#define PTHREAD_PRIO_INHERIT 1
#define PTHREAD_PRIO_PROTECT 2
#endif

#if defined(_UNIX98_THREAD_MUTEX_ATTRIBUTES)

/* Values for mutex type */

/* The following defines are part of the X/Open System Interface (XSI). */

/*
 * This type of mutex does not detect deadlock. A thread attempting to
 * relock this mutex without first unlocking it shall deadlock. Attempting
 * to unlock a mutex locked by a different thread results in undefined
 * behavior.  Attempting to unlock an unlocked mutex results in undefined
 * behavior.
 */
#define PTHREAD_MUTEX_NORMAL     0

/*
 * A thread attempting to relock this mutex without first unlocking
 * it shall succeed in locking the mutex.  The relocking deadlock which
 * can occur with mutexes of type PTHREAD_MUTEX_NORMAL cannot occur with
 * this type of mutex.  Multiple locks of this mutex shall require the
 * same number of unlocks to release the mutex before another thread can
 * acquire the mutex. A thread attempting to unlock a mutex which another
 * thread has locked shall return with an error.  A thread attempting to
 * unlock an unlocked mutex shall return with an error.
 */
#define PTHREAD_MUTEX_RECURSIVE  1

/*
 * This type of mutex provides error checking. A thread attempting
 * to relock this mutex without first unlocking it shall return with an
 * error. A thread attempting to unlock a mutex which another thread has
 * locked shall return with an error. A thread attempting to unlock an
 * unlocked mutex shall return with an error.
 */
#define PTHREAD_MUTEX_ERRORCHECK 2

/*
 * Attempting to recursively lock a mutex of this type results
 * in undefined behavior. Attempting to unlock a mutex of this type
 * which was not locked by the calling thread results in undefined
 * behavior. Attempting to unlock a mutex of this type which is not locked
 * results in undefined behavior. An implementation may map this mutex to
 * one of the other mutex types.
 */
#define PTHREAD_MUTEX_DEFAULT    3

#endif /* !defined(_UNIX98_THREAD_MUTEX_ATTRIBUTES) */

#if defined(__XMK__)
typedef unsigned int pthread_mutex_t;    /* identify a mutex */

typedef struct {
  int type;
} pthread_mutexattr_t;

#else /* !defined(__XMK__) */
typedef __uint32_t pthread_mutex_t;      /* identify a mutex */

typedef struct {
  int   is_initialized;
#if defined(_POSIX_THREAD_PROCESS_SHARED)
  int   process_shared;  /* allow mutex to be shared amongst processes */
#endif
#if defined(_POSIX_THREAD_PRIO_PROTECT)
  int   prio_ceiling;
  int   protocol;
#endif
#if defined(_UNIX98_THREAD_MUTEX_ATTRIBUTES)
  int type;
#endif
  int   recursive;
} pthread_mutexattr_t;
#endif /* !defined(__XMK__) */

#define _PTHREAD_MUTEX_INITIALIZER ((pthread_mutex_t) 0xFFFFFFFF)

/* Condition Variables */

typedef __uint32_t pthread_cond_t;       /* identify a condition variable */

#define _PTHREAD_COND_INITIALIZER ((pthread_cond_t) 0xFFFFFFFF)

typedef struct {
  int      is_initialized;
  clock_t  clock;             /* specifiy clock for timeouts */
#if defined(_POSIX_THREAD_PROCESS_SHARED)
  int      process_shared;    /* allow this to be shared amongst processes */
#endif
} pthread_condattr_t;         /* a condition attribute object */

/* Keys */

typedef __uint32_t pthread_key_t;        /* thread-specific data keys */

typedef struct {
  int   is_initialized;  /* is this structure initialized? */
  int   init_executed;   /* has the initialization routine been run? */
} pthread_once_t;       /* dynamic package initialization */

#define _PTHREAD_ONCE_INIT  { 1, 0 }  /* is initialized and not run */
#endif /* defined(_POSIX_THREADS) || __POSIX_VISIBLE >= 199506 */

/* POSIX Barrier Types */

#if defined(_POSIX_BARRIERS)
typedef __uint32_t pthread_barrier_t;        /* POSIX Barrier Object */
typedef struct {
  int   is_initialized;  /* is this structure initialized? */
#if defined(_POSIX_THREAD_PROCESS_SHARED)
  int   process_shared;       /* allow this to be shared amongst processes */
#endif
} pthread_barrierattr_t;
#endif /* defined(_POSIX_BARRIERS) */

/* POSIX Spin Lock Types */

#if defined(_POSIX_SPIN_LOCKS)
typedef __uint32_t pthread_spinlock_t;        /* POSIX Spin Lock Object */
#endif /* defined(_POSIX_SPIN_LOCKS) */

/* POSIX Reader/Writer Lock Types */

#if defined(_POSIX_READER_WRITER_LOCKS)
typedef __uint32_t pthread_rwlock_t;         /* POSIX RWLock Object */

#define _PTHREAD_RWLOCK_INITIALIZER ((pthread_rwlock_t) 0xFFFFFFFF)

typedef struct {
  int   is_initialized;       /* is this structure initialized? */
#if defined(_POSIX_THREAD_PROCESS_SHARED)
  int   process_shared;       /* allow this to be shared amongst processes */
#endif
} pthread_rwlockattr_t;
#endif /* defined(_POSIX_READER_WRITER_LOCKS) */

#endif /* CONFIG_QUEC_PROJECT_FEATURE_OHOS_POSIX */

#endif /* ! _SYS__PTHREADTYPES_H_ */
