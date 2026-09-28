/* PR rtl-optimization/83496 */
/* Reported by Hauke Mehrtens <gcc@hauke-m.de> */

extern void abort(void);

typedef unsigned long mp_digit;

typedef struct {
  int       used, alloc, sign;
  mp_digit *dp;
} mp_int;

int mytest(mp_int *a, mp_digit b) __attribute__((noclone, noinline));

int mytest(mp_int *a, mp_digit b) {
  if (a->sign == 1)
    return -1;
  if (a->used > 1)
    return 1;
  if (a->dp[0] > b)
    return 1;
  if (a->dp[0] < b)
    return -1;
  return 0;
}

int main(void) {
  mp_int i = {2, 0, -1};
  if (mytest(&i, 0) != 1)
    abort();
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
// DEFAULT-NEXT:     type @type0 mp_digit = u64;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 used: i32;
// DEFAULT-NEXT:         field1 alloc: i32;
// DEFAULT-NEXT:         field2 sign: i32;
// DEFAULT-NEXT:         field3 dp: ptr<u64>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type2 mp_int = @type1;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @mytest(%5 a: ptr<@type1>, %6 b: u64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field2(deref(read<ptr<@type1>>(%5)))), const<i32>(1))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(field0(deref(read<ptr<@type1>>(%5)))), const<i32>(1))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if gt<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(field3(deref(read<ptr<@type1>>(%5)))), const<i32>(0)))), read<u64>(%6))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if lt<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(field3(deref(read<ptr<@type1>>(%5)))), const<i32>(0)))), read<u64>(%6))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 i: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>(field0 = const<i32>(2), field1 = const<i32>(0), field2 = neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type1>, u64) -> i32>(%4, addr_of<ptr<@type1>>(%8), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
