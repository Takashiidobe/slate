/* On IRIX 6, PA is passed partially in registers and partially on the
   stack.  We therefore have two potential uses of pretend_args_size:
   one for the partial argument and one for the varargs save area.
   Make sure that these uses don't conflict.  */

void abort(void);
void exit(int);

struct s {
  int i[18];
};

int f(struct s pa, int pb, ...) { return pb; }

struct s gs;

int main() {
  if (f(gs, 0x1234) != 0x1234)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! On IRIX 6, PA is passed partially in registers and partially on the
// CHECK-NEXT: //! stack.  We therefore have two potential uses of pretend_args_size:
// CHECK-NEXT: //! one for the partial argument and one for the varargs save area.
// CHECK-NEXT: //! Make sure that these uses don't conflict.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     i: [i32; 18],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f(mut pa: s, mut pb: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     pb
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut gs: s = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { f(unsafe { gs }, 4660 as i32) }) != (4660 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
