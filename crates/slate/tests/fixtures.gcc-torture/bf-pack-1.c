void abort(void);
void exit(int);

struct foo {
  unsigned      half  : 16;
  unsigned long whole : 32 __attribute__((packed));
};

void f(struct foo *q) {
  if (q->half != 0x1234)
    abort();
  if (q->whole != 0x56789abcL)
    abort();
}

int main(void) {
  struct foo bar;

  bar.half  = 0x1234;
  bar.whole = 0x56789abcL;
  f(&bar);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;6],
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
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub half: u32,
// CHECK-NEXT:         #[bits(32)]
// CHECK-NEXT:         pub whole: u64,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut q: &mut foo) {
// CHECK-NEXT:     if ((unsafe { (*(q as *mut foo)).__slate_bits_0.__get_half() }) as i32)
// CHECK-NEXT:         != (4660 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*(q as *mut foo)).__slate_bits_0.__get_whole() })
// CHECK-NEXT:         != ((1450744508 as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut bar: foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     bar.__slate_bits_0.__set_half((4660 as i32) as u32);
// CHECK-NEXT:     bar.__slate_bits_0.__set_whole((1450744508 as i64) as u64);
// CHECK-NEXT:     f(unsafe { &mut (*std::ptr::addr_of_mut!(bar)) });
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
