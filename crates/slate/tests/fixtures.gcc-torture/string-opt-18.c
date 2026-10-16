/* Copyright (C) 2003  Free Software Foundation.

   Test equal pointer optimizations don't break anything.

   Written by Roger Sayle, July 14, 2003.  */

extern void           abort();
typedef __SIZE_TYPE__ size_t;

extern void *memcpy(void *, const void *, size_t);
extern void *mempcpy(void *, const void *, size_t);
extern void *memmove(void *, const void *, size_t);
extern char *strcpy(char *, const char *);
extern int   memcmp(const void *, const void *, size_t);
extern int   strcmp(const char *, const char *);
extern int   strncmp(const char *, const char *, size_t);

void test1(void *ptr) {
  if (memcpy(ptr, ptr, 8) != ptr)
    abort();
}

void test2(char *ptr) {
  if (mempcpy(ptr, ptr, 8) != ptr + 8)
    abort();
}

void test3(void *ptr) {
  if (memmove(ptr, ptr, 8) != ptr)
    abort();
}

void test4(char *ptr) {
  if (strcpy(ptr, ptr) != ptr)
    abort();
}

void test5(void *ptr) {
  if (memcmp(ptr, ptr, 8) != 0)
    abort();
}

void test6(const char *ptr) {
  if (strcmp(ptr, ptr) != 0)
    abort();
}

void test7(const char *ptr) {
  if (strncmp(ptr, ptr, 8) != 0)
    abort();
}

int main() {
  char buf[10];

  test1(buf);
  test2(buf);
  test3(buf);
  test4(buf);
  test5(buf);
  test6(buf);
  test7(buf);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2003  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test equal pointer optimizations don't break anything.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, July 14, 2003.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn mempcpy([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: *const (), [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn memmove([[V_6:__v[0-9]+]]: *mut (), [[V_7:__v[0-9]+]]: *const (), [[V_8:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn strcpy([[V_9:__v[0-9]+]]: *mut i8, [[V_10:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     fn memcmp([[V_11:__v[0-9]+]]: *const (), [[V_12:__v[0-9]+]]: *const (), [[V_13:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     fn strcmp([[V_14:__v[0-9]+]]: *const i8, [[V_15:__v[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     fn strncmp([[V_16:__v[0-9]+]]: *const i8, [[V_17:__v[0-9]+]]: *const i8, [[V_18:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1(mut ptr: *mut ()) {
// CHECK-NEXT:     if (unsafe { memcpy(ptr, ptr as *const (), ((8 as i32) as i64) as u64) }) != ptr {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut ptr: *mut i8) {
// CHECK-NEXT:     if (unsafe { mempcpy(ptr as *mut (), ptr as *const (), ((8 as i32) as i64) as u64) })
// CHECK-NEXT:         != ((unsafe { ptr.offset((8 as i32) as isize) }) as *mut ())
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut ptr: *mut ()) {
// CHECK-NEXT:     if (unsafe { memmove(ptr, ptr as *const (), ((8 as i32) as i64) as u64) }) != ptr {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4(mut ptr: *mut i8) {
// CHECK-NEXT:     if (unsafe { strcpy(ptr, ptr as *const i8) }) != ptr {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test5(mut ptr: *mut ()) {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             ptr as *const (),
// CHECK-NEXT:             ptr as *const (),
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test6(mut ptr: *const i8) {
// CHECK-NEXT:     if (unsafe { strcmp(ptr, ptr) }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test7(mut ptr: *const i8) {
// CHECK-NEXT:     if (unsafe { strncmp(ptr, ptr, ((8 as i32) as i64) as u64) }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 10] = [0 as i8; 10];
// CHECK-NEXT:     test1(buf.as_mut_ptr() as *mut ());
// CHECK-NEXT:     test2(buf.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     test3(buf.as_mut_ptr() as *mut ());
// CHECK-NEXT:     test4(buf.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     test5(buf.as_mut_ptr() as *mut ());
// CHECK-NEXT:     test6(buf.as_mut_ptr() as *const i8);
// CHECK-NEXT:     test7(buf.as_mut_ptr() as *const i8);
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
