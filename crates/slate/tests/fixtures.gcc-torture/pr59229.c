int i;

__attribute__((noinline, noclone)) void bar(char *p) {
  if (i < 1 || i > 6)
    __builtin_abort();
  if (__builtin_memcmp(p, "abcdefg", i + 1) != 0)
    __builtin_abort();
  __builtin_memset(p, ' ', 7);
}

__attribute__((noinline, noclone)) void foo(char *p, unsigned long l) {
  if (l < 1 || l > 6)
    return;
  char buf[7];
  __builtin_memcpy(buf, p, l + 1);
  bar(buf);
}

int main() {
  for (i = 0; i < 16; i++)
    foo("abcdefghijklmnop", i);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: i32, [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_6:__v[0-9]+]]: *mut (), [[V_7:__v[0-9]+]]: *const (), [[V_8:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn bar(mut p: *mut i8) {
// CHECK-NEXT:     if (unsafe { i }) < (1 as i32) || (unsafe { i }) > (6 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             p as *const (),
// CHECK-NEXT:             b"abcdefg\0".as_ptr() as *const (),
// CHECK-NEXT:             (((unsafe { i }) + (1 as i32)) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __builtin_memset(p as *mut (), 32 as i32, ((7 as i32) as i64) as u64) };
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: *mut i8, mut l: u64) {
// CHECK-NEXT:     if l < (((1 as i32) as i64) as u64) || l > (((6 as i32) as i64) as u64) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut buf: [i8; 7] = [0 as i8; 7];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             buf.as_mut_ptr() as *mut (),
// CHECK-NEXT:             p as *const (),
// CHECK-NEXT:             l.wrapping_add(((1 as i32) as i64) as u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     bar(buf.as_mut_ptr() as *mut i8);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { i }) < (16 as i32) {
// CHECK-NEXT:         foo(c"abcdefghijklmnop".as_ptr() as *mut i8, ((unsafe { i }) as i64) as u64);
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = unsafe { i };
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             i = [[V_10]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
