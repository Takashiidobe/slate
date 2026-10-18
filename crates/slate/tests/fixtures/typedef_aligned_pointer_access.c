#include <stdio.h>
#include <string.h>

typedef int A __attribute__((aligned(1)));
typedef struct S {
  long long x;
  int y;
} P __attribute__((aligned(1)));
typedef long long V __attribute__((vector_size(16)));
typedef struct W {
  V v;
} Q __attribute__((aligned(1)));

static int load(A *a) { return *a; }
static void store(A *a, int v) { *a = v; }
static long long sum(P *p) { return p->x + p->y; }
static void set(P *p) {
  p->x = 7;
  p->y = 9;
}
static long long lane(Q *q, int i) { return q->v[i]; }
static void put(Q *q, int i, long long v) { q->v[i] = v; }

int main(void) {
  char buffer[64];
  memset(buffer, 0, sizeof buffer);
  store((A *)(buffer + 1), 42);
  printf("%d\n", load((A *)(buffer + 1)));
  set((P *)(buffer + 3));
  printf("%lld\n", sum((P *)(buffer + 3)));
  put((Q *)(buffer + 17), 1, 5);
  put((Q *)(buffer + 17), 0, 4);
  printf("%lld %lld\n", lane((Q *)(buffer + 17), 0), lane((Q *)(buffer + 17), 1));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     x: i64,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct W {
// CHECK-NEXT:     v: std::simd::Simd<i64, 2>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn load(a: *mut i32) -> i32 {
// CHECK-NEXT:     unsafe { a.read_unaligned() }
// CHECK-NEXT: }
// CHECK-NEXT: fn store(a: *mut i32, v: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.write_unaligned(v);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn sum(p: *mut S) -> i64 {
// CHECK-NEXT:     (unsafe { std::ptr::addr_of!((* p).x).read_unaligned() })
// CHECK-NEXT:         + ((unsafe { std::ptr::addr_of!((* p).y).read_unaligned() }) as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn set(p: *mut S) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((* p).x).write_unaligned(7 as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((* p).y).write_unaligned(9 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn lane(q: *mut W, i: i32) -> i64 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of!((* q).v).read_unaligned()[i as usize] }
// CHECK-NEXT: }
// CHECK-NEXT: fn put(q: *mut W, i: i32, v: i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = v;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::ptr::addr_of_mut!((* q).v);
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]] = [[T_1]].read_unaligned();
// CHECK-NEXT:         [[T_2]][i as usize] = [[T_0]];
// CHECK-NEXT:         [[T_1]].write_unaligned([[T_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[i8; 64]> = __SlateAlign16([0 as i8; 64]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(buffer.0.as_mut_ptr() as *mut u8, 0 as u8, 64 as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     store(
// CHECK-NEXT:         (unsafe { (buffer.0.as_mut_ptr() as *mut i8).offset(1 as isize) }) as *mut i32,
// CHECK-NEXT:         42 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg0 = load(
// CHECK-NEXT:         (unsafe { (buffer.0.as_mut_ptr() as *mut i8).offset(1 as isize) }) as *mut i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     set((unsafe { (buffer.0.as_mut_ptr() as *mut i8).offset(3 as isize) }) as *mut S);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             sum(
// CHECK-NEXT:                 (unsafe { (buffer.0.as_mut_ptr() as *mut i8).offset(3 as isize) })
// CHECK-NEXT:                     as *mut S,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     put(
// CHECK-NEXT:         (unsafe { (buffer.0.as_mut_ptr() as *mut i8).offset(17 as isize) }) as *mut W,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         5 as i64,
// CHECK-NEXT:     );
// CHECK-NEXT:     put(
// CHECK-NEXT:         (unsafe { (buffer.0.as_mut_ptr() as *mut i8).offset(17 as isize) }) as *mut W,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         4 as i64,
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld %lld\n".as_ptr(),
// CHECK-NEXT:             lane(
// CHECK-NEXT:                 (unsafe { (buffer.0.as_mut_ptr() as *mut i8).offset(17 as isize) })
// CHECK-NEXT:                     as *mut W,
// CHECK-NEXT:                 0 as i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:             lane(
// CHECK-NEXT:                 (unsafe { (buffer.0.as_mut_ptr() as *mut i8).offset(17 as isize) })
// CHECK-NEXT:                     as *mut W,
// CHECK-NEXT:                 1 as i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
