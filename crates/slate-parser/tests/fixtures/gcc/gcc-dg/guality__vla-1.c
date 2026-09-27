/* PR debug/43150 */
/* { dg-do run } */
/* { dg-options "-g" } */

void __attribute__((noinline)) bar(short *p) {
  __builtin_memset(p, '\0', 17 * sizeof(*p));
  asm volatile("" : : "r"(p) : "memory");
}

int __attribute__((noinline)) f1(int i) {
  char a[i + 1];
  a[0] = 5;    /* { dg-final { gdb-test .+1 "i" "5" } } */
  return a[0]; /* { dg-final { gdb-test . "sizeof (a)" "6" } } */
}

int __attribute__((noinline)) f2(int i) {
  short a
      [i * 2 +
       7]; /* { dg-final { gdb-test .+1 "i" "5" { xfail { aarch64*-*-* && { any-opts "-fno-fat-lto-objects" } } } } } */
  bar(a); /* { dg-final { gdb-test . "sizeof (a)" "17 * sizeof (short)" { xfail { aarch64*-*-* && { any-opts "-fno-fat-lto-objects" } } } } } */
  return a[i + 4];
}

int
main() {
  volatile int j;
  int          i = 5;
  asm volatile("" : "=r"(i) : "0"(i));
  j = f1(i);
  f2(i);
  return 0;
}



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
// DEFAULT-NEXT:     fn %14 @__builtin_memset(%11 <unnamed>: ptr<void>, %12 <unnamed>: i32, %13 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %0 @bar(%1 p: ptr<i16>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(read<ptr<i16>>(%1)), const<i32>(0), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(17))), const<u64>(2)));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "r" [reg] read<ptr<i16>>(%1);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @f1(%3 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%3), const<i32>(1))));
// DEFAULT-NEXT:         let %4 a: vla<i8, %15> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%4), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(%4), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f2(%6 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%6), const<i32>(2)), const<i32>(7))));
// DEFAULT-NEXT:         let %7 a: vla<i16, %16> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i16>) -> void>(%0, array_decay<ptr<i16>, length=None>(%7));
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=None>(%7), add<i32, overflow=ub>(read<i32>(%6), const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 j: volatile i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] place<i32>(%10) from read<i32>(%10);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32, volatile>(%9, call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%10)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%2, read<i32>(%10));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%5, read<i32>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
