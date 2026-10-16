/* PR optimization/8726 */
/* Originator: Paul Eggert <eggert@twinsun.com> */

/* Verify that GCC doesn't miscompile tail calls on Sparc. */

extern void abort(void);

int fcntl_lock(int fd, int op, long long offset, long long count, int type);

int vfswrap_lock(char *fsp, int fd, int op, long long offset, long long count,
                 int type) {
  return fcntl_lock(fd, op, offset, count, type);
}

int fcntl_lock(int fd, int op, long long offset, long long count, int type) {
  return type;
}

int main(void) {
  if (vfswrap_lock(0, 1, 2, 3, 4, 5) != 5)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/8726
// CHECK-NEXT: //!
// CHECK-NEXT: //! Originator: Paul Eggert <eggert@twinsun.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Verify that GCC doesn't miscompile tail calls on Sparc.
// CHECK-NEXT: fn vfswrap_lock(
// CHECK-NEXT:     mut fsp: *mut i8,
// CHECK-NEXT:     mut fd: i32,
// CHECK-NEXT:     mut op: i32,
// CHECK-NEXT:     mut offset: i64,
// CHECK-NEXT:     mut count: i64,
// CHECK-NEXT:     mut r#type: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     fcntl_lock(fd, op, offset, count, r#type)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fcntl_lock(mut fd: i32, mut op: i32, mut offset: i64, mut count: i64, mut r#type: i32) -> i32 {
// CHECK-NEXT:     r#type
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if vfswrap_lock(
// CHECK-NEXT:         std::ptr::null_mut::<i8>(),
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         (3 as i32) as i64,
// CHECK-NEXT:         (4 as i32) as i64,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     ) != (5 as i32)
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
