void abort(void);
void exit(int);

int main(void) {
  int                tmp;
  unsigned long long utmp1, utmp2;

  tmp = 16;

  utmp1 = (~((unsigned long long)0)) >> tmp;
  utmp2 = (~((unsigned long long)0)) >> 16;

  if (utmp1 != utmp2)
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut tmp: i32 = 16 as i32;
// CHECK-NEXT:     let mut utmp1: u64 = !(((0 as i32) as i64) as u64) >> tmp;
// CHECK-NEXT:     let mut utmp2: u64 = !(((0 as i32) as i64) as u64) >> (16 as i32);
// CHECK-NEXT:     if utmp1 != utmp2 {
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
