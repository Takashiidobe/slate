char headline[256];
struct hdr {
  char part1[9];
  char part2[8];
} p;

void __attribute__((noinline, noclone)) init() {
  __builtin_memcpy(p.part1, "FOOBARFOO", sizeof(p.part1));
  __builtin_memcpy(p.part2, "SPEC CPU", sizeof(p.part2));
}

int main() {
  char *x;
  int   c;
  init();
  __builtin_memcpy(&headline[0], p.part1, 9);
  c = 9;
  x = &headline[0];
  x = x + c;
  __builtin_memset(x, ' ', 245);
  __builtin_memcpy(&headline[10], p.part2, 8);
  c = 18;
  x = &headline[0];
  x = x + c;
  __builtin_memset(x, ' ', 238);
  if (headline[10] != 'S')
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: i32, [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut headline: __SlateAlign16<[i8; 256]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct hdr {
// CHECK-NEXT:     part1: [i8; 9],
// CHECK-NEXT:     part2: [i8; 8],
// CHECK-NEXT: }
// CHECK-NEXT: static mut p: hdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn init() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(p.part1) as *mut i8 }) as *mut (),
// CHECK-NEXT:             b"FOOBARFOO\0".as_ptr() as *const (),
// CHECK-NEXT:             9 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(p.part2) as *mut i8 }) as *mut (),
// CHECK-NEXT:             b"SPEC CPU\0".as_ptr() as *const (),
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut c: i32 = 9 as i32;
// CHECK-NEXT:     init();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(headline.0) as *mut i8 }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(p.part1) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((9 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     x = unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(headline.0) as *mut i8 }
// CHECK-NEXT:             .offset((0 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     x = unsafe { x.offset(c as isize) };
// CHECK-NEXT:     unsafe { __builtin_memset(x as *mut (), 32 as i32, ((245 as i32) as i64) as u64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(headline.0) as *mut i8 }
// CHECK-NEXT:                     .offset((10 as i32) as isize)
// CHECK-NEXT:             }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(p.part2) as *mut i8 }) as *const (),
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     c = 18 as i32;
// CHECK-NEXT:     x = unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(headline.0) as *mut i8 }
// CHECK-NEXT:             .offset((0 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     x = unsafe { x.offset(c as isize) };
// CHECK-NEXT:     unsafe { __builtin_memset(x as *mut (), 32 as i32, ((238 as i32) as i64) as u64) };
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(headline.0) as *mut i8 }
// CHECK-NEXT:                 .offset((10 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as i32) != (83 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
