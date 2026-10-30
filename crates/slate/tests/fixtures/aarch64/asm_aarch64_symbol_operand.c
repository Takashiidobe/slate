#include <stdio.h>

int counter = 5;
int table[4] = {10, 20, 30, 40};
static long hidden = 9;

static int twice(int value) {
    return value * 2;
}

static void *address_of(void) {
    void *address;
    __asm__("adrp %0, %1\n\tadd %0, %0, :lo12:%1" : "=r"(address) : "S"(&counter));
    return address;
}

int main(void) {
    printf("%d\n", address_of() == (void *)&counter);
    int *slot;
    __asm__("adrp %0, %1\n\tadd %0, %0, :lo12:%1" : "=r"(slot) : "S"(&table[2]));
    printf("%d\n", *slot);
    long *place;
    __asm__("adrp %0, %c1\n\tadd %0, %0, :lo12:%c1" : "=r"(place) : "S"(&hidden));
    printf("%ld\n", *place);
    int (*function)(int);
    __asm__("adr %0, %1" : "=r"(function) : "S"(twice));
    printf("%d\n", function(21));
    int loaded;
    __asm__("adrp %x0, %1\n\tldr %w0, [%x0, :lo12:%1]" : "=&r"(loaded) : "S"(&table[3]));
    printf("%d\n", loaded);
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut counter: i32 = 5;
// CHECK-NEXT: static mut table: [i32; 4] = [10, 20, 30, 40];
// CHECK-NEXT: static mut hidden: i64 = 9;
// CHECK-NEXT: extern "C-unwind" fn twice(value: i32) -> i32 {
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn address_of() -> *mut () {
// CHECK-NEXT:     let mut address: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "adrp {0:x}, {1}\n\tadd {0:x}, {0:x}, :lo12:{1}", lateout(reg) [[T_0]], sym
// CHECK-NEXT:             counter, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         address = unsafe { std::mem::transmute::<u64, *mut ()>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     address
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (address_of() == ((unsafe { std::ptr::addr_of_mut!(counter) }) as * mut
// CHECK-NEXT:         ())) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut slot: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "adrp {0:x}, {1}+8\n\tadd {0:x}, {0:x}, :lo12:{1}+8", lateout(reg) [[T_0]], sym
// CHECK-NEXT:             table, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         slot = unsafe { std::mem::transmute::<u64, *mut i32>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { * slot });
// CHECK-NEXT:     let mut place: *mut i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "adrp {0:x}, {1}\n\tadd {0:x}, {0:x}, :lo12:{1}", lateout(reg) [[T_1]], sym
// CHECK-NEXT:             hidden, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         place = unsafe { std::mem::transmute::<u64, *mut i64>([[T_1]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), *place) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut function: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "adr {0:x}, {1}", lateout(reg) [[T_2]], sym twice, options(pure, nomem, nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         function = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 u64,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             >([[T_2]] as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { function.unwrap() (21i32) });
// CHECK-NEXT:     let mut loaded: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "adrp {0:x}, {1}+12\n\tldr {0:w}, [{0:x}, :lo12:{1}+12]", out(reg) [[T_3]], sym
// CHECK-NEXT:             table, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         loaded = unsafe { std::mem::transmute::<u32, i32>([[T_3]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", loaded);
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
