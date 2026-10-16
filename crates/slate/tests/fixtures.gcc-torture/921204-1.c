/* The bit-field below would have a problem if __INT_MAX__ is too
   small.  */
void abort(void);
void exit(int);

#if __INT_MAX__ < 2147483647
int main(void) { exit(0); }
#else
typedef struct {
  unsigned b0 : 1, f1 : 17, b18 : 1, b19 : 1, b20 : 1, f2 : 11;
} bf;

typedef union {
  bf       b;
  unsigned w;
} bu;

bu f(bu i) {
  bu o = i;

  if (o.b.b0)
    o.b.b18 = 1, o.b.b20 = 1;
  else
    o.b.b18 = 0, o.b.b20 = 0;

  return o;
}

int main(void) {
  bu a;
  bu r;

  a.w    = 0x4000000;
  a.b.b0 = 0;
  r      = f(a);
  if (a.w != r.w)
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
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     b: [[SLATERECORD_0]],
// CHECK-NEXT:     w: u32,
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
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub b0: u32,
// CHECK-NEXT:         #[bits(17)]
// CHECK-NEXT:         pub f1: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub b18: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub b19: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub b20: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub f2: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut o: [[SLATERECORD_1]]) -> [[SLATERECORD_1]] {
// CHECK-NEXT:     if ((unsafe { o.b.__slate_bits_0.__get_b0() }) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             o.b.__slate_bits_0.__set_b18((1 as i32) as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             o.b.__slate_bits_0.__set_b20((1 as i32) as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             o.b.__slate_bits_0.__set_b18((0 as i32) as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             o.b.__slate_bits_0.__set_b20((0 as i32) as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     return o;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut r: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.w = (67108864 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.b.__slate_bits_0.__set_b0((0 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     r = f(a);
// CHECK-NEXT:     if (unsafe { a.w }) != unsafe { r.w } {
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
