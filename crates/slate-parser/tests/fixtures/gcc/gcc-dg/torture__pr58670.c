/* PR middle-end/58670 */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */

#if defined(__i386__) || defined(__x86_64__)
#define ASM_STR "btsl $1, %0; jc %l[lab]"
#endif

__attribute__((noinline, noclone)) int foo(int a, int b) {
  if (a)
    return -3;
#ifdef ASM_STR
  asm volatile goto(ASM_STR : : "m"(b) : "memory" : lab);
  return 0;
lab:
#endif
  return 0;
}

int bar(int a, int b) {
  if (a)
    return -3;
#ifdef ASM_STR
  asm volatile goto(ASM_STR : : "m"(b) : "memory" : lab);
  return 0;
lab:
#endif
  return 0;
}

int
main() {
  if (foo(1, 0) != -3 || foo(0, 3) != 0 || foo(1, 0) != -3 || foo(0, 0) != 0 ||
      bar(1, 0) != -3 || bar(0, 3) != 0 || bar(1, 0) != -3 || bar(0, 0) != 0)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %0 @foo(%2 a: i32, %3 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(3));
// DEFAULT-NEXT:         asm volatile goto "btsl $1, %0; jc %l[lab]" {
// DEFAULT-NEXT:             template: "btsl $1, " %0 "; jc " %l0;
// DEFAULT-NEXT:             in 0 "m" read<i32>(%3);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:             labels: %1;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:         label %1 lab:
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%6 a: i32, %7 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(3));
// DEFAULT-NEXT:         asm volatile goto "btsl $1, %0; jc %l[lab]" {
// DEFAULT-NEXT:             template: "btsl $1, " %0 "; jc " %l0;
// DEFAULT-NEXT:             in 0 "m" read<i32>(%7);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:             labels: %5;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:         label %5 lab:
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%0, const<i32>(1), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(3)))
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%0, const<i32>(0), const<i32>(3)), const<i32>(0)));
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%0, const<i32>(1), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(3))));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%0, const<i32>(0), const<i32>(0)), const<i32>(0)));
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%4, const<i32>(1), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(3))));
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%4, const<i32>(0), const<i32>(3)), const<i32>(0)));
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%4, const<i32>(1), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(3))));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%4, const<i32>(0), const<i32>(0)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
