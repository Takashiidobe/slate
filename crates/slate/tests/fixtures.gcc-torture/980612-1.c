void abort(void);
void exit(int);

struct fd {
  unsigned char a;
  unsigned char b;
} f = {5};

struct fd *g() { return &f; }
int        h() { return -1; }

int main() {
  struct fd *f = g();
  f->b         = h();
  if (((f->a & 0x7f) & ~0x10) <= 2)
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct fd {
// CHECK-NEXT:     a: u8,
// CHECK-NEXT:     b: u8,
// CHECK-NEXT: }
// CHECK-NEXT: static mut f: fd = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: fd = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].a = ((5 as i32) as i8) as u8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn g() -> *mut fd {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(f) }
// CHECK-NEXT: }
// CHECK-NEXT: fn h() -> i32 {
// CHECK-NEXT:     -(1 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[F_0:f_[0-9]+]]: *mut fd = g();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[F_0]]).b = (h() as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { (*[[F_0]]).a }) as u32) as i32) & (127 as i32) & !(16 as i32)
// CHECK-NEXT:         <= (2 as i32)
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
