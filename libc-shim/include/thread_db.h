#ifndef _SLATE_THREAD_DB_H
#define _SLATE_THREAD_DB_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)

#include <pthread.h>
#include <stdint.h>
#include <sys/procfs.h>
#include <sys/types.h>

typedef enum {
  TD_OK,
  TD_ERR,
  TD_NOTHR,
  TD_NOSV,
  TD_NOLWP,
  TD_BADPH,
  TD_BADTH,
  TD_BADSH,
  TD_BADTA,
  TD_BADKEY,
  TD_NOMSG,
  TD_NOFPREGS,
  TD_NOLIBTHREAD,
  TD_NOEVENT,
  TD_NOCAPAB,
  TD_DBERR,
  TD_NOAPLIC,
  TD_NOTSD,
  TD_MALLOC,
  TD_PARTIALREG,
  TD_NOXREGS,
  TD_TLSDEFER,
  TD_NOTALLOC = TD_TLSDEFER,
  TD_VERSION,
  TD_NOTLS
} td_err_e;

typedef enum {
  TD_THR_ANY_STATE,
  TD_THR_UNKNOWN,
  TD_THR_STOPPED,
  TD_THR_RUN,
  TD_THR_ACTIVE,
  TD_THR_ZOMBIE,
  TD_THR_SLEEP,
  TD_THR_STOPPED_ASLEEP
} td_thr_state_e;

typedef enum { TD_THR_ANY_TYPE, TD_THR_USER, TD_THR_SYSTEM } td_thr_type_e;

typedef struct td_thragent td_thragent_t;
typedef struct td_thrhandle {
  td_thragent_t *th_ta_p;
  psaddr_t th_unique;
} td_thrhandle_t;

#define TD_THR_ANY_USER_FLAGS 0xffffffff
#define TD_THR_LOWEST_PRIORITY -20
#define TD_SIGNO_MASK NULL
#define TD_EVENTSIZE 2
#define BT_UISHIFT 5
#define BT_NBIPUI (1 << BT_UISHIFT)
#define BT_UIMASK (BT_NBIPUI - 1)

typedef struct td_thr_events {
  uint32_t event_bits[TD_EVENTSIZE];
} td_thr_events_t;

#define __td_eventmask(n) (UINT32_C(1) << (((n) - 1) & BT_UIMASK))
#define __td_eventword(n) (UINT32_C((n) - 1) >> BT_UISHIFT)
#define td_event_emptyset(setp) \
  do { \
    int __i; \
    for (__i = TD_EVENTSIZE; __i > 0; --__i) \
      (setp)->event_bits[__i - 1] = 0; \
  } while (0)
#define td_event_fillset(setp) \
  do { \
    int __i; \
    for (__i = TD_EVENTSIZE; __i > 0; --__i) \
      (setp)->event_bits[__i - 1] = UINT32_C(0xffffffff); \
  } while (0)
#define td_event_addset(setp, n) \
  (((setp)->event_bits[__td_eventword(n)]) |= __td_eventmask(n))
#define td_event_delset(setp, n) \
  (((setp)->event_bits[__td_eventword(n)]) &= ~__td_eventmask(n))
#define td_eventismember(setp, n) \
  (__td_eventmask(n) & ((setp)->event_bits[__td_eventword(n)]))
#define td_eventisempty(setp) \
  (!((setp)->event_bits[0]) && !((setp)->event_bits[1]))

typedef pthread_t thread_t;
typedef pthread_key_t thread_key_t;

typedef struct td_ta_stats {
  int nthreads;
  int r_concurrency;
  int nrunnable_num;
  int nrunnable_den;
  int a_concurrency_num;
  int a_concurrency_den;
  int nlwps_num;
  int nlwps_den;
  int nidle_num;
  int nidle_den;
} td_ta_stats_t;

typedef struct td_thrinfo {
  td_thragent_t *ti_ta_p;
  unsigned int ti_user_flags;
  thread_t ti_tid;
  char *ti_tls;
  psaddr_t ti_startfunc;
  psaddr_t ti_stkbase;
  long ti_stksize;
  psaddr_t ti_ro_area;
  int ti_ro_size;
  td_thr_state_e ti_state;
  unsigned char ti_db_suspended;
  td_thr_type_e ti_type;
  intptr_t ti_pc;
  intptr_t ti_sp;
  short ti_flags;
  int ti_pri;
  lwpid_t ti_lid;
  sigset_t ti_sigmask;
  unsigned char ti_traceme;
  unsigned char ti_preemptflag;
  unsigned char ti_pirecflag;
  sigset_t ti_pending;
  td_thr_events_t ti_events;
} td_thrinfo_t;

#endif

#endif
