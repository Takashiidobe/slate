/* Test argument passing of complex values.  The MIPS64 compiler had a
   bug when they were split between registers and the stack.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */

volatile _Complex float       f1  = 1.1f + 2.2if;
volatile _Complex float       f2  = 3.3f + 4.4if;
volatile _Complex float       f3  = 5.5f + 6.6if;
volatile _Complex float       f4  = 7.7f + 8.8if;
volatile _Complex float       f5  = 9.9f + 10.1if;
volatile _Complex double      d1  = 1.1 + 2.2i;
volatile _Complex double      d2  = 3.3 + 4.4i;
volatile _Complex double      d3  = 5.5 + 6.6i;
volatile _Complex double      d4  = 7.7 + 8.8i;
volatile _Complex double      d5  = 9.9 + 10.1i;
volatile _Complex long double ld1 = 1.1L + 2.2iL;
volatile _Complex long double ld2 = 3.3L + 4.4iL;
volatile _Complex long double ld3 = 5.5L + 6.6iL;
volatile _Complex long double ld4 = 7.7L + 8.8iL;
volatile _Complex long double ld5 = 9.9L + 10.1iL;

extern void abort(void);
extern void exit(int);

__attribute__((noinline)) void check_float(int a, _Complex float a1,
                                           _Complex float a2, _Complex float a3,
                                           _Complex float a4,
                                           _Complex float a5) {
  if (a1 != f1 || a2 != f2 || a3 != f3 || a4 != f4 || a5 != f5)
    abort();
}

__attribute__((noinline)) void
check_double(int a, _Complex double a1, _Complex double a2, _Complex double a3,
             _Complex double a4, _Complex double a5) {
  if (a1 != d1 || a2 != d2 || a3 != d3 || a4 != d4 || a5 != d5)
    abort();
}

__attribute__((noinline)) void check_long_double(int a, _Complex long double a1,
                                                 _Complex long double a2,
                                                 _Complex long double a3,
                                                 _Complex long double a4,
                                                 _Complex long double a5) {
  if (a1 != ld1 || a2 != ld2 || a3 != ld3 || a4 != ld4 || a5 != ld5)
    abort();
}

int main(void) {
  check_float(0, f1, f2, f3, f4, f5);
  check_double(0, d1, d2, d3, d4, d5);
  check_long_double(0, ld1, ld2, ld3, ld4, ld5);
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
// DEFAULT-NEXT:     global %[[VALUE_f1:[0-9]+]] f1: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(1.1), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f2:[0-9]+]] f2: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(3.3), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(4.4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f3:[0-9]+]] f3: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(5.5), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(6.6))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f4:[0-9]+]] f4: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(7.7), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(8.8))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f5:[0-9]+]] f5: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(9.9), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(10.1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d1:[0-9]+]] d1: volatile complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.1), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d2:[0-9]+]] d2: volatile complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.3), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d3:[0-9]+]] d3: volatile complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(5.5), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(6.6))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d4:[0-9]+]] d4: volatile complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(7.7), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(8.8))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d5:[0-9]+]] d5: volatile complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(9.9), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(10.1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ld1:[0-9]+]] ld1: volatile complex<f80> [storage=static] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(1.10000000000000000002), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2.20000000000000000004))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ld2:[0-9]+]] ld2: volatile complex<f80> [storage=static] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(3.29999999999999999996), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4.40000000000000000009))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ld3:[0-9]+]] ld3: volatile complex<f80> [storage=static] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(5.5), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(6.59999999999999999991))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ld4:[0-9]+]] ld4: volatile complex<f80> [storage=static] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(7.69999999999999999982), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(8.80000000000000000017))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ld5:[0-9]+]] ld5: volatile complex<f80> [storage=static] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(9.89999999999999999965), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(10.1000000000000000003))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check_float:[0-9]+]] @check_float(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_a1:[0-9]+]] a1: complex<f32>, %[[VALUE_a2:[0-9]+]] a2: complex<f32>, %[[VALUE_a3:[0-9]+]] a3: complex<f32>, %[[VALUE_a4:[0-9]+]] a4: complex<f32>, %[[VALUE_a5:[0-9]+]] a5: complex<f32>) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, native_c, native_c, native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE_a1]]), read<complex<f32>, volatile>(%[[VALUE_f1]])), ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE_a2]]), read<complex<f32>, volatile>(%[[VALUE_f2]]))), ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE_a3]]), read<complex<f32>, volatile>(%[[VALUE_f3]]))), ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE_a4]]), read<complex<f32>, volatile>(%[[VALUE_f4]]))), ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%[[VALUE_a5]]), read<complex<f32>, volatile>(%[[VALUE_f5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_double:[0-9]+]] @check_double(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_a1_2:[0-9]+]] a1: complex<f64>, %[[VALUE_a2_2:[0-9]+]] a2: complex<f64>, %[[VALUE_a3_2:[0-9]+]] a3: complex<f64>, %[[VALUE_a4_2:[0-9]+]] a4: complex<f64>, %[[VALUE_a5_2:[0-9]+]] a5: complex<f64>) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, native_c, native_c, native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_a1_2]]), read<complex<f64>, volatile>(%[[VALUE_d1]])), ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_a2_2]]), read<complex<f64>, volatile>(%[[VALUE_d2]]))), ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_a3_2]]), read<complex<f64>, volatile>(%[[VALUE_d3]]))), ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_a4_2]]), read<complex<f64>, volatile>(%[[VALUE_d4]]))), ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%[[VALUE_a5_2]]), read<complex<f64>, volatile>(%[[VALUE_d5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_long_double:[0-9]+]] @check_long_double(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_a1_3:[0-9]+]] a1: complex<f80>, %[[VALUE_a2_3:[0-9]+]] a2: complex<f80>, %[[VALUE_a3_3:[0-9]+]] a3: complex<f80>, %[[VALUE_a4_3:[0-9]+]] a4: complex<f80>, %[[VALUE_a5_3:[0-9]+]] a5: complex<f80>) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(scalar, byval<align=16>, byval<align=16>, byval<align=16>, byval<align=16>, byval<align=16>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE_a1_3]]), read<complex<f80>, volatile>(%[[VALUE_ld1]])), ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE_a2_3]]), read<complex<f80>, volatile>(%[[VALUE_ld2]]))), ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE_a3_3]]), read<complex<f80>, volatile>(%[[VALUE_ld3]]))), ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE_a4_3]]), read<complex<f80>, volatile>(%[[VALUE_ld4]]))), ne<complex<f80>, exceptions=observable>(read<complex<f80>>(%[[VALUE_a5_3]]), read<complex<f80>, volatile>(%[[VALUE_ld5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, complex<f32>, complex<f32>, complex<f32>, complex<f32>, complex<f32>) -> void, abi=sysv64(scalar, native_c, native_c, native_c, native_c, native_c) -> void>(%[[VALUE_check_float]], const<i32>(0), read<complex<f32>, volatile>(%[[VALUE_f1]]), read<complex<f32>, volatile>(%[[VALUE_f2]]), read<complex<f32>, volatile>(%[[VALUE_f3]]), read<complex<f32>, volatile>(%[[VALUE_f4]]), read<complex<f32>, volatile>(%[[VALUE_f5]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, complex<f64>, complex<f64>, complex<f64>, complex<f64>, complex<f64>) -> void, abi=sysv64(scalar, native_c, native_c, native_c, native_c, native_c) -> void>(%[[VALUE_check_double]], const<i32>(0), read<complex<f64>, volatile>(%[[VALUE_d1]]), read<complex<f64>, volatile>(%[[VALUE_d2]]), read<complex<f64>, volatile>(%[[VALUE_d3]]), read<complex<f64>, volatile>(%[[VALUE_d4]]), read<complex<f64>, volatile>(%[[VALUE_d5]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, complex<f80>, complex<f80>, complex<f80>, complex<f80>, complex<f80>) -> void, abi=sysv64(scalar, byval<align=16>, byval<align=16>, byval<align=16>, byval<align=16>, byval<align=16>) -> void>(%[[VALUE_check_long_double]], const<i32>(0), read<complex<f80>, volatile>(%[[VALUE_ld1]]), read<complex<f80>, volatile>(%[[VALUE_ld2]]), read<complex<f80>, volatile>(%[[VALUE_ld3]]), read<complex<f80>, volatile>(%[[VALUE_ld4]]), read<complex<f80>, volatile>(%[[VALUE_ld5]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
