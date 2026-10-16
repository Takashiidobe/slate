const char *__attribute__((noinline)) foo(const char *p) {
  const char *end;
  int         len = 1;
  for (;;) {
    int c = *p;
    c     = (c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c);
    if (c == 'B')
      end = p;
    else if (c == 'A') {
      end = p;
      do
        p++;
      while (*p == '+');
    } else
      break;
    p++;
    len++;
  }
  if (len > 2 && *p == ':')
    p = end;
  return p;
}

int main(void) {
  const char *input = "Bbb:";
  return foo(input) != input + 2;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn foo(mut p: *const i8) -> *const i8 {
// CHECK-NEXT:     let mut end: *const i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut len: i32 = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut c: i32 = (unsafe { *p }) as i32;
// CHECK-NEXT:         c = if c >= (97 as i32) && c <= (122 as i32) {
// CHECK-NEXT:             c - (97 as i32) + (65 as i32)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             c
// CHECK-NEXT:         };
// CHECK-NEXT:         if c == (66 as i32) {
// CHECK-NEXT:             end = p;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if c == (65 as i32) {
// CHECK-NEXT:                 end = p;
// CHECK-NEXT:                 '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:                     let mut [[V_0:__v[0-9]+]]: *const i8 = p;
// CHECK-NEXT:                     let mut [[V_1:__v[0-9]+]]: *const i8 = unsafe {
// CHECK-NEXT:                         [[V_0]].offset((1 as i32) as isize)
// CHECK-NEXT:                     };
// CHECK-NEXT:                     p = [[V_1]];
// CHECK-NEXT:                     if !(((unsafe { *p }) as i32) == (43 as i32)) {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *const i8 = p;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *const i8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_3]];
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = len;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         len = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if len > (2 as i32) && ((unsafe { *p }) as i32) == (58 as i32) {
// CHECK-NEXT:         p = end;
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut input: *const i8 = c"Bbb:".as_ptr() as *const i8;
// CHECK-NEXT:     (foo(input) != unsafe { input.offset((2 as i32) as isize) }) as i32
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
