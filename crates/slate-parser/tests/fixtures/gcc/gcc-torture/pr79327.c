/* PR tree-optimization/79327 */
/* { dg-require-effective-target c99_runtime } */

volatile int a;

int main(void) {
  int  i;
  char buf[64];
  if (__builtin_sprintf(buf, "%#hho", a) != 1)
    __builtin_abort();
  if (__builtin_sprintf(buf, "%#hhx", a) != 1)
    __builtin_abort();
  a = 1;
  if (__builtin_sprintf(buf, "%#hho", a) != 2)
    __builtin_abort();
  if (__builtin_sprintf(buf, "%#hhx", a) != 3)
    __builtin_abort();
  a = 127;
  if (__builtin_sprintf(buf, "%#hho", a) != 4)
    __builtin_abort();
  if (__builtin_sprintf(buf, "%#hhx", a) != 4)
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
// DEFAULT-NEXT:     global %0 a: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 .str4: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 35, 104, 104, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 .str5: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 35, 104, 104, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 35, 104, 104, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 35, 104, 104, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 35, 104, 104, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 35, 104, 104, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %3 buf: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(__builtin_sprintf, array_decay<ptr<i8>, length=Some(64)>(%3), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%4)), read<i32, volatile>(%0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(__builtin_sprintf, array_decay<ptr<i8>, length=Some(64)>(%3), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%5)), read<i32, volatile>(%0)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i32, volatile>(%0, const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(__builtin_sprintf, array_decay<ptr<i8>, length=Some(64)>(%3), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%6)), read<i32, volatile>(%0)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(__builtin_sprintf, array_decay<ptr<i8>, length=Some(64)>(%3), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%7)), read<i32, volatile>(%0)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i32, volatile>(%0, const<i32>(127));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(__builtin_sprintf, array_decay<ptr<i8>, length=Some(64)>(%3), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%8)), read<i32, volatile>(%0)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(__builtin_sprintf, array_decay<ptr<i8>, length=Some(64)>(%3), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%9)), read<i32, volatile>(%0)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
