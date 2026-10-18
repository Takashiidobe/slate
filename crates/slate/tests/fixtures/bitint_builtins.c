#include <stdio.h>

#define OVF(NAME, A, B, R, FMT, CAST)                                         \
  static void NAME(A a, B b) {                                                \
    R r; int o;                                                               \
    o = __builtin_add_overflow(a, b, &r); printf(FMT " %d ", (CAST)r, o);     \
    o = __builtin_sub_overflow(a, b, &r); printf(FMT " %d ", (CAST)r, o);     \
    o = __builtin_mul_overflow(a, b, &r); printf(FMT " %d\n", (CAST)r, o);    \
  }

#define OVF2(NAME, A, B, R, FMT, CAST)                                        \
  static void NAME(A a, B b) {                                                \
    R r; int o;                                                               \
    o = __builtin_add_overflow(a, b, &r); printf(FMT " %d ", (CAST)r, o);     \
    o = __builtin_sub_overflow(a, b, &r); printf(FMT " %d\n", (CAST)r, o);    \
  }

OVF(s5, _BitInt(5), _BitInt(5), _BitInt(5), "%lld", long long)
OVF(u5, unsigned _BitInt(5), unsigned _BitInt(5), unsigned _BitInt(5), "%llu", unsigned long long)
OVF(s9_mixed, _BitInt(9), unsigned _BitInt(7), _BitInt(12), "%lld", long long)
OVF(s33, _BitInt(33), _BitInt(33), _BitInt(33), "%lld", long long)
OVF(s64, _BitInt(64), _BitInt(64), _BitInt(64), "%lld", long long)
OVF(s100, _BitInt(100), _BitInt(100), _BitInt(100), "%lld", long long)
OVF(u100, unsigned _BitInt(100), unsigned _BitInt(100), unsigned _BitInt(100), "%llu", unsigned long long)
OVF2(s200, _BitInt(200), _BitInt(200), _BitInt(200), "%lld", long long)
OVF2(u200_to_s, unsigned _BitInt(200), _BitInt(150), _BitInt(70), "%lld", long long)
OVF2(s200_to_int, _BitInt(200), int, int, "%lld", long long)
OVF2(int_to_s200, int, long long, _BitInt(200), "%lld", long long)
OVF(b128, _BitInt(128), _BitInt(128), _BitInt(128), "%lld", long long)

#define COUNTS(NAME, T, SIGNED)                                               \
  static void NAME(T a) {                                                     \
    printf("%d ", __builtin_popcountg(a));                                    \
    if (a) printf("%d %d ", __builtin_clzg(a), __builtin_ctzg(a));            \
    printf("%d %d\n", __builtin_clzg(a, -1), __builtin_ctzg(a, -2));          \
  }
COUNTS(c_u5, unsigned _BitInt(5), 0)
COUNTS(c_u9, unsigned _BitInt(9), 0)
COUNTS(c_u33, unsigned _BitInt(33), 0)
COUNTS(c_u64, unsigned _BitInt(64), 0)
COUNTS(c_u100, unsigned _BitInt(100), 0)
COUNTS(c_u200, unsigned _BitInt(200), 0)
COUNTS(c_uint, unsigned int, 0)

#define SAT(NAME, T, FMT, CAST)                                               \
  static void NAME(T a, T b) {                                                \
    T x = __builtin_elementwise_add_sat(a, b);                                \
    T y = __builtin_elementwise_sub_sat(a, b);                                \
    printf(FMT " " FMT "\n", (CAST)x, (CAST)y);                               \
  }

SAT(sat_i, int, "%lld", long long)
SAT(sat_s5, _BitInt(5), "%lld", long long)
SAT(sat_u5, unsigned _BitInt(5), "%llu", unsigned long long)
SAT(sat_s9, _BitInt(9), "%lld", long long)
SAT(sat_s100, _BitInt(100), "%lld", long long)
SAT(sat_u100, unsigned _BitInt(100), "%llu", unsigned long long)
SAT(sat_s200, _BitInt(200), "%lld", long long)

int main(void) {
  s5(15, 1); s5(-16, 1); s5(7, 7); u5(31, 1); u5(0, 1); u5(9, 9);
  s9_mixed(255, 127); s9_mixed(-256, 100);
  s33(4294967295LL, 4294967295LL); s33(-4294967296LL, 1);
  s64(9223372036854775807LL, 1); s64(-9223372036854775807LL - 1, 2);
  s100(9223372036854775807LL, 9223372036854775807LL);
  u100(18446744073709551615ULL, 18446744073709551615ULL);
  s200(9223372036854775807LL, -9223372036854775807LL);
  u200_to_s(1000, -5); u200_to_s(0, 1);
  s200_to_int(2147483647, 1); s200_to_int(5, 6);
  int_to_s200(2147483647, 2147483647); b128(1, 2);
  c_u5(0); c_u5(1); c_u5(22); c_u9(256); c_u33(8589934591ULL);
  c_u64(0x8000000000000000ULL); c_u100(0); c_u100(12345678901234567ULL);
  c_u200(1); c_u200(3000000000000ULL); c_uint(40);
  sat_i(2147483647, 5); sat_i(-2147483647 - 1, 5);
  sat_s5(15, 5); sat_s5(-16, 5); sat_u5(30, 5); sat_u5(3, 5);
  sat_s9(255, 5); sat_s9(-256, 5); sat_s100(9223372036854775807LL, 5);
  sat_u100(3, 5); sat_s200(-5, 7);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn s5(a: i8, b: i8) {
// CHECK-NEXT:     let mut r: i8 = 0;
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_2]] as i8) << 3 >> 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || ((([[T_2]] as i8) << 3 >> 3) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: i128 = [[T_3]].wrapping_sub([[T_4]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_5]] as i8) << 3 >> 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_3]].checked_sub([[T_4]]).is_none() || ((([[T_5]] as i8) << 3 >> 3) as i128) != [[T_5]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_6:__t[0-9]+]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_7:__t[0-9]+]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_8:__t[0-9]+]]: i128 = [[T_6]].wrapping_mul([[T_7]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_8]] as i8) << 3 >> 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].checked_mul([[T_7]]).is_none() || ((([[T_8]] as i8) << 3 >> 3) as i128) != [[T_8]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u5(a: u8, b: u8) {
// CHECK-NEXT:     let mut r: u8 = 0;
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_2]] as u8) & 31;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || ((([[T_2]] as u8) & 31) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %d ".as_ptr(), r as u64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_3]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_4]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_5]]: i128 = [[T_3]].wrapping_sub([[T_4]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_5]] as u8) & 31;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_3]].checked_sub([[T_4]]).is_none() || ((([[T_5]] as u8) & 31) as i128) != [[T_5]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %d ".as_ptr(), r as u64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_6]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_7]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_8]]: i128 = [[T_6]].wrapping_mul([[T_7]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_8]] as u8) & 31;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].checked_mul([[T_7]]).is_none() || ((([[T_8]] as u8) & 31) as i128) != [[T_8]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %d\n".as_ptr(), r as u64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s9_mixed(a: i16, b: u8) {
// CHECK-NEXT:     let mut r: i16 = 0;
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_2]] as i16) << 4 >> 4;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || ((([[T_2]] as i16) << 4 >> 4) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_3]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_4]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_5]]: i128 = [[T_3]].wrapping_sub([[T_4]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_5]] as i16) << 4 >> 4;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_3]].checked_sub([[T_4]]).is_none() || ((([[T_5]] as i16) << 4 >> 4) as i128) != [[T_5]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_6]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_7]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_8]]: i128 = [[T_6]].wrapping_mul([[T_7]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_8]] as i16) << 4 >> 4;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].checked_mul([[T_7]]).is_none() || ((([[T_8]] as i16) << 4 >> 4) as i128) != [[T_8]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s33(a: i64, b: i64) {
// CHECK-NEXT:     let mut r: i64 = 0;
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_2]] as i64) << 31 >> 31;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || ((([[T_2]] as i64) << 31 >> 31) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_3]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_4]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_5]]: i128 = [[T_3]].wrapping_sub([[T_4]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_5]] as i64) << 31 >> 31;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_3]].checked_sub([[T_4]]).is_none() || ((([[T_5]] as i64) << 31 >> 31) as i128) != [[T_5]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_6]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_7]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_8]]: i128 = [[T_6]].wrapping_mul([[T_7]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = ([[T_8]] as i64) << 31 >> 31;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].checked_mul([[T_7]]).is_none() || ((([[T_8]] as i64) << 31 >> 31) as i128) != [[T_8]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s64(a: i64, b: i64) {
// CHECK-NEXT:     let mut r: i64 = 0;
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_2]] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as i64) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_3]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_4]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_5]]: i128 = [[T_3]].wrapping_sub([[T_4]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_5]] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_3]].checked_sub([[T_4]]).is_none() || (([[T_5]] as i64) as i128) != [[T_5]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_6]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_7]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_8]]: i128 = [[T_6]].wrapping_mul([[T_7]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_8]] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].checked_mul([[T_7]]).is_none() || (([[T_8]] as i64) as i128) != [[T_8]]
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s100(
// CHECK-NEXT:     a: bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BInt<100, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_1]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_2]]: bitint::BInt<202, 4, 32, bitint::A8> = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         let [[T_3]]: bitint::BInt<100, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_2]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].compare(<bitint::BInt<202, 4, 32, bitint::A8>>::from_bint([[T_3]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_4]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_5]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_6]]: bitint::BInt<202, 4, 32, bitint::A8> = [[T_4]].wrapping_sub([[T_5]]);
// CHECK-NEXT:         let [[T_7]]: bitint::BInt<100, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_6]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].compare(<bitint::BInt<202, 4, 32, bitint::A8>>::from_bint([[T_7]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_8]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_9:__t[0-9]+]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_10:__t[0-9]+]]: bitint::BInt<202, 4, 32, bitint::A8> = [[T_8]].wrapping_mul([[T_9]]);
// CHECK-NEXT:         let [[T_11:__t[0-9]+]]: bitint::BInt<100, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_10]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_11]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_10]].compare(<bitint::BInt<202, 4, 32, bitint::A8>>::from_bint([[T_11]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u100(
// CHECK-NEXT:     a: bitint::BUint<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     b: bitint::BUint<100, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BUint<100, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_buint(a);
// CHECK-NEXT:         let [[T_1]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_buint(b);
// CHECK-NEXT:         let [[T_2]]: bitint::BInt<202, 4, 32, bitint::A8> = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         let [[T_3]]: bitint::BUint<100, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_2]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].compare(<bitint::BInt<202, 4, 32, bitint::A8>>::from_buint([[T_3]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %d ".as_ptr(), r.to_u128() as u64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_4]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_buint(a);
// CHECK-NEXT:         let [[T_5]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_buint(b);
// CHECK-NEXT:         let [[T_6]]: bitint::BInt<202, 4, 32, bitint::A8> = [[T_4]].wrapping_sub([[T_5]]);
// CHECK-NEXT:         let [[T_7]]: bitint::BUint<100, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_6]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].compare(<bitint::BInt<202, 4, 32, bitint::A8>>::from_buint([[T_7]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %d ".as_ptr(), r.to_u128() as u64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_8]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_buint(a);
// CHECK-NEXT:         let [[T_9]]: bitint::BInt<202, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             202,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_buint(b);
// CHECK-NEXT:         let [[T_10]]: bitint::BInt<202, 4, 32, bitint::A8> = [[T_8]].wrapping_mul([[T_9]]);
// CHECK-NEXT:         let [[T_11]]: bitint::BUint<100, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_10]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_11]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_10]].compare(<bitint::BInt<202, 4, 32, bitint::A8>>::from_buint([[T_11]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %d\n".as_ptr(), r.to_u128() as u64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s200(
// CHECK-NEXT:     a: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BInt<200, 4, 32, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_1]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_2]]: bitint::BInt<402, 7, 56, bitint::A8> = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         let [[T_3]]: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_2]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].compare(<bitint::BInt<402, 7, 56, bitint::A8>>::from_bint([[T_3]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_4]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_5]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_6]]: bitint::BInt<402, 7, 56, bitint::A8> = [[T_4]].wrapping_sub([[T_5]]);
// CHECK-NEXT:         let [[T_7]]: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_6]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].compare(<bitint::BInt<402, 7, 56, bitint::A8>>::from_bint([[T_7]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn u200_to_s(
// CHECK-NEXT:     a: bitint::BUint<200, 4, 32, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<150, 3, 24, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BInt<70, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_buint(a);
// CHECK-NEXT:         let [[T_1]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_2]]: bitint::BInt<402, 7, 56, bitint::A8> = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         let [[T_3]]: bitint::BInt<70, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             70,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_2]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].compare(<bitint::BInt<402, 7, 56, bitint::A8>>::from_bint([[T_3]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_4]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_buint(a);
// CHECK-NEXT:         let [[T_5]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_6]]: bitint::BInt<402, 7, 56, bitint::A8> = [[T_4]].wrapping_sub([[T_5]]);
// CHECK-NEXT:         let [[T_7]]: bitint::BInt<70, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             70,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_6]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].compare(<bitint::BInt<402, 7, 56, bitint::A8>>::from_bint([[T_7]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn s200_to_int(a: bitint::BInt<200, 4, 32, bitint::A8>, b: i32) {
// CHECK-NEXT:     let mut r: i32 = 0;
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_1]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(b as i128);
// CHECK-NEXT:         let [[T_2]]: bitint::BInt<402, 7, 56, bitint::A8> = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         let [[T_3]]: i32 = [[T_2]].to_i128() as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].compare(<bitint::BInt<402, 7, 56, bitint::A8>>::from_i128([[T_3]] as i128))
// CHECK-NEXT:             != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_4]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_5]]: bitint::BInt<402, 7, 56, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             402,
// CHECK-NEXT:             7,
// CHECK-NEXT:             56,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(b as i128);
// CHECK-NEXT:         let [[T_6]]: bitint::BInt<402, 7, 56, bitint::A8> = [[T_4]].wrapping_sub([[T_5]]);
// CHECK-NEXT:         let [[T_7]]: i32 = [[T_6]].to_i128() as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].compare(<bitint::BInt<402, 7, 56, bitint::A8>>::from_i128([[T_7]] as i128))
// CHECK-NEXT:             != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn int_to_s200(a: i32, b: i64) {
// CHECK-NEXT:     let mut r: bitint::BInt<200, 4, 32, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: bitint::BInt<201, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             201,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(a as i128);
// CHECK-NEXT:         let [[T_1]]: bitint::BInt<201, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             201,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(b as i128);
// CHECK-NEXT:         let [[T_2]]: bitint::BInt<201, 4, 32, bitint::A8> = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         let [[T_3]]: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_2]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].compare(<bitint::BInt<201, 4, 32, bitint::A8>>::from_bint([[T_3]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_4]]: bitint::BInt<201, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             201,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(a as i128);
// CHECK-NEXT:         let [[T_5]]: bitint::BInt<201, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             201,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(b as i128);
// CHECK-NEXT:         let [[T_6]]: bitint::BInt<201, 4, 32, bitint::A8> = [[T_4]].wrapping_sub([[T_5]]);
// CHECK-NEXT:         let [[T_7]]: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_6]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].compare(<bitint::BInt<201, 4, 32, bitint::A8>>::from_bint([[T_7]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn b128(
// CHECK-NEXT:     a: bitint::BInt<128, 2, 16, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<128, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut r: bitint::BInt<128, 2, 16, bitint::A8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_1]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_2]]: bitint::BInt<258, 5, 40, bitint::A8> = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         let [[T_3]]: bitint::BInt<128, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             128,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_2]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].compare(<bitint::BInt<258, 5, 40, bitint::A8>>::from_bint([[T_3]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_4]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_5]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_6]]: bitint::BInt<258, 5, 40, bitint::A8> = [[T_4]].wrapping_sub([[T_5]]);
// CHECK-NEXT:         let [[T_7]]: bitint::BInt<128, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             128,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_6]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].compare(<bitint::BInt<258, 5, 40, bitint::A8>>::from_bint([[T_7]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d ".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_8]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(a);
// CHECK-NEXT:         let [[T_9]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(b);
// CHECK-NEXT:         let [[T_10]]: bitint::BInt<258, 5, 40, bitint::A8> = [[T_8]].wrapping_mul([[T_9]]);
// CHECK-NEXT:         let [[T_11]]: bitint::BInt<128, 2, 16, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             128,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint([[T_10]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_11]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_10]].compare(<bitint::BInt<258, 5, 40, bitint::A8>>::from_bint([[T_11]])) != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %d\n".as_ptr(), r.to_i128() as i64, o) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn c_u5(a: u8) {
// CHECK-NEXT:     print!("{} ", ((a as u8) & 31).count_ones() as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     if a != 0 {
// CHECK-NEXT:         print!(
// CHECK-NEXT:             "{} {} ", (((a as u8) & 31).leading_zeros() - 3) as i32, ((a as u8) & 31)
// CHECK-NEXT:             .trailing_zeros().min(5 as u32) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", { let [[T_0]] = a; let [[T_1]] = - (1 as i32); if [[T_0]] == 0 { [[T_1]] } else {
// CHECK-NEXT:         ((([[T_0]] as u8) & 31).leading_zeros() - 3) as i32 } }, { let [[T_2]] = a; let [[T_3]] =
// CHECK-NEXT:         - (2 as i32); if [[T_2]] == 0 { [[T_3]] } else { (([[T_2]] as u8) & 31).trailing_zeros()
// CHECK-NEXT:         .min(5 as u32) as i32 } }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn c_u9(a: u16) {
// CHECK-NEXT:     print!("{} ", ((a as u16) & 511).count_ones() as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     if a != 0 {
// CHECK-NEXT:         print!(
// CHECK-NEXT:             "{} {} ", (((a as u16) & 511).leading_zeros() - 7) as i32, ((a as u16) & 511)
// CHECK-NEXT:             .trailing_zeros().min(9 as u32) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", { let [[T_0]] = a; let [[T_1]] = - (1 as i32); if [[T_0]] == 0 { [[T_1]] } else {
// CHECK-NEXT:         ((([[T_0]] as u16) & 511).leading_zeros() - 7) as i32 } }, { let [[T_2]] = a; let [[T_3]]
// CHECK-NEXT:         = - (2 as i32); if [[T_2]] == 0 { [[T_3]] } else { (([[T_2]] as u16) & 511)
// CHECK-NEXT:         .trailing_zeros().min(9 as u32) as i32 } }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn c_u33(a: u64) {
// CHECK-NEXT:     print!("{} ", ((a as u64) & 8589934591).count_ones() as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     if a != 0 {
// CHECK-NEXT:         print!(
// CHECK-NEXT:             "{} {} ", (((a as u64) & 8589934591).leading_zeros() - 31) as i32, ((a as
// CHECK-NEXT:             u64) & 8589934591).trailing_zeros().min(33 as u32) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", { let [[T_0]] = a; let [[T_1]] = - (1 as i32); if [[T_0]] == 0 { [[T_1]] } else {
// CHECK-NEXT:         ((([[T_0]] as u64) & 8589934591).leading_zeros() - 31) as i32 } }, { let [[T_2]] = a;
// CHECK-NEXT:         let [[T_3]] = - (2 as i32); if [[T_2]] == 0 { [[T_3]] } else { (([[T_2]] as u64) &
// CHECK-NEXT:         8589934591).trailing_zeros().min(33 as u32) as i32 } }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn c_u64(a: u64) {
// CHECK-NEXT:     print!("{} ", a.count_ones() as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     if a != 0 {
// CHECK-NEXT:         print!("{} {} ", a.leading_zeros() as i32, a.trailing_zeros() as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", { let [[T_0]] = a; let [[T_1]] = - (1 as i32); if [[T_0]] == 0 { [[T_1]] } else {
// CHECK-NEXT:         [[T_0]].leading_zeros() as i32 } }, { let [[T_2]] = a; let [[T_3]] = - (2 as i32); if [[T_2]]
// CHECK-NEXT:         == 0 { [[T_3]] } else { [[T_2]].trailing_zeros() as i32 } }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn c_u100(mut a: bitint::BUint<100, 2, 16, bitint::A8>) {
// CHECK-NEXT:     print!("{} ", a.count_ones() as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     if a.compare(<bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         print!("{} {} ", a.leading_zeros() as i32, a.trailing_zeros() as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", { let [[T_0]] = a; let [[T_1]] = - (1 as i32); if [[T_0]] == < bitint::BUint <
// CHECK-NEXT:         100, 2, 16, bitint::A8 >>::from_u128(0) { [[T_1]] } else { [[T_0]].leading_zeros() as
// CHECK-NEXT:         i32 } }, { let [[T_2]] = a; let [[T_3]] = - (2 as i32); if [[T_2]] == < bitint::BUint <
// CHECK-NEXT:         100, 2, 16, bitint::A8 >>::from_u128(0) { [[T_3]] } else { [[T_2]].trailing_zeros() as
// CHECK-NEXT:         i32 } }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn c_u200(mut a: bitint::BUint<200, 4, 32, bitint::A8>) {
// CHECK-NEXT:     print!("{} ", a.count_ones() as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     if a.compare(<bitint::BUint<200, 4, 32, bitint::A8>>::from_u128(0)) != 0 {
// CHECK-NEXT:         print!("{} {} ", a.leading_zeros() as i32, a.trailing_zeros() as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", { let [[T_0]] = a; let [[T_1]] = - (1 as i32); if [[T_0]] == < bitint::BUint <
// CHECK-NEXT:         200, 4, 32, bitint::A8 >>::from_u128(0) { [[T_1]] } else { [[T_0]].leading_zeros() as
// CHECK-NEXT:         i32 } }, { let [[T_2]] = a; let [[T_3]] = - (2 as i32); if [[T_2]] == < bitint::BUint <
// CHECK-NEXT:         200, 4, 32, bitint::A8 >>::from_u128(0) { [[T_3]] } else { [[T_2]].trailing_zeros() as
// CHECK-NEXT:         i32 } }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn c_uint(a: u32) {
// CHECK-NEXT:     print!("{} ", a.count_ones() as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     if a != 0 {
// CHECK-NEXT:         print!("{} {} ", a.leading_zeros() as i32, a.trailing_zeros() as i32);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", { let [[T_0]] = a; let [[T_1]] = - (1 as i32); if [[T_0]] == 0 { [[T_1]] } else {
// CHECK-NEXT:         [[T_0]].leading_zeros() as i32 } }, { let [[T_2]] = a; let [[T_3]] = - (2 as i32); if [[T_2]]
// CHECK-NEXT:         == 0 { [[T_3]] } else { [[T_2]].trailing_zeros() as i32 } }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_i(a: i32, b: i32) {
// CHECK-NEXT:     let x: i32 = a.saturating_add(b);
// CHECK-NEXT:     let y: i32 = a.saturating_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), x as i64, y as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_s5(a: i8, b: i8) {
// CHECK-NEXT:     let x: i8 = a.saturating_add(b).clamp(-16 as i8, 15 as i8);
// CHECK-NEXT:     let y: i8 = a.saturating_sub(b).clamp(-16 as i8, 15 as i8);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), x as i64, y as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_u5(a: u8, b: u8) {
// CHECK-NEXT:     let x: u8 = a.saturating_add(b).clamp(0 as u8, 31 as u8);
// CHECK-NEXT:     let y: u8 = a.saturating_sub(b).clamp(0 as u8, 31 as u8);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %llu\n".as_ptr(), x as u64, y as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_s9(a: i16, b: i16) {
// CHECK-NEXT:     let x: i16 = a.saturating_add(b).clamp(-256 as i16, 255 as i16);
// CHECK-NEXT:     let y: i16 = a.saturating_sub(b).clamp(-256 as i16, 255 as i16);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), x as i64, y as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_s100(
// CHECK-NEXT:     mut a: bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut x: bitint::BInt<100, 2, 16, bitint::A8> = a.saturating_add(b);
// CHECK-NEXT:     let mut y: bitint::BInt<100, 2, 16, bitint::A8> = a.saturating_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), x.to_i128() as i64, y.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_u100(
// CHECK-NEXT:     mut a: bitint::BUint<100, 2, 16, bitint::A8>,
// CHECK-NEXT:     b: bitint::BUint<100, 2, 16, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut x: bitint::BUint<100, 2, 16, bitint::A8> = a.saturating_add(b);
// CHECK-NEXT:     let mut y: bitint::BUint<100, 2, 16, bitint::A8> = a.saturating_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %llu\n".as_ptr(), x.to_u128() as u64, y.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn sat_s200(
// CHECK-NEXT:     mut a: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut x: bitint::BInt<200, 4, 32, bitint::A8> = a.saturating_add(b);
// CHECK-NEXT:     let mut y: bitint::BInt<200, 4, 32, bitint::A8> = a.saturating_sub(b);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld %lld\n".as_ptr(), x.to_i128() as i64, y.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     s5((15 as i8) << 3 >> 3, (1 as i8) << 3 >> 3);
// CHECK-NEXT:     s5((-16 as i8) << 3 >> 3, (1 as i8) << 3 >> 3);
// CHECK-NEXT:     s5((7 as i8) << 3 >> 3, (7 as i8) << 3 >> 3);
// CHECK-NEXT:     u5((((31 as i8) << 3 >> 3) as u8) & 31, (((1 as i8) << 3 >> 3) as u8) & 31);
// CHECK-NEXT:     u5((((0 as i8) << 3 >> 3) as u8) & 31, (((1 as i8) << 3 >> 3) as u8) & 31);
// CHECK-NEXT:     u5((((9 as i8) << 3 >> 3) as u8) & 31, (((9 as i8) << 3 >> 3) as u8) & 31);
// CHECK-NEXT:     s9_mixed((255 as i16) << 7 >> 7, (((127 as i8) << 1 >> 1) as u8) & 127);
// CHECK-NEXT:     s9_mixed((-256 as i16) << 7 >> 7, (((100 as i8) << 1 >> 1) as u8) & 127);
// CHECK-NEXT:     s33((4294967295 as i64) << 31 >> 31, (4294967295 as i64) << 31 >> 31);
// CHECK-NEXT:     s33((-4294967296 as i64) << 31 >> 31, (1 as i64) << 31 >> 31);
// CHECK-NEXT:     s64(9223372036854775807 as i64, 1 as i64);
// CHECK-NEXT:     s64((-(9223372036854775807 as i64) - (1 as i64)) as i64, 2 as i64);
// CHECK-NEXT:     s100(
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(9223372036854775807 as i128),
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(9223372036854775807 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     u100(
// CHECK-NEXT:         <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(18446744073709551615 as u128),
// CHECK-NEXT:         <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(18446744073709551615 as u128),
// CHECK-NEXT:     );
// CHECK-NEXT:     s200(
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(9223372036854775807 as i128),
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(-9223372036854775807 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     u200_to_s(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1000 as i128)),
// CHECK-NEXT:         <bitint::BInt<150, 3, 24, bitint::A8>>::from_i128(-5 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     u200_to_s(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(0 as i128)),
// CHECK-NEXT:         <bitint::BInt<150, 3, 24, bitint::A8>>::from_i128(1 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     s200_to_int(
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(2147483647 as i128),
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     s200_to_int(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(5 as i128), 6 as i32);
// CHECK-NEXT:     int_to_s200(2147483647 as i32, 2147483647 as i64);
// CHECK-NEXT:     b128(
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(1 as i128),
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(2 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     c_u5((((0 as i8) << 3 >> 3) as u8) & 31);
// CHECK-NEXT:     c_u5((((1 as i8) << 3 >> 3) as u8) & 31);
// CHECK-NEXT:     c_u5((((22 as i8) << 3 >> 3) as u8) & 31);
// CHECK-NEXT:     c_u9((((256 as i16) << 7 >> 7) as u16) & 511);
// CHECK-NEXT:     c_u33((8589934591 as u64) & (8589934591 as u64));
// CHECK-NEXT:     c_u64(9223372036854775808 as u64);
// CHECK-NEXT:     c_u100(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(0 as i128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     c_u100(
// CHECK-NEXT:         <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(12345678901234567 as u128),
// CHECK-NEXT:     );
// CHECK-NEXT:     c_u200(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     c_u200(<bitint::BUint<200, 4, 32, bitint::A8>>::from_u128(3000000000000 as u128));
// CHECK-NEXT:     c_uint(40 as u32);
// CHECK-NEXT:     sat_i(2147483647 as i32, 5 as i32);
// CHECK-NEXT:     sat_i(-(2147483647 as i32) - (1 as i32), 5 as i32);
// CHECK-NEXT:     sat_s5((15 as i8) << 3 >> 3, (5 as i8) << 3 >> 3);
// CHECK-NEXT:     sat_s5((-16 as i8) << 3 >> 3, (5 as i8) << 3 >> 3);
// CHECK-NEXT:     sat_u5((((30 as i8) << 3 >> 3) as u8) & 31, (((5 as i8) << 3 >> 3) as u8) & 31);
// CHECK-NEXT:     sat_u5((((3 as i8) << 3 >> 3) as u8) & 31, (((5 as i8) << 3 >> 3) as u8) & 31);
// CHECK-NEXT:     sat_s9((255 as i16) << 7 >> 7, (5 as i16) << 7 >> 7);
// CHECK-NEXT:     sat_s9((-256 as i16) << 7 >> 7, (5 as i16) << 7 >> 7);
// CHECK-NEXT:     sat_s100(
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(9223372036854775807 as i128),
// CHECK-NEXT:         <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(5 as i128),
// CHECK-NEXT:     );
// CHECK-NEXT:     sat_u100(
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(3 as i128)),
// CHECK-NEXT:         <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(5 as i128)),
// CHECK-NEXT:     );
// CHECK-NEXT:     sat_s200(
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(-5 as i128),
// CHECK-NEXT:         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(7 as i128),
// CHECK-NEXT:     );
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
