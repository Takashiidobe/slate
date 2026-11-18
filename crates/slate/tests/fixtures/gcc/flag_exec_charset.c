/* { dg-options "-fexec-charset=IBM1047" } */
int putchar(int);

static void put_hex(unsigned value) {
  unsigned high = value >> 4, low = value & 15;
  putchar(high < 10 ? 48 + high : 87 + high);
  putchar(low < 10 ? 48 + low : 87 + low);
  putchar(32);
}

int main(void) {
  const char text[] = "Az09 !";
  for (unsigned i = 0; i < sizeof text; i++)
    put_hex((unsigned char)text[i]);
  put_hex((unsigned char)'a');
  put_hex((unsigned char)'\n');
  putchar(10);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn putchar([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fexec-charset=IBM1047" }
// CHECK-NEXT: fn put_hex(value: u32) {
// CHECK-NEXT:     let high: u32 = value >> 4i32;
// CHECK-NEXT:     let low: u32 = value & 15;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         putchar(
// CHECK-NEXT:             (if high < 10 {
// CHECK-NEXT:                 48u32.wrapping_add(high)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 87u32.wrapping_add(high)
// CHECK-NEXT:             }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         putchar(
// CHECK-NEXT:             (if low < 10 {
// CHECK-NEXT:                 48u32.wrapping_add(low)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 87u32.wrapping_add(low)
// CHECK-NEXT:             }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { putchar(32i32) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let text: [i8; 7] = [-63, -87, -16, -7, 64, 90, 0];
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < (core::mem::size_of::<[i8; 7]>() as u64) {
// CHECK-NEXT:         put_hex((text[i as usize] as u8) as u32);
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     put_hex(129u32);
// CHECK-NEXT:     put_hex(37u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { putchar(10i32) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
