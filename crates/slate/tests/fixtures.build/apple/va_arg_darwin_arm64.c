// SLATE-BUILD-TARGETS: aarch64-apple-darwin
#include <stdarg.h>

struct small { int a, b; };
struct big { long a, b, c; };
struct hfa { double x, y; };
struct quad { double a, b, c, d; };
struct wide { _Alignas(16) long a; long b; };

int take_int(va_list ap) { return va_arg(ap, int); }
char take_char(va_list ap) { return (char)va_arg(ap, int); }
double take_double(va_list ap) { return va_arg(ap, double); }
long double take_long_double(va_list ap) { return va_arg(ap, long double); }
char *take_pointer(va_list ap) { return va_arg(ap, char *); }
__int128 take_int128(va_list ap) { return va_arg(ap, __int128); }
struct small take_small(va_list ap) { return va_arg(ap, struct small); }
struct big take_big(va_list ap) { return va_arg(ap, struct big); }
struct hfa take_hfa(va_list ap) { return va_arg(ap, struct hfa); }
struct quad take_quad(va_list ap) { return va_arg(ap, struct quad); }
struct wide take_wide(va_list ap) { return va_arg(ap, struct wide); }

long sum(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long total = 0;
  for (int i = 0; i < count; i++)
    total += take_int(ap);
  struct big b = va_arg(ap, struct big);
  va_end(ap);
  return total + b.a + b.c;
}

// SLATE-FILECHECK-BEGIN aarch64-apple-darwin
// CHECK: {{^}}mod __slate_darwin_arm64_va {
// CHECK-NEXT:     pub unsafe fn arg<L, T>(list: *mut L) -> T {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let cursor = &mut *(list as *mut *const u8);
// CHECK-NEXT:             let align = core::mem::align_of::<T>().max(8);
// CHECK-NEXT:             *cursor = cursor.map_addr(|address| (address + align - 1) & !(align - 1));
// CHECK-NEXT:             let value = core::ptr::read(*cursor as *const T);
// CHECK-NEXT:             *cursor = cursor.add(core::mem::size_of::<T>().next_multiple_of(8));
// CHECK-NEXT:             value
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub unsafe fn indirect<L, T>(list: *mut L) -> T {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let cursor = &mut *(list as *mut *const *const T);
// CHECK-NEXT:             let value = core::ptr::read(*cursor).read();
// CHECK-NEXT:             *cursor = cursor.add(1);
// CHECK-NEXT:             value
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct small {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct big {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: i64,
// CHECK-NEXT:     c: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct hfa {
// CHECK-NEXT:     x: f64,
// CHECK-NEXT:     y: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct quad {
// CHECK-NEXT:     a: f64,
// CHECK-NEXT:     b: f64,
// CHECK-NEXT:     c: f64,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct wide {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_int(mut ap: *mut i8) -> i32 {
// CHECK-NEXT:     unsafe { __slate_darwin_arm64_va::arg::<_, i32>(std::ptr::addr_of_mut!(ap)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_char(mut ap: *mut i8) -> i8 {
// CHECK-NEXT:     (unsafe { __slate_darwin_arm64_va::arg::<_, i32>(std::ptr::addr_of_mut!(ap)) }) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_double(mut ap: *mut i8) -> f64 {
// CHECK-NEXT:     unsafe { __slate_darwin_arm64_va::arg::<_, f64>(std::ptr::addr_of_mut!(ap)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_long_double(mut ap: *mut i8) -> f64 {
// CHECK-NEXT:     unsafe { __slate_darwin_arm64_va::arg::<_, f64>(std::ptr::addr_of_mut!(ap)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_pointer(mut ap: *mut i8) -> *mut i8 {
// CHECK-NEXT:     unsafe { __slate_darwin_arm64_va::arg::<_, *mut i8>(std::ptr::addr_of_mut!(ap)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_int128(mut ap: *mut i8) -> i128 {
// CHECK-NEXT:     unsafe { __slate_darwin_arm64_va::arg::<_, i128>(std::ptr::addr_of_mut!(ap)) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_small(mut ap: *mut i8) -> small {
// CHECK-NEXT:     return unsafe { __slate_darwin_arm64_va::arg::<_, small>(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_big(mut ap: *mut i8) -> big {
// CHECK-NEXT:     return unsafe { __slate_darwin_arm64_va::indirect::<_, big>(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_hfa(mut ap: *mut i8) -> hfa {
// CHECK-NEXT:     return unsafe { __slate_darwin_arm64_va::arg::<_, hfa>(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_quad(mut ap: *mut i8) -> quad {
// CHECK-NEXT:     return unsafe { __slate_darwin_arm64_va::arg::<_, quad>(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn take_wide(mut ap: *mut i8) -> wide {
// CHECK-NEXT:     return unsafe { __slate_darwin_arm64_va::arg::<_, wide>(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn sum(count: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut ap: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe { core::mem::transmute::<core::ffi::VaList<'_>, *mut i8>(__va_args.clone()) };
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = take_int(ap);
// CHECK-NEXT:         total += [[V_0]] as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let b: big = unsafe { __slate_darwin_arm64_va::indirect::<_, big>(std::ptr::addr_of_mut!(ap)) };
// CHECK-NEXT:     total + b.a + b.c
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
