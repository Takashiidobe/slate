#include <stdarg.h>
#include <stdio.h>

struct Tiny { char c[3]; };
struct Pair { long long a; int b; };
struct F2 { float x, y; };
struct F3 { float x, y, z; };
struct D4 { double a, b, c, d; };
struct Mixed { float f; int i; };
struct Big { long long v[5]; };
union UD { double d; double e[2]; };

static void dump(int count, ...) {
  va_list ap;
  va_start(ap, count);
  for (int round = 0; round < count; round++) {
    struct Tiny t = va_arg(ap, struct Tiny);
    struct Pair p = va_arg(ap, struct Pair);
    int marker = va_arg(ap, int);
    struct F2 f2 = va_arg(ap, struct F2);
    struct F3 f3 = va_arg(ap, struct F3);
    double scale = va_arg(ap, double);
    struct D4 d4 = va_arg(ap, struct D4);
    struct Mixed m = va_arg(ap, struct Mixed);
    struct Big big = va_arg(ap, struct Big);
    union UD ud = va_arg(ap, union UD);
    printf("%d: %c%c%c %lld %d %d %g %g %g %g %g %g %g %g %g %g %g %d", round, t.c[0],
           t.c[1], t.c[2], p.a, p.b, marker, f2.x, f2.y, f3.x, f3.y, f3.z, scale, d4.a,
           d4.b, d4.c, d4.d, m.f, m.i);
    printf(" %lld %lld %g %g\n", big.v[0], big.v[4], ud.e[0], ud.e[1]);
  }
  va_end(ap);
}

int main(void) {
  struct Tiny t = {{'a', 'b', 'c'}};
  struct Pair p = {1LL << 40, -7};
  struct F2 f2 = {1.5f, 2.5f};
  struct F3 f3 = {3.25f, 4.5f, -5.75f};
  struct D4 d4 = {0.5, 1.25, 2.125, -3.0625};
  struct Mixed m = {6.5f, 99};
  struct Big big = {{10, 20, 30, 40, 50}};
  union UD ud = {.e = {7.5, -8.25}};
  dump(3, t, p, 11, f2, f3, 0.125, d4, m, big, ud, t, p, 22, f2, f3, 0.25, d4, m, big, ud,
       t, p, 33, f2, f3, 0.5, d4, m, big, ud);
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
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Tiny {
// CHECK-NEXT:     c: [i8; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct F2 {
// CHECK-NEXT:     x: f32,
// CHECK-NEXT:     y: f32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct F3 {
// CHECK-NEXT:     x: f32,
// CHECK-NEXT:     y: f32,
// CHECK-NEXT:     z: f32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct D4 {
// CHECK-NEXT:     a: f64,
// CHECK-NEXT:     b: f64,
// CHECK-NEXT:     c: f64,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Mixed {
// CHECK-NEXT:     f: f32,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Big {
// CHECK-NEXT:     v: [i64; 5],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union UD {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     e: [f64; 2],
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn dump(count: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     for round in 0..count {
// CHECK-NEXT:         let t: Tiny = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, Tiny>(std::ptr::addr_of_mut!(ap), 1u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let p: Pair = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, Pair>(std::ptr::addr_of_mut!(ap), 5u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let marker: i32 = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:         let f2: F2 = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, F2>(std::ptr::addr_of_mut!(ap), 2u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let f3: F3 = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, F3>(std::ptr::addr_of_mut!(ap), 10u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let scale: f64 = unsafe { ap.next_arg::<f64>() };
// CHECK-NEXT:         let d4: D4 = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, D4>(std::ptr::addr_of_mut!(ap), 0u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let m: Mixed = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, Mixed>(std::ptr::addr_of_mut!(ap), 1u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let big: Big = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, Big>(std::ptr::addr_of_mut!(ap), 0u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let ud: UD = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<_, UD>(std::ptr::addr_of_mut!(ap), 10u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d: %c%c%c %lld %d %d %g %g %g %g %g %g %g %g %g %g %g %d".as_ptr(),
// CHECK-NEXT:                 round,
// CHECK-NEXT:                 t.c[0] as i32,
// CHECK-NEXT:                 t.c[1] as i32,
// CHECK-NEXT:                 t.c[2] as i32,
// CHECK-NEXT:                 p.a,
// CHECK-NEXT:                 p.b,
// CHECK-NEXT:                 marker,
// CHECK-NEXT:                 f2.x as f64,
// CHECK-NEXT:                 f2.y as f64,
// CHECK-NEXT:                 f3.x as f64,
// CHECK-NEXT:                 f3.y as f64,
// CHECK-NEXT:                 f3.z as f64,
// CHECK-NEXT:                 scale,
// CHECK-NEXT:                 d4.a,
// CHECK-NEXT:                 d4.b,
// CHECK-NEXT:                 d4.c,
// CHECK-NEXT:                 d4.d,
// CHECK-NEXT:                 m.f as f64,
// CHECK-NEXT:                 m.i,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(c" %lld %lld %g %g\n".as_ptr(), big.v[0], big.v[4], ud.e[0], ud.e[1])
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let t: Tiny = Tiny { c: [97, 98, 99] };
// CHECK-NEXT:     let p: Pair = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].a = 1099511627776i64;
// CHECK-NEXT:         [[T_0]].b = -7i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let f2: F2 = F2 { x: 1.5f32, y: 2.5f32 };
// CHECK-NEXT:     let f3: F3 = F3 {
// CHECK-NEXT:         x: 3.25f32,
// CHECK-NEXT:         y: 4.5f32,
// CHECK-NEXT:         z: -5.75f32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let d4: D4 = D4 {
// CHECK-NEXT:         a: 0.5f64,
// CHECK-NEXT:         b: 1.25f64,
// CHECK-NEXT:         c: 2.125f64,
// CHECK-NEXT:         d: -3.0625f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     let m: Mixed = Mixed { f: 6.5f32, i: 99 };
// CHECK-NEXT:     let big: Big = Big { v: [10, 20, 30, 40, 50] };
// CHECK-NEXT:     let ud: UD = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: UD = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].e = [7.5f64, -8.25f64];
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         dump(
// CHECK-NEXT:             3i32,
// CHECK-NEXT:             t,
// CHECK-NEXT:             p,
// CHECK-NEXT:             11i32,
// CHECK-NEXT:             f2,
// CHECK-NEXT:             f3,
// CHECK-NEXT:             0.125f64,
// CHECK-NEXT:             d4,
// CHECK-NEXT:             m,
// CHECK-NEXT:             big,
// CHECK-NEXT:             ud,
// CHECK-NEXT:             t,
// CHECK-NEXT:             p,
// CHECK-NEXT:             22i32,
// CHECK-NEXT:             f2,
// CHECK-NEXT:             f3,
// CHECK-NEXT:             0.25f64,
// CHECK-NEXT:             d4,
// CHECK-NEXT:             m,
// CHECK-NEXT:             big,
// CHECK-NEXT:             ud,
// CHECK-NEXT:             t,
// CHECK-NEXT:             p,
// CHECK-NEXT:             33i32,
// CHECK-NEXT:             f2,
// CHECK-NEXT:             f3,
// CHECK-NEXT:             0.5f64,
// CHECK-NEXT:             d4,
// CHECK-NEXT:             m,
// CHECK-NEXT:             big,
// CHECK-NEXT:             ud,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
