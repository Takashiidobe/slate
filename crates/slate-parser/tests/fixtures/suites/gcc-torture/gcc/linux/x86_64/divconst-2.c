void abort(void);
void exit(int);

long f(long x) { return x / (-0x7fffffffL - 1L); }

long r(long x) { return x % (-0x7fffffffL - 1L); }

/* Since we have a negative divisor, this equation must hold for the
   results of / and %; no specific results are guaranteed.  */
long std_eqn(long num, long denom, long quot, long rem) {
  /* For completeness, a check for "ABS (rem) < ABS (denom)" belongs here,
     but causes trouble on 32-bit machines and isn't worthwhile.  */
  return quot * (-0x7fffffffL - 1L) + rem == num;
}

long nums[] = {-1L, 0x7fffffffL, -0x7fffffffL - 1L};

int main(void) {
  int i;

  for (i = 0; i < sizeof(nums) / sizeof(nums[0]); i++)
    if (std_eqn(nums[i], -0x7fffffffL - 1L, f(nums[i]), r(nums[i])) == 0)
      abort();

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
// DEFAULT-NEXT:     global %[[VALUE_nums:[0-9]+]] nums: array<i64, 3> [storage=static] [align=16] = aggregate<array<i64, 3>, zero_fill=false>(index0 = neg<i64, overflow=ub>(const<i64>(1)), index1 = const<i64>(2147483647), index2 = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_x]]), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_r:[0-9]+]] @r(%[[VALUE_x_2:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return rem<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_x_2]]), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_std_eqn:[0-9]+]] @std_eqn(%[[VALUE_num:[0-9]+]] num: i64, %[[VALUE_denom:[0-9]+]] denom: i64, %[[VALUE_quot:[0-9]+]] quot: i64, %[[VALUE_rem:[0-9]+]] rem: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i64, reason=return>(eq<i64>(add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%[[VALUE_quot]]), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1))), read<i64>(%[[VALUE_rem]])), read<i64>(%[[VALUE_num]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(24), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i64>(call<i64, signature=fn(i64, i64, i64, i64) -> i64>(%[[VALUE_std_eqn]], read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_nums]]), read<i32>(%[[VALUE_i]])))), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1)), call<i64, signature=fn(i64) -> i64>(%[[VALUE_f]], read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_nums]]), read<i32>(%[[VALUE_i]]))))), call<i64, signature=fn(i64) -> i64>(%[[VALUE_r]], read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_nums]]), read<i32>(%[[VALUE_i]])))))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
