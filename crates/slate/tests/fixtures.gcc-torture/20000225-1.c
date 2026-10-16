void abort(void);
void exit(int);

int main() {
  int nResult;
  int b = 0;
  int i = -1;

  do {
    if (b != 0) {
      abort();
      nResult = 1;
    } else {
      nResult = 0;
    }
    i++;
    b = (i + 2) * 4;
  } while (i < 0);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut nResult: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = -(1 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         if b != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:             nResult = 1 as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             nResult = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:         b = (i + (2 as i32)) * (4 as i32);
// CHECK-NEXT:         if !(i < (0 as i32)) {
// CHECK-NEXT:             break;
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
