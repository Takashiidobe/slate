void abort(void);
void exit(int);

void test(long double val, int *eval) {
  long double tmp = 1.0l;
  int         i   = 0;

  if (val < 0.0l)
    val = -val;

  if (val >= tmp)
    while (tmp < val) {
      tmp *= 2.0l;
      if (i++ >= 10)
        abort();
    }
  else if (val != 0.0l)
    while (val < tmp) {
      tmp /= 2.0l;
      if (i++ >= 10)
        abort();
    }

  *eval = i;
}

int main(void) {
  int eval;

  test(3.0, &eval);
  test(3.5, &eval);
  test(4.0, &eval);
  test(5.0, &eval);
  exit(0);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test(%3 val: f80, %4 eval: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 tmp: f80 [storage=automatic] = const<f80>(1);
// DEFAULT-NEXT:         let %6 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(read<f80>(%3), const<f80>(0))
// DEFAULT-NEXT:             write<f80>(%3, neg<f80>(read<f80>(%3)));
// DEFAULT-NEXT:         if ge<f80, exceptions=ignore>(read<f80>(%3), read<f80>(%5))
// DEFAULT-NEXT:             while %10 lt<f80, exceptions=ignore>(read<f80>(%5), read<f80>(%3))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12: f80 [synthetic] = read<f80>(%5);
// DEFAULT-NEXT:                     let %13: f80 [synthetic] = mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%12), const<f80>(2));
// DEFAULT-NEXT:                     write<f80>(%5, read<f80>(%13));
// DEFAULT-NEXT:                     let %14: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                     let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%6, read<i32>(%15));
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%14), const<i32>(10))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<f80, exceptions=ignore>(read<f80>(%3), const<f80>(0))
// DEFAULT-NEXT:                 while %11 lt<f80, exceptions=ignore>(read<f80>(%3), read<f80>(%5))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %16: f80 [synthetic] = read<f80>(%5);
// DEFAULT-NEXT:                         let %17: f80 [synthetic] = div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%16), const<f80>(2));
// DEFAULT-NEXT:                         write<f80>(%5, read<f80>(%17));
// DEFAULT-NEXT:                         let %18: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                         let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%6, read<i32>(%19));
// DEFAULT-NEXT:                         if ge<i32>(read<i32>(%18), const<i32>(10))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%4)), read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 eval: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<i32>) -> void>(%2, float_widen<f80, reason=arg>(const<f64>(3.0)), addr_of<ptr<i32>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<i32>) -> void>(%2, float_widen<f80, reason=arg>(const<f64>(3.5)), addr_of<ptr<i32>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<i32>) -> void>(%2, float_widen<f80, reason=arg>(const<f64>(4.0)), addr_of<ptr<i32>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<i32>) -> void>(%2, float_widen<f80, reason=arg>(const<f64>(5.0)), addr_of<ptr<i32>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
