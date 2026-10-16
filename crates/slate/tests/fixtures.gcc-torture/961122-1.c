void abort(void);
void exit(int);

long long acc;

void addhi(short a) { acc += (long long)a << 32; }

void subhi(short a) { acc -= (long long)a << 32; }

int main(void) {
  acc = 0xffff00000000ll;
  addhi(1);
  if (acc != 0x1000000000000ll)
    abort();
  subhi(1);
  if (acc != 0xffff00000000ll)
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
// CHECK-NEXT: static mut acc: i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn addhi(mut a: i16) {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64 = unsafe { acc };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]] + ((a as i64) << (32 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         acc = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn subhi(mut a: i16) {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i64 = unsafe { acc };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i64 = [[V_3]] - ((a as i64) << (32 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         acc = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         acc = 281470681743360 as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     addhi((1 as i32) as i16);
// CHECK-NEXT:     if (unsafe { acc }) != (281474976710656 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     subhi((1 as i32) as i16);
// CHECK-NEXT:     if (unsafe { acc }) != (281470681743360 as i64) {
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
