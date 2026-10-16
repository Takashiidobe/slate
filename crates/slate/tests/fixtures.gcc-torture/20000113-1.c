void abort(void);
void exit(int);

struct x {
  unsigned x1 : 1;
  unsigned x2 : 2;
  unsigned x3 : 3;
};

void foobar(int x, int y, int z) {
  struct x  a = {x, y, z};
  struct x  b = {x, y, z};
  struct x *c = &b;

  c->x3 += (a.x2 - a.x1) * c->x2;
  if (a.x1 != 1 || c->x3 != 5)
    abort();
  exit(0);
}

int main(void) { foobar(1, 2, 3); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct x {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
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
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub x1: u32,
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub x2: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub x3: u32,
// CHECK-NEXT:         #[bits(2, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foobar(mut x: i32, mut y: i32, mut z: i32) {
// CHECK-NEXT:     let mut a: x = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: x = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_x1(x as u32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_x2(y as u32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_x3(z as u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: x = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: x = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_x1(x as u32);
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_x2(y as u32);
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_x3(z as u32);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: *mut x = std::ptr::addr_of_mut!(b);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = unsafe { (*c).__slate_bits_0.__get_x3() };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = (([[V_1]] as i32)
// CHECK-NEXT:         + ((a.__slate_bits_0.__get_x2() as i32) - (a.__slate_bits_0.__get_x1() as i32))
// CHECK-NEXT:             * ((unsafe { (*c).__slate_bits_0.__get_x2() }) as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*c).__slate_bits_0.__set_x3([[V_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (a.__slate_bits_0.__get_x1() as i32) != (1 as i32)
// CHECK-NEXT:         || ((unsafe { (*c).__slate_bits_0.__get_x3() }) as i32) != (5 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foobar(1 as i32, 2 as i32, 3 as i32);
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
