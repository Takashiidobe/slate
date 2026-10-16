extern void abort(void);
extern void exit(int);

int   a[6], c = 1, d;
short e;

int __attribute__((noinline)) fn1(int p) { return a[p]; }

int main() {
  if (sizeof(long long) != 8)
    exit(0);

  a[0] = 1;
  if (c)
    e--;
  d           = e;
  long long f = e;
  if (fn1((f >> 56) & 1) != 0)
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut a: __SlateAlign16<[i32; 6]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: static mut c: i32 = 1 as i32;
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut e: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fn1(mut p: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }.offset(p as isize) }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as u64) != (((8 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut i32 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i16 = unsafe { e };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i16 = (([[V_1]] as i32) - (1 as i32)) as i16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = (unsafe { e }) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut f: i64 = (unsafe { e }) as i64;
// CHECK-NEXT:     if fn1((f >> (56 as i32) & ((1 as i32) as i64)) as i32) != (0 as i32) {
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
