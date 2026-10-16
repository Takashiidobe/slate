extern void abort();
extern void exit(int);

struct s {
  int   value;
  char *string;
};

int main(void) {
  int i;
  for (i = 0; i < 4; i++) {
    struct s *t = &(struct s){3, "hey there"};
    if (t->value != 3)
      abort();
    t->value = 4;
    if (t->value != 4)
      abort();
  }
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     string: *mut i8,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (4 as i32) {
// CHECK-NEXT:         let mut t: *mut s = unsafe {
// CHECK-NEXT:             std::ptr::addr_of_mut!(
// CHECK-NEXT:                 * { [[SLATE_COMPOUND_0]] = s { value : 3 as i32, string : c"hey there"
// CHECK-NEXT:                 .as_ptr() as * mut i8 }; std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]]) }
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         if (unsafe { (*t).value }) != (3 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*t).value = 4 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { (*t).value }) != (4 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
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
