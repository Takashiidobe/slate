void abort(void);
void exit(int);

typedef enum foo E;
enum foo { e0, e1 };

struct {
  E eval;
} s;

void p(void) { abort(); }

void f(void) {
  switch (s.eval) {
  case e0:
    p();
  }
}

int main(void) {
  s.eval = e1;
  f();
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
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     eval: u32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut s: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn p() {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     match (unsafe { s.eval }) as u32 {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             p();
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.eval = (1 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     f();
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
