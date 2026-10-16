void abort(void);
void exit(int);

struct F {
  int i;
};

void f1(struct F *x, struct F *y) {
  int timeout = 0;
  for (; ((const struct F *)x)->i < y->i; x->i++)
    if (++timeout > 5)
      abort();
}

int main(void) {
  struct F x, y;
  x.i = 0;
  y.i = 1;
  f1(&x, &y);
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
// CHECK-NEXT: struct F {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn f1(mut x: &mut F, mut y: &F) {
// CHECK-NEXT:     let mut timeout: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { (*((x as *mut F) as *const F)).i })
// CHECK-NEXT:         < unsafe { (*(y as *const F)).i }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = timeout;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         timeout = [[V_2]];
// CHECK-NEXT:         if [[V_2]] > (5 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = unsafe { (*x).i };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*x).i = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: F = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut y: F = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     x.i = 0 as i32;
// CHECK-NEXT:     y.i = 1 as i32;
// CHECK-NEXT:     f1(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(y)) },
// CHECK-NEXT:     );
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
