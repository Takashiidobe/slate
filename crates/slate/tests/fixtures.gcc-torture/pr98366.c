/* PR middle-end/98366 */
/* { dg-require-effective-target int32 } */

typedef struct S {
  int a, b, c : 7, d : 8, e : 17;
} S;
const S f[] = {{0, 3, 4, 2, 0}};

int main() {
  if (__builtin_memcmp(f, (S[]){{.b = 3, .c = 4, .d = 2, .e = 0}}, sizeof(S)))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/98366
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32 }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub c: i32,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub d: i32,
// CHECK-NEXT:         #[bits(17)]
// CHECK-NEXT:         pub e: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: [S; 1] = [{
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].a = 0 as i32;
// CHECK-NEXT:     [[T_0]].b = 3 as i32;
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c(4 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_d(2 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_e(0 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: }];
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: [S; 1] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of!(f) as *const S }) as *const (),
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 (*{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = [{
// CHECK-NEXT:                         let mut [[T_0]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                         [[T_0]].b = 3 as i32;
// CHECK-NEXT:                         [[T_0]].__slate_bits_0.__set_c(4 as i32);
// CHECK-NEXT:                         [[T_0]].__slate_bits_0.__set_d(2 as i32);
// CHECK-NEXT:                         [[T_0]].__slate_bits_0.__set_e(0 as i32);
// CHECK-NEXT:                         [[T_0]]
// CHECK-NEXT:                     }];
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_mut_ptr() as *mut S
// CHECK-NEXT:             }) as *const (),
// CHECK-NEXT:             12 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
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
