#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static void show(const char *name, const void *data, int bytes) {
  const unsigned char *p = data;
  unsigned hash = 2166136261u;
  for (int i = 0; i < bytes; i++)
    hash = (hash ^ p[i]) * 16777619u;
  printf("%s %08x\n", name, hash);
}

#define SHOW(name, v) do { __typeof__(v) t_ = (v); show(name, &t_, sizeof t_); } while (0)

#define CLASSES_PS(a, m)                                                                         \
  printf("ps %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",                               \
         _mm512_fpclass_ps_mask(a, 0x01), _mm512_fpclass_ps_mask(a, 0x02),                       \
         _mm512_fpclass_ps_mask(a, 0x04), _mm512_fpclass_ps_mask(a, 0x08),                       \
         _mm512_fpclass_ps_mask(a, 0x10), _mm512_fpclass_ps_mask(a, 0x20),                       \
         _mm512_fpclass_ps_mask(a, 0x40), _mm512_fpclass_ps_mask(a, 0x80),                       \
         _mm512_fpclass_ps_mask(a, 0x99), _mm512_mask_fpclass_ps_mask(m, a, 0xff))

#define CLASSES_PD(a, m)                                                                         \
  printf("pd %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",                               \
         _mm512_fpclass_pd_mask(a, 0x01), _mm512_fpclass_pd_mask(a, 0x02),                       \
         _mm512_fpclass_pd_mask(a, 0x04), _mm512_fpclass_pd_mask(a, 0x08),                       \
         _mm512_fpclass_pd_mask(a, 0x10), _mm512_fpclass_pd_mask(a, 0x20),                       \
         _mm512_fpclass_pd_mask(a, 0x40), _mm512_fpclass_pd_mask(a, 0x80),                       \
         _mm512_fpclass_pd_mask(a, 0x66), _mm512_mask_fpclass_pd_mask(m, a, 0xff))

__attribute__((target("avx512f,avx512dq,avx512vl"), noinline)) static void
avx512(const float *fs, const double *ds, int *table, const int *index, __mmask16 m) {
  __m512 a = _mm512_loadu_ps(fs);
  __m512d b = _mm512_loadu_pd(ds), c = _mm512_loadu_pd(ds + 8);
  CLASSES_PS(a, m);
  CLASSES_PD(b, (__mmask8)m);
  CLASSES_PD(c, (__mmask8)(m >> 8));
  printf("ps128 %x ps256 %x pd128 %x pd256 %x\n",
         _mm_fpclass_ps_mask(_mm512_castps512_ps128(a), 0x81),
         _mm256_mask_fpclass_ps_mask((__mmask8)m, _mm512_castps512_ps256(a), 0x26),
         _mm_fpclass_pd_mask(_mm512_castpd512_pd128(c), 0x18),
         _mm256_fpclass_pd_mask(_mm512_castpd512_pd256(b), 0x06));

  __m256i idx = _mm256_loadu_si256((const __m256i *)index);
  __m256i old = _mm256_set1_epi32(-1);
  SHOW("gather3siv8si", _mm256_mmask_i32gather_epi32(old, (__mmask8)m, idx, table, 4));
  SHOW("gather3siv8sf",
       _mm256_mmask_i32gather_ps(_mm256_set1_ps(-1), (__mmask8)m, idx, (const float *)table, 4));
  SHOW("gather3siv4si", _mm_mmask_i32gather_epi32(_mm_set1_epi32(7), (__mmask8)m,
                                                  _mm256_castsi256_si128(idx), table, 4));
  SHOW("gather3div4di", _mm256_mmask_i64gather_epi64(_mm256_set1_epi64x(9), (__mmask8)m,
                                                     _mm256_cvtepi32_epi64(_mm256_castsi256_si128(idx)),
                                                     (const long long *)table, 4));
  _mm256_mask_i32scatter_epi32(table, (__mmask8)(m >> 3), idx, _mm256_set1_epi32(1000 + m), 4);
  _mm_mask_i32scatter_epi32(table + 32, (__mmask8)m, _mm256_castsi256_si128(idx),
                            _mm_set1_epi32(-5), 4);
  show("scattered", table, 64 * sizeof *table);
}

int main(void) {
  float fs[16] = {1.5f, -2.5f, NAN, -NAN, 0.0f, -0.0f, INFINITY, -INFINITY,
                  1e-40f, -1e-40f, 3.4e38f, -1.2e-38f, 1.17549435e-38f, -7.0f, 0, 0};
  unsigned snan = 0x7fa00000u, nsnan = 0xff800001u;
  memcpy(&fs[14], &snan, 4);
  memcpy(&fs[15], &nsnan, 4);
  double ds[16] = {1.5, -2.5, NAN, -0.0, 0.0, INFINITY, -INFINITY, 4.9e-324,
                   -4.9e-324, 2.2250738585072014e-308, -1e308, 7, 0, 0, -3, 0};
  unsigned long long dsnan = 0x7ff4000000000000ull;
  memcpy(&ds[12], &dsnan, 8);
  ds[13] = -NAN;
  int table[64], index[8] = {3, 0, 17, 9, 31, 5, 12, 2};
  for (int i = 0; i < 64; i++)
    table[i] = i * 37 - 100;
  if (__builtin_cpu_supports("avx512f") && __builtin_cpu_supports("avx512dq") &&
      __builtin_cpu_supports("avx512vl")) {
    avx512(fs, ds, table, index, 0xffff);
    avx512(fs, ds, table, index, 0xa5c3);
    avx512(fs, ds, table, index, 0);
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! CLASSES_PD {
// CHECK-NEXT:     ($a:expr, $m:expr) => {
// CHECK-NEXT:         let __v : std::simd::Simd < f64, 8 > = $a; let [[V_0:__v[0-9]+]] : std::simd::Simd < f64, 8 >
// CHECK-NEXT:         = $a; let [[V_1:__v[0-9]+]] : std::simd::Simd < f64, 8 > = $a; let [[V_2:__v[0-9]+]] : std::simd::Simd <
// CHECK-NEXT:         f64, 8 > = $a; let [[V_3:__v[0-9]+]] : std::simd::Simd < f64, 8 > = $a; let [[V_4:__v[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < f64, 8 > = $a; let [[V_5:__v[0-9]+]] : std::simd::Simd < f64, 8 > = $a; let
// CHECK-NEXT:         [[V_6:__v[0-9]+]] : std::simd::Simd < f64, 8 > = $a; let [[V_7:__v[0-9]+]] : std::simd::Simd < f64, 8 > =
// CHECK-NEXT:         $a; let [[V_8:__v[0-9]+]] : std::simd::Simd < f64, 8 > = $a; let _ = std::io::Write::flush(&
// CHECK-NEXT:         mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"pd %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n"
// CHECK-NEXT:         .as_ptr(), (std::simd::cmp::SimdPartialOrd::simd_lt({ let __t : std::simd::Simd <
// CHECK-NEXT:         u64, 8 > = (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >,
// CHECK-NEXT:         std::simd::Simd < u64, 8 >> (__v) }) &
// CHECK-NEXT:         std::simd::Simd::splat(9223372036854775807u64); let [[T_0:__t[0-9]+]] = __t.to_array(); let
// CHECK-NEXT:         [[T_1:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9221120237041090560u64); let [[T_2:__t[0-9]+]] = [[T_1]].to_array(); let
// CHECK-NEXT:         [[T_3:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_0]][__lane] >=
// CHECK-NEXT:         [[T_2]][__lane]) as i64))); [[T_3]] }, std::simd::Simd::< i64, 8 >::default())
// CHECK-NEXT:         .to_bitmask() as u8) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt({ let [[T_4:__t[0-9]+]]
// CHECK-NEXT:         : std::simd::Simd < u64, 8 > = unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_0]]) }; let [[T_5:__t[0-9]+]] = [[T_4]].to_array(); let
// CHECK-NEXT:         [[T_6:__t[0-9]+]] : std::simd::Simd < u64, 8 > = std::simd::Simd::splat(0u64); let [[T_7:__t[0-9]+]] = [[T_6]]
// CHECK-NEXT:         .to_array(); let [[T_8:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_5]][__lane] ==
// CHECK-NEXT:         [[T_7]][__lane]) as i64))); [[T_8]] }, std::simd::Simd::< i64, 8 >::default())
// CHECK-NEXT:         .to_bitmask() as u8) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt({ let [[T_9:__t[0-9]+]]
// CHECK-NEXT:         : std::simd::Simd < u64, 8 > = unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_1]]) }; let [[T_10:__t[0-9]+]] = [[T_9]].to_array(); let
// CHECK-NEXT:         [[T_11:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9223372036854775808u64); let [[T_12:__t[0-9]+]] = [[T_11]].to_array(); let
// CHECK-NEXT:         [[T_13:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_10]][__lane] ==
// CHECK-NEXT:         [[T_12]][__lane]) as i64))); [[T_13]] }, std::simd::Simd::< i64, 8 >::default())
// CHECK-NEXT:         .to_bitmask() as u8) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt({ let [[T_14:__t[0-9]+]]
// CHECK-NEXT:         : std::simd::Simd < u64, 8 > = unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_2]]) }; let [[T_15:__t[0-9]+]] = [[T_14]].to_array(); let
// CHECK-NEXT:         [[T_16:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9218868437227405312u64); let [[T_17:__t[0-9]+]] = [[T_16]].to_array(); let
// CHECK-NEXT:         [[T_18:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_15]][__lane] ==
// CHECK-NEXT:         [[T_17]][__lane]) as i64))); [[T_18]] }, std::simd::Simd::< i64, 8 >::default())
// CHECK-NEXT:         .to_bitmask() as u8) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt({ let [[T_19:__t[0-9]+]]
// CHECK-NEXT:         : std::simd::Simd < u64, 8 > = unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_3]]) }; let [[T_20:__t[0-9]+]] = [[T_19]].to_array(); let
// CHECK-NEXT:         [[T_21:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(18442240474082181120u64); let [[T_22:__t[0-9]+]] = [[T_21]].to_array();
// CHECK-NEXT:         let [[T_23:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_20]][__lane] ==
// CHECK-NEXT:         [[T_22]][__lane]) as i64))); [[T_23]] }, std::simd::Simd::< i64, 8 >::default())
// CHECK-NEXT:         .to_bitmask() as u8) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt(({ let
// CHECK-NEXT:         [[T_24:__t[0-9]+]] : std::simd::Simd < u64, 8 > = (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_4]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(9223372036854775807u64); let [[T_25:__t[0-9]+]] = [[T_24]].to_array(); let
// CHECK-NEXT:         [[T_26:__t[0-9]+]] : std::simd::Simd < u64, 8 > = std::simd::Simd::splat(0u64); let [[T_27:__t[0-9]+]] =
// CHECK-NEXT:         [[T_26]].to_array(); let [[T_28:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_25]][__lane] !=
// CHECK-NEXT:         [[T_27]][__lane]) as i64))); [[T_28]] }) & { let [[T_29:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_4]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_30:__t[0-9]+]] =
// CHECK-NEXT:         [[T_29]].to_array(); let [[T_31:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(4503599627370496u64); let [[T_32:__t[0-9]+]] = [[T_31]].to_array(); let
// CHECK-NEXT:         [[T_33:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_30]][__lane] <
// CHECK-NEXT:         [[T_32]][__lane]) as i64))); [[T_33]] }, std::simd::Simd::< i64, 8 >::default())
// CHECK-NEXT:         .to_bitmask() as u8) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt(({ let
// CHECK-NEXT:         [[T_34:__t[0-9]+]] : std::simd::Simd < u64, 8 > = unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_5]]) }; let [[T_35:__t[0-9]+]] =
// CHECK-NEXT:         [[T_34]].to_array(); let [[T_36:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9223372036854775808u64); let [[T_37:__t[0-9]+]] = [[T_36]].to_array(); let
// CHECK-NEXT:         [[T_38:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_35]][__lane] >=
// CHECK-NEXT:         [[T_37]][__lane]) as i64))); [[T_38]] }) & { let [[T_39:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_5]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_40:__t[0-9]+]] =
// CHECK-NEXT:         [[T_39]].to_array(); let [[T_41:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9218868437227405312u64); let [[T_42:__t[0-9]+]] = [[T_41]].to_array(); let
// CHECK-NEXT:         [[T_43:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_40]][__lane] <
// CHECK-NEXT:         [[T_42]][__lane]) as i64))); [[T_43]] } & { let [[T_44:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_5]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_45:__t[0-9]+]] =
// CHECK-NEXT:         [[T_44]].to_array(); let [[T_46:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(0u64); let [[T_47:__t[0-9]+]] = [[T_46]].to_array(); let [[T_48:__t[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < i64, 8 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_45]][__lane] != [[T_47]][__lane]) as i64))); [[T_48]] },
// CHECK-NEXT:         std::simd::Simd::< i64, 8 >::default()).to_bitmask() as u8) as i32,
// CHECK-NEXT:         (std::simd::cmp::SimdPartialOrd::simd_lt(({ let [[T_49:__t[0-9]+]] : std::simd::Simd < u64, 8
// CHECK-NEXT:         > = (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd
// CHECK-NEXT:         < u64, 8 >> ([[V_6]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_50:__t[0-9]+]]
// CHECK-NEXT:         = [[T_49]].to_array(); let [[T_51:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9218868437227405312u64); let [[T_52:__t[0-9]+]] = [[T_51]].to_array(); let
// CHECK-NEXT:         [[T_53:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_50]][__lane] >
// CHECK-NEXT:         [[T_52]][__lane]) as i64))); [[T_53]] }) & { let [[T_54:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_6]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_55:__t[0-9]+]] =
// CHECK-NEXT:         [[T_54]].to_array(); let [[T_56:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9221120237041090560u64); let [[T_57:__t[0-9]+]] = [[T_56]].to_array(); let
// CHECK-NEXT:         [[T_58:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_55]][__lane] <
// CHECK-NEXT:         [[T_57]][__lane]) as i64))); [[T_58]] }, std::simd::Simd::< i64, 8 >::default())
// CHECK-NEXT:         .to_bitmask() as u8) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt(({ let
// CHECK-NEXT:         [[T_59:__t[0-9]+]] : std::simd::Simd < u64, 8 > = unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_7]]) }; let [[T_60:__t[0-9]+]] =
// CHECK-NEXT:         [[T_59]].to_array(); let [[T_61:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(0u64); let [[T_62:__t[0-9]+]] = [[T_61]].to_array(); let [[T_63:__t[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < i64, 8 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_60]][__lane] == [[T_62]][__lane]) as i64))); [[T_63]] }) | { let [[T_64:__t[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < u64, 8 > = unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_7]]) }; let [[T_65:__t[0-9]+]] = [[T_64]].to_array(); let
// CHECK-NEXT:         [[T_66:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9223372036854775808u64); let [[T_67:__t[0-9]+]] = [[T_66]].to_array(); let
// CHECK-NEXT:         [[T_68:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_65]][__lane] ==
// CHECK-NEXT:         [[T_67]][__lane]) as i64))); [[T_68]] } | ({ let [[T_69:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_7]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_70:__t[0-9]+]] =
// CHECK-NEXT:         [[T_69]].to_array(); let [[T_71:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(0u64); let [[T_72:__t[0-9]+]] = [[T_71]].to_array(); let [[T_73:__t[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < i64, 8 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_70]][__lane] != [[T_72]][__lane]) as i64))); [[T_73]] }) & { let [[T_74:__t[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < u64, 8 > = (unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_7]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(9223372036854775807u64); let [[T_75:__t[0-9]+]] = [[T_74]].to_array(); let
// CHECK-NEXT:         [[T_76:__t[0-9]+]] : std::simd::Simd < u64, 8 > = std::simd::Simd::splat(4503599627370496u64);
// CHECK-NEXT:         let [[T_77:__t[0-9]+]] = [[T_76]].to_array(); let [[T_78:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_75]][__lane] <
// CHECK-NEXT:         [[T_77]][__lane]) as i64))); [[T_78]] } | ({ let [[T_79:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_7]]) }; let [[T_80:__t[0-9]+]] = [[T_79]].to_array(); let [[T_81:__t[0-9]+]] : std::simd::Simd <
// CHECK-NEXT:         u64, 8 > = std::simd::Simd::splat(9223372036854775808u64); let [[T_82:__t[0-9]+]] = [[T_81]]
// CHECK-NEXT:         .to_array(); let [[T_83:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_80]][__lane] >=
// CHECK-NEXT:         [[T_82]][__lane]) as i64))); [[T_83]] }) & { let [[T_84:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_7]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_85:__t[0-9]+]] =
// CHECK-NEXT:         [[T_84]].to_array(); let [[T_86:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9218868437227405312u64); let [[T_87:__t[0-9]+]] = [[T_86]].to_array(); let
// CHECK-NEXT:         [[T_88:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_85]][__lane] <
// CHECK-NEXT:         [[T_87]][__lane]) as i64))); [[T_88]] } & { let [[T_89:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_7]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_90:__t[0-9]+]] =
// CHECK-NEXT:         [[T_89]].to_array(); let [[T_91:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(0u64); let [[T_92:__t[0-9]+]] = [[T_91]].to_array(); let [[T_93:__t[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < i64, 8 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_90]][__lane] != [[T_92]][__lane]) as i64))); [[T_93]] },
// CHECK-NEXT:         std::simd::Simd::< i64, 8 >::default()).to_bitmask() as u8) as i32,
// CHECK-NEXT:         ((std::simd::cmp::SimdPartialOrd::simd_lt(({ let [[T_94:__t[0-9]+]] : std::simd::Simd < u64, 8
// CHECK-NEXT:         > = (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd
// CHECK-NEXT:         < u64, 8 >> ([[V_8]]) }) & std::simd::Simd::splat(9223372036854775807u64); let
// CHECK-NEXT:         [[T_95:__t[0-9]+]] = [[T_94]].to_array(); let [[T_96:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9221120237041090560u64); let [[T_97:__t[0-9]+]] = [[T_96]].to_array(); let
// CHECK-NEXT:         [[T_98:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_95]][__lane] >=
// CHECK-NEXT:         [[T_97]][__lane]) as i64))); [[T_98]] }) | { let [[T_99:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }; let [[T_100:__t[0-9]+]] = [[T_99]].to_array(); let [[T_101:__t[0-9]+]] : std::simd::Simd
// CHECK-NEXT:         < u64, 8 > = std::simd::Simd::splat(0u64); let [[T_102:__t[0-9]+]] = [[T_101]].to_array(); let
// CHECK-NEXT:         [[T_103:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_100]][__lane] ==
// CHECK-NEXT:         [[T_102]][__lane]) as i64))); [[T_103]] } | { let [[T_104:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }; let [[T_105:__t[0-9]+]] = [[T_104]].to_array(); let [[T_106:__t[0-9]+]] : std::simd::Simd
// CHECK-NEXT:         < u64, 8 > = std::simd::Simd::splat(9223372036854775808u64); let [[T_107:__t[0-9]+]] = [[T_106]]
// CHECK-NEXT:         .to_array(); let [[T_108:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_105]][__lane] ==
// CHECK-NEXT:         [[T_107]][__lane]) as i64))); [[T_108]] } | { let [[T_109:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }; let [[T_110:__t[0-9]+]] = [[T_109]].to_array(); let [[T_111:__t[0-9]+]] : std::simd::Simd
// CHECK-NEXT:         < u64, 8 > = std::simd::Simd::splat(9218868437227405312u64); let [[T_112:__t[0-9]+]] = [[T_111]]
// CHECK-NEXT:         .to_array(); let [[T_113:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_110]][__lane] ==
// CHECK-NEXT:         [[T_112]][__lane]) as i64))); [[T_113]] } | { let [[T_114:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }; let [[T_115:__t[0-9]+]] = [[T_114]].to_array(); let [[T_116:__t[0-9]+]] : std::simd::Simd
// CHECK-NEXT:         < u64, 8 > = std::simd::Simd::splat(18442240474082181120u64); let [[T_117:__t[0-9]+]] = [[T_116]]
// CHECK-NEXT:         .to_array(); let [[T_118:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_115]][__lane] ==
// CHECK-NEXT:         [[T_117]][__lane]) as i64))); [[T_118]] } | ({ let [[T_119:__t[0-9]+]] : std::simd::Simd < u64, 8 >
// CHECK-NEXT:         = (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_120:__t[0-9]+]]
// CHECK-NEXT:         = [[T_119]].to_array(); let [[T_121:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(0u64); let [[T_122:__t[0-9]+]] = [[T_121]].to_array(); let [[T_123:__t[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < i64, 8 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_120]][__lane] != [[T_122]][__lane]) as i64))); [[T_123]] }) & { let
// CHECK-NEXT:         [[T_124:__t[0-9]+]] : std::simd::Simd < u64, 8 > = (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_8]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(9223372036854775807u64); let [[T_125:__t[0-9]+]] = [[T_124]].to_array();
// CHECK-NEXT:         let [[T_126:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(4503599627370496u64); let [[T_127:__t[0-9]+]] = [[T_126]].to_array(); let
// CHECK-NEXT:         [[T_128:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_125]][__lane] <
// CHECK-NEXT:         [[T_127]][__lane]) as i64))); [[T_128]] } | ({ let [[T_129:__t[0-9]+]] : std::simd::Simd < u64, 8 >
// CHECK-NEXT:         = unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }; let [[T_130:__t[0-9]+]] = [[T_129]].to_array(); let [[T_131:__t[0-9]+]] : std::simd::Simd
// CHECK-NEXT:         < u64, 8 > = std::simd::Simd::splat(9223372036854775808u64); let [[T_132:__t[0-9]+]] = [[T_131]]
// CHECK-NEXT:         .to_array(); let [[T_133:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_130]][__lane] >=
// CHECK-NEXT:         [[T_132]][__lane]) as i64))); [[T_133]] }) & { let [[T_134:__t[0-9]+]] : std::simd::Simd < u64, 8 >
// CHECK-NEXT:         = (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_135:__t[0-9]+]]
// CHECK-NEXT:         = [[T_134]].to_array(); let [[T_136:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9218868437227405312u64); let [[T_137:__t[0-9]+]] = [[T_136]].to_array();
// CHECK-NEXT:         let [[T_138:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_135]][__lane] <
// CHECK-NEXT:         [[T_137]][__lane]) as i64))); [[T_138]] } & { let [[T_139:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_140:__t[0-9]+]]
// CHECK-NEXT:         = [[T_139]].to_array(); let [[T_141:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(0u64); let [[T_142:__t[0-9]+]] = [[T_141]].to_array(); let [[T_143:__t[0-9]+]] :
// CHECK-NEXT:         std::simd::Simd < i64, 8 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_140]][__lane] != [[T_142]][__lane]) as i64))); [[T_143]] } | ({ let
// CHECK-NEXT:         [[T_144:__t[0-9]+]] : std::simd::Simd < u64, 8 > = (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f64, 8 >, std::simd::Simd < u64, 8 >> ([[V_8]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(9223372036854775807u64); let [[T_145:__t[0-9]+]] = [[T_144]].to_array();
// CHECK-NEXT:         let [[T_146:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9218868437227405312u64); let [[T_147:__t[0-9]+]] = [[T_146]].to_array();
// CHECK-NEXT:         let [[T_148:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_145]][__lane] >
// CHECK-NEXT:         [[T_147]][__lane]) as i64))); [[T_148]] }) & { let [[T_149:__t[0-9]+]] : std::simd::Simd < u64, 8 >
// CHECK-NEXT:         = (unsafe { std::mem::transmute::< std::simd::Simd < f64, 8 >, std::simd::Simd <
// CHECK-NEXT:         u64, 8 >> ([[V_8]]) }) & std::simd::Simd::splat(9223372036854775807u64); let [[T_150:__t[0-9]+]]
// CHECK-NEXT:         = [[T_149]].to_array(); let [[T_151:__t[0-9]+]] : std::simd::Simd < u64, 8 > =
// CHECK-NEXT:         std::simd::Simd::splat(9221120237041090560u64); let [[T_152:__t[0-9]+]] = [[T_151]].to_array();
// CHECK-NEXT:         let [[T_153:__t[0-9]+]] : std::simd::Simd < i64, 8 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_150]][__lane] <
// CHECK-NEXT:         [[T_152]][__lane]) as i64))); [[T_153]] }, std::simd::Simd::< i64, 8 >::default())
// CHECK-NEXT:         .to_bitmask() as u8) & (($m) as u8)) as i32) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! CLASSES_PS {
// CHECK-NEXT:     ($a:expr, $m:expr) => {
// CHECK-NEXT:         let __v : std::simd::Simd < f32, 16 > = $a; let [[V_0]] : std::simd::Simd < f32, 16
// CHECK-NEXT:         > = $a; let [[V_1]] : std::simd::Simd < f32, 16 > = $a; let [[V_2]] : std::simd::Simd <
// CHECK-NEXT:         f32, 16 > = $a; let [[V_3]] : std::simd::Simd < f32, 16 > = $a; let [[V_4]] :
// CHECK-NEXT:         std::simd::Simd < f32, 16 > = $a; let [[V_5]] : std::simd::Simd < f32, 16 > = $a;
// CHECK-NEXT:         let [[V_6]] : std::simd::Simd < f32, 16 > = $a; let [[V_7]] : std::simd::Simd < f32, 16
// CHECK-NEXT:         > = $a; let [[V_8]] : std::simd::Simd < f32, 16 > = $a; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"ps %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n"
// CHECK-NEXT:         .as_ptr(), (std::simd::cmp::SimdPartialOrd::simd_lt({ let __t : std::simd::Simd <
// CHECK-NEXT:         u32, 16 > = (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >,
// CHECK-NEXT:         std::simd::Simd < u32, 16 >> (__v) }) & std::simd::Simd::splat(2147483647u32);
// CHECK-NEXT:         let [[T_0]] = __t.to_array(); let [[T_1]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(2143289344u32); let [[T_2]] = [[T_1]].to_array(); let [[T_3]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_0]][__lane] >= [[T_2]][__lane]) as i32))); [[T_3]] }, std::simd::Simd::<
// CHECK-NEXT:         i32, 16 >::default()).to_bitmask() as u16) as i32,
// CHECK-NEXT:         (std::simd::cmp::SimdPartialOrd::simd_lt({ let [[T_4]] : std::simd::Simd < u32, 16 >
// CHECK-NEXT:         = unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_0]]) }; let [[T_5]] = [[T_4]].to_array(); let [[T_6]] : std::simd::Simd <
// CHECK-NEXT:         u32, 16 > = std::simd::Simd::splat(0u32); let [[T_7]] = [[T_6]].to_array(); let [[T_8]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_5]][__lane] == [[T_7]][__lane]) as i32))); [[T_8]] }, std::simd::Simd::<
// CHECK-NEXT:         i32, 16 >::default()).to_bitmask() as u16) as i32,
// CHECK-NEXT:         (std::simd::cmp::SimdPartialOrd::simd_lt({ let [[T_9]] : std::simd::Simd < u32, 16
// CHECK-NEXT:         > = unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd
// CHECK-NEXT:         < u32, 16 >> ([[V_1]]) }; let [[T_10]] = [[T_9]].to_array(); let [[T_11]] : std::simd::Simd
// CHECK-NEXT:         < u32, 16 > = std::simd::Simd::splat(2147483648u32); let [[T_12]] = [[T_11]]
// CHECK-NEXT:         .to_array(); let [[T_13]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_10]][__lane] ==
// CHECK-NEXT:         [[T_12]][__lane]) as i32))); [[T_13]] }, std::simd::Simd::< i32, 16 >::default())
// CHECK-NEXT:         .to_bitmask() as u16) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt({ let
// CHECK-NEXT:         [[T_14]] : std::simd::Simd < u32, 16 > = unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_2]]) }; let [[T_15]] =
// CHECK-NEXT:         [[T_14]].to_array(); let [[T_16]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(2139095040u32); let [[T_17]] = [[T_16]].to_array(); let [[T_18]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_15]][__lane] == [[T_17]][__lane]) as i32))); [[T_18]] },
// CHECK-NEXT:         std::simd::Simd::< i32, 16 >::default()).to_bitmask() as u16) as i32,
// CHECK-NEXT:         (std::simd::cmp::SimdPartialOrd::simd_lt({ let [[T_19]] : std::simd::Simd < u32, 16
// CHECK-NEXT:         > = unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd
// CHECK-NEXT:         < u32, 16 >> ([[V_3]]) }; let [[T_20]] = [[T_19]].to_array(); let [[T_21]] : std::simd::Simd
// CHECK-NEXT:         < u32, 16 > = std::simd::Simd::splat(4286578688u32); let [[T_22]] = [[T_21]]
// CHECK-NEXT:         .to_array(); let [[T_23]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_20]][__lane] ==
// CHECK-NEXT:         [[T_22]][__lane]) as i32))); [[T_23]] }, std::simd::Simd::< i32, 16 >::default())
// CHECK-NEXT:         .to_bitmask() as u16) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt(({ let
// CHECK-NEXT:         [[T_24]] : std::simd::Simd < u32, 16 > = (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_4]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(2147483647u32); let [[T_25]] = [[T_24]].to_array(); let [[T_26]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = std::simd::Simd::splat(0u32); let [[T_27]] = [[T_26]]
// CHECK-NEXT:         .to_array(); let [[T_28]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_25]][__lane] !=
// CHECK-NEXT:         [[T_27]][__lane]) as i32))); [[T_28]] }) & { let [[T_29]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_4]]) }) & std::simd::Simd::splat(2147483647u32); let [[T_30]] = [[T_29]]
// CHECK-NEXT:         .to_array(); let [[T_31]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(8388608u32); let [[T_32]] = [[T_31]].to_array(); let [[T_33]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_30]][__lane] < [[T_32]][__lane]) as i32))); [[T_33]] },
// CHECK-NEXT:         std::simd::Simd::< i32, 16 >::default()).to_bitmask() as u16) as i32,
// CHECK-NEXT:         (std::simd::cmp::SimdPartialOrd::simd_lt(({ let [[T_34]] : std::simd::Simd < u32, 16
// CHECK-NEXT:         > = unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd
// CHECK-NEXT:         < u32, 16 >> ([[V_5]]) }; let [[T_35]] = [[T_34]].to_array(); let [[T_36]] : std::simd::Simd
// CHECK-NEXT:         < u32, 16 > = std::simd::Simd::splat(2147483648u32); let [[T_37]] = [[T_36]]
// CHECK-NEXT:         .to_array(); let [[T_38]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_35]][__lane] >=
// CHECK-NEXT:         [[T_37]][__lane]) as i32))); [[T_38]] }) & { let [[T_39]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_5]]) }) & std::simd::Simd::splat(2147483647u32); let [[T_40]] = [[T_39]]
// CHECK-NEXT:         .to_array(); let [[T_41]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(2139095040u32); let [[T_42]] = [[T_41]].to_array(); let [[T_43]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_40]][__lane] < [[T_42]][__lane]) as i32))); [[T_43]] } & { let [[T_44]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = (unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_5]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(2147483647u32); let [[T_45]] = [[T_44]].to_array(); let [[T_46]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = std::simd::Simd::splat(0u32); let [[T_47]] = [[T_46]]
// CHECK-NEXT:         .to_array(); let [[T_48]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_45]][__lane] !=
// CHECK-NEXT:         [[T_47]][__lane]) as i32))); [[T_48]] }, std::simd::Simd::< i32, 16 >::default())
// CHECK-NEXT:         .to_bitmask() as u16) as i32, (std::simd::cmp::SimdPartialOrd::simd_lt(({ let
// CHECK-NEXT:         [[T_49]] : std::simd::Simd < u32, 16 > = (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_6]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(2147483647u32); let [[T_50]] = [[T_49]].to_array(); let [[T_51]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = std::simd::Simd::splat(2139095040u32); let [[T_52]] =
// CHECK-NEXT:         [[T_51]].to_array(); let [[T_53]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_50]][__lane] >
// CHECK-NEXT:         [[T_52]][__lane]) as i32))); [[T_53]] }) & { let [[T_54]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_6]]) }) & std::simd::Simd::splat(2147483647u32); let [[T_55]] = [[T_54]]
// CHECK-NEXT:         .to_array(); let [[T_56]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(2143289344u32); let [[T_57]] = [[T_56]].to_array(); let [[T_58]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_55]][__lane] < [[T_57]][__lane]) as i32))); [[T_58]] },
// CHECK-NEXT:         std::simd::Simd::< i32, 16 >::default()).to_bitmask() as u16) as i32,
// CHECK-NEXT:         (std::simd::cmp::SimdPartialOrd::simd_lt(({ let [[T_59]] : std::simd::Simd < u32, 16
// CHECK-NEXT:         > = (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd
// CHECK-NEXT:         < u32, 16 >> ([[V_7]]) }) & std::simd::Simd::splat(2147483647u32); let [[T_60]] = [[T_59]]
// CHECK-NEXT:         .to_array(); let [[T_61]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(2143289344u32); let [[T_62]] = [[T_61]].to_array(); let [[T_63]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_60]][__lane] >= [[T_62]][__lane]) as i32))); [[T_63]] }) | { let [[T_64]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_7]]) }; let [[T_65]] = [[T_64]].to_array();
// CHECK-NEXT:         let [[T_66]] : std::simd::Simd < u32, 16 > = std::simd::Simd::splat(2139095040u32);
// CHECK-NEXT:         let [[T_67]] = [[T_66]].to_array(); let [[T_68]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_65]][__lane] ==
// CHECK-NEXT:         [[T_67]][__lane]) as i32))); [[T_68]] } | { let [[T_69]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_7]]) }; let [[T_70]] = [[T_69]].to_array(); let [[T_71]] : std::simd::Simd <
// CHECK-NEXT:         u32, 16 > = std::simd::Simd::splat(4286578688u32); let [[T_72]] = [[T_71]].to_array();
// CHECK-NEXT:         let [[T_73]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_70]][__lane] ==
// CHECK-NEXT:         [[T_72]][__lane]) as i32))); [[T_73]] } | ({ let [[T_74]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_7]]) }) & std::simd::Simd::splat(2147483647u32); let [[T_75]] = [[T_74]]
// CHECK-NEXT:         .to_array(); let [[T_76]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(2139095040u32); let [[T_77]] = [[T_76]].to_array(); let [[T_78]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_75]][__lane] > [[T_77]][__lane]) as i32))); [[T_78]] }) & { let [[T_79]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = (unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_7]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(2147483647u32); let [[T_80]] = [[T_79]].to_array(); let [[T_81]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = std::simd::Simd::splat(2143289344u32); let [[T_82]] =
// CHECK-NEXT:         [[T_81]].to_array(); let [[T_83]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_80]][__lane] <
// CHECK-NEXT:         [[T_82]][__lane]) as i32))); [[T_83]] }, std::simd::Simd::< i32, 16 >::default())
// CHECK-NEXT:         .to_bitmask() as u16) as i32, ((std::simd::cmp::SimdPartialOrd::simd_lt(({ let
// CHECK-NEXT:         [[T_84]] : std::simd::Simd < u32, 16 > = (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_8]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(2147483647u32); let [[T_85]] = [[T_84]].to_array(); let [[T_86]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = std::simd::Simd::splat(2143289344u32); let [[T_87]] =
// CHECK-NEXT:         [[T_86]].to_array(); let [[T_88]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_85]][__lane] >=
// CHECK-NEXT:         [[T_87]][__lane]) as i32))); [[T_88]] }) | { let [[T_89]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_8]]) }; let [[T_90]] = [[T_89]].to_array(); let [[T_91]] : std::simd::Simd <
// CHECK-NEXT:         u32, 16 > = std::simd::Simd::splat(0u32); let [[T_92]] = [[T_91]].to_array(); let [[T_93]]
// CHECK-NEXT:         : std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_90]][__lane] == [[T_92]][__lane]) as i32))); [[T_93]] } | { let [[T_94]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_8]]) }; let [[T_95]] = [[T_94]].to_array();
// CHECK-NEXT:         let [[T_96]] : std::simd::Simd < u32, 16 > = std::simd::Simd::splat(2147483648u32);
// CHECK-NEXT:         let [[T_97]] = [[T_96]].to_array(); let [[T_98]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_95]][__lane] ==
// CHECK-NEXT:         [[T_97]][__lane]) as i32))); [[T_98]] } | { let [[T_99]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_8]]) }; let [[T_100]] = [[T_99]].to_array(); let [[T_101]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = std::simd::Simd::splat(2139095040u32); let [[T_102]] =
// CHECK-NEXT:         [[T_101]].to_array(); let [[T_103]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_100]][__lane] ==
// CHECK-NEXT:         [[T_102]][__lane]) as i32))); [[T_103]] } | { let [[T_104]] : std::simd::Simd < u32, 16 >
// CHECK-NEXT:         = unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_8]]) }; let [[T_105]] = [[T_104]].to_array(); let [[T_106]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = std::simd::Simd::splat(4286578688u32); let [[T_107]] =
// CHECK-NEXT:         [[T_106]].to_array(); let [[T_108]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_105]][__lane] ==
// CHECK-NEXT:         [[T_107]][__lane]) as i32))); [[T_108]] } | ({ let [[T_109]] : std::simd::Simd < u32, 16 >
// CHECK-NEXT:         = (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_8]]) }) & std::simd::Simd::splat(2147483647u32); let [[T_110]] =
// CHECK-NEXT:         [[T_109]].to_array(); let [[T_111]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(0u32); let [[T_112]] = [[T_111]].to_array(); let [[T_113]] :
// CHECK-NEXT:         std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_110]][__lane] != [[T_112]][__lane]) as i32))); [[T_113]] }) & { let
// CHECK-NEXT:         [[T_114]] : std::simd::Simd < u32, 16 > = (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_8]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(2147483647u32); let [[T_115]] = [[T_114]].to_array(); let [[T_116]]
// CHECK-NEXT:         : std::simd::Simd < u32, 16 > = std::simd::Simd::splat(8388608u32); let [[T_117]] =
// CHECK-NEXT:         [[T_116]].to_array(); let [[T_118]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_115]][__lane] <
// CHECK-NEXT:         [[T_117]][__lane]) as i32))); [[T_118]] } | ({ let [[T_119]] : std::simd::Simd < u32, 16 >
// CHECK-NEXT:         = unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_8]]) }; let [[T_120]] = [[T_119]].to_array(); let [[T_121]] :
// CHECK-NEXT:         std::simd::Simd < u32, 16 > = std::simd::Simd::splat(2147483648u32); let [[T_122]] =
// CHECK-NEXT:         [[T_121]].to_array(); let [[T_123]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_120]][__lane] >=
// CHECK-NEXT:         [[T_122]][__lane]) as i32))); [[T_123]] }) & { let [[T_124]] : std::simd::Simd < u32, 16 >
// CHECK-NEXT:         = (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_8]]) }) & std::simd::Simd::splat(2147483647u32); let [[T_125]] =
// CHECK-NEXT:         [[T_124]].to_array(); let [[T_126]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(2139095040u32); let [[T_127]] = [[T_126]].to_array(); let [[T_128]]
// CHECK-NEXT:         : std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_125]][__lane] < [[T_127]][__lane]) as i32))); [[T_128]] } & { let [[T_129]]
// CHECK-NEXT:         : std::simd::Simd < u32, 16 > = (unsafe { std::mem::transmute::< std::simd::Simd
// CHECK-NEXT:         < f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_8]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(2147483647u32); let [[T_130]] = [[T_129]].to_array(); let [[T_131]]
// CHECK-NEXT:         : std::simd::Simd < u32, 16 > = std::simd::Simd::splat(0u32); let [[T_132]] = [[T_131]]
// CHECK-NEXT:         .to_array(); let [[T_133]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_130]][__lane] !=
// CHECK-NEXT:         [[T_132]][__lane]) as i32))); [[T_133]] } | ({ let [[T_134]] : std::simd::Simd < u32, 16 >
// CHECK-NEXT:         = (unsafe { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd <
// CHECK-NEXT:         u32, 16 >> ([[V_8]]) }) & std::simd::Simd::splat(2147483647u32); let [[T_135]] =
// CHECK-NEXT:         [[T_134]].to_array(); let [[T_136]] : std::simd::Simd < u32, 16 > =
// CHECK-NEXT:         std::simd::Simd::splat(2139095040u32); let [[T_137]] = [[T_136]].to_array(); let [[T_138]]
// CHECK-NEXT:         : std::simd::Simd < i32, 16 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_135]][__lane] > [[T_137]][__lane]) as i32))); [[T_138]] }) & { let [[T_139]]
// CHECK-NEXT:         : std::simd::Simd < u32, 16 > = (unsafe { std::mem::transmute::< std::simd::Simd
// CHECK-NEXT:         < f32, 16 >, std::simd::Simd < u32, 16 >> ([[V_8]]) }) &
// CHECK-NEXT:         std::simd::Simd::splat(2147483647u32); let [[T_140]] = [[T_139]].to_array(); let [[T_141]]
// CHECK-NEXT:         : std::simd::Simd < u32, 16 > = std::simd::Simd::splat(2143289344u32); let [[T_142]]
// CHECK-NEXT:         = [[T_141]].to_array(); let [[T_143]] : std::simd::Simd < i32, 16 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_140]][__lane] <
// CHECK-NEXT:         [[T_142]][__lane]) as i32))); [[T_143]] }, std::simd::Simd::< i32, 16 >::default())
// CHECK-NEXT:         .to_bitmask() as u16) & (($m) as u16)) as i32) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 2 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__3 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 8 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 4 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_cpu_supports([[ARG_0:arg[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.gather3div4.di"]
// CHECK-NEXT:     fn __slate_intrinsic_276a67a3279ccf52(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i64, 4>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *const (),
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: std::simd::Simd<i64, 4>,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: u8,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<i64, 4>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.scattersiv8.si"]
// CHECK-NEXT:     fn __slate_intrinsic_6d0a0c56eeee8011(
// CHECK-NEXT:         [[ARG_0]]: *mut (),
// CHECK-NEXT:         [[ARG_1]]: u8,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i32, 8>,
// CHECK-NEXT:         [[ARG_3]]: std::simd::Simd<i32, 8>,
// CHECK-NEXT:         [[ARG_4]]: i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.gather3siv8.si"]
// CHECK-NEXT:     fn __slate_intrinsic_704989ab100419e7(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 8>,
// CHECK-NEXT:         [[ARG_1]]: *const (),
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i32, 8>,
// CHECK-NEXT:         [[ARG_3]]: u8,
// CHECK-NEXT:         [[ARG_4]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 8>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.gather3siv8.sf"]
// CHECK-NEXT:     fn __slate_intrinsic_a7ad3f7c58ba8c0d(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:         [[ARG_1]]: *const (),
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i32, 8>,
// CHECK-NEXT:         [[ARG_3]]: u8,
// CHECK-NEXT:         [[ARG_4]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 8>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.gather3siv4.si"]
// CHECK-NEXT:     fn __slate_intrinsic_ce634a86dfaf8f6d(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: *const (),
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_3]]: u8,
// CHECK-NEXT:         [[ARG_4]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.scattersiv4.si"]
// CHECK-NEXT:     fn __slate_intrinsic_d4aec945be1d3a97(
// CHECK-NEXT:         [[ARG_0]]: *mut (),
// CHECK-NEXT:         [[ARG_1]]: u8,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_3]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_4]]: i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si256 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 4>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_pd {
// CHECK-NEXT:     __v: std::simd::Simd<f64, 8>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_ps {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 16>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_set_epi32(
// CHECK-NEXT:     __i3: i32,
// CHECK-NEXT:     __i2: i32,
// CHECK-NEXT:     __i1: i32,
// CHECK-NEXT:     __i0: i32,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __i0,
// CHECK-NEXT:                     __i1,
// CHECK-NEXT:                     __i2,
// CHECK-NEXT:                     __i3,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_set1_epi32(__i: i32) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe { _mm_set_epi32(__i, __i, __i, __i) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_loadu_si256(
// CHECK-NEXT:     __p: *const std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_si256)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set_ps(
// CHECK-NEXT:     __a: f32,
// CHECK-NEXT:     __b: f32,
// CHECK-NEXT:     __c: f32,
// CHECK-NEXT:     __d: f32,
// CHECK-NEXT:     __e: f32,
// CHECK-NEXT:     __f: f32,
// CHECK-NEXT:     __g: f32,
// CHECK-NEXT:     __h: f32,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 __h,
// CHECK-NEXT:                 __g,
// CHECK-NEXT:                 __f,
// CHECK-NEXT:                 __e,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __c,
// CHECK-NEXT:                 __b,
// CHECK-NEXT:                 __a,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set_epi32(
// CHECK-NEXT:     __i0: i32,
// CHECK-NEXT:     __i1: i32,
// CHECK-NEXT:     __i2: i32,
// CHECK-NEXT:     __i3: i32,
// CHECK-NEXT:     __i4: i32,
// CHECK-NEXT:     __i5: i32,
// CHECK-NEXT:     __i6: i32,
// CHECK-NEXT:     __i7: i32,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __i7,
// CHECK-NEXT:                     __i6,
// CHECK-NEXT:                     __i5,
// CHECK-NEXT:                     __i4,
// CHECK-NEXT:                     __i3,
// CHECK-NEXT:                     __i2,
// CHECK-NEXT:                     __i1,
// CHECK-NEXT:                     __i0,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set_epi64x(
// CHECK-NEXT:     __a: i64,
// CHECK-NEXT:     __b: i64,
// CHECK-NEXT:     __c: i64,
// CHECK-NEXT:     __d: i64,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([__d, __c, __b, __a]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_ps(__w: f32) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { _mm256_set_ps(__w, __w, __w, __w, __w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_epi32(__i: i32) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi32(__i, __i, __i, __i, __i, __i, __i, __i) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_epi64x(__q: i64) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi64x(__q, __q, __q, __q) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_castsi256_si128(
// CHECK-NEXT:     mut __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, __a, [0, 1]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn _mm256_cvtepi32_epi64(
// CHECK-NEXT:     __V: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return std::simd::num::SimdInt::cast::<
// CHECK-NEXT:         i64,
// CHECK-NEXT:     >(unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i32, 4>>(__V)
// CHECK-NEXT:     });
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_castpd512_pd128(
// CHECK-NEXT:     mut __a: std::simd::Simd<f64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, __a, [0, 1]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_castpd512_pd256(
// CHECK-NEXT:     mut __A: std::simd::Simd<f64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__A, __A, [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_castps512_ps128(
// CHECK-NEXT:     mut __a: std::simd::Simd<f32, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, __a, [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_castps512_ps256(
// CHECK-NEXT:     mut __A: std::simd::Simd<f32, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__A, __A, [0, 1, 2, 3, 4, 5, 6, 7]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_loadu_pd(__p: *const ()) -> std::simd::Simd<f64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_pd)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_loadu_ps(__p: *const ()) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_ps)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn show(name: *const i8, data: *const (), bytes: i32) {
// CHECK-NEXT:     let p: *const u8 = data as *const u8;
// CHECK-NEXT:     let mut hash: u32 = 2166136261;
// CHECK-NEXT:     for i in 0..bytes {
// CHECK-NEXT:         hash = (hash ^ (((unsafe { *p.offset(i as isize) }) as i32) as u32))
// CHECK-NEXT:             .wrapping_mul(16777619);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s %08x\n".as_ptr(), name, hash) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f,avx512dq,avx512vl")]
// CHECK-NEXT: unsafe fn avx512(
// CHECK-NEXT:     fs: *const f32,
// CHECK-NEXT:     ds: *const f64,
// CHECK-NEXT:     table: *mut i32,
// CHECK-NEXT:     index: *const i32,
// CHECK-NEXT:     m: u16,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let a: std::simd::Simd<f32, 16> = unsafe { _mm512_loadu_ps(fs as *const ()) };
// CHECK-NEXT:     let b: std::simd::Simd<f64, 8> = unsafe { _mm512_loadu_pd(ds as *const ()) };
// CHECK-NEXT:     let c: std::simd::Simd<f64, 8> = unsafe { _mm512_loadu_pd(ds.add(8) as *const ()) };
// CHECK-NEXT:     CLASSES_PS!(a, m);
// CHECK-NEXT:     CLASSES_PD!(b, m as u8);
// CHECK-NEXT:     CLASSES_PD!(c, ((m as i32) >> 8i32) as u8);
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { _mm512_castps512_ps128(a) };
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: std::simd::Simd<f32, 8> = unsafe { _mm512_castps512_ps256(a) };
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { _mm512_castpd512_pd128(c) };
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: std::simd::Simd<f64, 4> = unsafe { _mm512_castpd512_pd256(b) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"ps128 %x ps256 %x pd128 %x pd256 %x\n".as_ptr(),
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_154:__t[0-9]+]]: std::simd::Simd<u32, 4> = (unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:                             >([[V_9]])
// CHECK-NEXT:                         }) & std::simd::Simd::splat(2147483647u32);
// CHECK-NEXT:                         let [[T_155:__t[0-9]+]] = [[T_154]].to_array();
// CHECK-NEXT:                         let [[T_156:__t[0-9]+]]: std::simd::Simd<u32, 4> = std::simd::Simd::splat(
// CHECK-NEXT:                             2143289344u32,
// CHECK-NEXT:                         );
// CHECK-NEXT:                         let [[T_157:__t[0-9]+]] = [[T_156]].to_array();
// CHECK-NEXT:                         let [[T_158:__t[0-9]+]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_155]][__lane] >= [[T_157]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_158]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | ({
// CHECK-NEXT:                             let [[T_159:__t[0-9]+]]: std::simd::Simd<u32, 4> = (unsafe {
// CHECK-NEXT:                                 std::mem::transmute::<
// CHECK-NEXT:                                     std::simd::Simd<f32, 4>,
// CHECK-NEXT:                                     std::simd::Simd<u32, 4>,
// CHECK-NEXT:                                 >([[V_9]])
// CHECK-NEXT:                             }) & std::simd::Simd::splat(2147483647u32);
// CHECK-NEXT:                             let [[T_160:__t[0-9]+]] = [[T_159]].to_array();
// CHECK-NEXT:                             let [[T_161:__t[0-9]+]]: std::simd::Simd<u32, 4> = std::simd::Simd::splat(
// CHECK-NEXT:                                 2139095040u32,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             let [[T_162:__t[0-9]+]] = [[T_161]].to_array();
// CHECK-NEXT:                             let [[T_163:__t[0-9]+]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_160]][__lane] > [[T_162]][__lane]) as i32)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_163]]
// CHECK-NEXT:                         })
// CHECK-NEXT:                             & {
// CHECK-NEXT:                                 let [[T_164:__t[0-9]+]]: std::simd::Simd<u32, 4> = (unsafe {
// CHECK-NEXT:                                     std::mem::transmute::<
// CHECK-NEXT:                                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                                         std::simd::Simd<u32, 4>,
// CHECK-NEXT:                                     >([[V_9]])
// CHECK-NEXT:                                 }) & std::simd::Simd::splat(2147483647u32);
// CHECK-NEXT:                                 let [[T_165:__t[0-9]+]] = [[T_164]].to_array();
// CHECK-NEXT:                                 let [[T_166:__t[0-9]+]]: std::simd::Simd<u32, 4> = std::simd::Simd::splat(
// CHECK-NEXT:                                     2143289344u32,
// CHECK-NEXT:                                 );
// CHECK-NEXT:                                 let [[T_167:__t[0-9]+]] = [[T_166]].to_array();
// CHECK-NEXT:                                 let [[T_168:__t[0-9]+]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                                         -(([[T_165]][__lane] < [[T_167]][__lane]) as i32)
// CHECK-NEXT:                                     }),
// CHECK-NEXT:                                 );
// CHECK-NEXT:                                 [[T_168]]
// CHECK-NEXT:                             },
// CHECK-NEXT:                     std::simd::Simd::<i32, 4>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u8) as i32,
// CHECK-NEXT:             ((std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_169:__t[0-9]+]]: std::simd::Simd<u32, 8> = unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:                                 std::simd::Simd<u32, 8>,
// CHECK-NEXT:                             >([[V_10]])
// CHECK-NEXT:                         };
// CHECK-NEXT:                         let [[T_170:__t[0-9]+]] = [[T_169]].to_array();
// CHECK-NEXT:                         let [[T_171:__t[0-9]+]]: std::simd::Simd<u32, 8> = std::simd::Simd::splat(
// CHECK-NEXT:                             0u32,
// CHECK-NEXT:                         );
// CHECK-NEXT:                         let [[T_172:__t[0-9]+]] = [[T_171]].to_array();
// CHECK-NEXT:                         let [[T_173:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_170]][__lane] == [[T_172]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_173]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_174:__t[0-9]+]]: std::simd::Simd<u32, 8> = unsafe {
// CHECK-NEXT:                                 std::mem::transmute::<
// CHECK-NEXT:                                     std::simd::Simd<f32, 8>,
// CHECK-NEXT:                                     std::simd::Simd<u32, 8>,
// CHECK-NEXT:                                 >([[V_10]])
// CHECK-NEXT:                             };
// CHECK-NEXT:                             let [[T_175:__t[0-9]+]] = [[T_174]].to_array();
// CHECK-NEXT:                             let [[T_176:__t[0-9]+]]: std::simd::Simd<u32, 8> = std::simd::Simd::splat(
// CHECK-NEXT:                                 2147483648u32,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             let [[T_177:__t[0-9]+]] = [[T_176]].to_array();
// CHECK-NEXT:                             let [[T_178:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_175]][__lane] == [[T_177]][__lane]) as i32)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_178]]
// CHECK-NEXT:                         }
// CHECK-NEXT:                         | ({
// CHECK-NEXT:                             let [[T_179:__t[0-9]+]]: std::simd::Simd<u32, 8> = (unsafe {
// CHECK-NEXT:                                 std::mem::transmute::<
// CHECK-NEXT:                                     std::simd::Simd<f32, 8>,
// CHECK-NEXT:                                     std::simd::Simd<u32, 8>,
// CHECK-NEXT:                                 >([[V_10]])
// CHECK-NEXT:                             }) & std::simd::Simd::splat(2147483647u32);
// CHECK-NEXT:                             let [[T_180:__t[0-9]+]] = [[T_179]].to_array();
// CHECK-NEXT:                             let [[T_181:__t[0-9]+]]: std::simd::Simd<u32, 8> = std::simd::Simd::splat(
// CHECK-NEXT:                                 0u32,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             let [[T_182:__t[0-9]+]] = [[T_181]].to_array();
// CHECK-NEXT:                             let [[T_183:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_180]][__lane] != [[T_182]][__lane]) as i32)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_183]]
// CHECK-NEXT:                         })
// CHECK-NEXT:                             & {
// CHECK-NEXT:                                 let [[T_184:__t[0-9]+]]: std::simd::Simd<u32, 8> = (unsafe {
// CHECK-NEXT:                                     std::mem::transmute::<
// CHECK-NEXT:                                         std::simd::Simd<f32, 8>,
// CHECK-NEXT:                                         std::simd::Simd<u32, 8>,
// CHECK-NEXT:                                     >([[V_10]])
// CHECK-NEXT:                                 }) & std::simd::Simd::splat(2147483647u32);
// CHECK-NEXT:                                 let [[T_185:__t[0-9]+]] = [[T_184]].to_array();
// CHECK-NEXT:                                 let [[T_186:__t[0-9]+]]: std::simd::Simd<u32, 8> = std::simd::Simd::splat(
// CHECK-NEXT:                                     8388608u32,
// CHECK-NEXT:                                 );
// CHECK-NEXT:                                 let [[T_187:__t[0-9]+]] = [[T_186]].to_array();
// CHECK-NEXT:                                 let [[T_188:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                                         -(([[T_185]][__lane] < [[T_187]][__lane]) as i32)
// CHECK-NEXT:                                     }),
// CHECK-NEXT:                                 );
// CHECK-NEXT:                                 [[T_188]]
// CHECK-NEXT:                             },
// CHECK-NEXT:                     std::simd::Simd::<i32, 8>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u8) & (m as u8)) as i32,
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_189:__t[0-9]+]]: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:                             >([[V_11]])
// CHECK-NEXT:                         };
// CHECK-NEXT:                         let [[T_190:__t[0-9]+]] = [[T_189]].to_array();
// CHECK-NEXT:                         let [[T_191:__t[0-9]+]]: std::simd::Simd<u64, 2> = std::simd::Simd::splat(
// CHECK-NEXT:                             9218868437227405312u64,
// CHECK-NEXT:                         );
// CHECK-NEXT:                         let [[T_192:__t[0-9]+]] = [[T_191]].to_array();
// CHECK-NEXT:                         let [[T_193:__t[0-9]+]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_190]][__lane] == [[T_192]][__lane]) as i64)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_193]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_194:__t[0-9]+]]: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:                                 std::mem::transmute::<
// CHECK-NEXT:                                     std::simd::Simd<f64, 2>,
// CHECK-NEXT:                                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                                 >([[V_11]])
// CHECK-NEXT:                             };
// CHECK-NEXT:                             let [[T_195:__t[0-9]+]] = [[T_194]].to_array();
// CHECK-NEXT:                             let [[T_196:__t[0-9]+]]: std::simd::Simd<u64, 2> = std::simd::Simd::splat(
// CHECK-NEXT:                                 18442240474082181120u64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             let [[T_197:__t[0-9]+]] = [[T_196]].to_array();
// CHECK-NEXT:                             let [[T_198:__t[0-9]+]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_195]][__lane] == [[T_197]][__lane]) as i64)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_198]]
// CHECK-NEXT:                         },
// CHECK-NEXT:                     std::simd::Simd::<i64, 2>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u8) as i32,
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_199:__t[0-9]+]]: std::simd::Simd<u64, 4> = unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<f64, 4>,
// CHECK-NEXT:                                 std::simd::Simd<u64, 4>,
// CHECK-NEXT:                             >([[V_12]])
// CHECK-NEXT:                         };
// CHECK-NEXT:                         let [[T_200:__t[0-9]+]] = [[T_199]].to_array();
// CHECK-NEXT:                         let [[T_201:__t[0-9]+]]: std::simd::Simd<u64, 4> = std::simd::Simd::splat(
// CHECK-NEXT:                             0u64,
// CHECK-NEXT:                         );
// CHECK-NEXT:                         let [[T_202:__t[0-9]+]] = [[T_201]].to_array();
// CHECK-NEXT:                         let [[T_203:__t[0-9]+]]: std::simd::Simd<i64, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_200]][__lane] == [[T_202]][__lane]) as i64)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_203]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_204:__t[0-9]+]]: std::simd::Simd<u64, 4> = unsafe {
// CHECK-NEXT:                                 std::mem::transmute::<
// CHECK-NEXT:                                     std::simd::Simd<f64, 4>,
// CHECK-NEXT:                                     std::simd::Simd<u64, 4>,
// CHECK-NEXT:                                 >([[V_12]])
// CHECK-NEXT:                             };
// CHECK-NEXT:                             let [[T_205:__t[0-9]+]] = [[T_204]].to_array();
// CHECK-NEXT:                             let [[T_206:__t[0-9]+]]: std::simd::Simd<u64, 4> = std::simd::Simd::splat(
// CHECK-NEXT:                                 9223372036854775808u64,
// CHECK-NEXT:                             );
// CHECK-NEXT:                             let [[T_207:__t[0-9]+]] = [[T_206]].to_array();
// CHECK-NEXT:                             let [[T_208:__t[0-9]+]]: std::simd::Simd<i64, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_205]][__lane] == [[T_207]][__lane]) as i64)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_208]]
// CHECK-NEXT:                         },
// CHECK-NEXT:                     std::simd::Simd::<i64, 4>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u8) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let idx: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(index as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     let old: std::simd::Simd<i64, 4> = unsafe { _mm256_set1_epi32(-1i32) };
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"gather3siv8si".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 8 >, std::simd::Simd < i64, 4 >> (unsafe {
// CHECK-NEXT:         __slate_intrinsic_704989ab100419e7(unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 4 >, std::simd::Simd < i32, 8 >> (old) }, table as * const
// CHECK-NEXT:         (), unsafe { std::mem::transmute::< std::simd::Simd < i64, 4 >, std::simd::Simd <
// CHECK-NEXT:         i32, 8 >> (idx) }, m as u8, const { 4 as i32 }) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"gather3siv8sf".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         __slate_intrinsic_a7ad3f7c58ba8c0d(_mm256_set1_ps(- 1i32 as f32), table as *
// CHECK-NEXT:         const (), unsafe { std::mem::transmute::< std::simd::Simd < i64, 4 >,
// CHECK-NEXT:         std::simd::Simd < i32, 8 >> (idx) }, m as u8, const { 4 as i32 }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"gather3siv4si".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 4 >, std::simd::Simd < i64, 2 >> (unsafe {
// CHECK-NEXT:         __slate_intrinsic_ce634a86dfaf8f6d(unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i32, 4 >> (_mm_set1_epi32(7i32)) },
// CHECK-NEXT:         table as * const (), unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >,
// CHECK-NEXT:         std::simd::Simd < i32, 4 >> (_mm256_castsi256_si128(idx)) }, m as u8, const { 4
// CHECK-NEXT:         as i32 }) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"gather3div4di".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         __slate_intrinsic_276a67a3279ccf52(_mm256_set1_epi64x(9i64), table as * const (),
// CHECK-NEXT:         _mm256_cvtepi32_epi64(_mm256_castsi256_si128(idx)), m as u8, const { 4 as i32 })
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_6d0a0c56eeee8011(
// CHECK-NEXT:             table as *mut (),
// CHECK-NEXT:             ((m as i32) >> 3i32) as u8,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 4>,
// CHECK-NEXT:                     std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 >(idx)
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 4>,
// CHECK-NEXT:                     std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 >(_mm256_set1_epi32(1000 + (m as i32)))
// CHECK-NEXT:             },
// CHECK-NEXT:             const { (4 as i32) as i32 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_d4aec945be1d3a97(
// CHECK-NEXT:             table.add(32) as *mut (),
// CHECK-NEXT:             m as u8,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 >(_mm256_castsi256_si128(idx))
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 >(_mm_set1_epi32(-5i32))
// CHECK-NEXT:             },
// CHECK-NEXT:             const { (4 as i32) as i32 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"scattered".as_ptr(),
// CHECK-NEXT:         table as *const (),
// CHECK-NEXT:         64u64.wrapping_mul(std::mem::size_of::<i32>() as u64) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut fs: __SlateAlign16<[f32; 16]> = __SlateAlign16([
// CHECK-NEXT:         1.5f32,
// CHECK-NEXT:         -2.5f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         -f32::from_bits(0x7fc00000),
// CHECK-NEXT:         0.0f32,
// CHECK-NEXT:         -0.0f32,
// CHECK-NEXT:         f32::INFINITY,
// CHECK-NEXT:         -f32::INFINITY,
// CHECK-NEXT:         1e-40f32,
// CHECK-NEXT:         -1e-40f32,
// CHECK-NEXT:         3.4e38f32,
// CHECK-NEXT:         -1.2e-38f32,
// CHECK-NEXT:         1.1754944e-38f32,
// CHECK-NEXT:         -7.0f32,
// CHECK-NEXT:         0i32 as f32,
// CHECK-NEXT:         0i32 as f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut snan: u32 = 2141192192;
// CHECK-NEXT:     let mut nsnan: u32 = 4286578689;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(snan) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             (((fs.0.as_mut_ptr() as *mut f32).add(14) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (4u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(nsnan) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             (((fs.0.as_mut_ptr() as *mut f32).add(15) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (4u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut ds: __SlateAlign16<[f64; 16]> = __SlateAlign16([
// CHECK-NEXT:         1.5f64,
// CHECK-NEXT:         -2.5f64,
// CHECK-NEXT:         f32::from_bits(0x7fc00000) as f64,
// CHECK-NEXT:         -0.0f64,
// CHECK-NEXT:         0.0f64,
// CHECK-NEXT:         f32::INFINITY as f64,
// CHECK-NEXT:         -f32::INFINITY as f64,
// CHECK-NEXT:         5e-324f64,
// CHECK-NEXT:         -5e-324f64,
// CHECK-NEXT:         2.2250738585072014e-308f64,
// CHECK-NEXT:         -1e308f64,
// CHECK-NEXT:         7i32 as f64,
// CHECK-NEXT:         0i32 as f64,
// CHECK-NEXT:         0i32 as f64,
// CHECK-NEXT:         -3i32 as f64,
// CHECK-NEXT:         0i32 as f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut dsnan: u64 = 9219994337134247936;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(dsnan) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             (((ds.0.as_mut_ptr() as *mut f64).add(12) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (8u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     ds.0[13] = -f32::from_bits(0x7fc00000) as f64;
// CHECK-NEXT:     let mut table: __SlateAlign16<[i32; 64]> = __SlateAlign16([0i32; 64]);
// CHECK-NEXT:     let mut index: __SlateAlign16<[i32; 8]> = __SlateAlign16([
// CHECK-NEXT:         3i32, 0i32, 17i32, 9i32, 31i32, 5i32, 12i32, 2i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     for i in 0i32..64i32 {
// CHECK-NEXT:         table.0[i as usize] = i * 37 - 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx512f")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("avx512dq")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("avx512vl")
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             avx512(
// CHECK-NEXT:                 fs.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                 ds.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:                 table.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 index.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:                 65535u16,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             avx512(
// CHECK-NEXT:                 fs.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                 ds.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:                 table.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 index.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:                 42435u16,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             avx512(
// CHECK-NEXT:                 fs.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                 ds.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:                 table.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 index.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:                 0u16,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
