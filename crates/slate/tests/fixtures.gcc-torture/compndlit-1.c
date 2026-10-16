/* The bit-field below would have a problem if __INT_MAX__ is too
   small.  */
void abort(void);
void exit(int);

#if __INT_MAX__ < 2147483647
int main(void) { exit(0); }
#else
struct S {
  int      a : 3;
  unsigned b : 1, c : 28;
};

struct S x = {1, 1, 1};

int main(void) {
  x = (struct S){
    b : 0,
    a : 0,
    c : ({
      struct S o = x;
      o.a == 1 ? 10 : 20;
    })
  };
  if (x.c != 10)
    abort();
  exit(0);
}
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // The bit-field below would have a problem if __INT_MAX__ is too
// CHECK-NEXT: // small.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
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
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(28)]
// CHECK-NEXT:         pub c: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: S = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(1 as i32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c((1 as i32) as u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32;
// CHECK-NEXT:     let mut o: S = unsafe { x };
// CHECK-NEXT:     [[V_1]] = if o.__slate_bits_0.__get_a() == (1 as i32) {
// CHECK-NEXT:         10 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         20 as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = {
// CHECK-NEXT:                     let mut [[T_0]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                     [[T_0]].__slate_bits_0.__set_a(0 as i32);
// CHECK-NEXT:                     [[T_0]].__slate_bits_0.__set_b((0 as i32) as u32);
// CHECK-NEXT:                     [[T_0]].__slate_bits_0.__set_c([[V_1]] as u32);
// CHECK-NEXT:                     [[T_0]]
// CHECK-NEXT:                 };
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(x.__slate_bits_0)).__get_c() }) as i32) != (10 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
