void abort(void);
void exit(int);

int main(void) {
  int i = 0;

  if (sizeof(unsigned long int) == 4) {
    unsigned long int z = 0;

    do {
      z -= 0x00004000;
      i++;
      if (i > 0x00040000)
        abort();
    } while (z > 0);
    exit(0);
  } else if (sizeof(unsigned int) == 4) {
    unsigned int z = 0;

    do {
      z -= 0x00004000;
      i++;
      if (i > 0x00040000)
        abort();
    } while (z > 0);
    exit(0);
  } else
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
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     if (8 as u64) == (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         let mut z: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: u64 = z;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: u64 = [[V_1]].wrapping_sub(((16384 as i32) as i64) as u64);
// CHECK-NEXT:             z = [[V_2]];
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:             i = [[V_4]];
// CHECK-NEXT:             if i > (262144 as i32) {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             if !(z > (((0 as i32) as i64) as u64)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if (4 as u64) == (((4 as i32) as i64) as u64) {
// CHECK-NEXT:             let mut z: u32 = (0 as i32) as u32;
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: u32 = z;
// CHECK-NEXT:                 let mut [[V_6:__v[0-9]+]]: u32 = [[V_5]].wrapping_sub((16384 as i32) as u32);
// CHECK-NEXT:                 z = [[V_6]];
// CHECK-NEXT:                 let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                 let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:                 i = [[V_8]];
// CHECK-NEXT:                 if i > (262144 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !(z > ((0 as i32) as u32)) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
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
