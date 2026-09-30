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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_val:[0-9]+]] val: f80, %[[VALUE_eval:[0-9]+]] eval: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: f80 [storage=automatic] = const<f80>(1);
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(read<f80>(%[[VALUE_val]]), const<f80>(0))
// DEFAULT-NEXT:             write<f80>(%[[VALUE_val]], neg<f80>(read<f80>(%[[VALUE_val]])));
// DEFAULT-NEXT:         if ge<f80, exceptions=ignore>(read<f80>(%[[VALUE_val]]), read<f80>(%[[VALUE_tmp]]))
// DEFAULT-NEXT:             while %[[VALUE1:[0-9]+]] lt<f80, exceptions=ignore>(read<f80>(%[[VALUE_tmp]]), read<f80>(%[[VALUE_val]]))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: f80 [synthetic] = read<f80>(%[[VALUE_tmp]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: f80 [synthetic] = mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE2]]), const<f80>(2));
// DEFAULT-NEXT:                     write<f80>(%[[VALUE_tmp]], read<f80>(%[[VALUE3]]));
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%[[VALUE4]]), const<i32>(10))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_val]]), const<f80>(0))
// DEFAULT-NEXT:                 while %[[VALUE6:[0-9]+]] lt<f80, exceptions=ignore>(read<f80>(%[[VALUE_val]]), read<f80>(%[[VALUE_tmp]]))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: f80 [synthetic] = read<f80>(%[[VALUE_tmp]]);
// DEFAULT-NEXT:                         let %[[VALUE8:[0-9]+]]: f80 [synthetic] = div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE7]]), const<f80>(2));
// DEFAULT-NEXT:                         write<f80>(%[[VALUE_tmp]], read<f80>(%[[VALUE8]]));
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                         if ge<i32>(read<i32>(%[[VALUE9]]), const<i32>(10))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_eval]])), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_eval_2:[0-9]+]] eval: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<i32>) -> void>(%[[VALUE_test]], float_widen<f80, reason=arg>(const<f64>(3.0)), addr_of<ptr<i32>>(%[[VALUE_eval_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<i32>) -> void>(%[[VALUE_test]], float_widen<f80, reason=arg>(const<f64>(3.5)), addr_of<ptr<i32>>(%[[VALUE_eval_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<i32>) -> void>(%[[VALUE_test]], float_widen<f80, reason=arg>(const<f64>(4.0)), addr_of<ptr<i32>>(%[[VALUE_eval_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<i32>) -> void>(%[[VALUE_test]], float_widen<f80, reason=arg>(const<f64>(5.0)), addr_of<ptr<i32>>(%[[VALUE_eval_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
