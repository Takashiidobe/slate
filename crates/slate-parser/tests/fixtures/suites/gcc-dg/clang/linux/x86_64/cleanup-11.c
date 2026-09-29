/* { dg-do run { target hppa*-*-hpux* *-*-linux* *-*-gnu* powerpc*-*-darwin* *-*-darwin[912]* *-*-uclinux* } } */
/* { dg-options "-fexceptions -fnon-call-exceptions -O2" } */
/* { dg-require-effective-target exceptions } */
/* Verify that cleanups work with exception handling through realtime signal
   frames on alternate stack.  */

#include <unwind.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

static _Unwind_Reason_Code
force_unwind_stop (int version, _Unwind_Action actions,
                   _Unwind_Exception_Class exc_class,
                   struct _Unwind_Exception *exc_obj,
                   struct _Unwind_Context *context,
                   void *stop_parameter)
{
  if (actions & _UA_END_OF_STACK)
    abort ();
  return _URC_NO_REASON;
}

static void force_unwind ()
{
  struct _Unwind_Exception *exc = malloc (sizeof (*exc));
  memset (&exc->exception_class, 0, sizeof (exc->exception_class));
  exc->exception_cleanup = 0;
                   
#ifndef __USING_SJLJ_EXCEPTIONS__
  _Unwind_ForcedUnwind (exc, force_unwind_stop, 0);
#else
  _Unwind_SjLj_ForcedUnwind (exc, force_unwind_stop, 0);
#endif
                   
  abort ();
}

int count;
char *null;

static void counter (void *p __attribute__((unused)))
{
  ++count;
}

static void handler (void *p __attribute__((unused)))
{
  if (count != 2)
    abort ();
  exit (0);
}

static int __attribute__((noinline)) fn5 ()
{
  char dummy __attribute__((cleanup (counter)));
  force_unwind ();
  return 0;
}

static void fn4 (int sig, siginfo_t *info, void *ctx)
{
  char dummy __attribute__((cleanup (counter)));
  fn5 ();
  null = NULL;
}

static void fn3 ()
{
  abort ();
}

static int __attribute__((noinline)) fn2 ()
{
  *null = 0;
  fn3 ();
  return 0;
}

static int __attribute__((noinline)) fn1 ()
{
  stack_t ss;
  struct sigaction s;

  ss.ss_size = 4 * sysconf (_SC_PAGESIZE);
  if (ss.ss_size < SIGSTKSZ)
    ss.ss_size = SIGSTKSZ;
  ss.ss_sp = malloc (ss.ss_size);
  if (ss.ss_sp == NULL)
    exit (1);
  ss.ss_flags = 0;
  if (sigaltstack (&ss, NULL) < 0)
    exit (1);

  sigemptyset (&s.sa_mask);
  s.sa_sigaction = fn4;
  s.sa_flags = SA_RESETHAND | SA_ONSTACK | SA_SIGINFO;
  sigaction (SIGSEGV, &s, NULL);
  sigaction (SIGBUS, &s, NULL);
  fn2 ();
  return 0;
}

static int __attribute__((noinline)) fn0 ()
{
  char dummy __attribute__((cleanup (handler)));
  fn1 ();
  null = 0;
  return 0;
}

int main()
{ 
  fn0 ();
  abort ();
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uid_t:[0-9]+]] __uid_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___pid_t:[0-9]+]] __pid_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___clock_t:[0-9]+]] __clock_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Word:[0-9]+]] _Unwind_Word = u64;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Exception_Class:[0-9]+]] _Unwind_Exception_Class = u64;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Context:[0-9]+]] _Unwind_Context = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Exception:[0-9]+]] _Unwind_Exception = struct {
// DEFAULT-NEXT:         field0 exception_class: u64;
// DEFAULT-NEXT:         field1 exception_cleanup: ptr<fn(@type[[TYPE0:[0-9]+]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>;
// DEFAULT-NEXT:         field2 private_1: u64;
// DEFAULT-NEXT:         field3 private_2: u64;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Exception_2:[0-9]+]] _Unwind_Exception = @type[[TYPE__Unwind_Exception]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE__URC_NO_REASON:[0-9]+]] _URC_NO_REASON = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE__URC_FOREIGN_EXCEPTION_CAUGHT:[0-9]+]] _URC_FOREIGN_EXCEPTION_CAUGHT = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE__URC_FATAL_PHASE2_ERROR:[0-9]+]] _URC_FATAL_PHASE2_ERROR = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE__URC_FATAL_PHASE1_ERROR:[0-9]+]] _URC_FATAL_PHASE1_ERROR = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE__URC_NORMAL_STOP:[0-9]+]] _URC_NORMAL_STOP = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE__URC_END_OF_STACK:[0-9]+]] _URC_END_OF_STACK = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE__URC_HANDLER_FOUND:[0-9]+]] _URC_HANDLER_FOUND = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE__URC_INSTALL_CONTEXT:[0-9]+]] _URC_INSTALL_CONTEXT = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE__URC_CONTINUE_UNWIND:[0-9]+]] _URC_CONTINUE_UNWIND = const<i32>(8);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Reason_Code:[0-9]+]] _Unwind_Reason_Code = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE__URC_NO_REASON]] _UA_SEARCH_PHASE = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE__URC_FOREIGN_EXCEPTION_CAUGHT]] _UA_CLEANUP_PHASE = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE__URC_FATAL_PHASE2_ERROR]] _UA_HANDLER_FRAME = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE__URC_FATAL_PHASE1_ERROR]] _UA_FORCE_UNWIND = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE__URC_NORMAL_STOP]] _UA_END_OF_STACK = const<i32>(16);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Action:[0-9]+]] _Unwind_Action = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Exception_Cleanup_Fn:[0-9]+]] _Unwind_Exception_Cleanup_Fn = ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Stop_Fn:[0-9]+]] _Unwind_Stop_Fn = ptr<fn(i32, @type[[TYPE1]], u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>;
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __val: array<u64, 16>;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___sigset_t:[0-9]+]] __sigset_t = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE_sigset_t:[0-9]+]] sigset_t = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE_sigval:[0-9]+]] sigval = union {
// DEFAULT-NEXT:         field0 sival_int: i32;
// DEFAULT-NEXT:         field1 sival_ptr: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE___sigval_t:[0-9]+]] __sigval_t = @type[[TYPE_sigval]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 si_signo: i32;
// DEFAULT-NEXT:         field1 si_errno: i32;
// DEFAULT-NEXT:         field2 si_code: i32;
// DEFAULT-NEXT:         field3 __pad0: i32;
// DEFAULT-NEXT:         field4 _sifields: @type[[TYPE4:[0-9]+]];
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE4]] = union {
// DEFAULT-NEXT:         field0 _pad: array<i32, 28>;
// DEFAULT-NEXT:         field1 _kill: @type[[TYPE5:[0-9]+]];
// DEFAULT-NEXT:         field2 _timer: @type[[TYPE6:[0-9]+]];
// DEFAULT-NEXT:         field3 _rt: @type[[TYPE7:[0-9]+]];
// DEFAULT-NEXT:         field4 _sigchld: @type[[TYPE8:[0-9]+]];
// DEFAULT-NEXT:         field5 _sigfault: @type[[TYPE9:[0-9]+]];
// DEFAULT-NEXT:         field6 _sigpoll: @type[[TYPE12:[0-9]+]];
// DEFAULT-NEXT:         field7 _sigsys: @type[[TYPE13:[0-9]+]];
// DEFAULT-NEXT:     } [size=112, align=8, offsets=[0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE5]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE6]] = struct {
// DEFAULT-NEXT:         field0 si_tid: i32;
// DEFAULT-NEXT:         field1 si_overrun: i32;
// DEFAULT-NEXT:         field2 si_sigval: @type[[TYPE_sigval]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE7]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:         field2 si_sigval: @type[[TYPE_sigval]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE8]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:         field2 si_status: i32;
// DEFAULT-NEXT:         field3 si_utime: i64;
// DEFAULT-NEXT:         field4 si_stime: i64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 4, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE9]] = struct {
// DEFAULT-NEXT:         field0 si_addr: ptr<void>;
// DEFAULT-NEXT:         field1 si_addr_lsb: i16;
// DEFAULT-NEXT:         field2 _bounds: @type[[TYPE10:[0-9]+]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE10]] = union {
// DEFAULT-NEXT:         field0 _addr_bnd: @type[[TYPE11:[0-9]+]];
// DEFAULT-NEXT:         field1 _pkey: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE11]] = struct {
// DEFAULT-NEXT:         field0 _lower: ptr<void>;
// DEFAULT-NEXT:         field1 _upper: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE12]] = struct {
// DEFAULT-NEXT:         field0 si_band: i64;
// DEFAULT-NEXT:         field1 si_fd: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE13]] = struct {
// DEFAULT-NEXT:         field0 _call_addr: ptr<void>;
// DEFAULT-NEXT:         field1 _syscall: i32;
// DEFAULT-NEXT:         field2 _arch: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_siginfo_t:[0-9]+]] siginfo_t = @type[[TYPE3]];
// DEFAULT-NEXT:     type @type[[TYPE___sighandler_t:[0-9]+]] __sighandler_t = ptr<fn(i32) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_sigaction:[0-9]+]] sigaction = struct {
// DEFAULT-NEXT:         field0 __sigaction_handler: @type[[TYPE14:[0-9]+]];
// DEFAULT-NEXT:         field1 sa_mask: @type[[TYPE2]];
// DEFAULT-NEXT:         field2 sa_flags: i32;
// DEFAULT-NEXT:         field3 sa_restorer: ptr<fn() -> void>;
// DEFAULT-NEXT:     } [size=152, align=8, offsets=[0, 8, 136, 144]];
// DEFAULT-NEXT:     type @type[[TYPE14]] = union {
// DEFAULT-NEXT:         field0 sa_handler: ptr<fn(i32) -> void>;
// DEFAULT-NEXT:         field1 sa_sigaction: ptr<fn(i32, ptr<@type[[TYPE3]]>, ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE15:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 ss_sp: ptr<void>;
// DEFAULT-NEXT:         field1 ss_flags: i32;
// DEFAULT-NEXT:         field2 ss_size: u64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_stack_t:[0-9]+]] stack_t = @type[[TYPE15]];
// DEFAULT-NEXT:     type @type[[TYPE16:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE__URC_NO_REASON]] _SC_ARG_MAX = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE__URC_FOREIGN_EXCEPTION_CAUGHT]] _SC_CHILD_MAX = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE__URC_FATAL_PHASE2_ERROR]] _SC_CLK_TCK = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE__URC_FATAL_PHASE1_ERROR]] _SC_NGROUPS_MAX = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE__URC_NORMAL_STOP]] _SC_OPEN_MAX = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE__URC_END_OF_STACK]] _SC_STREAM_MAX = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE__URC_HANDLER_FOUND]] _SC_TZNAME_MAX = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE__URC_INSTALL_CONTEXT]] _SC_JOB_CONTROL = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE__URC_CONTINUE_UNWIND]] _SC_SAVED_IDS = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE__SC_REALTIME_SIGNALS:[0-9]+]] _SC_REALTIME_SIGNALS = const<i32>(9);
// DEFAULT-NEXT:         %[[VALUE__SC_PRIORITY_SCHEDULING:[0-9]+]] _SC_PRIORITY_SCHEDULING = const<i32>(10);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMERS:[0-9]+]] _SC_TIMERS = const<i32>(11);
// DEFAULT-NEXT:         %[[VALUE__SC_ASYNCHRONOUS_IO:[0-9]+]] _SC_ASYNCHRONOUS_IO = const<i32>(12);
// DEFAULT-NEXT:         %[[VALUE__SC_PRIORITIZED_IO:[0-9]+]] _SC_PRIORITIZED_IO = const<i32>(13);
// DEFAULT-NEXT:         %[[VALUE__SC_SYNCHRONIZED_IO:[0-9]+]] _SC_SYNCHRONIZED_IO = const<i32>(14);
// DEFAULT-NEXT:         %[[VALUE__SC_FSYNC:[0-9]+]] _SC_FSYNC = const<i32>(15);
// DEFAULT-NEXT:         %[[VALUE__SC_MAPPED_FILES:[0-9]+]] _SC_MAPPED_FILES = const<i32>(16);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMLOCK:[0-9]+]] _SC_MEMLOCK = const<i32>(17);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMLOCK_RANGE:[0-9]+]] _SC_MEMLOCK_RANGE = const<i32>(18);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMORY_PROTECTION:[0-9]+]] _SC_MEMORY_PROTECTION = const<i32>(19);
// DEFAULT-NEXT:         %[[VALUE__SC_MESSAGE_PASSING:[0-9]+]] _SC_MESSAGE_PASSING = const<i32>(20);
// DEFAULT-NEXT:         %[[VALUE__SC_SEMAPHORES:[0-9]+]] _SC_SEMAPHORES = const<i32>(21);
// DEFAULT-NEXT:         %[[VALUE__SC_SHARED_MEMORY_OBJECTS:[0-9]+]] _SC_SHARED_MEMORY_OBJECTS = const<i32>(22);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_LISTIO_MAX:[0-9]+]] _SC_AIO_LISTIO_MAX = const<i32>(23);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_MAX:[0-9]+]] _SC_AIO_MAX = const<i32>(24);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_PRIO_DELTA_MAX:[0-9]+]] _SC_AIO_PRIO_DELTA_MAX = const<i32>(25);
// DEFAULT-NEXT:         %[[VALUE__SC_DELAYTIMER_MAX:[0-9]+]] _SC_DELAYTIMER_MAX = const<i32>(26);
// DEFAULT-NEXT:         %[[VALUE__SC_MQ_OPEN_MAX:[0-9]+]] _SC_MQ_OPEN_MAX = const<i32>(27);
// DEFAULT-NEXT:         %[[VALUE__SC_MQ_PRIO_MAX:[0-9]+]] _SC_MQ_PRIO_MAX = const<i32>(28);
// DEFAULT-NEXT:         %[[VALUE__SC_VERSION:[0-9]+]] _SC_VERSION = const<i32>(29);
// DEFAULT-NEXT:         %[[VALUE__SC_PAGESIZE:[0-9]+]] _SC_PAGESIZE = const<i32>(30);
// DEFAULT-NEXT:         %[[VALUE__SC_RTSIG_MAX:[0-9]+]] _SC_RTSIG_MAX = const<i32>(31);
// DEFAULT-NEXT:         %[[VALUE__SC_SEM_NSEMS_MAX:[0-9]+]] _SC_SEM_NSEMS_MAX = const<i32>(32);
// DEFAULT-NEXT:         %[[VALUE__SC_SEM_VALUE_MAX:[0-9]+]] _SC_SEM_VALUE_MAX = const<i32>(33);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGQUEUE_MAX:[0-9]+]] _SC_SIGQUEUE_MAX = const<i32>(34);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMER_MAX:[0-9]+]] _SC_TIMER_MAX = const<i32>(35);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_BASE_MAX:[0-9]+]] _SC_BC_BASE_MAX = const<i32>(36);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_DIM_MAX:[0-9]+]] _SC_BC_DIM_MAX = const<i32>(37);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_SCALE_MAX:[0-9]+]] _SC_BC_SCALE_MAX = const<i32>(38);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_STRING_MAX:[0-9]+]] _SC_BC_STRING_MAX = const<i32>(39);
// DEFAULT-NEXT:         %[[VALUE__SC_COLL_WEIGHTS_MAX:[0-9]+]] _SC_COLL_WEIGHTS_MAX = const<i32>(40);
// DEFAULT-NEXT:         %[[VALUE__SC_EQUIV_CLASS_MAX:[0-9]+]] _SC_EQUIV_CLASS_MAX = const<i32>(41);
// DEFAULT-NEXT:         %[[VALUE__SC_EXPR_NEST_MAX:[0-9]+]] _SC_EXPR_NEST_MAX = const<i32>(42);
// DEFAULT-NEXT:         %[[VALUE__SC_LINE_MAX:[0-9]+]] _SC_LINE_MAX = const<i32>(43);
// DEFAULT-NEXT:         %[[VALUE__SC_RE_DUP_MAX:[0-9]+]] _SC_RE_DUP_MAX = const<i32>(44);
// DEFAULT-NEXT:         %[[VALUE__SC_CHARCLASS_NAME_MAX:[0-9]+]] _SC_CHARCLASS_NAME_MAX = const<i32>(45);
// DEFAULT-NEXT:         %[[VALUE__SC_2_VERSION:[0-9]+]] _SC_2_VERSION = const<i32>(46);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_BIND:[0-9]+]] _SC_2_C_BIND = const<i32>(47);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_DEV:[0-9]+]] _SC_2_C_DEV = const<i32>(48);
// DEFAULT-NEXT:         %[[VALUE__SC_2_FORT_DEV:[0-9]+]] _SC_2_FORT_DEV = const<i32>(49);
// DEFAULT-NEXT:         %[[VALUE__SC_2_FORT_RUN:[0-9]+]] _SC_2_FORT_RUN = const<i32>(50);
// DEFAULT-NEXT:         %[[VALUE__SC_2_SW_DEV:[0-9]+]] _SC_2_SW_DEV = const<i32>(51);
// DEFAULT-NEXT:         %[[VALUE__SC_2_LOCALEDEF:[0-9]+]] _SC_2_LOCALEDEF = const<i32>(52);
// DEFAULT-NEXT:         %[[VALUE__SC_PII:[0-9]+]] _SC_PII = const<i32>(53);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_XTI:[0-9]+]] _SC_PII_XTI = const<i32>(54);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_SOCKET:[0-9]+]] _SC_PII_SOCKET = const<i32>(55);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET:[0-9]+]] _SC_PII_INTERNET = const<i32>(56);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI:[0-9]+]] _SC_PII_OSI = const<i32>(57);
// DEFAULT-NEXT:         %[[VALUE__SC_POLL:[0-9]+]] _SC_POLL = const<i32>(58);
// DEFAULT-NEXT:         %[[VALUE__SC_SELECT:[0-9]+]] _SC_SELECT = const<i32>(59);
// DEFAULT-NEXT:         %[[VALUE__SC_UIO_MAXIOV:[0-9]+]] _SC_UIO_MAXIOV = const<i32>(60);
// DEFAULT-NEXT:         %[[VALUE__SC_IOV_MAX:[0-9]+]] _SC_IOV_MAX = const<i32>(60);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET_STREAM:[0-9]+]] _SC_PII_INTERNET_STREAM = const<i32>(61);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET_DGRAM:[0-9]+]] _SC_PII_INTERNET_DGRAM = const<i32>(62);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_COTS:[0-9]+]] _SC_PII_OSI_COTS = const<i32>(63);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_CLTS:[0-9]+]] _SC_PII_OSI_CLTS = const<i32>(64);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_M:[0-9]+]] _SC_PII_OSI_M = const<i32>(65);
// DEFAULT-NEXT:         %[[VALUE__SC_T_IOV_MAX:[0-9]+]] _SC_T_IOV_MAX = const<i32>(66);
// DEFAULT-NEXT:         %[[VALUE__SC_THREADS:[0-9]+]] _SC_THREADS = const<i32>(67);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_SAFE_FUNCTIONS:[0-9]+]] _SC_THREAD_SAFE_FUNCTIONS = const<i32>(68);
// DEFAULT-NEXT:         %[[VALUE__SC_GETGR_R_SIZE_MAX:[0-9]+]] _SC_GETGR_R_SIZE_MAX = const<i32>(69);
// DEFAULT-NEXT:         %[[VALUE__SC_GETPW_R_SIZE_MAX:[0-9]+]] _SC_GETPW_R_SIZE_MAX = const<i32>(70);
// DEFAULT-NEXT:         %[[VALUE__SC_LOGIN_NAME_MAX:[0-9]+]] _SC_LOGIN_NAME_MAX = const<i32>(71);
// DEFAULT-NEXT:         %[[VALUE__SC_TTY_NAME_MAX:[0-9]+]] _SC_TTY_NAME_MAX = const<i32>(72);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_DESTRUCTOR_ITERATIONS:[0-9]+]] _SC_THREAD_DESTRUCTOR_ITERATIONS = const<i32>(73);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_KEYS_MAX:[0-9]+]] _SC_THREAD_KEYS_MAX = const<i32>(74);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_STACK_MIN:[0-9]+]] _SC_THREAD_STACK_MIN = const<i32>(75);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_THREADS_MAX:[0-9]+]] _SC_THREAD_THREADS_MAX = const<i32>(76);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ATTR_STACKADDR:[0-9]+]] _SC_THREAD_ATTR_STACKADDR = const<i32>(77);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ATTR_STACKSIZE:[0-9]+]] _SC_THREAD_ATTR_STACKSIZE = const<i32>(78);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIORITY_SCHEDULING:[0-9]+]] _SC_THREAD_PRIORITY_SCHEDULING = const<i32>(79);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIO_INHERIT:[0-9]+]] _SC_THREAD_PRIO_INHERIT = const<i32>(80);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIO_PROTECT:[0-9]+]] _SC_THREAD_PRIO_PROTECT = const<i32>(81);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PROCESS_SHARED:[0-9]+]] _SC_THREAD_PROCESS_SHARED = const<i32>(82);
// DEFAULT-NEXT:         %[[VALUE__SC_NPROCESSORS_CONF:[0-9]+]] _SC_NPROCESSORS_CONF = const<i32>(83);
// DEFAULT-NEXT:         %[[VALUE__SC_NPROCESSORS_ONLN:[0-9]+]] _SC_NPROCESSORS_ONLN = const<i32>(84);
// DEFAULT-NEXT:         %[[VALUE__SC_PHYS_PAGES:[0-9]+]] _SC_PHYS_PAGES = const<i32>(85);
// DEFAULT-NEXT:         %[[VALUE__SC_AVPHYS_PAGES:[0-9]+]] _SC_AVPHYS_PAGES = const<i32>(86);
// DEFAULT-NEXT:         %[[VALUE__SC_ATEXIT_MAX:[0-9]+]] _SC_ATEXIT_MAX = const<i32>(87);
// DEFAULT-NEXT:         %[[VALUE__SC_PASS_MAX:[0-9]+]] _SC_PASS_MAX = const<i32>(88);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_VERSION:[0-9]+]] _SC_XOPEN_VERSION = const<i32>(89);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XCU_VERSION:[0-9]+]] _SC_XOPEN_XCU_VERSION = const<i32>(90);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_UNIX:[0-9]+]] _SC_XOPEN_UNIX = const<i32>(91);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_CRYPT:[0-9]+]] _SC_XOPEN_CRYPT = const<i32>(92);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_ENH_I18N:[0-9]+]] _SC_XOPEN_ENH_I18N = const<i32>(93);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_SHM:[0-9]+]] _SC_XOPEN_SHM = const<i32>(94);
// DEFAULT-NEXT:         %[[VALUE__SC_2_CHAR_TERM:[0-9]+]] _SC_2_CHAR_TERM = const<i32>(95);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_VERSION:[0-9]+]] _SC_2_C_VERSION = const<i32>(96);
// DEFAULT-NEXT:         %[[VALUE__SC_2_UPE:[0-9]+]] _SC_2_UPE = const<i32>(97);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG2:[0-9]+]] _SC_XOPEN_XPG2 = const<i32>(98);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG3:[0-9]+]] _SC_XOPEN_XPG3 = const<i32>(99);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG4:[0-9]+]] _SC_XOPEN_XPG4 = const<i32>(100);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_BIT:[0-9]+]] _SC_CHAR_BIT = const<i32>(101);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_MAX:[0-9]+]] _SC_CHAR_MAX = const<i32>(102);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_MIN:[0-9]+]] _SC_CHAR_MIN = const<i32>(103);
// DEFAULT-NEXT:         %[[VALUE__SC_INT_MAX:[0-9]+]] _SC_INT_MAX = const<i32>(104);
// DEFAULT-NEXT:         %[[VALUE__SC_INT_MIN:[0-9]+]] _SC_INT_MIN = const<i32>(105);
// DEFAULT-NEXT:         %[[VALUE__SC_LONG_BIT:[0-9]+]] _SC_LONG_BIT = const<i32>(106);
// DEFAULT-NEXT:         %[[VALUE__SC_WORD_BIT:[0-9]+]] _SC_WORD_BIT = const<i32>(107);
// DEFAULT-NEXT:         %[[VALUE__SC_MB_LEN_MAX:[0-9]+]] _SC_MB_LEN_MAX = const<i32>(108);
// DEFAULT-NEXT:         %[[VALUE__SC_NZERO:[0-9]+]] _SC_NZERO = const<i32>(109);
// DEFAULT-NEXT:         %[[VALUE__SC_SSIZE_MAX:[0-9]+]] _SC_SSIZE_MAX = const<i32>(110);
// DEFAULT-NEXT:         %[[VALUE__SC_SCHAR_MAX:[0-9]+]] _SC_SCHAR_MAX = const<i32>(111);
// DEFAULT-NEXT:         %[[VALUE__SC_SCHAR_MIN:[0-9]+]] _SC_SCHAR_MIN = const<i32>(112);
// DEFAULT-NEXT:         %[[VALUE__SC_SHRT_MAX:[0-9]+]] _SC_SHRT_MAX = const<i32>(113);
// DEFAULT-NEXT:         %[[VALUE__SC_SHRT_MIN:[0-9]+]] _SC_SHRT_MIN = const<i32>(114);
// DEFAULT-NEXT:         %[[VALUE__SC_UCHAR_MAX:[0-9]+]] _SC_UCHAR_MAX = const<i32>(115);
// DEFAULT-NEXT:         %[[VALUE__SC_UINT_MAX:[0-9]+]] _SC_UINT_MAX = const<i32>(116);
// DEFAULT-NEXT:         %[[VALUE__SC_ULONG_MAX:[0-9]+]] _SC_ULONG_MAX = const<i32>(117);
// DEFAULT-NEXT:         %[[VALUE__SC_USHRT_MAX:[0-9]+]] _SC_USHRT_MAX = const<i32>(118);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_ARGMAX:[0-9]+]] _SC_NL_ARGMAX = const<i32>(119);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_LANGMAX:[0-9]+]] _SC_NL_LANGMAX = const<i32>(120);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_MSGMAX:[0-9]+]] _SC_NL_MSGMAX = const<i32>(121);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_NMAX:[0-9]+]] _SC_NL_NMAX = const<i32>(122);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_SETMAX:[0-9]+]] _SC_NL_SETMAX = const<i32>(123);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_TEXTMAX:[0-9]+]] _SC_NL_TEXTMAX = const<i32>(124);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_ILP32_OFF32:[0-9]+]] _SC_XBS5_ILP32_OFF32 = const<i32>(125);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_ILP32_OFFBIG:[0-9]+]] _SC_XBS5_ILP32_OFFBIG = const<i32>(126);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_LP64_OFF64:[0-9]+]] _SC_XBS5_LP64_OFF64 = const<i32>(127);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_LPBIG_OFFBIG:[0-9]+]] _SC_XBS5_LPBIG_OFFBIG = const<i32>(128);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_LEGACY:[0-9]+]] _SC_XOPEN_LEGACY = const<i32>(129);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_REALTIME:[0-9]+]] _SC_XOPEN_REALTIME = const<i32>(130);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_REALTIME_THREADS:[0-9]+]] _SC_XOPEN_REALTIME_THREADS = const<i32>(131);
// DEFAULT-NEXT:         %[[VALUE__SC_ADVISORY_INFO:[0-9]+]] _SC_ADVISORY_INFO = const<i32>(132);
// DEFAULT-NEXT:         %[[VALUE__SC_BARRIERS:[0-9]+]] _SC_BARRIERS = const<i32>(133);
// DEFAULT-NEXT:         %[[VALUE__SC_BASE:[0-9]+]] _SC_BASE = const<i32>(134);
// DEFAULT-NEXT:         %[[VALUE__SC_C_LANG_SUPPORT:[0-9]+]] _SC_C_LANG_SUPPORT = const<i32>(135);
// DEFAULT-NEXT:         %[[VALUE__SC_C_LANG_SUPPORT_R:[0-9]+]] _SC_C_LANG_SUPPORT_R = const<i32>(136);
// DEFAULT-NEXT:         %[[VALUE__SC_CLOCK_SELECTION:[0-9]+]] _SC_CLOCK_SELECTION = const<i32>(137);
// DEFAULT-NEXT:         %[[VALUE__SC_CPUTIME:[0-9]+]] _SC_CPUTIME = const<i32>(138);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_CPUTIME:[0-9]+]] _SC_THREAD_CPUTIME = const<i32>(139);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_IO:[0-9]+]] _SC_DEVICE_IO = const<i32>(140);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_SPECIFIC:[0-9]+]] _SC_DEVICE_SPECIFIC = const<i32>(141);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_SPECIFIC_R:[0-9]+]] _SC_DEVICE_SPECIFIC_R = const<i32>(142);
// DEFAULT-NEXT:         %[[VALUE__SC_FD_MGMT:[0-9]+]] _SC_FD_MGMT = const<i32>(143);
// DEFAULT-NEXT:         %[[VALUE__SC_FIFO:[0-9]+]] _SC_FIFO = const<i32>(144);
// DEFAULT-NEXT:         %[[VALUE__SC_PIPE:[0-9]+]] _SC_PIPE = const<i32>(145);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_ATTRIBUTES:[0-9]+]] _SC_FILE_ATTRIBUTES = const<i32>(146);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_LOCKING:[0-9]+]] _SC_FILE_LOCKING = const<i32>(147);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_SYSTEM:[0-9]+]] _SC_FILE_SYSTEM = const<i32>(148);
// DEFAULT-NEXT:         %[[VALUE__SC_MONOTONIC_CLOCK:[0-9]+]] _SC_MONOTONIC_CLOCK = const<i32>(149);
// DEFAULT-NEXT:         %[[VALUE__SC_MULTI_PROCESS:[0-9]+]] _SC_MULTI_PROCESS = const<i32>(150);
// DEFAULT-NEXT:         %[[VALUE__SC_SINGLE_PROCESS:[0-9]+]] _SC_SINGLE_PROCESS = const<i32>(151);
// DEFAULT-NEXT:         %[[VALUE__SC_NETWORKING:[0-9]+]] _SC_NETWORKING = const<i32>(152);
// DEFAULT-NEXT:         %[[VALUE__SC_READER_WRITER_LOCKS:[0-9]+]] _SC_READER_WRITER_LOCKS = const<i32>(153);
// DEFAULT-NEXT:         %[[VALUE__SC_SPIN_LOCKS:[0-9]+]] _SC_SPIN_LOCKS = const<i32>(154);
// DEFAULT-NEXT:         %[[VALUE__SC_REGEXP:[0-9]+]] _SC_REGEXP = const<i32>(155);
// DEFAULT-NEXT:         %[[VALUE__SC_REGEX_VERSION:[0-9]+]] _SC_REGEX_VERSION = const<i32>(156);
// DEFAULT-NEXT:         %[[VALUE__SC_SHELL:[0-9]+]] _SC_SHELL = const<i32>(157);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGNALS:[0-9]+]] _SC_SIGNALS = const<i32>(158);
// DEFAULT-NEXT:         %[[VALUE__SC_SPAWN:[0-9]+]] _SC_SPAWN = const<i32>(159);
// DEFAULT-NEXT:         %[[VALUE__SC_SPORADIC_SERVER:[0-9]+]] _SC_SPORADIC_SERVER = const<i32>(160);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_SPORADIC_SERVER:[0-9]+]] _SC_THREAD_SPORADIC_SERVER = const<i32>(161);
// DEFAULT-NEXT:         %[[VALUE__SC_SYSTEM_DATABASE:[0-9]+]] _SC_SYSTEM_DATABASE = const<i32>(162);
// DEFAULT-NEXT:         %[[VALUE__SC_SYSTEM_DATABASE_R:[0-9]+]] _SC_SYSTEM_DATABASE_R = const<i32>(163);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMEOUTS:[0-9]+]] _SC_TIMEOUTS = const<i32>(164);
// DEFAULT-NEXT:         %[[VALUE__SC_TYPED_MEMORY_OBJECTS:[0-9]+]] _SC_TYPED_MEMORY_OBJECTS = const<i32>(165);
// DEFAULT-NEXT:         %[[VALUE__SC_USER_GROUPS:[0-9]+]] _SC_USER_GROUPS = const<i32>(166);
// DEFAULT-NEXT:         %[[VALUE__SC_USER_GROUPS_R:[0-9]+]] _SC_USER_GROUPS_R = const<i32>(167);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS:[0-9]+]] _SC_2_PBS = const<i32>(168);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_ACCOUNTING:[0-9]+]] _SC_2_PBS_ACCOUNTING = const<i32>(169);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_LOCATE:[0-9]+]] _SC_2_PBS_LOCATE = const<i32>(170);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_MESSAGE:[0-9]+]] _SC_2_PBS_MESSAGE = const<i32>(171);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_TRACK:[0-9]+]] _SC_2_PBS_TRACK = const<i32>(172);
// DEFAULT-NEXT:         %[[VALUE__SC_SYMLOOP_MAX:[0-9]+]] _SC_SYMLOOP_MAX = const<i32>(173);
// DEFAULT-NEXT:         %[[VALUE__SC_STREAMS:[0-9]+]] _SC_STREAMS = const<i32>(174);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_CHECKPOINT:[0-9]+]] _SC_2_PBS_CHECKPOINT = const<i32>(175);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_ILP32_OFF32:[0-9]+]] _SC_V6_ILP32_OFF32 = const<i32>(176);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_ILP32_OFFBIG:[0-9]+]] _SC_V6_ILP32_OFFBIG = const<i32>(177);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_LP64_OFF64:[0-9]+]] _SC_V6_LP64_OFF64 = const<i32>(178);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_LPBIG_OFFBIG:[0-9]+]] _SC_V6_LPBIG_OFFBIG = const<i32>(179);
// DEFAULT-NEXT:         %[[VALUE__SC_HOST_NAME_MAX:[0-9]+]] _SC_HOST_NAME_MAX = const<i32>(180);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE:[0-9]+]] _SC_TRACE = const<i32>(181);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_EVENT_FILTER:[0-9]+]] _SC_TRACE_EVENT_FILTER = const<i32>(182);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_INHERIT:[0-9]+]] _SC_TRACE_INHERIT = const<i32>(183);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_LOG:[0-9]+]] _SC_TRACE_LOG = const<i32>(184);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_SIZE:[0-9]+]] _SC_LEVEL1_ICACHE_SIZE = const<i32>(185);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_ASSOC:[0-9]+]] _SC_LEVEL1_ICACHE_ASSOC = const<i32>(186);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_LINESIZE:[0-9]+]] _SC_LEVEL1_ICACHE_LINESIZE = const<i32>(187);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_SIZE:[0-9]+]] _SC_LEVEL1_DCACHE_SIZE = const<i32>(188);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_ASSOC:[0-9]+]] _SC_LEVEL1_DCACHE_ASSOC = const<i32>(189);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_LINESIZE:[0-9]+]] _SC_LEVEL1_DCACHE_LINESIZE = const<i32>(190);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_SIZE:[0-9]+]] _SC_LEVEL2_CACHE_SIZE = const<i32>(191);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_ASSOC:[0-9]+]] _SC_LEVEL2_CACHE_ASSOC = const<i32>(192);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL2_CACHE_LINESIZE = const<i32>(193);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_SIZE:[0-9]+]] _SC_LEVEL3_CACHE_SIZE = const<i32>(194);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_ASSOC:[0-9]+]] _SC_LEVEL3_CACHE_ASSOC = const<i32>(195);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL3_CACHE_LINESIZE = const<i32>(196);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_SIZE:[0-9]+]] _SC_LEVEL4_CACHE_SIZE = const<i32>(197);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_ASSOC:[0-9]+]] _SC_LEVEL4_CACHE_ASSOC = const<i32>(198);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL4_CACHE_LINESIZE = const<i32>(199);
// DEFAULT-NEXT:         %[[VALUE__SC_IPV6:[0-9]+]] _SC_IPV6 = const<i32>(235);
// DEFAULT-NEXT:         %[[VALUE__SC_RAW_SOCKETS:[0-9]+]] _SC_RAW_SOCKETS = const<i32>(236);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_ILP32_OFF32:[0-9]+]] _SC_V7_ILP32_OFF32 = const<i32>(237);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_ILP32_OFFBIG:[0-9]+]] _SC_V7_ILP32_OFFBIG = const<i32>(238);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_LP64_OFF64:[0-9]+]] _SC_V7_LP64_OFF64 = const<i32>(239);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_LPBIG_OFFBIG:[0-9]+]] _SC_V7_LPBIG_OFFBIG = const<i32>(240);
// DEFAULT-NEXT:         %[[VALUE__SC_SS_REPL_MAX:[0-9]+]] _SC_SS_REPL_MAX = const<i32>(241);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_EVENT_NAME_MAX:[0-9]+]] _SC_TRACE_EVENT_NAME_MAX = const<i32>(242);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_NAME_MAX:[0-9]+]] _SC_TRACE_NAME_MAX = const<i32>(243);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_SYS_MAX:[0-9]+]] _SC_TRACE_SYS_MAX = const<i32>(244);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_USER_EVENT_MAX:[0-9]+]] _SC_TRACE_USER_EVENT_MAX = const<i32>(245);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_STREAMS:[0-9]+]] _SC_XOPEN_STREAMS = const<i32>(246);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ROBUST_PRIO_INHERIT:[0-9]+]] _SC_THREAD_ROBUST_PRIO_INHERIT = const<i32>(247);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ROBUST_PRIO_PROTECT:[0-9]+]] _SC_THREAD_ROBUST_PRIO_PROTECT = const<i32>(248);
// DEFAULT-NEXT:         %[[VALUE__SC_MINSIGSTKSZ:[0-9]+]] _SC_MINSIGSTKSZ = const<i32>(249);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGSTKSZ:[0-9]+]] _SC_SIGSTKSZ = const<i32>(250);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_null:[0-9]+]] null: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_SEM_NSEMS_MAX]] @_Unwind_ForcedUnwind(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE__Unwind_Exception]]>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<fn(i32, @type[[TYPE1]], u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<void>) -> @type[[TYPE0]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_BC_SCALE_MAX]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_BC_STRING_MAX]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE__SC_EQUIV_CLASS_MAX]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE__SC_POLL]] @sigemptyset(%[[VALUE___set:[0-9]+]] __set: ptr<@type[[TYPE2]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PII_OSI_COTS]] @sigaction(%[[VALUE___sig:[0-9]+]] __sig: i32, %[[VALUE___act:[0-9]+]] __act: ptr<const @type[[TYPE_sigaction]]> [restrict], %[[VALUE___oact:[0-9]+]] __oact: ptr<@type[[TYPE_sigaction]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_THREAD_SAFE_FUNCTIONS]] @sigaltstack(%[[VALUE___ss:[0-9]+]] __ss: ptr<const @type[[TYPE15]]> [restrict], %[[VALUE___oss:[0-9]+]] __oss: ptr<@type[[TYPE15]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sysconf:[0-9]+]] @sysconf(%[[VALUE___name:[0-9]+]] __name: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_force_unwind_stop:[0-9]+]] @force_unwind_stop(%[[VALUE_version:[0-9]+]] version: i32, %[[VALUE_actions:[0-9]+]] actions: @type[[TYPE1]], %[[VALUE_exc_class:[0-9]+]] exc_class: u64, %[[VALUE_exc_obj:[0-9]+]] exc_obj: ptr<@type[[TYPE__Unwind_Exception]]>, %[[VALUE_context:[0-9]+]] context: ptr<@type[[TYPE__Unwind_Context]]>, %[[VALUE_stop_parameter:[0-9]+]] stop_parameter: ptr<void>) -> @type[[TYPE0]] [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u32>(and<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE1]]>(%[[VALUE_actions]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16))), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE__SC_BC_STRING_MAX]]);
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE0]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_force_unwind:[0-9]+]] @force_unwind() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_exc:[0-9]+]] exc: ptr<@type[[TYPE__Unwind_Exception]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE__Unwind_Exception]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE__SC_BC_SCALE_MAX]], const<u64>(32)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u64>>(field0(deref(read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]))))), const<i32>(0), const<u64>(8));
// DEFAULT-NEXT:         write<ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>>(field1(deref(read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]))), null<ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>>);
// DEFAULT-NEXT:         call<@type[[TYPE0]], signature=fn(ptr<@type[[TYPE__Unwind_Exception]]>, ptr<fn(i32, @type[[TYPE1]], u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>, ptr<void>) -> @type[[TYPE0]]>(%[[VALUE__SC_SEM_NSEMS_MAX]], read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]), function_decay<ptr<fn(i32, @type[[TYPE1]], u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>>(%[[VALUE_force_unwind_stop]]), null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE__SC_BC_STRING_MAX]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_counter:[0-9]+]] @counter(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_handler:[0-9]+]] @handler(%[[VALUE_p_2:[0-9]+]] p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE__SC_BC_STRING_MAX]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE__SC_EQUIV_CLASS_MAX]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5:[0-9]+]] @fn5() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_dummy:[0-9]+]] dummy: i8 [storage=automatic] [cleanup=%[[VALUE_counter]]];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_force_unwind]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(%[[VALUE_sig:[0-9]+]] sig: i32, %[[VALUE_info:[0-9]+]] info: ptr<@type[[TYPE3]]>, %[[VALUE_ctx:[0-9]+]] ctx: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_dummy_2:[0-9]+]] dummy: i8 [storage=automatic] [cleanup=%[[VALUE_counter]]];
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_fn5]]);
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_null]], null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE__SC_BC_STRING_MAX]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_null]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fn3]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ss:[0-9]+]] ss: @type[[TYPE15]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_sigaction]] [storage=automatic];
// DEFAULT-NEXT:         write<u64>(field2(%[[VALUE_ss]]), reinterpret<u64, reason=assign, fits=unknown>(mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(const<i32>(4)), call<i64, signature=fn(i32) -> i64>(%[[VALUE_sysconf]], const<i32>(30)))));
// DEFAULT-NEXT:         reinterpret<u64, reason=assign, fits=unknown>(mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(const<i32>(4)), call<i64, signature=fn(i32) -> i64>(%[[VALUE_sysconf]], const<i32>(30))));
// DEFAULT-NEXT:         if lt<u64>(read<u64>(field2(%[[VALUE_ss]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8192))))
// DEFAULT-NEXT:             write<u64>(field2(%[[VALUE_ss]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(8192))));
// DEFAULT-NEXT:         write<ptr<void>>(field0(%[[VALUE_ss]]), call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE__SC_BC_SCALE_MAX]], read<u64>(field2(%[[VALUE_ss]]))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE__SC_BC_SCALE_MAX]], read<u64>(field2(%[[VALUE_ss]])));
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(field0(%[[VALUE_ss]])), null<ptr<void>>)
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE__SC_EQUIV_CLASS_MAX]], const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_ss]]), const<i32>(0));
// DEFAULT-NEXT:         if lt<i32>(call<i32, signature=fn(ptr<const @type[[TYPE15]]>, ptr<@type[[TYPE15]]>) -> i32>(%[[VALUE__SC_THREAD_SAFE_FUNCTIONS]], pointer_cast<ptr<const @type[[TYPE15]]>, reason=arg>(addr_of<ptr<@type[[TYPE15]]>>(%[[VALUE_ss]])), null<ptr<@type[[TYPE15]]>>), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE__SC_EQUIV_CLASS_MAX]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE2]]>) -> i32>(%[[VALUE__SC_POLL]], addr_of<ptr<@type[[TYPE2]]>>(field1(%[[VALUE_s]])));
// DEFAULT-NEXT:         write<ptr<fn(i32, ptr<@type[[TYPE3]]>, ptr<void>) -> void>>(field1(field0(%[[VALUE_s]])), function_decay<ptr<fn(i32, ptr<@type[[TYPE3]]>, ptr<void>) -> void>>(%[[VALUE_fn4]]));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_s]]), reinterpret<i32, reason=assign, fits=unknown>(or<u32>(or<u32>(const<u32>(2147483648), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(134217728))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type[[TYPE_sigaction]]>, ptr<@type[[TYPE_sigaction]]>) -> i32>(%[[VALUE__SC_PII_OSI_COTS]], const<i32>(11), pointer_cast<ptr<const @type[[TYPE_sigaction]]>, reason=arg>(addr_of<ptr<@type[[TYPE_sigaction]]>>(%[[VALUE_s]])), null<ptr<@type[[TYPE_sigaction]]>>);
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type[[TYPE_sigaction]]>, ptr<@type[[TYPE_sigaction]]>) -> i32>(%[[VALUE__SC_PII_OSI_COTS]], const<i32>(7), pointer_cast<ptr<const @type[[TYPE_sigaction]]>, reason=arg>(addr_of<ptr<@type[[TYPE_sigaction]]>>(%[[VALUE_s]])), null<ptr<@type[[TYPE_sigaction]]>>);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_fn2]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn0:[0-9]+]] @fn0() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_dummy_3:[0-9]+]] dummy: i8 [storage=automatic] [cleanup=%[[VALUE_handler]]];
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_fn1]]);
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_null]], null<ptr<i8>>);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_fn0]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE__SC_BC_STRING_MAX]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
