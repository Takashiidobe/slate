#include <stdarg.h>
#include <stdio.h>

struct tiny { int c; };
struct pair { long a, b; };
struct mixed { double d; long l; };
struct floats { float f[4]; };
struct big { long a, b, c; };
struct odd { char c[3]; };
union number { double d; int i; };

static va_list saved;

static void sum(int n, ...) {
  va_list ap;
  va_start(ap, n);
  long total = 0;
  for (int i = 0; i < n; i++) {
    struct tiny t = va_arg(ap, struct tiny);
    struct pair p = va_arg(ap, struct pair);
    struct mixed m = va_arg(ap, struct mixed);
    struct floats f = va_arg(ap, struct floats);
    struct big b = va_arg(ap, struct big);
    struct odd o = va_arg(ap, struct odd);
    union number u = va_arg(ap, union number);
    long tail = va_arg(ap, long);
    total += t.c + p.a + p.b + (long)m.d + m.l + (long)(f.f[0] + f.f[1] + f.f[2] + f.f[3]);
    total += b.a + b.b + b.c + o.c[0] + o.c[1] + o.c[2] + (long)u.d + tail;
    printf("round %d: %ld\n", i, total);
  }
  va_end(ap);
}

static void copied(int n, ...) {
  va_list ap;
  va_start(ap, n);
  va_copy(saved, ap);
  struct pair first = va_arg(ap, struct pair);
  struct pair again = va_arg(saved, struct pair);
  printf("%ld %ld %ld %ld\n", first.a, first.b, again.a, again.b);
  va_end(ap);
  va_end(saved);
}

int main(void) {
  struct tiny t = {1};
  struct pair p = {2, 3};
  struct mixed m = {4.0, 5};
  struct floats f = {{1, 2, 3, 4}};
  struct big b = {6, 7, 8};
  struct odd o = {{1, 2, 3}};
  union number u = {.d = 9.0};
  sum(2, t, p, m, f, b, o, u, 10L, t, p, m, f, b, o, u, 11L);
  copied(1, p);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_sysv_va {
// CHECK-NEXT:     #[repr(C)]
// CHECK-NEXT:     struct Tag {
// CHECK-NEXT:         gp_offset: u32,
// CHECK-NEXT:         fp_offset: u32,
// CHECK-NEXT:         overflow: *const u8,
// CHECK-NEXT:         registers: *const u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     const GP_END: u32 = 48;
// CHECK-NEXT:     const FP_END: u32 = 176;
// CHECK-NEXT:     pub unsafe fn arg<L, T>(list: *mut L, classes: u32) -> T {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let tag = &mut *(list as *mut Tag);
// CHECK-NEXT:             let size = core::mem::size_of::<T>();
// CHECK-NEXT:             let mut out = core::mem::MaybeUninit::<T>::uninit();
// CHECK-NEXT:             let dst = out.as_mut_ptr() as *mut u8;
// CHECK-NEXT:             let (mut gp, mut sse) = (0, 0);
// CHECK-NEXT:             let mut rest = classes;
// CHECK-NEXT:             while rest != 0 {
// CHECK-NEXT:                 match rest & 3 {
// CHECK-NEXT:                     1 => gp += 8,
// CHECK-NEXT:                     _ => sse += 16,
// CHECK-NEXT:                 }
// CHECK-NEXT:                 rest >>= 2;
// CHECK-NEXT:             }
// CHECK-NEXT:             if classes != 0 && tag.gp_offset + gp <= GP_END
// CHECK-NEXT:                 && tag.fp_offset + sse <= FP_END
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut copied = 0;
// CHECK-NEXT:                 let mut rest = classes;
// CHECK-NEXT:                 while rest != 0 {
// CHECK-NEXT:                     let class = rest & 3;
// CHECK-NEXT:                     let (offset, width) = if class == 1 {
// CHECK-NEXT:                         (&mut tag.gp_offset, 8)
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         (&mut tag.fp_offset, if class == 3 { 16 } else { 8 })
// CHECK-NEXT:                     };
// CHECK-NEXT:                     let step = if class == 1 { 8 } else { 16 };
// CHECK-NEXT:                     core::ptr::copy_nonoverlapping(
// CHECK-NEXT:                         tag.registers.add(*offset as usize),
// CHECK-NEXT:                         dst.add(copied),
// CHECK-NEXT:                         width.min(size - copied),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     *offset += step;
// CHECK-NEXT:                     copied += width;
// CHECK-NEXT:                     rest >>= 2;
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let align = core::mem::align_of::<T>().max(8);
// CHECK-NEXT:                 let slot = ((tag.overflow as usize + align - 1) & !(align - 1))
// CHECK-NEXT:                     as *const u8;
// CHECK-NEXT:                 core::ptr::copy_nonoverlapping(slot, dst, size);
// CHECK-NEXT:                 tag.overflow = slot.add((size + 7) & !7);
// CHECK-NEXT:             }
// CHECK-NEXT:             out.assume_init()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tiny {
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mixed {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     l: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct floats {
// CHECK-NEXT:     f: [f32; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct big {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: i64,
// CHECK-NEXT:     c: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct odd {
// CHECK-NEXT:     c: [i8; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union number {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut saved: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT: unsafe extern "C-unwind" fn sum(n: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let t: tiny = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, tiny>(std::ptr::addr_of_mut!(ap), 1u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let p: pair = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, pair>(std::ptr::addr_of_mut!(ap), 5u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let m: mixed = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, mixed>(std::ptr::addr_of_mut!(ap), 6u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let f: floats = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, floats>(std::ptr::addr_of_mut!(ap), 10u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let b: big = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, big>(std::ptr::addr_of_mut!(ap), 0u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let o: odd = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, odd>(std::ptr::addr_of_mut!(ap), 1u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let u: number = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, number>(std::ptr::addr_of_mut!(ap), 1u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let tail: i64 = unsafe { ap.next_arg::<i64>() };
// CHECK-NEXT:         total
// CHECK-NEXT:             += (t.c as i64) + p.a + p.b + (m.d as i64) + m.l
// CHECK-NEXT:                 + ((f.f[0] + f.f[1] + f.f[2] + f.f[3]) as i64);
// CHECK-NEXT:         total
// CHECK-NEXT:             += b.a + b.b + b.c + ((o.c[0] as i32) as i64) + ((o.c[1] as i32) as i64)
// CHECK-NEXT:                 + ((o.c[2] as i32) as i64) + ((unsafe { u.d }) as i64) + tail;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"round %d: %ld\n".as_ptr(), i, total) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn copied(n: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         saved = ap.clone();
// CHECK-NEXT:     }
// CHECK-NEXT:     let first: pair = unsafe {
// CHECK-NEXT:         __slate_sysv_va::arg::<_, pair>(std::ptr::addr_of_mut!(ap), 5u32)
// CHECK-NEXT:     };
// CHECK-NEXT:     let again: pair = unsafe {
// CHECK-NEXT:         __slate_sysv_va::arg::<_, pair>(std::ptr::addr_of_mut!(saved), 5u32)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld %ld %ld %ld\n".as_ptr(), first.a, first.b, again.a, again.b) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let t: tiny = tiny { c: 1 };
// CHECK-NEXT:     let p: pair = pair { a: 2, b: 3 };
// CHECK-NEXT:     let m: mixed = mixed { d: 4.0f64, l: 5 };
// CHECK-NEXT:     let f: floats = floats {
// CHECK-NEXT:         f: [1i32 as f32, 2i32 as f32, 3i32 as f32, 4i32 as f32],
// CHECK-NEXT:     };
// CHECK-NEXT:     let b: big = big { a: 6, b: 7, c: 8 };
// CHECK-NEXT:     let o: odd = odd { c: [1, 2, 3] };
// CHECK-NEXT:     let u: number = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: number = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].d = 9.0f64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { sum(2i32, t, p, m, f, b, o, u, 10i64, t, p, m, f, b, o, u, 11i64) };
// CHECK-NEXT:     unsafe { copied(1i32, p) };
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
