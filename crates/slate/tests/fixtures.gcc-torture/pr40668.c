#if (__SIZEOF_INT__ == 2)
#define TESTVALUE 0x1234
#else
#define TESTVALUE 0x12345678
#endif
static void foo(unsigned int x, void *p) { __builtin_memcpy(p, &x, sizeof x); }

void bar(int type, void *number) {
  switch (type) {
  case 1:
    foo(TESTVALUE, number);
    break;
  case 7:
    foo(0, number);
    break;
  case 8:
    foo(0, number);
    break;
  case 9:
    foo(0, number);
    break;
  }
}

int main(void) {
  unsigned int x;
  bar(1, &x);
  if (x != TESTVALUE)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: u32, mut p: *mut ()) {
// CHECK-NEXT:     unsafe { __builtin_memcpy(p, std::ptr::addr_of_mut!(x) as *const (), 4 as u64) };
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut r#type: i32, mut number: *mut ()) {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match r#type {
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 foo((305419896 as i32) as u32, number);
// CHECK-NEXT:             }
// CHECK-NEXT:             7 => {
// CHECK-NEXT:                 foo((0 as i32) as u32, number);
// CHECK-NEXT:             }
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 foo((0 as i32) as u32, number);
// CHECK-NEXT:             }
// CHECK-NEXT:             9 => {
// CHECK-NEXT:                 foo((0 as i32) as u32, number);
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u32 = 0 as u32;
// CHECK-NEXT:     bar(1 as i32, std::ptr::addr_of_mut!(x) as *mut ());
// CHECK-NEXT:     if x != ((305419896 as i32) as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
