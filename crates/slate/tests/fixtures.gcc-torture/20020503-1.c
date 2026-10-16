/* PR 6534 */
/* GCSE unified the two i<0 tests, but if-conversion to ui=abs(i)
   insertted the code at the wrong place corrupting the i<0 test.  */

void         abort(void);
static char *inttostr(long i, char buf[128]) {
  unsigned long ui = i;
  char         *p  = buf + 127;
  *p               = '\0';
  if (i < 0)
    ui = -ui;
  do
    *--p = '0' + ui % 10;
  while ((ui /= 10) != 0);
  if (i < 0)
    *--p = '-';
  return p;
}

int main() {
  char buf[128], *p;

  p = inttostr(-1, buf);
  if (*p != '-')
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 6534
// CHECK-NEXT: //!
// CHECK-NEXT: //! GCSE unified the two i<0 tests, but if-conversion to ui=abs(i)
// CHECK-NEXT: //! insertted the code at the wrong place corrupting the i<0 test.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn inttostr(mut i: i64, mut buf: *mut i8) -> *mut i8 {
// CHECK-NEXT:     let mut ui: u64 = i as u64;
// CHECK-NEXT:     let mut p: *mut i8 = unsafe { buf.offset((127 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if i < ((0 as i32) as i64) {
// CHECK-NEXT:         ui = ui.wrapping_neg();
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut i8 = unsafe { [[V_0]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         p = [[V_1]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_1]] = ((((48 as i32) as i64) as u64).wrapping_add(ui % (((10 as i32) as i64) as u64))
// CHECK-NEXT:                 as u8) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u64 = ui;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]] / (((10 as i32) as i64) as u64);
// CHECK-NEXT:         ui = [[V_3]];
// CHECK-NEXT:         if !([[V_3]] != (((0 as i32) as i64) as u64)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if i < ((0 as i32) as i64) {
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut i8 = unsafe { [[V_4]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         p = [[V_5]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_5]] = (45 as i32) as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 128]> = __SlateAlign16([0 as i8; 128]);
// CHECK-NEXT:     let mut p: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p = inttostr(-(1 as i32) as i64, buf.0.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     if ((unsafe { *p }) as i32) != (45 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
