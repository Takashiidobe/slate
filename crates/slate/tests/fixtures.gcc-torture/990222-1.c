void abort(void);

char line[4] = {'1', '9', '9', '\0'};

int main() {
  char *ptr = line + 3;

  while ((*--ptr += 1) > '9')
    *ptr = '0';
  if (line[0] != '2' || line[1] != '0' || line[2] != '0')
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut line: [i8; 4] = [
// CHECK-NEXT:     (49 as i32) as i8,
// CHECK-NEXT:     (57 as i32) as i8,
// CHECK-NEXT:     (57 as i32) as i8,
// CHECK-NEXT:     (0 as i32) as i8,
// CHECK-NEXT: ];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ptr: *mut i8 = unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(line) as *mut i8 }.offset((3 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut i8 = ptr;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut i8 = unsafe { [[V_0]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         ptr = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i8 = unsafe { *[[V_1]] };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i8 = (([[V_2]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_1]] = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !(([[V_3]] as i32) > (57 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *ptr = (48 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(line) as *mut i8 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as i32) != (50 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(line) as *mut i8 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32) != (48 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(line) as *mut i8 }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32) != (48 as i32)
// CHECK-NEXT:     {
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
