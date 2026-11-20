extern void console_put(int value);

static int counter;

static void step(int amount) { counter += amount; }

void *memset(void *dest, int value, unsigned long count) {
  unsigned char *bytes = dest;
  while (count--)
    *bytes++ = (unsigned char)value;
  return dest;
}

int main(void) {
  int buffer[4];
  memset(buffer, 0, sizeof buffer);
  step(3);
  step(4);
  console_put(counter + buffer[2]);
  return counter == 7 ? 0 : 1;
}

// merge-second-target: linux-i686 i686-unknown-linux-gnu clang
// merge-linux-args: -ffreestanding -fno-builtin
// merge-target-args: linux-i686 -ffreestanding -fno-builtin

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-i686: 1 compiles
// CHECK-NEXT:   freestanding_runtime.c: [] 1 compiled
// CHECK-NEXT: target linux-x86_64: 1 compiles
// CHECK-NEXT:   freestanding_runtime.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit freestanding_runtime: 12 items, 2 gated, 0 duplicated functions
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 6 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file freestanding_runtime.rs
// CHECK-NEXT: #[cfg(target_arch = "x86")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_arch = "x86_64")]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(export_name = "memset")]
// CHECK-NEXT: unsafe extern "C" fn __slate_export_memset(
// CHECK-NEXT:     dest: *mut core::ffi::c_void,
// CHECK-NEXT:     value: core::ffi::c_int,
// CHECK-NEXT:     count: usize,
// CHECK-NEXT: ) -> *mut core::ffi::c_void {
// CHECK-NEXT:     memset(dest as *mut (), value as i32, count as c_ulong) as *mut core::ffi::c_void
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C" fn main() -> i32 {
// CHECK-NEXT:     let mut buffer: __SlateAlignLargeArray<[i32; 4]> = __SlateAlignLargeArray([0i32; 4]);
// CHECK-NEXT:     memset(
// CHECK-NEXT:         buffer.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:         0i32,
// CHECK-NEXT:         core::mem::size_of::<[i32; 4]>() as c_ulong,
// CHECK-NEXT:     );
// CHECK-NEXT:     step(3i32);
// CHECK-NEXT:     step(4i32);
// CHECK-NEXT:     unsafe { console_put(counter + buffer.0[2]) };
// CHECK-NEXT:     if (unsafe { counter }) == 7 { 0 } else { 1 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // merge-second-target: linux-i686 i686-unknown-linux-gnu clang
// CHECK-NEXT: //
// CHECK-NEXT: // merge-linux-args: -ffreestanding -fno-builtin
// CHECK-NEXT: //
// CHECK-NEXT: // merge-target-args: linux-i686 -ffreestanding -fno-builtin
// CHECK-EMPTY:
// CHECK-NEXT: fn memset(dest: *mut (), value: i32, mut count: c_ulong) -> *mut () {
// CHECK-NEXT:     let mut bytes: *mut u8 = dest as *mut u8;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: c_ulong = count;
// CHECK-NEXT:         count = [[V_0]].wrapping_sub(1);
// CHECK-NEXT:         if [[V_0]] == 0 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: *mut u8 = bytes;
// CHECK-NEXT:         bytes = unsafe { [[V_1]].add(1) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_1]] = value as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     dest
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn step(amount: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter = counter + amount;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn console_put([[ARG_0:arg[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlignLargeArray<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-NEXT: file lib.rs
// CHECK-NEXT:     #[linkage = "weak"]
// CHECK-NEXT:     #[unsafe(no_mangle)]
// CHECK-NEXT:     extern "C" fn rust_eh_personality() {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #![no_std]
// CHECK-NEXT: #![no_builtins]
// CHECK-NEXT: #![allow(non_camel_case_types, non_snake_case, non_upper_case_globals, rustdoc::all)]
// CHECK-NEXT: #![allow(suspicious_runtime_symbol_definitions)]
// CHECK-NEXT: #![feature(linkage)]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(all(any(target_arch = "x86", target_arch = "x86_64"), target_os = "linux", target_env = "gnu")))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "freestanding_runtime.rs"]
// CHECK-NEXT: mod __slate_unit_freestanding_runtime;
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_panic {
// CHECK-NEXT:     #[panic_handler]
// CHECK-NEXT:     fn __slate_panic_handler(_: &core::panic::PanicInfo) -> ! {
// CHECK-NEXT:         loop {}
// CHECK-NEXT:     }
// CHECK-NEXT: file Cargo.toml profiles
// CHECK-NEXT: [profile.dev.package."<crate>"]
// CHECK-NEXT: codegen-units = 4096
// CHECK-NEXT: rustflags = ["-Zcodegen-source-order", "-Zhuman-readable-cgu-names", "-Zcross-crate-inline-threshold=never"]
// CHECK-NEXT: [profile.release.package."<crate>"]
// CHECK-NEXT: codegen-units = 4096
// CHECK-NEXT: rustflags = ["-Zcodegen-source-order", "-Zhuman-readable-cgu-names", "-Zcross-crate-inline-threshold=never"]
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
