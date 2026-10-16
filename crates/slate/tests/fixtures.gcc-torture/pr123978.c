/* PR middle-end/123978 */

struct A {
  unsigned b, c, *d;
};

[[gnu::noipa]] int foo(struct A *a) {
  __builtin_memset(a->d, 0, ((long long)sizeof(unsigned)) * a->b * a->c);
  return 0;
}

int main() {
  struct A a;
  unsigned b[256];
  __builtin_memset(b, 0x55, sizeof(b));
  a.b = 15;
  a.c = 15;
  a.d = b;
  foo(&a);
  for (int i = 0; i < 225; ++i)
    if (b[i] != 0)
      __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/123978
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     b: u32,
// CHECK-NEXT:     c: u32,
// CHECK-NEXT:     d: *mut u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: &A) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             (unsafe { (*(a as *const A)).d }) as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (((4 as u64) as i64)
// CHECK-NEXT:                 * (((unsafe { (*(a as *const A)).b }) as u64) as i64)
// CHECK-NEXT:                 * (((unsafe { (*(a as *const A)).c }) as u64) as i64)) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: __SlateAlign16<[u32; 256]> = __SlateAlign16([0 as u32; 256]);
// CHECK-NEXT:     unsafe { __builtin_memset(b.0.as_mut_ptr() as *mut (), 85 as i32, 1024 as u64) };
// CHECK-NEXT:     a.b = (15 as i32) as u32;
// CHECK-NEXT:     a.c = (15 as i32) as u32;
// CHECK-NEXT:     a.d = b.0.as_mut_ptr() as *mut u32;
// CHECK-NEXT:     foo(unsafe { &(*std::ptr::addr_of_mut!(a)) });
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (225 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { (b.0.as_mut_ptr() as *mut u32).offset(i as isize) } })
// CHECK-NEXT:             != ((0 as i32) as u32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
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
