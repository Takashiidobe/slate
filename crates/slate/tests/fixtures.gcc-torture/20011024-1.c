/* Test whether store motion recognizes pure functions as potentially reading
   any memory.  */

typedef __SIZE_TYPE__ size_t;
extern void           abort(void);
extern void          *memcpy(void *dest, const void *src, size_t n);
extern size_t         strlen(const char *s);
extern int strcmp(const char *s1, const char *s2) __attribute__((pure));

char buf[50];

static void foo(void) {
  if (memcpy(buf, "abc", 4) != buf)
    abort();
  if (strcmp(buf, "abc"))
    abort();
  memcpy(buf, "abcdefgh", strlen("abcdefgh") + 1);
}

int main(void) {
  foo();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test whether store motion recognizes pure functions as potentially reading
// CHECK-NEXT: //! any memory.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memcpy(dest: *mut (), src: *const (), n: u64) -> *mut ();
// CHECK-NEXT:     fn strlen(s: *const i8) -> u64;
// CHECK-NEXT:     fn strcmp(s1: *const i8, s2: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut buf: __SlateAlign16<[i8; 50]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             b"abc\0".as_ptr() as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != ((unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *mut ())
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *const i8,
// CHECK-NEXT:             c"abc".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             b"abcdefgh\0".as_ptr() as *const (),
// CHECK-NEXT:             unsafe { strlen(c"abcdefgh".as_ptr() as *const i8) }
// CHECK-NEXT:                 .wrapping_add(((1 as i32) as i64) as u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
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
