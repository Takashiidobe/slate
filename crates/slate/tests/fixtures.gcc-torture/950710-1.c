void abort(void);
void exit(int);

struct twelve {
  int a;
  int b;
  int c;
};

struct pair {
  int first;
  int second;
};

struct pair g() {
  struct pair p;
  return p;
}

static void f() {
  int i;
  for (i = 0; i < 1; i++) {
    int j;
    for (j = 0; j < 1; j++) {
      if (0) {
        int k;
        for (k = 0; k < 1; k++) {
          struct pair e = g();
        }
      } else {
        struct twelve a, b;
        if ((((char *)&b - (char *)&a) < 0
                 ? (-((char *)&b - (char *)&a))
                 : ((char *)&b - (char *)&a)) < sizeof(a))
          abort();
      }
    }
  }
}

int main(void) {
  f();
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
// CHECK-NEXT: struct twelve {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT:     second: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn g() -> pair {
// CHECK-NEXT:     let mut p: pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return p;
// CHECK-NEXT: }
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (1 as i32) {
// CHECK-NEXT:         let mut j: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (1 as i32) {
// CHECK-NEXT:             if (0 as i32) != (0 as i32) {
// CHECK-NEXT:                 let mut k: i32 = 0 as i32;
// CHECK-NEXT:                 '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while k < (1 as i32) {
// CHECK-NEXT:                     g();
// CHECK-NEXT:                     let mut [[V_1:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:                     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:                     k = [[V_2]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let mut a: twelve = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 let mut b: twelve = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 if ((if ((unsafe {
// CHECK-NEXT:                     (std::ptr::addr_of_mut!(b) as *mut i8)
// CHECK-NEXT:                         .offset_from(std::ptr::addr_of_mut!(a) as *mut i8)
// CHECK-NEXT:                 }) as i64) < ((0 as i32) as i64)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     -((unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(b) as *mut i8)
// CHECK-NEXT:                             .offset_from(std::ptr::addr_of_mut!(a) as *mut i8)
// CHECK-NEXT:                     }) as i64)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         (std::ptr::addr_of_mut!(b) as *mut i8)
// CHECK-NEXT:                             .offset_from(std::ptr::addr_of_mut!(a) as *mut i8)
// CHECK-NEXT:                     }) as i64
// CHECK-NEXT:                 }) as u64) < (12 as u64)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:             j = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f();
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
