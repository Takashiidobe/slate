/* { dg-require-effective-target indirect_jumps } */
/* { dg-require-effective-target label_values } */
/* { dg-skip-if "label differences not supported" { avr-*-* } } */

/* PR rtl-optimization/70460 */

int c;

__attribute__((noinline, noclone)) void foo(int x) {
  static int b[] = {&&lab1 - &&lab0, &&lab2 - &&lab0};
  void      *a   = &&lab0 + b[x];
  goto      *a;
lab1:
  c += 2;
lab2:
  c++;
lab0:;
}

int main() {
  foo(0);
  if (c != 3)
    __builtin_abort();
  foo(1);
  if (c != 4)
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
// DEFAULT-NEXT:     global %0 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 b: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%2), label_addr<ptr<void>>(%4))), index1 = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%3), label_addr<ptr<void>>(%4)))) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @foo(%5 x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 a: ptr<void> [storage=automatic] = ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(label_addr<ptr<void>>(%4), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%6), read<i32>(%5)))));
// DEFAULT-NEXT:         goto *read<ptr<void>>(%7);
// DEFAULT-NEXT:         label %2 lab1:
// DEFAULT-NEXT:             let %10: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:             let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(2));
// DEFAULT-NEXT:             write<i32>(%0, read<i32>(%11));
// DEFAULT-NEXT:         label %3 lab2:
// DEFAULT-NEXT:             let %12: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:             let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%0, read<i32>(%13));
// DEFAULT-NEXT:         label %4 lab0:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
