/* { dg-options "-flax-vector-conversions=all" } */
int printf(const char *, ...);

typedef int v4si __attribute__((vector_size(16)));
typedef float v4sf __attribute__((vector_size(16)));
typedef unsigned char v16qi __attribute__((vector_size(16)));

static v4si as_ints(v4si value) { return value; }

int main(void) {
  v4sf floats = {1.0f, -2.0f, 0.5f, 0.0f};
  v4si ints = as_ints(floats);
  v16qi bytes = ints;
  printf("%x %x %x %x %u\n", ints[0], ints[1], ints[2], ints[3], bytes[3]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-flax-vector-conversions=all" }
// CHECK-NEXT: fn as_ints(value: core::simd::Simd<i32, 4>) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     return value;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let floats: core::simd::Simd<f32, 4> =
// CHECK-NEXT:         core::simd::Simd::from_array([1.0f32, -2.0f32, 0.5f32, 0.0f32]);
// CHECK-NEXT:     let ints: core::simd::Simd<i32, 4> = as_ints(unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i32, 4>>(floats)
// CHECK-NEXT:     });
// CHECK-NEXT:     let bytes: core::simd::Simd<u8, 16> =
// CHECK-NEXT:         unsafe { core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u8, 16>>(ints) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%x %x %x %x %u\n".as_ptr(),
// CHECK-NEXT:             ints[0],
// CHECK-NEXT:             ints[1],
// CHECK-NEXT:             ints[2],
// CHECK-NEXT:             ints[3],
// CHECK-NEXT:             bytes[3] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
