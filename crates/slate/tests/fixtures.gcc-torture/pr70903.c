typedef unsigned char      V8 __attribute__((vector_size(32)));
typedef unsigned int       V32 __attribute__((vector_size(32)));
typedef unsigned long long V64 __attribute__((vector_size(32)));

static V32 __attribute__((noinline, noclone)) foo(V64 x) {
  V64 y = (V64)(V8){((V8)(V64){65535, x[0]})[1]};
  return (V32){y[0], 255};
}

int main() {
  V32 x = foo((V64){});
  //  __builtin_printf ("%08x %08x %08x %08x %08x %08x %08x %08x\n", x[0], x[1],
  //  x[2], x[3], x[4], x[5], x[6], x[7]);
  if (x[1] != 255)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: std::simd::Simd<u64, 4>) -> std::simd::Simd<u32, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<u64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<u8, 32> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: std::simd::Simd<u32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut y: std::simd::Simd<u64, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<u8, 32>, std::simd::Simd<u64, 4>>(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array({
// CHECK-NEXT:                     let mut [[T_0:__t[0-9]+]]: [u8; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                     [[T_0]][0] = unsafe {
// CHECK-NEXT:                         std::mem::transmute::<std::simd::Simd<u64, 4>, std::simd::Simd<u8, 32>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 *{
// CHECK-NEXT:                                     [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array({
// CHECK-NEXT:                                         let mut [[T_1:__t[0-9]+]]: [u64; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                                         [[T_1]][0] = ((65535 as i32) as i64) as u64;
// CHECK-NEXT:                                         [[T_1]][1] = x[((0 as i32) as usize)];
// CHECK-NEXT:                                         [[T_1]]
// CHECK-NEXT:                                     });
// CHECK-NEXT:                                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     }[((1 as i32) as usize)];
// CHECK-NEXT:                     [[T_0]]
// CHECK-NEXT:                 });
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_2]] = std::simd::Simd::from_array({
// CHECK-NEXT:                 let mut [[T_2:__t[0-9]+]]: [u32; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_2]][0] = y[((0 as i32) as usize)] as u32;
// CHECK-NEXT:                 [[T_2]][1] = (255 as i32) as u32;
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             });
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: std::simd::Simd<u64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut x: std::simd::Simd<u32, 8> = foo(unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_3]] = std::simd::Simd::from_array({
// CHECK-NEXT:                 let mut [[T_0]]: [u64; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             });
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     //  __builtin_printf ("%08x %08x %08x %08x %08x %08x %08x %08x\n", x[0], x[1],
// CHECK-NEXT:     //
// CHECK-NEXT:     //  x[2], x[3], x[4], x[5], x[6], x[7]);
// CHECK-NEXT:     if x[((1 as i32) as usize)] != ((255 as i32) as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
