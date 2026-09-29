/* { dg-do run { target hppa*-*-hpux* *-*-linux* *-*-gnu* powerpc*-*-darwin* *-*-darwin[912]* *-*-uclinux* } } */
/* { dg-options "-fexceptions -fnon-call-exceptions -O2" } */
/* { dg-require-effective-target exceptions } */
/* Verify that cleanups work with exception handling through realtime
   signal frames.  */

#include <unwind.h>
#include <stdlib.h>
#include <signal.h>
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
  struct sigaction s;
  sigemptyset (&s.sa_mask);
  s.sa_sigaction = fn4;
  s.sa_flags = SA_RESETHAND | SA_SIGINFO;
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
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Word:[0-9]+]] _Unwind_Word = u64;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Exception_Class:[0-9]+]] _Unwind_Exception_Class = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
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
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Exception:[0-9]+]] _Unwind_Exception = struct {
// DEFAULT-NEXT:         field0 exception_class: u64;
// DEFAULT-NEXT:         field1 exception_cleanup: ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>;
// DEFAULT-NEXT:         field2 private_1: u64;
// DEFAULT-NEXT:         field3 private_2: u64;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Exception_Cleanup_Fn:[0-9]+]] _Unwind_Exception_Cleanup_Fn = ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Action:[0-9]+]] _Unwind_Action = i32;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Context:[0-9]+]] _Unwind_Context = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__Unwind_Stop_Fn:[0-9]+]] _Unwind_Stop_Fn = ptr<fn(i32, i32, u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>;
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uid_t:[0-9]+]] __uid_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___pid_t:[0-9]+]] __pid_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___clock_t:[0-9]+]] __clock_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __val: array<u64, 16>;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___sigset_t:[0-9]+]] __sigset_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE_sigset_t:[0-9]+]] sigset_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE_sigval:[0-9]+]] sigval = union {
// DEFAULT-NEXT:         field0 sival_int: i32;
// DEFAULT-NEXT:         field1 sival_ptr: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE___sigval_t:[0-9]+]] __sigval_t = @type[[TYPE_sigval]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 si_signo: i32;
// DEFAULT-NEXT:         field1 si_errno: i32;
// DEFAULT-NEXT:         field2 si_code: i32;
// DEFAULT-NEXT:         field3 __pad0: i32;
// DEFAULT-NEXT:         field4 _sifields: @type[[TYPE3:[0-9]+]];
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE3]] = union {
// DEFAULT-NEXT:         field0 _pad: array<i32, 28>;
// DEFAULT-NEXT:         field1 _kill: @type[[TYPE4:[0-9]+]];
// DEFAULT-NEXT:         field2 _timer: @type[[TYPE5:[0-9]+]];
// DEFAULT-NEXT:         field3 _rt: @type[[TYPE6:[0-9]+]];
// DEFAULT-NEXT:         field4 _sigchld: @type[[TYPE7:[0-9]+]];
// DEFAULT-NEXT:         field5 _sigfault: @type[[TYPE8:[0-9]+]];
// DEFAULT-NEXT:         field6 _sigpoll: @type[[TYPE11:[0-9]+]];
// DEFAULT-NEXT:         field7 _sigsys: @type[[TYPE12:[0-9]+]];
// DEFAULT-NEXT:     } [size=112, align=8, offsets=[0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE4]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE5]] = struct {
// DEFAULT-NEXT:         field0 si_tid: i32;
// DEFAULT-NEXT:         field1 si_overrun: i32;
// DEFAULT-NEXT:         field2 si_sigval: @type[[TYPE_sigval]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE6]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:         field2 si_sigval: @type[[TYPE_sigval]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE7]] = struct {
// DEFAULT-NEXT:         field0 si_pid: i32;
// DEFAULT-NEXT:         field1 si_uid: u32;
// DEFAULT-NEXT:         field2 si_status: i32;
// DEFAULT-NEXT:         field3 si_utime: i64;
// DEFAULT-NEXT:         field4 si_stime: i64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 4, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE8]] = struct {
// DEFAULT-NEXT:         field0 si_addr: ptr<void>;
// DEFAULT-NEXT:         field1 si_addr_lsb: i16;
// DEFAULT-NEXT:         field2 _bounds: @type[[TYPE9:[0-9]+]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE9]] = union {
// DEFAULT-NEXT:         field0 _addr_bnd: @type[[TYPE10:[0-9]+]];
// DEFAULT-NEXT:         field1 _pkey: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE10]] = struct {
// DEFAULT-NEXT:         field0 _lower: ptr<void>;
// DEFAULT-NEXT:         field1 _upper: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE11]] = struct {
// DEFAULT-NEXT:         field0 si_band: i64;
// DEFAULT-NEXT:         field1 si_fd: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE12]] = struct {
// DEFAULT-NEXT:         field0 _call_addr: ptr<void>;
// DEFAULT-NEXT:         field1 _syscall: i32;
// DEFAULT-NEXT:         field2 _arch: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_siginfo_t:[0-9]+]] siginfo_t = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE___sighandler_t:[0-9]+]] __sighandler_t = ptr<fn(i32) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_sigaction:[0-9]+]] sigaction = struct {
// DEFAULT-NEXT:         field0 __sigaction_handler: @type[[TYPE13:[0-9]+]];
// DEFAULT-NEXT:         field1 sa_mask: @type[[TYPE1]];
// DEFAULT-NEXT:         field2 sa_flags: i32;
// DEFAULT-NEXT:         field3 sa_restorer: ptr<fn() -> void>;
// DEFAULT-NEXT:     } [size=152, align=8, offsets=[0, 8, 136, 144]];
// DEFAULT-NEXT:     type @type[[TYPE13]] = union {
// DEFAULT-NEXT:         field0 sa_handler: ptr<fn(i32) -> void>;
// DEFAULT-NEXT:         field1 sa_sigaction: ptr<fn(i32, ptr<@type[[TYPE2]]>, ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_null:[0-9]+]] null: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__Unwind_ForcedUnwind:[0-9]+]] @_Unwind_ForcedUnwind(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE__Unwind_Exception]]>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<fn(i32, i32, u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<void>) -> @type[[TYPE0]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_sigemptyset:[0-9]+]] @sigemptyset(%[[VALUE___set:[0-9]+]] __set: ptr<@type[[TYPE1]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sigaction:[0-9]+]] @sigaction(%[[VALUE___sig:[0-9]+]] __sig: i32, %[[VALUE___act:[0-9]+]] __act: ptr<const @type[[TYPE_sigaction]]> [restrict], %[[VALUE___oact:[0-9]+]] __oact: ptr<@type[[TYPE_sigaction]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_force_unwind_stop:[0-9]+]] @force_unwind_stop(%[[VALUE_version:[0-9]+]] version: i32, %[[VALUE_actions:[0-9]+]] actions: i32, %[[VALUE_exc_class:[0-9]+]] exc_class: u64, %[[VALUE_exc_obj:[0-9]+]] exc_obj: ptr<@type[[TYPE__Unwind_Exception]]>, %[[VALUE_context:[0-9]+]] context: ptr<@type[[TYPE__Unwind_Context]]>, %[[VALUE_stop_parameter:[0-9]+]] stop_parameter: ptr<void>) -> @type[[TYPE0]] [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(%[[VALUE_actions]]), const<i32>(16)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE0]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_force_unwind:[0-9]+]] @force_unwind() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_exc:[0-9]+]] exc: ptr<@type[[TYPE__Unwind_Exception]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE__Unwind_Exception]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(32)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u64>>(field0(deref(read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]))))), const<i32>(0), const<u64>(8));
// DEFAULT-NEXT:         write<ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>>(field1(deref(read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]))), null<ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>>);
// DEFAULT-NEXT:         call<@type[[TYPE0]], signature=fn(ptr<@type[[TYPE__Unwind_Exception]]>, ptr<fn(i32, i32, u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>, ptr<void>) -> @type[[TYPE0]]>(%[[VALUE__Unwind_ForcedUnwind]], read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]), function_decay<ptr<fn(i32, i32, u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>>(%[[VALUE_force_unwind_stop]]), null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_counter:[0-9]+]] @counter(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_handler:[0-9]+]] @handler(%[[VALUE_p_2:[0-9]+]] p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5:[0-9]+]] @fn5() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_dummy:[0-9]+]] dummy: i8 [storage=automatic] [cleanup=%[[VALUE_counter]]];
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_force_unwind]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(%[[VALUE_sig:[0-9]+]] sig: i32, %[[VALUE_info:[0-9]+]] info: ptr<@type[[TYPE2]]>, %[[VALUE_ctx:[0-9]+]] ctx: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_dummy_2:[0-9]+]] dummy: i8 [storage=automatic] [cleanup=%[[VALUE_counter]]];
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_fn5]]);
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_null]], null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_null]])), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fn3]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_sigaction]] [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE1]]>) -> i32>(%[[VALUE_sigemptyset]], addr_of<ptr<@type[[TYPE1]]>>(field1(%[[VALUE_s]])));
// DEFAULT-NEXT:         write<ptr<fn(i32, ptr<@type[[TYPE2]]>, ptr<void>) -> void>>(field1(field0(%[[VALUE_s]])), function_decay<ptr<fn(i32, ptr<@type[[TYPE2]]>, ptr<void>) -> void>>(%[[VALUE_fn4]]));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_s]]), reinterpret<i32, reason=assign, fits=unknown>(or<u32>(const<u32>(2147483648), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)))));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type[[TYPE_sigaction]]>, ptr<@type[[TYPE_sigaction]]>) -> i32>(%[[VALUE_sigaction]], const<i32>(11), pointer_cast<ptr<const @type[[TYPE_sigaction]]>, reason=arg>(addr_of<ptr<@type[[TYPE_sigaction]]>>(%[[VALUE_s]])), null<ptr<@type[[TYPE_sigaction]]>>);
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<const @type[[TYPE_sigaction]]>, ptr<@type[[TYPE_sigaction]]>) -> i32>(%[[VALUE_sigaction]], const<i32>(7), pointer_cast<ptr<const @type[[TYPE_sigaction]]>, reason=arg>(addr_of<ptr<@type[[TYPE_sigaction]]>>(%[[VALUE_s]])), null<ptr<@type[[TYPE_sigaction]]>>);
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
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
