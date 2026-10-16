void abort(void);
void exit(int);

int a[2] = {2, 0};

void foo(int *sp, int cnt) {
  int *p, *top;

  top  = sp;
  sp  -= cnt;

  for (p = sp; p <= top; p++)
    if (*p < 2)
      exit(0);
}

int main() {
  foo(a + 1, 1);
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: [i32; 2] = [2 as i32, 0 as i32];
// CHECK-NEXT: fn foo(mut sp: *mut i32, mut cnt: i32) {
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut top: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     top = sp;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i32 = sp;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i32 = unsafe { [[V_1]].offset(-(cnt as isize)) };
// CHECK-NEXT:     sp = [[V_2]];
// CHECK-NEXT:     p = sp;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while p <= top {
// CHECK-NEXT:         if (unsafe { *p }) < (2 as i32) {
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: *mut i32 = unsafe { [[V_3]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         },
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { abort() };
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
