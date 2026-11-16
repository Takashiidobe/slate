#include <stdio.h>

struct device_id {
  unsigned vendor, device;
  unsigned long data;
};

#define REPORT_CONFIG(option) (0 ? 0 : (unsigned long)&#option)

static const struct device_id table[] = {
    {1, 2, 0},
    {3, 4, REPORT_CONFIG(PARPORT_SERIAL)},
    {5, 6, (unsigned long)&"SERIAL_8250_MID"},
};

static unsigned long plain = (unsigned long)&"plain";
static const char (*whole)[6] = &"whole";

int main(void) {
  unsigned long local = (unsigned long)&"local";
  for (int i = 0; i < 3; i++)
    printf("%u %u %s\n", table[i].vendor, table[i].device,
           table[i].data ? (const char *)table[i].data : "-");
  printf("%s %s %s %zu\n", (const char *)plain, *whole, (const char *)local, sizeof *whole);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct device_id {
// CHECK-NEXT:     vendor: u32,
// CHECK-NEXT:     device: u32,
// CHECK-NEXT:     data: u64,
// CHECK-NEXT: }
// CHECK-NEXT: type __SlateImage_table = __SlateAlign16<[device_id; 3]>;
// CHECK-NEXT: type __SlateImage_plain = u64;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: static mut __slate_str10: [i8; 15] = [
// CHECK-NEXT:     80, 65, 82, 80, 79, 82, 84, 95, 83, 69, 82, 73, 65, 76, 0,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut __slate_str11: [i8; 16] = [
// CHECK-NEXT:     83, 69, 82, 73, 65, 76, 95, 56, 50, 53, 48, 95, 77, 73, 68, 0,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut table: core::mem::MaybeUninit<__SlateImage_table> = unsafe {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: core::mem::MaybeUninit<__SlateImage_table> = core::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (*[[T_1]]).0[0].vendor = 1u32;
// CHECK-NEXT:     (*[[T_1]]).0[0].device = 2u32;
// CHECK-NEXT:     (*[[T_1]]).0[0].data = 0u64;
// CHECK-NEXT:     (*[[T_1]]).0[1].vendor = 3u32;
// CHECK-NEXT:     (*[[T_1]]).0[1].device = 4u32;
// CHECK-NEXT:     (core::ptr::addr_of_mut!((* [[T_1]]).0[1].data) as *mut *const ())
// CHECK-NEXT:         .write_unaligned(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 *mut [i8; 15],
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(core::ptr::addr_of_mut!(__slate_str10))
// CHECK-NEXT:         });
// CHECK-NEXT:     (*[[T_1]]).0[2].vendor = 5u32;
// CHECK-NEXT:     (*[[T_1]]).0[2].device = 6u32;
// CHECK-NEXT:     (core::ptr::addr_of_mut!((* [[T_1]]).0[2].data) as *mut *const ())
// CHECK-NEXT:         .write_unaligned(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 *mut [i8; 16],
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(core::ptr::addr_of_mut!(__slate_str11))
// CHECK-NEXT:         });
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut __slate_str12: [i8; 6] = [112, 108, 97, 105, 110, 0];
// CHECK-NEXT: static mut plain: core::mem::MaybeUninit<__SlateImage_plain> = unsafe {
// CHECK-NEXT:     let mut [[T_0]]: core::mem::MaybeUninit<__SlateImage_plain> = core::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (core::ptr::addr_of_mut!(* [[T_1]]) as *mut *const ())
// CHECK-NEXT:         .write_unaligned(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 *mut [i8; 6],
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(core::ptr::addr_of_mut!(__slate_str12))
// CHECK-NEXT:         });
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut __slate_str13: [i8; 6] = [119, 104, 111, 108, 101, 0];
// CHECK-NEXT: static mut whole: *const [i8; 6] = core::ptr::addr_of_mut!(__slate_str13)
// CHECK-NEXT:     as *const [i8; 6];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let local: u64 = core::ptr::addr_of_mut!(__slate_str14) as u64;
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%u %u %s\n".as_ptr(),
// CHECK-NEXT:                 (*(core::ptr::addr_of_mut!(table) as *mut __SlateImage_table))
// CHECK-NEXT:                     .0[i as usize]
// CHECK-NEXT:                     .vendor,
// CHECK-NEXT:                 (*(core::ptr::addr_of_mut!(table) as *mut __SlateImage_table))
// CHECK-NEXT:                     .0[i as usize]
// CHECK-NEXT:                     .device,
// CHECK-NEXT:                 if (*(core::ptr::addr_of_mut!(table) as *mut __SlateImage_table))
// CHECK-NEXT:                     .0[i as usize]
// CHECK-NEXT:                     .data != 0
// CHECK-NEXT:                 {
// CHECK-NEXT:                     (*(core::ptr::addr_of_mut!(table) as *mut __SlateImage_table))
// CHECK-NEXT:                         .0[i as usize]
// CHECK-NEXT:                         .data as *const i8
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     c"-".as_ptr()
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %s %s %zu\n".as_ptr(),
// CHECK-NEXT:             *(core::ptr::addr_of_mut!(plain) as *mut __SlateImage_plain) as *const i8,
// CHECK-NEXT:             (*whole).as_ptr() as *const i8,
// CHECK-NEXT:             local as *const i8,
// CHECK-NEXT:             core::mem::size_of::<[i8; 6]>() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut __slate_str14: [i8; 6] = [108, 111, 99, 97, 108, 0];
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
