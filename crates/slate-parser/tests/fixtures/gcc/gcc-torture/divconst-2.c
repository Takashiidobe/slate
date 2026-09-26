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
// DEFAULT-NEXT:     global %11 nums: array<i64, 3> [storage=static] [align=16] = aggregate<array<i64, 3>, zero_fill=false>(index0 = neg<i64, overflow=ub>(const<i64>(1)), index1 = const<i64>(2147483647), index2 = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%14 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%3), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @r(%5 x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return rem<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%5), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @std_eqn(%7 num: i64, %8 denom: i64, %9 quot: i64, %10 rem: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i64, reason=return>(eq<i64>(add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%9), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1))), read<i64>(%10)), read<i64>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%13))), div<u64, by_zero=ub>(const<u64>(24), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i64>(call<i64, signature=fn(i64, i64, i64, i64) -> i64>(%6, read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%11), read<i32>(%13)))), sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1)), call<i64, signature=fn(i64) -> i64>(%2, read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%11), read<i32>(%13))))), call<i64, signature=fn(i64) -> i64>(%4, read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%11), read<i32>(%13)))))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
