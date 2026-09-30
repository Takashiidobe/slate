/* This test needs to use setrlimit to set the stack size, so it can
   only run on Unix.  */
/* { dg-do run { target *-*-linux* *-*-gnu* *-*-solaris* *-*-darwin* } } */
/* { dg-require-effective-target split_stack } */
/* { dg-options "-fsplit-stack" } */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>

/* Use a noinline function to ensure that the buffer is not removed
   from the stack.  */
static void use_buffer (char *buf, size_t) __attribute__ ((noinline));
static void
use_buffer (char *buf, size_t c)
{
  size_t i;

  for (i = 0; i < c; ++i)
    buf[i] = (char) i;
}

/* Each recursive call uses 10 * i bytes.  We call it 1000 times,
   using a total of 5,000,000 bytes.  If -fsplit-stack is not working,
   that will overflow our stack limit.  */

static void
down1 (int i)
{
  char buf[10 * i];

  if (i > 0)
    {
      use_buffer (buf, 10 * i);
      down1 (i - 1);
    }
}

/* Same thing, using alloca.  */

static void
down2 (int i)
{
  char *buf = alloca (10 * i);

  if (i > 0)
    {
      use_buffer (buf, 10 * i);
      down2 (i - 1);
    }
}

int
main (void)
{
  struct rlimit r;

  /* We set a stack limit because we are usually invoked via make, and
     make sets the stack limit to be as large as possible.  */
  r.rlim_cur = 8192 * 1024;
  r.rlim_max = 8192 * 1024;
  if (setrlimit (RLIMIT_STACK, &r) != 0)
    abort ();
  down1 (1000);
  down2 (1000);
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___rlim_t:[0-9]+]] __rlim_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___rlimit_resource:[0-9]+]] __rlimit_resource = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_RLIMIT_CPU:[0-9]+]] RLIMIT_CPU = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_RLIMIT_FSIZE:[0-9]+]] RLIMIT_FSIZE = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_RLIMIT_DATA:[0-9]+]] RLIMIT_DATA = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_RLIMIT_STACK:[0-9]+]] RLIMIT_STACK = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_RLIMIT_CORE:[0-9]+]] RLIMIT_CORE = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_RSS:[0-9]+]] __RLIMIT_RSS = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE_RLIMIT_NOFILE:[0-9]+]] RLIMIT_NOFILE = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_OFILE:[0-9]+]] __RLIMIT_OFILE = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE_RLIMIT_AS:[0-9]+]] RLIMIT_AS = const<i32>(9);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_NPROC:[0-9]+]] __RLIMIT_NPROC = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_MEMLOCK:[0-9]+]] __RLIMIT_MEMLOCK = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_LOCKS:[0-9]+]] __RLIMIT_LOCKS = const<i32>(10);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_SIGPENDING:[0-9]+]] __RLIMIT_SIGPENDING = const<i32>(11);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_MSGQUEUE:[0-9]+]] __RLIMIT_MSGQUEUE = const<i32>(12);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_NICE:[0-9]+]] __RLIMIT_NICE = const<i32>(13);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_RTPRIO:[0-9]+]] __RLIMIT_RTPRIO = const<i32>(14);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_RTTIME:[0-9]+]] __RLIMIT_RTTIME = const<i32>(15);
// DEFAULT-NEXT:         %[[VALUE___RLIMIT_NLIMITS:[0-9]+]] __RLIMIT_NLIMITS = const<i32>(16);
// DEFAULT-NEXT:         %[[VALUE___RLIM_NLIMITS:[0-9]+]] __RLIM_NLIMITS = const<i32>(16);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_rlim_t:[0-9]+]] rlim_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_rlimit:[0-9]+]] rlimit = struct {
// DEFAULT-NEXT:         field0 rlim_cur: u64;
// DEFAULT-NEXT:         field1 rlim_max: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE___rlimit_resource_t:[0-9]+]] __rlimit_resource_t = i32;
// DEFAULT-NEXT:     fn %[[VALUE_RLIMIT_DATA]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_setrlimit:[0-9]+]] @setrlimit(%[[VALUE___resource:[0-9]+]] __resource: i32, %[[VALUE___rlimits:[0-9]+]] __rlimits: ptr<const @type[[TYPE_rlimit]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_use_buffer:[0-9]+]] @use_buffer(%[[VALUE_buf:[0-9]+]] buf: ptr<i8>, %[[VALUE_c:[0-9]+]] c: u64) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u64 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_i]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_i]]), read<u64>(%[[VALUE_c]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_i]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_buf]]), read<u64>(%[[VALUE_i]]))), reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(read<u64>(%[[VALUE_i]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_down1:[0-9]+]] @down1(%[[VALUE_i_2:[0-9]+]] i: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(mul<i32, overflow=ub>(const<i32>(10), read<i32>(%[[VALUE_i_2]]))));
// DEFAULT-NEXT:         let %[[VALUE_buf_2:[0-9]+]] buf: vla<i8, %[[VALUE3]]> [storage=automatic];
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<i8>, u64) -> void>(%[[VALUE_use_buffer]], array_decay<ptr<i8>, length=None>(%[[VALUE_buf_2]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(const<i32>(10), read<i32>(%[[VALUE_i_2]])))));
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_down1]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE4:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_down2:[0-9]+]] @down2(%[[VALUE_i_3:[0-9]+]] i: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf_3:[0-9]+]] buf: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(const<i32>(10), read<i32>(%[[VALUE_i_3]]))))));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<i8>, u64) -> void>(%[[VALUE_use_buffer]], read<ptr<i8>>(%[[VALUE_buf_3]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(mul<i32, overflow=ub>(const<i32>(10), read<i32>(%[[VALUE_i_3]])))));
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_down2]], sub<i32, overflow=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_rlimit]] [storage=automatic];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_r]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(mul<i32, overflow=ub>(const<i32>(8192), const<i32>(1024)))));
// DEFAULT-NEXT:         write<u64>(field1(%[[VALUE_r]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(mul<i32, overflow=ub>(const<i32>(8192), const<i32>(1024)))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<const @type[[TYPE_rlimit]]>) -> i32>(%[[VALUE_setrlimit]], const<i32>(3), pointer_cast<ptr<const @type[[TYPE_rlimit]]>, reason=arg>(addr_of<ptr<@type[[TYPE_rlimit]]>>(%[[VALUE_r]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_RLIMIT_DATA]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_down1]], const<i32>(1000));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_down2]], const<i32>(1000));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
