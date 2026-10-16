void abort(void);
void exit(int);

unsigned long bad(int reg, unsigned long inWord) {
  union {
    unsigned long word;
    unsigned char byte[4];
  } data;

  data.word      = inWord;
  data.byte[reg] = 0;

  return data.word;
}

int main(void) {
  /* XXX This test could be generalized.  */
  if (sizeof(long) != 4)
    exit(0);

  if (bad(0, 0xdeadbeef) == 0xdeadbeef)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     word: u64,
// CHECK-NEXT:     byte: [u8; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bad(mut reg: i32, mut inWord: u64) -> u64 {
// CHECK-NEXT:     let mut data: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         data.word = inWord;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { data.byte.as_mut_ptr() as *mut u8 }.offset(reg as isize) } =
// CHECK-NEXT:             ((0 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { data.word }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // XXX This test could be generalized.
// CHECK-NEXT:     if (8 as u64) != (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bad(0 as i32, (3735928559 as u32) as u64) == ((3735928559 as u32) as u64) {
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
