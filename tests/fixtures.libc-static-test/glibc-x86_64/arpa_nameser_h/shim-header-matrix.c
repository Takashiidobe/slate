#include <arpa/nameser.h>

extern int slate_oracle_gettid(void);
extern int slate_oracle_pthread_sigmask(int, const __sigset_t *restrict, __sigset_t *restrict);
extern int slate_oracle_tgkill(int, int, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gettid), __typeof__(gettid)),
    "arpa/nameser.h:gettid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pthread_sigmask), __typeof__(pthread_sigmask)),
    "arpa/nameser.h:pthread_sigmask declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgkill), __typeof__(tgkill)),
    "arpa/nameser.h:tgkill declaration differs from oracle");

static __typeof__(gettid) *const slate_reference_gettid = &gettid;
static __typeof__(pthread_sigmask) *const slate_reference_pthread_sigmask = &pthread_sigmask;
static __typeof__(tgkill) *const slate_reference_tgkill = &tgkill;

extern char * slate_oracle_optarg;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_optarg), __typeof__(optarg)), "optarg object type differs from oracle");

static __typeof__(optarg) *const slate_reference_optarg = &optarg;

typedef long slate_oracle_typedef_clock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clock_t, clock_t), "typedef clock_t differs from oracle");

typedef int slate_oracle_typedef_clockid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clockid_t, clockid_t), "typedef clockid_t differs from oracle");

typedef signed char slate_oracle_typedef_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int8_t, int8_t), "typedef int8_t differs from oracle");

typedef signed char slate_oracle_typedef_int_least8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int_least8_t, int_least8_t), "typedef int_least8_t differs from oracle");

typedef unsigned long slate_oracle_typedef_pthread_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_t, pthread_t), "typedef pthread_t differs from oracle");

typedef int slate_oracle_typedef_sig_atomic_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sig_atomic_t, sig_atomic_t), "typedef sig_atomic_t differs from oracle");

typedef struct __sigset_t slate_oracle_typedef_sigset_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sigset_t, sigset_t), "typedef sigset_t differs from oracle");

typedef union sigval slate_oracle_typedef_sigval_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sigval_t, sigval_t), "typedef sigval_t differs from oracle");

typedef long slate_oracle_typedef_time_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_time_t, time_t), "typedef time_t differs from oracle");

typedef void * slate_oracle_typedef_timer_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_timer_t, timer_t), "typedef timer_t differs from oracle");

typedef unsigned char slate_oracle_typedef_uint8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint8_t, uint8_t), "typedef uint8_t differs from oracle");

typedef struct __sigset_t slate_oracle_struct_sigaction_sa_mask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigaction *)0)->sa_mask), slate_oracle_struct_sigaction_sa_mask), "struct sigaction.sa_mask field type differs from oracle");

typedef int slate_oracle_struct_sigaction_sa_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigaction *)0)->sa_flags), slate_oracle_struct_sigaction_sa_flags), "struct sigaction.sa_flags field type differs from oracle");

typedef union sigval slate_oracle_struct_sigevent_sigev_value;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigevent *)0)->sigev_value), slate_oracle_struct_sigevent_sigev_value), "struct sigevent.sigev_value field type differs from oracle");

typedef int slate_oracle_struct_sigevent_sigev_signo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigevent *)0)->sigev_signo), slate_oracle_struct_sigevent_sigev_signo), "struct sigevent.sigev_signo field type differs from oracle");

typedef int slate_oracle_struct_sigevent_sigev_notify;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigevent *)0)->sigev_notify), slate_oracle_struct_sigevent_sigev_notify), "struct sigevent.sigev_notify field type differs from oracle");

_Static_assert(sizeof(struct sigstack) == 16, "struct sigstack size differs from oracle");

_Static_assert(_Alignof(struct sigstack) == 8, "struct sigstack alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sigstack, ss_sp) == 0, "struct sigstack.ss_sp offset differs from oracle");

typedef void * slate_oracle_struct_sigstack_ss_sp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigstack *)0)->ss_sp), slate_oracle_struct_sigstack_ss_sp), "struct sigstack.ss_sp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sigstack, ss_onstack) == 8, "struct sigstack.ss_onstack offset differs from oracle");

typedef int slate_oracle_struct_sigstack_ss_onstack;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sigstack *)0)->ss_onstack), slate_oracle_struct_sigstack_ss_onstack), "struct sigstack.ss_onstack field type differs from oracle");

_Static_assert(sizeof(union sigval) == 8, "union sigval size differs from oracle");

_Static_assert(_Alignof(union sigval) == 8, "union sigval alignment differs from oracle");

_Static_assert(__builtin_offsetof(union sigval, sival_int) == 0, "union sigval.sival_int offset differs from oracle");

typedef int slate_oracle_union_sigval_sival_int;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union sigval *)0)->sival_int), slate_oracle_union_sigval_sival_int), "union sigval.sival_int field type differs from oracle");

_Static_assert(__builtin_offsetof(union sigval, sival_ptr) == 0, "union sigval.sival_ptr offset differs from oracle");

typedef void * slate_oracle_union_sigval_sival_ptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union sigval *)0)->sival_ptr), slate_oracle_union_sigval_sival_ptr), "union sigval.sival_ptr field type differs from oracle");

_Static_assert(sizeof(struct timespec) == 16, "struct timespec size differs from oracle");

_Static_assert(_Alignof(struct timespec) == 8, "struct timespec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_sec) == 0, "struct timespec.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_sec), slate_oracle_struct_timespec_tv_sec), "struct timespec.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_nsec) == 8, "struct timespec.tv_nsec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_nsec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_nsec), slate_oracle_struct_timespec_tv_nsec), "struct timespec.tv_nsec field type differs from oracle");

_Static_assert(sizeof(struct timeval) == 16, "struct timeval size differs from oracle");

_Static_assert(_Alignof(struct timeval) == 8, "struct timeval alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_sec) == 0, "struct timeval.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_sec), slate_oracle_struct_timeval_tv_sec), "struct timeval.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_usec) == 8, "struct timeval.tv_usec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_usec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_usec), slate_oracle_struct_timeval_tv_usec), "struct timeval.tv_usec field type differs from oracle");

#ifndef AIO_PRIO_DELTA_MAX
#error "arpa/nameser.h:AIO_PRIO_DELTA_MAX macro is missing from libc-shim"
#endif

#ifndef BC_BASE_MAX
#error "arpa/nameser.h:BC_BASE_MAX macro is missing from libc-shim"
#endif

#ifndef BC_DIM_MAX
#error "arpa/nameser.h:BC_DIM_MAX macro is missing from libc-shim"
#endif

#ifndef BC_SCALE_MAX
#error "arpa/nameser.h:BC_SCALE_MAX macro is missing from libc-shim"
#endif

#ifndef BC_STRING_MAX
#error "arpa/nameser.h:BC_STRING_MAX macro is missing from libc-shim"
#endif

#ifndef BUS_ADRALN
#error "arpa/nameser.h:BUS_ADRALN macro is missing from libc-shim"
#endif

#ifndef BUS_ADRERR
#error "arpa/nameser.h:BUS_ADRERR macro is missing from libc-shim"
#endif

#ifndef BUS_MCEERR_AO
#error "arpa/nameser.h:BUS_MCEERR_AO macro is missing from libc-shim"
#endif

#ifndef BUS_MCEERR_AR
#error "arpa/nameser.h:BUS_MCEERR_AR macro is missing from libc-shim"
#endif

#ifndef BUS_OBJERR
#error "arpa/nameser.h:BUS_OBJERR macro is missing from libc-shim"
#endif

#ifndef CHARCLASS_NAME_MAX
#error "arpa/nameser.h:CHARCLASS_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef CLD_CONTINUED
#error "arpa/nameser.h:CLD_CONTINUED macro is missing from libc-shim"
#endif

#ifndef CLD_DUMPED
#error "arpa/nameser.h:CLD_DUMPED macro is missing from libc-shim"
#endif

#ifndef CLD_EXITED
#error "arpa/nameser.h:CLD_EXITED macro is missing from libc-shim"
#endif

#ifndef CLD_KILLED
#error "arpa/nameser.h:CLD_KILLED macro is missing from libc-shim"
#endif

#ifndef CLD_STOPPED
#error "arpa/nameser.h:CLD_STOPPED macro is missing from libc-shim"
#endif

#ifndef CLD_TRAPPED
#error "arpa/nameser.h:CLD_TRAPPED macro is missing from libc-shim"
#endif

#ifndef COLL_WEIGHTS_MAX
#error "arpa/nameser.h:COLL_WEIGHTS_MAX macro is missing from libc-shim"
#endif

#ifndef DELAYTIMER_MAX
#error "arpa/nameser.h:DELAYTIMER_MAX macro is missing from libc-shim"
#endif

#ifndef EXPR_NEST_MAX
#error "arpa/nameser.h:EXPR_NEST_MAX macro is missing from libc-shim"
#endif

#ifndef FPE_CONDTRAP
#error "arpa/nameser.h:FPE_CONDTRAP macro is missing from libc-shim"
#endif

#ifndef FPE_FLTDIV
#error "arpa/nameser.h:FPE_FLTDIV macro is missing from libc-shim"
#endif

#ifndef FPE_FLTINV
#error "arpa/nameser.h:FPE_FLTINV macro is missing from libc-shim"
#endif

#ifndef FPE_FLTOVF
#error "arpa/nameser.h:FPE_FLTOVF macro is missing from libc-shim"
#endif

#ifndef FPE_FLTRES
#error "arpa/nameser.h:FPE_FLTRES macro is missing from libc-shim"
#endif

#ifndef FPE_FLTSUB
#error "arpa/nameser.h:FPE_FLTSUB macro is missing from libc-shim"
#endif

#ifndef FPE_FLTUND
#error "arpa/nameser.h:FPE_FLTUND macro is missing from libc-shim"
#endif

#ifndef FPE_FLTUNK
#error "arpa/nameser.h:FPE_FLTUNK macro is missing from libc-shim"
#endif

#ifndef FPE_INTDIV
#error "arpa/nameser.h:FPE_INTDIV macro is missing from libc-shim"
#endif

#ifndef FPE_INTOVF
#error "arpa/nameser.h:FPE_INTOVF macro is missing from libc-shim"
#endif

#ifndef FP_XSTATE_MAGIC1
#error "arpa/nameser.h:FP_XSTATE_MAGIC1 macro is missing from libc-shim"
#endif

#ifndef FP_XSTATE_MAGIC2
#error "arpa/nameser.h:FP_XSTATE_MAGIC2 macro is missing from libc-shim"
#endif

#ifndef FP_XSTATE_MAGIC2_SIZE
#error "arpa/nameser.h:FP_XSTATE_MAGIC2_SIZE macro is missing from libc-shim"
#endif

#ifndef HOST_NAME_MAX
#error "arpa/nameser.h:HOST_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef ILL_BADIADDR
#error "arpa/nameser.h:ILL_BADIADDR macro is missing from libc-shim"
#endif

#ifndef ILL_BADSTK
#error "arpa/nameser.h:ILL_BADSTK macro is missing from libc-shim"
#endif

#ifndef ILL_COPROC
#error "arpa/nameser.h:ILL_COPROC macro is missing from libc-shim"
#endif

#ifndef ILL_ILLADR
#error "arpa/nameser.h:ILL_ILLADR macro is missing from libc-shim"
#endif

#ifndef ILL_ILLOPC
#error "arpa/nameser.h:ILL_ILLOPC macro is missing from libc-shim"
#endif

#ifndef ILL_ILLOPN
#error "arpa/nameser.h:ILL_ILLOPN macro is missing from libc-shim"
#endif

#ifndef ILL_ILLTRP
#error "arpa/nameser.h:ILL_ILLTRP macro is missing from libc-shim"
#endif

#ifndef ILL_PRVOPC
#error "arpa/nameser.h:ILL_PRVOPC macro is missing from libc-shim"
#endif

#ifndef ILL_PRVREG
#error "arpa/nameser.h:ILL_PRVREG macro is missing from libc-shim"
#endif

#ifndef IOV_MAX
#error "arpa/nameser.h:IOV_MAX macro is missing from libc-shim"
#endif

#ifndef LINE_MAX
#error "arpa/nameser.h:LINE_MAX macro is missing from libc-shim"
#endif

#ifndef LOGIN_NAME_MAX
#error "arpa/nameser.h:LOGIN_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef LONG_BIT
#error "arpa/nameser.h:LONG_BIT macro is missing from libc-shim"
#endif

#ifndef MAXSYMLINKS
#error "arpa/nameser.h:MAXSYMLINKS macro is missing from libc-shim"
#endif

#ifndef MINSIGSTKSZ
#error "arpa/nameser.h:MINSIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef MQ_PRIO_MAX
#error "arpa/nameser.h:MQ_PRIO_MAX macro is missing from libc-shim"
#endif

#ifndef NCARGS
#error "arpa/nameser.h:NCARGS macro is missing from libc-shim"
#endif

#ifndef NL_ARGMAX
#error "arpa/nameser.h:NL_ARGMAX macro is missing from libc-shim"
#endif

#ifndef NL_LANGMAX
#error "arpa/nameser.h:NL_LANGMAX macro is missing from libc-shim"
#endif

#ifndef NL_MSGMAX
#error "arpa/nameser.h:NL_MSGMAX macro is missing from libc-shim"
#endif

#ifndef NL_NMAX
#error "arpa/nameser.h:NL_NMAX macro is missing from libc-shim"
#endif

#ifndef NL_SETMAX
#error "arpa/nameser.h:NL_SETMAX macro is missing from libc-shim"
#endif

#ifndef NL_TEXTMAX
#error "arpa/nameser.h:NL_TEXTMAX macro is missing from libc-shim"
#endif

#ifndef NOFILE
#error "arpa/nameser.h:NOFILE macro is missing from libc-shim"
#endif

#ifndef NS_CMPRSFLGS
#error "arpa/nameser.h:NS_CMPRSFLGS macro is missing from libc-shim"
#endif

#ifndef NS_DEFAULTPORT
#error "arpa/nameser.h:NS_DEFAULTPORT macro is missing from libc-shim"
#endif

#ifndef NS_GET16
#error "arpa/nameser.h:NS_GET16 macro is missing from libc-shim"
#endif

#ifndef NS_GET32
#error "arpa/nameser.h:NS_GET32 macro is missing from libc-shim"
#endif

#ifndef NS_HFIXEDSZ
#error "arpa/nameser.h:NS_HFIXEDSZ macro is missing from libc-shim"
#endif

#ifndef NS_IN6ADDRSZ
#error "arpa/nameser.h:NS_IN6ADDRSZ macro is missing from libc-shim"
#endif

#ifndef NS_INADDRSZ
#error "arpa/nameser.h:NS_INADDRSZ macro is missing from libc-shim"
#endif

#ifndef NS_INT16SZ
#error "arpa/nameser.h:NS_INT16SZ macro is missing from libc-shim"
#endif

#ifndef NS_INT32SZ
#error "arpa/nameser.h:NS_INT32SZ macro is missing from libc-shim"
#endif

#ifndef NS_INT8SZ
#error "arpa/nameser.h:NS_INT8SZ macro is missing from libc-shim"
#endif

#ifndef NS_MAXCDNAME
#error "arpa/nameser.h:NS_MAXCDNAME macro is missing from libc-shim"
#endif

#ifndef NS_MAXDNAME
#error "arpa/nameser.h:NS_MAXDNAME macro is missing from libc-shim"
#endif

#ifndef NS_MAXLABEL
#error "arpa/nameser.h:NS_MAXLABEL macro is missing from libc-shim"
#endif

#ifndef NS_MAXMSG
#error "arpa/nameser.h:NS_MAXMSG macro is missing from libc-shim"
#endif

#ifndef NS_OPT_DNSSEC_OK
#error "arpa/nameser.h:NS_OPT_DNSSEC_OK macro is missing from libc-shim"
#endif

#ifndef NS_OPT_NSID
#error "arpa/nameser.h:NS_OPT_NSID macro is missing from libc-shim"
#endif

#ifndef NS_PACKETSZ
#error "arpa/nameser.h:NS_PACKETSZ macro is missing from libc-shim"
#endif

#ifndef NS_PUT16
#error "arpa/nameser.h:NS_PUT16 macro is missing from libc-shim"
#endif

#ifndef NS_PUT32
#error "arpa/nameser.h:NS_PUT32 macro is missing from libc-shim"
#endif

#ifndef NS_QFIXEDSZ
#error "arpa/nameser.h:NS_QFIXEDSZ macro is missing from libc-shim"
#endif

#ifndef NS_RRFIXEDSZ
#error "arpa/nameser.h:NS_RRFIXEDSZ macro is missing from libc-shim"
#endif

#ifndef NS_TSIG_ALG_HMAC_MD5
#error "arpa/nameser.h:NS_TSIG_ALG_HMAC_MD5 macro is missing from libc-shim"
#endif

#ifndef NS_TSIG_ERROR_FORMERR
#error "arpa/nameser.h:NS_TSIG_ERROR_FORMERR macro is missing from libc-shim"
#endif

#ifndef NS_TSIG_ERROR_NO_SPACE
#error "arpa/nameser.h:NS_TSIG_ERROR_NO_SPACE macro is missing from libc-shim"
#endif

#ifndef NS_TSIG_ERROR_NO_TSIG
#error "arpa/nameser.h:NS_TSIG_ERROR_NO_TSIG macro is missing from libc-shim"
#endif

#ifndef NS_TSIG_FUDGE
#error "arpa/nameser.h:NS_TSIG_FUDGE macro is missing from libc-shim"
#endif

#ifndef NS_TSIG_TCP_COUNT
#error "arpa/nameser.h:NS_TSIG_TCP_COUNT macro is missing from libc-shim"
#endif

#ifndef NZERO
#error "arpa/nameser.h:NZERO macro is missing from libc-shim"
#endif

#ifndef POLL_ERR
#error "arpa/nameser.h:POLL_ERR macro is missing from libc-shim"
#endif

#ifndef POLL_HUP
#error "arpa/nameser.h:POLL_HUP macro is missing from libc-shim"
#endif

#ifndef POLL_IN
#error "arpa/nameser.h:POLL_IN macro is missing from libc-shim"
#endif

#ifndef POLL_MSG
#error "arpa/nameser.h:POLL_MSG macro is missing from libc-shim"
#endif

#ifndef POLL_OUT
#error "arpa/nameser.h:POLL_OUT macro is missing from libc-shim"
#endif

#ifndef POLL_PRI
#error "arpa/nameser.h:POLL_PRI macro is missing from libc-shim"
#endif

#ifndef PTHREAD_DESTRUCTOR_ITERATIONS
#error "arpa/nameser.h:PTHREAD_DESTRUCTOR_ITERATIONS macro is missing from libc-shim"
#endif

#ifndef PTHREAD_KEYS_MAX
#error "arpa/nameser.h:PTHREAD_KEYS_MAX macro is missing from libc-shim"
#endif

#ifndef PTHREAD_STACK_MIN
#error "arpa/nameser.h:PTHREAD_STACK_MIN macro is missing from libc-shim"
#endif

#ifndef RE_DUP_MAX
#error "arpa/nameser.h:RE_DUP_MAX macro is missing from libc-shim"
#endif

#ifndef SA_INTERRUPT
#error "arpa/nameser.h:SA_INTERRUPT macro is missing from libc-shim"
#endif

#ifndef SA_NOCLDSTOP
#error "arpa/nameser.h:SA_NOCLDSTOP macro is missing from libc-shim"
#endif

#ifndef SA_NOCLDWAIT
#error "arpa/nameser.h:SA_NOCLDWAIT macro is missing from libc-shim"
#endif

#ifndef SA_NODEFER
#error "arpa/nameser.h:SA_NODEFER macro is missing from libc-shim"
#endif

#ifndef SA_NOMASK
#error "arpa/nameser.h:SA_NOMASK macro is missing from libc-shim"
#endif

#ifndef SA_ONESHOT
#error "arpa/nameser.h:SA_ONESHOT macro is missing from libc-shim"
#endif

#ifndef SA_ONSTACK
#error "arpa/nameser.h:SA_ONSTACK macro is missing from libc-shim"
#endif

#ifndef SA_RESETHAND
#error "arpa/nameser.h:SA_RESETHAND macro is missing from libc-shim"
#endif

#ifndef SA_RESTART
#error "arpa/nameser.h:SA_RESTART macro is missing from libc-shim"
#endif

#ifndef SA_SIGINFO
#error "arpa/nameser.h:SA_SIGINFO macro is missing from libc-shim"
#endif

#ifndef SA_STACK
#error "arpa/nameser.h:SA_STACK macro is missing from libc-shim"
#endif

#ifndef SEGV_ACCADI
#error "arpa/nameser.h:SEGV_ACCADI macro is missing from libc-shim"
#endif

#ifndef SEGV_ACCERR
#error "arpa/nameser.h:SEGV_ACCERR macro is missing from libc-shim"
#endif

#ifndef SEGV_ADIDERR
#error "arpa/nameser.h:SEGV_ADIDERR macro is missing from libc-shim"
#endif

#ifndef SEGV_ADIPERR
#error "arpa/nameser.h:SEGV_ADIPERR macro is missing from libc-shim"
#endif

#ifndef SEGV_BNDERR
#error "arpa/nameser.h:SEGV_BNDERR macro is missing from libc-shim"
#endif

#ifndef SEGV_CPERR
#error "arpa/nameser.h:SEGV_CPERR macro is missing from libc-shim"
#endif

#ifndef SEGV_MAPERR
#error "arpa/nameser.h:SEGV_MAPERR macro is missing from libc-shim"
#endif

#ifndef SEGV_MTEAERR
#error "arpa/nameser.h:SEGV_MTEAERR macro is missing from libc-shim"
#endif

#ifndef SEGV_MTESERR
#error "arpa/nameser.h:SEGV_MTESERR macro is missing from libc-shim"
#endif

#ifndef SEGV_PKUERR
#error "arpa/nameser.h:SEGV_PKUERR macro is missing from libc-shim"
#endif

#ifndef SEM_VALUE_MAX
#error "arpa/nameser.h:SEM_VALUE_MAX macro is missing from libc-shim"
#endif

#ifndef SIGABRT
#error "arpa/nameser.h:SIGABRT macro is missing from libc-shim"
#endif

#ifndef SIGALRM
#error "arpa/nameser.h:SIGALRM macro is missing from libc-shim"
#endif

#ifndef SIGBUS
#error "arpa/nameser.h:SIGBUS macro is missing from libc-shim"
#endif

#ifndef SIGCHLD
#error "arpa/nameser.h:SIGCHLD macro is missing from libc-shim"
#endif

#ifndef SIGCLD
#error "arpa/nameser.h:SIGCLD macro is missing from libc-shim"
#endif

#ifndef SIGCONT
#error "arpa/nameser.h:SIGCONT macro is missing from libc-shim"
#endif

#ifndef SIGEV_NONE
#error "arpa/nameser.h:SIGEV_NONE macro is missing from libc-shim"
#endif

#ifndef SIGEV_SIGNAL
#error "arpa/nameser.h:SIGEV_SIGNAL macro is missing from libc-shim"
#endif

#ifndef SIGEV_THREAD
#error "arpa/nameser.h:SIGEV_THREAD macro is missing from libc-shim"
#endif

#ifndef SIGEV_THREAD_ID
#error "arpa/nameser.h:SIGEV_THREAD_ID macro is missing from libc-shim"
#endif

#ifndef SIGFPE
#error "arpa/nameser.h:SIGFPE macro is missing from libc-shim"
#endif

#ifndef SIGHUP
#error "arpa/nameser.h:SIGHUP macro is missing from libc-shim"
#endif

#ifndef SIGILL
#error "arpa/nameser.h:SIGILL macro is missing from libc-shim"
#endif

#ifndef SIGINT
#error "arpa/nameser.h:SIGINT macro is missing from libc-shim"
#endif

#ifndef SIGIO
#error "arpa/nameser.h:SIGIO macro is missing from libc-shim"
#endif

#ifndef SIGIOT
#error "arpa/nameser.h:SIGIOT macro is missing from libc-shim"
#endif

#ifndef SIGKILL
#error "arpa/nameser.h:SIGKILL macro is missing from libc-shim"
#endif

#ifndef SIGPIPE
#error "arpa/nameser.h:SIGPIPE macro is missing from libc-shim"
#endif

#ifndef SIGPOLL
#error "arpa/nameser.h:SIGPOLL macro is missing from libc-shim"
#endif

#ifndef SIGPROF
#error "arpa/nameser.h:SIGPROF macro is missing from libc-shim"
#endif

#ifndef SIGPWR
#error "arpa/nameser.h:SIGPWR macro is missing from libc-shim"
#endif

#ifndef SIGQUIT
#error "arpa/nameser.h:SIGQUIT macro is missing from libc-shim"
#endif

#ifndef SIGSEGV
#error "arpa/nameser.h:SIGSEGV macro is missing from libc-shim"
#endif

#ifndef SIGSTKFLT
#error "arpa/nameser.h:SIGSTKFLT macro is missing from libc-shim"
#endif

#ifndef SIGSTKSZ
#error "arpa/nameser.h:SIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef SIGSTOP
#error "arpa/nameser.h:SIGSTOP macro is missing from libc-shim"
#endif

#ifndef SIGSYS
#error "arpa/nameser.h:SIGSYS macro is missing from libc-shim"
#endif

#ifndef SIGTERM
#error "arpa/nameser.h:SIGTERM macro is missing from libc-shim"
#endif

#ifndef SIGTRAP
#error "arpa/nameser.h:SIGTRAP macro is missing from libc-shim"
#endif

#ifndef SIGTSTP
#error "arpa/nameser.h:SIGTSTP macro is missing from libc-shim"
#endif

#ifndef SIGTTIN
#error "arpa/nameser.h:SIGTTIN macro is missing from libc-shim"
#endif

#ifndef SIGTTOU
#error "arpa/nameser.h:SIGTTOU macro is missing from libc-shim"
#endif

#ifndef SIGURG
#error "arpa/nameser.h:SIGURG macro is missing from libc-shim"
#endif

#ifndef SIGUSR1
#error "arpa/nameser.h:SIGUSR1 macro is missing from libc-shim"
#endif

#ifndef SIGUSR2
#error "arpa/nameser.h:SIGUSR2 macro is missing from libc-shim"
#endif

#ifndef SIGVTALRM
#error "arpa/nameser.h:SIGVTALRM macro is missing from libc-shim"
#endif

#ifndef SIGWINCH
#error "arpa/nameser.h:SIGWINCH macro is missing from libc-shim"
#endif

#ifndef SIGXCPU
#error "arpa/nameser.h:SIGXCPU macro is missing from libc-shim"
#endif

#ifndef SIGXFSZ
#error "arpa/nameser.h:SIGXFSZ macro is missing from libc-shim"
#endif

#ifndef SIG_BLOCK
#error "arpa/nameser.h:SIG_BLOCK macro is missing from libc-shim"
#endif

#ifndef SIG_DFL
#error "arpa/nameser.h:SIG_DFL macro is missing from libc-shim"
#endif

#ifndef SIG_ERR
#error "arpa/nameser.h:SIG_ERR macro is missing from libc-shim"
#endif

#ifndef SIG_HOLD
#error "arpa/nameser.h:SIG_HOLD macro is missing from libc-shim"
#endif

#ifndef SIG_IGN
#error "arpa/nameser.h:SIG_IGN macro is missing from libc-shim"
#endif

#ifndef SIG_SETMASK
#error "arpa/nameser.h:SIG_SETMASK macro is missing from libc-shim"
#endif

#ifndef SIG_UNBLOCK
#error "arpa/nameser.h:SIG_UNBLOCK macro is missing from libc-shim"
#endif

#ifndef SI_ASYNCIO
#error "arpa/nameser.h:SI_ASYNCIO macro is missing from libc-shim"
#endif

#ifndef SI_ASYNCNL
#error "arpa/nameser.h:SI_ASYNCNL macro is missing from libc-shim"
#endif

#ifndef SI_DETHREAD
#error "arpa/nameser.h:SI_DETHREAD macro is missing from libc-shim"
#endif

#ifndef SI_KERNEL
#error "arpa/nameser.h:SI_KERNEL macro is missing from libc-shim"
#endif

#ifndef SI_MESGQ
#error "arpa/nameser.h:SI_MESGQ macro is missing from libc-shim"
#endif

#ifndef SI_QUEUE
#error "arpa/nameser.h:SI_QUEUE macro is missing from libc-shim"
#endif

#ifndef SI_SIGIO
#error "arpa/nameser.h:SI_SIGIO macro is missing from libc-shim"
#endif

#ifndef SI_TIMER
#error "arpa/nameser.h:SI_TIMER macro is missing from libc-shim"
#endif

#ifndef SI_TKILL
#error "arpa/nameser.h:SI_TKILL macro is missing from libc-shim"
#endif

#ifndef SI_USER
#error "arpa/nameser.h:SI_USER macro is missing from libc-shim"
#endif

#ifndef SSIZE_MAX
#error "arpa/nameser.h:SSIZE_MAX macro is missing from libc-shim"
#endif

#ifndef SS_DISABLE
#error "arpa/nameser.h:SS_DISABLE macro is missing from libc-shim"
#endif

#ifndef SS_ONSTACK
#error "arpa/nameser.h:SS_ONSTACK macro is missing from libc-shim"
#endif

#ifndef SYS_SECCOMP
#error "arpa/nameser.h:SYS_SECCOMP macro is missing from libc-shim"
#endif

#ifndef SYS_USER_DISPATCH
#error "arpa/nameser.h:SYS_USER_DISPATCH macro is missing from libc-shim"
#endif

#ifndef TRAP_BRANCH
#error "arpa/nameser.h:TRAP_BRANCH macro is missing from libc-shim"
#endif

#ifndef TRAP_BRKPT
#error "arpa/nameser.h:TRAP_BRKPT macro is missing from libc-shim"
#endif

#ifndef TRAP_HWBKPT
#error "arpa/nameser.h:TRAP_HWBKPT macro is missing from libc-shim"
#endif

#ifndef TRAP_PERF
#error "arpa/nameser.h:TRAP_PERF macro is missing from libc-shim"
#endif

#ifndef TRAP_TRACE
#error "arpa/nameser.h:TRAP_TRACE macro is missing from libc-shim"
#endif

#ifndef TRAP_UNK
#error "arpa/nameser.h:TRAP_UNK macro is missing from libc-shim"
#endif

#ifndef TTY_NAME_MAX
#error "arpa/nameser.h:TTY_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef WORD_BIT
#error "arpa/nameser.h:WORD_BIT macro is missing from libc-shim"
#endif

#ifndef ns_msg_base
#error "arpa/nameser.h:ns_msg_base macro is missing from libc-shim"
#endif

#ifndef ns_msg_count
#error "arpa/nameser.h:ns_msg_count macro is missing from libc-shim"
#endif

#ifndef ns_msg_end
#error "arpa/nameser.h:ns_msg_end macro is missing from libc-shim"
#endif

#ifndef ns_msg_id
#error "arpa/nameser.h:ns_msg_id macro is missing from libc-shim"
#endif

#ifndef ns_msg_size
#error "arpa/nameser.h:ns_msg_size macro is missing from libc-shim"
#endif

#ifndef ns_rr_class
#error "arpa/nameser.h:ns_rr_class macro is missing from libc-shim"
#endif

#ifndef ns_rr_name
#error "arpa/nameser.h:ns_rr_name macro is missing from libc-shim"
#endif

#ifndef ns_rr_rdata
#error "arpa/nameser.h:ns_rr_rdata macro is missing from libc-shim"
#endif

#ifndef ns_rr_rdlen
#error "arpa/nameser.h:ns_rr_rdlen macro is missing from libc-shim"
#endif

#ifndef ns_rr_ttl
#error "arpa/nameser.h:ns_rr_ttl macro is missing from libc-shim"
#endif

#ifndef ns_rr_type
#error "arpa/nameser.h:ns_rr_type macro is missing from libc-shim"
#endif

#ifndef sa_handler
#error "arpa/nameser.h:sa_handler macro is missing from libc-shim"
#endif

#ifndef sa_sigaction
#error "arpa/nameser.h:sa_sigaction macro is missing from libc-shim"
#endif

#ifndef si_addr
#error "arpa/nameser.h:si_addr macro is missing from libc-shim"
#endif

#ifndef si_addr_lsb
#error "arpa/nameser.h:si_addr_lsb macro is missing from libc-shim"
#endif

#ifndef si_arch
#error "arpa/nameser.h:si_arch macro is missing from libc-shim"
#endif

#ifndef si_band
#error "arpa/nameser.h:si_band macro is missing from libc-shim"
#endif

#ifndef si_call_addr
#error "arpa/nameser.h:si_call_addr macro is missing from libc-shim"
#endif

#ifndef si_fd
#error "arpa/nameser.h:si_fd macro is missing from libc-shim"
#endif

#ifndef si_int
#error "arpa/nameser.h:si_int macro is missing from libc-shim"
#endif

#ifndef si_lower
#error "arpa/nameser.h:si_lower macro is missing from libc-shim"
#endif

#ifndef si_overrun
#error "arpa/nameser.h:si_overrun macro is missing from libc-shim"
#endif

#ifndef si_pid
#error "arpa/nameser.h:si_pid macro is missing from libc-shim"
#endif

#ifndef si_pkey
#error "arpa/nameser.h:si_pkey macro is missing from libc-shim"
#endif

#ifndef si_ptr
#error "arpa/nameser.h:si_ptr macro is missing from libc-shim"
#endif

#ifndef si_status
#error "arpa/nameser.h:si_status macro is missing from libc-shim"
#endif

#ifndef si_stime
#error "arpa/nameser.h:si_stime macro is missing from libc-shim"
#endif

#ifndef si_syscall
#error "arpa/nameser.h:si_syscall macro is missing from libc-shim"
#endif

#ifndef si_timerid
#error "arpa/nameser.h:si_timerid macro is missing from libc-shim"
#endif

#ifndef si_uid
#error "arpa/nameser.h:si_uid macro is missing from libc-shim"
#endif

#ifndef si_upper
#error "arpa/nameser.h:si_upper macro is missing from libc-shim"
#endif

#ifndef si_utime
#error "arpa/nameser.h:si_utime macro is missing from libc-shim"
#endif

#ifndef si_value
#error "arpa/nameser.h:si_value macro is missing from libc-shim"
#endif

#ifndef sigev_notify_attributes
#error "arpa/nameser.h:sigev_notify_attributes macro is missing from libc-shim"
#endif

#ifndef sigev_notify_function
#error "arpa/nameser.h:sigev_notify_function macro is missing from libc-shim"
#endif

int main(void) { return 0; }
