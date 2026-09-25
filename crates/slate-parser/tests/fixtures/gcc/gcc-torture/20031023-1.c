// SLATE-FILECHECK-DEFINES DEFAULT

/* Declaration of the frame size doesn't work on ptx.  */
/* { dg-require-effective-target untyped_assembly } */
/* { dg-require-effective-target indirect_calls } */

#ifndef ASIZE
# define ASIZE 0x10000000000UL
#endif

#include <limits.h>

#if LONG_MAX < 8 * ASIZE
# undef ASIZE
# define ASIZE 4096
#endif

extern void abort (void);

int __attribute__((noinline))
foo (const char *s)
{
  if (!s)
    return 1;
  if (s[0] != 'a')
    abort ();
  s += ASIZE - 1;
  if (s[0] != 'b')
    abort ();
  return 0;
}

int (*fn) (const char *) = foo;

int __attribute__((noinline))
bar (void)
{
  char s[ASIZE];
  s[0] = 'a';
  s[ASIZE - 1] = 'b';
  foo (s);
  foo (s);
  return 0;
}

int __attribute__((noinline))
baz (long i)
{
  if (i)
    return fn (0);
  else
    {
      char s[ASIZE];
      s[0] = 'a';
      s[ASIZE - 1] = 'b';
      foo (s);
      foo (s);
      return fn (0);
    }
}

int
main (void)
{
  if (bar ())
    abort ();
  if (baz (0) != 1)
    abort ();
  if (baz (1) != 1)
    abort ();
  return 0;
}

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
// DEFAULT-NEXT:     global %3 fn: ptr<fn(ptr<const i8>) -> i32> [storage=static] = function_decay<ptr<fn(ptr<const i8>) -> i32>>(%1) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 s: ptr<const i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%2), null<ptr<const i8>>))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%2), const<i32>(0))))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %10: ptr<const i8> [synthetic] = read<ptr<const i8>>(%2);
// DEFAULT-NEXT:         let %11: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%10), sub<u64, overflow=wrap>(const<u64>(1099511627776), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(%2, read<ptr<const i8>>(%11));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%2), const<i32>(0))))), const<i32>(98))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 s: array<i8, 1099511627776> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1099511627776)>(%5), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1099511627776)>(%5), sub<u64, overflow=wrap>(const<u64>(1099511627776), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), truncate<i8, reason=assign, fits=always>(const<i32>(98)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1099511627776)>(%5)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1099511627776)>(%5)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @baz(%7 i: i64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%7), const<i64>(0))
// DEFAULT-NEXT:             return call<i32, signature=fn(ptr<const i8>) -> i32>(read<ptr<fn(ptr<const i8>) -> i32>>(%3), null<ptr<const i8>>);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %8 s: array<i8, 1099511627776> [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1099511627776)>(%8), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1099511627776)>(%8), sub<u64, overflow=wrap>(const<u64>(1099511627776), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), truncate<i8, reason=assign, fits=always>(const<i32>(98)));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1099511627776)>(%8)));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1099511627776)>(%8)));
// DEFAULT-NEXT:                 return call<i32, signature=fn(ptr<const i8>) -> i32>(read<ptr<fn(ptr<const i8>) -> i32>>(%3), null<ptr<const i8>>);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%6, widen<i64, reason=arg>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%6, widen<i64, reason=arg>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
