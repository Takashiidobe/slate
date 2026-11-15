#include <stdio.h>

struct desc_ptr {
    unsigned short size;
    unsigned long address;
} __attribute__((packed));

static int table[16];

static struct desc_ptr d = { .size = sizeof(table) - 1, .address = (unsigned long)table };

int main(void) {
    table[3] = 7;
    int *p = (int *)d.address;
    printf("%u %d %d\n", d.size, p[3], p == table);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct desc_ptr {
// CHECK-NEXT:     size: u16,
// CHECK-NEXT:     address: u64,
// CHECK-NEXT: }
// CHECK-NEXT: type __SlateImage_d = desc_ptr;
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut table: __SlateAlign16<[i32; 16]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     core::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut d: core::mem::MaybeUninit<__SlateImage_d> = unsafe {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: core::mem::MaybeUninit<__SlateImage_d> = core::mem::MaybeUninit::zeroed();
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = [[T_0]].as_mut_ptr();
// CHECK-NEXT:     (*[[T_1]]).size = (core::mem::size_of::<[i32; 16]>() as u64).wrapping_sub(1u64) as u16;
// CHECK-NEXT:     (core::ptr::addr_of_mut!((* [[T_1]]).address) as *mut *const ())
// CHECK-NEXT:         .write_unaligned(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 *mut i32,
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:             >(unsafe { core::ptr::addr_of_mut!(table.0) as *mut i32 })
// CHECK-NEXT:         });
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         table.0[3] = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     let p: *mut i32 = (unsafe {
// CHECK-NEXT:         (*(core::ptr::addr_of_mut!(d) as *mut __SlateImage_d)).address
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %d %d\n".as_ptr(),
// CHECK-NEXT:             (*(core::ptr::addr_of_mut!(d) as *mut __SlateImage_d)).size as i32,
// CHECK-NEXT:             *p.add(3),
// CHECK-NEXT:             (p == (core::ptr::addr_of_mut!(table.0) as *mut i32)) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
