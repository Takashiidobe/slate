void abort(void);
void exit(int);

static int g_7;
static int *volatile g_6 = &g_7;
int g_3;

static int f1(int *p_58) { return *p_58; }

void f2(int i) __attribute__((noinline));
void f2(int i) { g_3 = i; }

int f3(void) __attribute__((noinline));
int f3(void) {
  *g_6 = 1;
  f2(f1(&g_7));
  return 0;
}

int main() {
  f3();
  if (g_3 != 1)
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
// CHECK-NEXT: static mut g_7: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut g_6: *mut i32 = unsafe { std::ptr::addr_of_mut!(g_7) };
// CHECK-NEXT: static mut g_3: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn f1(mut p_58: *mut i32) -> i32 {
// CHECK-NEXT:     unsafe { *p_58 }
// CHECK-NEXT: }
// CHECK-NEXT: fn f2(mut i: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_3 = i;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f3() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { std::ptr::read_volatile(std::ptr::addr_of!(g_6)) } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     f2(f1(unsafe { std::ptr::addr_of_mut!(g_7) }));
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f3();
// CHECK-NEXT:     if (unsafe { g_3 }) != (1 as i32) {
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
