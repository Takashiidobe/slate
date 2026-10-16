struct A {
  int  i, j;
  char pad[512];
} a;

int __attribute__((noinline)) foo(void) {
  __builtin_memset(&a, 0x26, sizeof a);
  return a.i;
}

void __attribute__((noinline)) bar(void) {
  __builtin_memset(&a, 0x36, sizeof a);
  a.i = 0x36363636;
  a.j = 0x36373636;
}

int main(void) {
  int i;
  if (sizeof(int) != 4 || __CHAR_BIT__ != 8)
    return 0;

  if (foo() != 0x26262626)
    __builtin_abort();
  for (i = 0; i < sizeof a; i++)
    if (((char *)&a)[i] != 0x26)
      __builtin_abort();

  bar();
  if (a.j != 0x36373636)
    __builtin_abort();
  a.j = 0x36363636;
  for (i = 0; i < sizeof a; i++)
    if (((char *)&a)[i] != 0x36)
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     j: i32,
// CHECK-NEXT:     pad: [i8; 512],
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(a) }) as *mut (),
// CHECK-NEXT:             38 as i32,
// CHECK-NEXT:             520 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { a.i }
// CHECK-NEXT: }
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(a) }) as *mut (),
// CHECK-NEXT:             54 as i32,
// CHECK-NEXT:             520 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.i = 909522486 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.j = 909588022 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     if (4 as u64) != (((4 as i32) as i64) as u64) || (8 as i32) != (8 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo() != (640034342 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (520 as u64) {
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 ((unsafe { std::ptr::addr_of_mut!(a) }) as *mut i8).offset(i as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32) != (38 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     bar();
// CHECK-NEXT:     if (unsafe { a.j }) != (909588022 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.j = 909522486 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (520 as u64) {
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 ((unsafe { std::ptr::addr_of_mut!(a) }) as *mut i8).offset(i as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32) != (54 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
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
