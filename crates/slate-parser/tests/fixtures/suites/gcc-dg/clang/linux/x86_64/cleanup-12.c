/* PR middle-end/32758 */
/* HP-UX libunwind.so doesn't provide _UA_END_OF_STACK */
/* { dg-do run } */
/* { dg-options "-O2 -fexceptions" } */
/* { dg-skip-if "" { "ia64-*-hpux11.*" } } */
/* { dg-require-effective-target exceptions } */
/* Verify unwind info in presence of alloca.  */

#include <unwind.h>
#include <stdlib.h>
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

static void force_unwind (void)
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

__attribute__((noinline))
void foo (void *x __attribute__((unused)))
{
  force_unwind ();
}

__attribute__((noinline))
int bar (unsigned int x)
{
  void *y = __builtin_alloca (x);
  foo (y);
  return 1;
}

static void handler (void *p __attribute__((unused)))
{
  exit (0);
}

__attribute__((noinline))
static void doit ()
{
  char dummy __attribute__((cleanup (handler)));
  bar (1024);
}

int main ()
{
  doit ();
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
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
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
// DEFAULT-NEXT:     fn %[[VALUE__Unwind_ForcedUnwind:[0-9]+]] @_Unwind_ForcedUnwind(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE__Unwind_Exception]]>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<fn(i32, @type[[TYPE1]], u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<void>) -> @type[[TYPE0]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_force_unwind_stop:[0-9]+]] @force_unwind_stop(%[[VALUE_version:[0-9]+]] version: i32, %[[VALUE_actions:[0-9]+]] actions: @type[[TYPE1]], %[[VALUE_exc_class:[0-9]+]] exc_class: u64, %[[VALUE_exc_obj:[0-9]+]] exc_obj: ptr<@type[[TYPE__Unwind_Exception]]>, %[[VALUE_context:[0-9]+]] context: ptr<@type[[TYPE__Unwind_Context]]>, %[[VALUE_stop_parameter:[0-9]+]] stop_parameter: ptr<void>) -> @type[[TYPE0]] [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u32>(and<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE1]]>(%[[VALUE_actions]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16))), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE0]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_force_unwind:[0-9]+]] @force_unwind() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_exc:[0-9]+]] exc: ptr<@type[[TYPE__Unwind_Exception]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE__Unwind_Exception]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(32)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u64>>(field0(deref(read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]))))), const<i32>(0), const<u64>(8));
// DEFAULT-NEXT:         write<ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>>(field1(deref(read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]))), null<ptr<fn(@type[[TYPE0]], ptr<@type[[TYPE__Unwind_Exception]]>) -> void>>);
// DEFAULT-NEXT:         call<@type[[TYPE0]], signature=fn(ptr<@type[[TYPE__Unwind_Exception]]>, ptr<fn(i32, @type[[TYPE1]], u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>, ptr<void>) -> @type[[TYPE0]]>(%[[VALUE__Unwind_ForcedUnwind]], read<ptr<@type[[TYPE__Unwind_Exception]]>>(%[[VALUE_exc]]), function_decay<ptr<fn(i32, @type[[TYPE1]], u64, ptr<@type[[TYPE__Unwind_Exception]]>, ptr<@type[[TYPE__Unwind_Context]]>, ptr<void>) -> @type[[TYPE0]]>>(%[[VALUE_force_unwind_stop]]), null<ptr<void>>);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<void>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_force_unwind]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], widen<u64, reason=arg>(read<u32>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_foo]], read<ptr<void>>(%[[VALUE_y]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_handler:[0-9]+]] @handler(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_doit:[0-9]+]] @doit() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_dummy:[0-9]+]] dummy: i8 [storage=automatic] [cleanup=%[[VALUE_handler]]];
// DEFAULT-NEXT:         call<i32, signature=fn(u32) -> i32>(%[[VALUE_bar]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1024)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_doit]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
