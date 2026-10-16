void abort(void);
void exit(int);

int x[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main() {
  int niterations = 0, i;

  for (;;) {
    int i, mi, max;
    max = 0;
    for (i = 0; i < 10; i++) {
      if (x[i] > max) {
        max = x[i];
        mi  = i;
      }
    }
    if (max == 0)
      break;
    x[mi] = 0;
    niterations++;
    if (niterations > 10)
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut x: __SlateAlign16<[i32; 10]> = __SlateAlign16([
// CHECK-NEXT:     0 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     3 as i32,
// CHECK-NEXT:     4 as i32,
// CHECK-NEXT:     5 as i32,
// CHECK-NEXT:     6 as i32,
// CHECK-NEXT:     7 as i32,
// CHECK-NEXT:     8 as i32,
// CHECK-NEXT:     9 as i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut niterations: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut i: i32 = 0 as i32;
// CHECK-NEXT:         let mut mi: i32 = 0 as i32;
// CHECK-NEXT:         let mut max: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(x.0) as *mut i32 }.offset(i as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) > max
// CHECK-NEXT:             {
// CHECK-NEXT:                 max = unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(x.0) as *mut i32 }
// CHECK-NEXT:                             .offset(i as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:                 mi = i;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:             i = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if max == (0 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(x.0) as *mut i32 }.offset(mi as isize)
// CHECK-NEXT:             } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = niterations;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         niterations = [[V_4]];
// CHECK-NEXT:         if niterations > (10 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
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
