/* PR middle-end/27260 */

extern void  abort(void);
extern void *memset(void *, int, __SIZE_TYPE__);

char buf[65];

void foo(int x) { memset(buf, x != 2 ? 1 : 0, 64); }

int main(void) {
  int i;
  buf[64] = 2;
  for (i = 0; i < 64; i++)
    if (buf[i] != 0)
      abort();
  foo(0);
  for (i = 0; i < 64; i++)
    if (buf[i] != 1)
      abort();
  foo(2);
  for (i = 0; i < 64; i++)
    if (buf[i] != 0)
      abort();
  if (buf[64] != 2)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/27260
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut buf: __SlateAlign16<[i8; 65]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             if x != (2 as i32) { 1 as i32 } else { 0 as i32 },
// CHECK-NEXT:             ((64 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset((64 as i32) as isize)
// CHECK-NEXT:         } = (2 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset(i as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(0 as i32);
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset(i as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (1 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(2 as i32);
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (64 as i32) {
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset(i as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         i = [[V_8]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset((64 as i32) as isize) }
// CHECK-NEXT:     }) as i32)
// CHECK-NEXT:         != (2 as i32)
// CHECK-NEXT:     {
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
