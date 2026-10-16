void abort(void);
void exit(int);

#define MASK(N) ((1UL << (N)) - 1)
#define BITS(N) ((1UL << ((N) - 1)) + 2)

#define FUNC(N)                                                                \
  void f##N(long j) {                                                          \
    if ((j & MASK(N)) >= BITS(N))                                              \
      abort();                                                                 \
  }

FUNC(3)
FUNC(4)
FUNC(5)
FUNC(6)
FUNC(7)
FUNC(8)
FUNC(9)
FUNC(10)
FUNC(11)
FUNC(12)
FUNC(13)
FUNC(14)
FUNC(15)
FUNC(16)
FUNC(17)
FUNC(18)
FUNC(19)
FUNC(20)
FUNC(21)
FUNC(22)
FUNC(23)
FUNC(24)
FUNC(25)
FUNC(26)
FUNC(27)
FUNC(28)
FUNC(29)
FUNC(30)
FUNC(31)

int main() {
  f3(0);
  f4(0);
  f5(0);
  f6(0);
  f7(0);
  f8(0);
  f9(0);
  f10(0);
  f11(0);
  f12(0);
  f13(0);
  f14(0);
  f15(0);
  f16(0);
  f17(0);
  f18(0);
  f19(0);
  f20(0);
  f21(0);
  f22(0);
  f23(0);
  f24(0);
  f25(0);
  f26(0);
  f27(0);
  f28(0);
  f29(0);
  f30(0);
  f31(0);

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f3(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (3 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (3 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f4(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (4 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (4 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f5(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (5 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (5 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f6(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (6 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (6 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f7(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (7 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (7 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f8(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (8 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (8 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f9(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (9 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (9 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f10(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (10 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (10 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f11(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (11 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (11 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f12(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (12 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (12 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f13(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (13 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (13 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f14(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (14 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (14 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f15(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (15 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (15 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f16(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (16 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (16 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f17(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (17 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (17 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f18(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (18 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (18 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f19(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (19 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (19 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f20(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (20 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (20 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f21(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (21 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (21 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f22(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (22 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (22 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f23(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (23 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (23 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f24(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (24 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (24 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f25(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (25 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (25 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f26(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (26 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (26 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f27(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (27 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (27 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f28(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (28 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (28 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f29(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (29 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (29 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f30(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (30 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (30 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f31(mut j: i64) {
// CHECK-NEXT:     if (j as u64) & ((1 as u64) << (31 as i32)).wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:         >= ((1 as u64) << (31 as i32) - (1 as i32))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f3((0 as i32) as i64);
// CHECK-NEXT:     f4((0 as i32) as i64);
// CHECK-NEXT:     f5((0 as i32) as i64);
// CHECK-NEXT:     f6((0 as i32) as i64);
// CHECK-NEXT:     f7((0 as i32) as i64);
// CHECK-NEXT:     f8((0 as i32) as i64);
// CHECK-NEXT:     f9((0 as i32) as i64);
// CHECK-NEXT:     f10((0 as i32) as i64);
// CHECK-NEXT:     f11((0 as i32) as i64);
// CHECK-NEXT:     f12((0 as i32) as i64);
// CHECK-NEXT:     f13((0 as i32) as i64);
// CHECK-NEXT:     f14((0 as i32) as i64);
// CHECK-NEXT:     f15((0 as i32) as i64);
// CHECK-NEXT:     f16((0 as i32) as i64);
// CHECK-NEXT:     f17((0 as i32) as i64);
// CHECK-NEXT:     f18((0 as i32) as i64);
// CHECK-NEXT:     f19((0 as i32) as i64);
// CHECK-NEXT:     f20((0 as i32) as i64);
// CHECK-NEXT:     f21((0 as i32) as i64);
// CHECK-NEXT:     f22((0 as i32) as i64);
// CHECK-NEXT:     f23((0 as i32) as i64);
// CHECK-NEXT:     f24((0 as i32) as i64);
// CHECK-NEXT:     f25((0 as i32) as i64);
// CHECK-NEXT:     f26((0 as i32) as i64);
// CHECK-NEXT:     f27((0 as i32) as i64);
// CHECK-NEXT:     f28((0 as i32) as i64);
// CHECK-NEXT:     f29((0 as i32) as i64);
// CHECK-NEXT:     f30((0 as i32) as i64);
// CHECK-NEXT:     f31((0 as i32) as i64);
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
