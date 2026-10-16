typedef __SIZE_TYPE__ size_t;
extern void          *memcpy(void *__restrict, const void *__restrict, size_t);
extern void           abort(void);
extern void           exit(int);

typedef struct t {
  unsigned a : 16;
  unsigned b : 8;
  unsigned c : 8;
  long     d[4];
} *T;

typedef struct {
  long r[3];
} U;

T bar(U, unsigned int);

T foo(T x) {
  U d, u;

  memcpy(&u, &x->d[1], sizeof u);
  d = u;
  return bar(d, x->b);
}

T baz(T x) {
  U d, u;

  d.r[0] = 0x123456789;
  d.r[1] = 0xfedcba987;
  d.r[2] = 0xabcdef123;
  memcpy(&u, &x->d[1], sizeof u);
  d = u;
  return bar(d, x->b);
}

T bar(U d, unsigned int m) {
  if (d.r[0] != 21 || d.r[1] != 22 || d.r[2] != 23)
    abort();
  return 0;
}

struct t t = {26, 0, 0, {0, 21, 22, 23}};

int main(void) {
  baz(&t);
  foo(&t);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_3:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct t {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT:     d: [i64; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     r: [i64; 3],
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub a: u32,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub c: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: *mut t) -> *mut t {
// CHECK-NEXT:     let mut d: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(u) as *mut (),
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { (*x).d.as_mut_ptr() as *mut i64 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }) as *const (),
// CHECK-NEXT:             24 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     d = u;
// CHECK-NEXT:     bar(d, ((unsafe { (*x).__slate_bits_0.__get_b() }) as i32) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn baz(mut x: *mut t) -> *mut t {
// CHECK-NEXT:     let mut d: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (d.r.as_mut_ptr() as *mut i64).offset((0 as i32) as isize) } = 4886718345
// CHECK-NEXT:             as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (d.r.as_mut_ptr() as *mut i64).offset((1 as i32) as isize) } = 68414056839
// CHECK-NEXT:             as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (d.r.as_mut_ptr() as *mut i64).offset((2 as i32) as isize) } = 46118400291
// CHECK-NEXT:             as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             std::ptr::addr_of_mut!(u) as *mut (),
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { (*x).d.as_mut_ptr() as *mut i64 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }) as *const (),
// CHECK-NEXT:             24 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     d = u;
// CHECK-NEXT:     bar(d, ((unsafe { (*x).__slate_bits_0.__get_b() }) as i32) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut d: [[SLATERECORD_0]], mut m: u32) -> *mut t {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { (d.r.as_mut_ptr() as *mut i64).offset((0 as i32) as isize) }
// CHECK-NEXT:     }) != ((21 as i32) as i64)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (d.r.as_mut_ptr() as *mut i64).offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != ((22 as i32) as i64)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { (d.r.as_mut_ptr() as *mut i64).offset((2 as i32) as isize) }
// CHECK-NEXT:         }) != ((23 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<t>()
// CHECK-NEXT: }
// CHECK-NEXT: static mut t: t = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: t = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a((26 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b((0 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_c((0 as i32) as u32);
// CHECK-NEXT:     [[T_0]].d = [
// CHECK-NEXT:         (0 as i32) as i64,
// CHECK-NEXT:         (21 as i32) as i64,
// CHECK-NEXT:         (22 as i32) as i64,
// CHECK-NEXT:         (23 as i32) as i64,
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     baz(unsafe { std::ptr::addr_of_mut!(t) });
// CHECK-NEXT:     foo(unsafe { std::ptr::addr_of_mut!(t) });
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
