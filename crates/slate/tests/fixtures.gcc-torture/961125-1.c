void abort(void);
void exit(int);

static char *begfield(int tab, char *ptr, char *lim, int sword, int schar) {
  if (tab) {
    while (ptr < lim && sword--) {
      while (ptr < lim && *ptr != tab)
        ++ptr;
      if (ptr < lim)
        ++ptr;
    }
  } else {
    while (1)
      ;
  }

  if (ptr + schar <= lim)
    ptr += schar;

  return ptr;
}

int main(void) {
  char *s   = ":ab";
  char *lim = s + 3;
  if (begfield(':', s, lim, 1, 1) != s + 2)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn begfield(
// CHECK-NEXT:     mut tab: i32,
// CHECK-NEXT:     mut ptr: *mut i8,
// CHECK-NEXT:     mut lim: *mut i8,
// CHECK-NEXT:     mut sword: i32,
// CHECK-NEXT:     mut schar: i32,
// CHECK-NEXT: ) -> *mut i8 {
// CHECK-NEXT:     if tab != (0 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:             if ptr < lim {
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = sword;
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:                 sword = [[V_3]];
// CHECK-NEXT:                 [[V_1]] = [[V_2]] != (0 as i32);
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 [[V_1]] = false as bool;
// CHECK-NEXT:             }
// CHECK-NEXT:             if ![[V_1]] {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ptr < lim && ((unsafe { *ptr }) as i32) != tab {
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: *mut i8 = ptr;
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: *mut i8 = unsafe { [[V_4]].offset((1 as i32) as isize) };
// CHECK-NEXT:                 ptr = [[V_5]];
// CHECK-NEXT:             }
// CHECK-NEXT:             if ptr < lim {
// CHECK-NEXT:                 let mut [[V_6:__v[0-9]+]]: *mut i8 = ptr;
// CHECK-NEXT:                 let mut [[V_7:__v[0-9]+]]: *mut i8 = unsafe { [[V_6]].offset((1 as i32) as isize) };
// CHECK-NEXT:                 ptr = [[V_7]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:             {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ptr.offset(schar as isize) }) <= lim {
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: *mut i8 = ptr;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: *mut i8 = unsafe { [[V_8]].offset(schar as isize) };
// CHECK-NEXT:         ptr = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     ptr
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: *mut i8 = c":ab".as_ptr() as *mut i8;
// CHECK-NEXT:     let mut lim: *mut i8 = unsafe { s.offset((3 as i32) as isize) };
// CHECK-NEXT:     if begfield(58 as i32, s, lim, 1 as i32, 1 as i32)
// CHECK-NEXT:         != unsafe { s.offset((2 as i32) as isize) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
