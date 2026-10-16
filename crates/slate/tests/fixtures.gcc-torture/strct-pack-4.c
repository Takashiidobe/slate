void abort(void);
void exit(int);

typedef struct {
  unsigned char  a __attribute__((packed));
  unsigned short b __attribute__((packed));
} three_char_t;

unsigned char my_set_a(void) { return 0xab; }

unsigned short my_set_b(void) { return 0x1234; }

int main(void) {
  three_char_t three_char;

  three_char.a = my_set_a();
  three_char.b = my_set_b();
  if (three_char.a != 0xab || three_char.b != 0x1234)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: u8,
// CHECK-NEXT:     b: u16,
// CHECK-NEXT: }
// CHECK-NEXT: fn my_set_a() -> u8 {
// CHECK-NEXT:     ((171 as i32) as i8) as u8
// CHECK-NEXT: }
// CHECK-NEXT: fn my_set_b() -> u16 {
// CHECK-NEXT:     ((4660 as i32) as i16) as u16
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut three_char: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     three_char.a = my_set_a();
// CHECK-NEXT:     three_char.b = my_set_b();
// CHECK-NEXT:     if ((three_char.a as u32) as i32) != (171 as i32)
// CHECK-NEXT:         || ((three_char.b as u32) as i32) != (4660 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
