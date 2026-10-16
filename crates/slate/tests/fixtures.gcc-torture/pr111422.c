/* PR middle-end/111422 */

int         a, b;
int        *c = &b;
unsigned    d;
signed char e;
int         f = 1;

int foo(int k, signed char *l) {
  if (k < 6)
    return a;
  l[0] = l[1] = l[k - 1] = 8;
  return 0;
}

int bar(int k) {
  signed char g[11];
  int         h = foo(k, g);
  return h;
}

int main() {
  for (; b < 8; b = b + 1)
    ;
  int  j;
  int *n[8];
  for (j = 0; 18446744073709551608ULL + bar(*c) + *c + j < 2; j++)
    n[j] = &f;
  for (; e <= 4; e++)
    d = *n[0] == f;
  if (d != 1)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/111422
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut i32 = unsafe { std::ptr::addr_of_mut!(b) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut k: i32, mut l: *mut i8) -> i32 {
// CHECK-NEXT:     if k < (6 as i32) {
// CHECK-NEXT:         return unsafe { a };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { l.offset((k - (1 as i32)) as isize) } = (8 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { l.offset((1 as i32) as isize) } = (8 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { l.offset((0 as i32) as isize) } = (8 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut k: i32) -> i32 {
// CHECK-NEXT:     let mut g: [i8; 11] = [0 as i8; 11];
// CHECK-NEXT:     let mut h: i32 = foo(k, g.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     h
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { b }) < (8 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = (unsafe { b }) + (1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut n: __SlateAlign16<[*mut i32; 8]> = __SlateAlign16([0 as *mut i32; 8]);
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (18446744073709551608 as u64)
// CHECK-NEXT:         .wrapping_add((bar(unsafe { *unsafe { c } }) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(((unsafe { *unsafe { c } }) as i64) as u64)
// CHECK-NEXT:         .wrapping_add((j as i64) as u64)
// CHECK-NEXT:         < (((2 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (n.0.as_mut_ptr() as *mut *mut i32).offset(j as isize) } =
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(f) };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         j = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while ((unsafe { e }) as i32) <= (4 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = ((unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     *unsafe { (n.0.as_mut_ptr() as *mut *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) == unsafe { f }) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i8 = unsafe { e };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i8 = (([[V_2]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { d }) != ((1 as i32) as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
