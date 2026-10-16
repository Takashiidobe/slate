/* Bombed with a segfault on powerpc-linux.  doloop.c generated wrong
   loop count.  */
void abort(void);

void foo(unsigned long *start, unsigned long *end) {
  unsigned long *temp = end - 1;

  while (end > start)
    *end-- = *temp--;
}

int main(void) {
  unsigned long a[5];
  int           start, end, k;

  for (start = 0; start < 5; start++)
    for (end = 0; end < 5; end++) {
      for (k = 0; k < 5; k++)
        a[k] = k;

      foo(a + start, a + end);

      for (k = 0; k <= start; k++)
        if (a[k] != k)
          abort();

      for (k = start + 1; k <= end; k++)
        if (a[k] != k - 1)
          abort();

      for (k = end + 1; k < 5; k++)
        if (a[k] != k)
          abort();
    }

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // Bombed with a segfault on powerpc-linux.  doloop.c generated wrong
// CHECK-NEXT: // loop count.
// CHECK-NEXT: fn foo(mut start: *mut u64, mut end: *mut u64) {
// CHECK-NEXT:     let mut temp: *mut u64 = unsafe { end.offset(-((1 as i32) as isize)) };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while end > start {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut u64 = temp;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut u64 = unsafe { [[V_0]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         temp = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut u64 = end;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut u64 = unsafe { [[V_2]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         end = [[V_3]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_2]] = unsafe { *[[V_0]] };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[u64; 5]> = __SlateAlign16([0 as u64; 5]);
// CHECK-NEXT:     let mut start: i32 = 0 as i32;
// CHECK-NEXT:     let mut end: i32 = 0 as i32;
// CHECK-NEXT:     let mut k: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while start < (5 as i32) {
// CHECK-NEXT:         end = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while end < (5 as i32) {
// CHECK-NEXT:             k = 0 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while k < (5 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { (a.0.as_mut_ptr() as *mut u64).offset(k as isize) } =
// CHECK-NEXT:                         (k as i64) as u64;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:                 k = [[V_5]];
// CHECK-NEXT:             }
// CHECK-NEXT:             foo(
// CHECK-NEXT:                 unsafe { (a.0.as_mut_ptr() as *mut u64).offset(start as isize) },
// CHECK-NEXT:                 unsafe { (a.0.as_mut_ptr() as *mut u64).offset(end as isize) },
// CHECK-NEXT:             );
// CHECK-NEXT:             k = 0 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while k <= start {
// CHECK-NEXT:                 if (unsafe { *unsafe { (a.0.as_mut_ptr() as *mut u64).offset(k as isize) } })
// CHECK-NEXT:                     != ((k as i64) as u64)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_6:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:                 let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:                 k = [[V_7]];
// CHECK-NEXT:             }
// CHECK-NEXT:             k = start + (1 as i32);
// CHECK-NEXT:             '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while k <= end {
// CHECK-NEXT:                 if (unsafe { *unsafe { (a.0.as_mut_ptr() as *mut u64).offset(k as isize) } })
// CHECK-NEXT:                     != (((k - (1 as i32)) as i64) as u64)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_8:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:                 let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:                 k = [[V_9]];
// CHECK-NEXT:             }
// CHECK-NEXT:             k = end + (1 as i32);
// CHECK-NEXT:             '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while k < (5 as i32) {
// CHECK-NEXT:                 if (unsafe { *unsafe { (a.0.as_mut_ptr() as *mut u64).offset(k as isize) } })
// CHECK-NEXT:                     != ((k as i64) as u64)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_10:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:                 let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:                 k = [[V_11]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_12:__v[0-9]+]]: i32 = end;
// CHECK-NEXT:             let mut [[V_13:__v[0-9]+]]: i32 = [[V_12]] + (1 as i32);
// CHECK-NEXT:             end = [[V_13]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: i32 = start;
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] + (1 as i32);
// CHECK-NEXT:         start = [[V_15]];
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
