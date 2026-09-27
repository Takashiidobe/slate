/* PR tree-optimization/78482 */
/* { dg-do run } */

short       a = 65531;
int         b = 3, f;
signed char c, d;
static void fn1(int p1) {
  short e;
  b = f;
  if (f > p1 && p1)
  L:
    for (e = 0; 0;)
      ;
  else if (d)
    b = 0 >= b;
  for (; e <= 3; e++) {
    if (b)
      continue;
    b = 3;
    goto L;
  }
}

__attribute__((noinline, noclone)) int bar(const char *x, int y) {
  asm volatile("" : "+g"(x), "+g"(y) : : "memory");
  if (y == 2)
    __builtin_abort();
  return 0;
}

int main() {
  for (; c >= 0; c--) {
    if (!b) {
      bar("%d\n", 2);
      continue;
    }
    fn1(a);
  }
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
// DEFAULT-NEXT:     global %0 a: i16 [storage=static] = truncate<i16, reason=assign, fits=unknown>(const<i32>(65531)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %2 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %5 @fn1(%7 p1: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 e: i16 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%2));
// DEFAULT-NEXT:         if logical_and<bool>(gt<i32>(read<i32>(%2), read<i32>(%7)), ne<i32>(read<i32>(%7), const<i32>(0)))
// DEFAULT-NEXT:             label %6 L:
// DEFAULT-NEXT:                 for %13
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i16>(%8, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     condition: ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:                     increment: omitted
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<i8>(read<i8>(%4), const<i8>(0))
// DEFAULT-NEXT:                 write<i32>(%1, from_bool<i32, reason=assign>(ge<i32>(const<i32>(0), read<i32>(%1))));
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<i32>(widen<i32, reason=promotion>(read<i16>(%8)), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i16 [synthetic] = read<i16>(%8);
// DEFAULT-NEXT:                 let %19: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%18)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i16>(%8, read<i16>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                         continue %14;
// DEFAULT-NEXT:                     write<i32>(%1, const<i32>(3));
// DEFAULT-NEXT:                     goto %6;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @bar(%10 x: ptr<const i8>, %11 y: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "g" [reg | mem | imm | sym] -> reg width 64 place<ptr<const i8>>(%10);
// DEFAULT-NEXT:             inlateout 1 "g" [reg | mem | imm | sym] -> reg width 32 place<i32>(%11);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%11), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ge<i32>(widen<i32, reason=promotion>(read<i8>(%3)), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %20: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:                 let %21: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%20)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%3, read<i8>(%21));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if not<bool>(ne<i32>(read<i32>(%1), const<i32>(0)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, i32) -> i32>(%9, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), const<i32>(2));
// DEFAULT-NEXT:                             continue %16;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%5, widen<i32, reason=arg>(read<i16>(%0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
