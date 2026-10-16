/* PR tree-optimization/83383 */

unsigned long long int a    = 16ULL;
unsigned char          b    = 195;
unsigned long long int c    = ~0ULL;
unsigned char          d    = 1;
unsigned long long int e[2] = {3625445792498952486ULL, 0};
unsigned long long int f[2] = {0, 8985037393681294663ULL};
unsigned long long int g    = 5052410635626804928ULL;

void foo() {
  a = ((signed char)a) < b;
  c = (d ? e[0] : 0) - (f[1] * a ? 1 : g);
}

int main() {
  foo();
  if (a != 1 || c != 3625445792498952485ULL)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/83383
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u64 = 16 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u8 = ((195 as i32) as i8) as u8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: u64 = !(0 as u64);
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: u8 = ((1 as i32) as i8) as u8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: __SlateAlign16<[u64; 2]> =
// CHECK-NEXT:     __SlateAlign16([3625445792498952486 as u64, ((0 as i32) as i64) as u64]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: __SlateAlign16<[u64; 2]> =
// CHECK-NEXT:     __SlateAlign16([((0 as i32) as i64) as u64, 8985037393681294663 as u64]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: u64 = 5052410635626804928 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (((((unsafe { a }) as u8) as i8) as i32) < (((unsafe { b }) as u32) as i32)) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = if (unsafe { d }) != (0 as u8) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(e.0) as *mut u64 }.offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             ((0 as i32) as i64) as u64
// CHECK-NEXT:         }
// CHECK-NEXT:         .wrapping_sub(
// CHECK-NEXT:             if unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(f.0) as *mut u64 }.offset((1 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             .wrapping_mul(unsafe { a })
// CHECK-NEXT:                 != (0 as u64)
// CHECK-NEXT:             {
// CHECK-NEXT:                 ((1 as i32) as i64) as u64
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 unsafe { g }
// CHECK-NEXT:             },
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { a }) != (((1 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { c }) != (3625445792498952485 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
