void abort(void);
void exit(int);

typedef struct {
  short  s __attribute__((aligned(2), packed));
  double d __attribute__((aligned(2), packed));
} TRIAL;

int check(TRIAL *t) {
  if (t->s != 1 || t->d != 16.0)
    return 1;
  return 0;
}

int main(void) {
  TRIAL trial;

  trial.s = 1;
  trial.d = 16.0;

  if (check(&trial) != 0)
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
// CHECK-NEXT: #[repr(C, packed(2))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     s: i16,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-NEXT: fn check(mut t: &[[SLATERECORD_0]]) -> i32 {
// CHECK-NEXT:     if ((unsafe { (*(t as *const [[SLATERECORD_0]])).s }) as i32) != (1 as i32)
// CHECK-NEXT:         || (unsafe { (*(t as *const [[SLATERECORD_0]])).d }) != 16.0f64
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut trial: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     trial.s = (1 as i32) as i16;
// CHECK-NEXT:     trial.d = 16.0f64;
// CHECK-NEXT:     if check(unsafe { &(*std::ptr::addr_of_mut!(trial)) }) != (0 as i32) {
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
