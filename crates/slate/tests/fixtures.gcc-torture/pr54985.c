
typedef struct st {
  int a;
} ST;

int __attribute__((noinline, noclone)) foo(ST *s, int c) {
  int first = 1;
  int count = c;
  ST *item  = s;
  int a     = s->a;
  int x;

  while (count--) {
    x = item->a;
    if (first)
      first = 0;
    else if (x >= a)
      return 1;
    a = x;
    item++;
  }
  return 0;
}

extern void abort(void);

int main() {
  ST _1[2] = {{2}, {1}};
  if (foo(_1, 2) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct st {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut s: *mut st, mut count: i32) -> i32 {
// CHECK-NEXT:     let mut first: i32 = 1 as i32;
// CHECK-NEXT:     let mut item: *mut st = s;
// CHECK-NEXT:     let mut a: i32 = unsafe { (*s).a };
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = count;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:         count = [[V_1]];
// CHECK-NEXT:         if !([[V_0]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         x = unsafe { (*item).a };
// CHECK-NEXT:         if first != (0 as i32) {
// CHECK-NEXT:             first = 0 as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if x >= a {
// CHECK-NEXT:                 return 1 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         a = x;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut st = item;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut st = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:         item = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut _1: [st; 2] = [st { a: 2 as i32 }, st { a: 1 as i32 }];
// CHECK-NEXT:     if foo(_1.as_mut_ptr() as *mut st, 2 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
