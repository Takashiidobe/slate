#include <stdio.h>

#define weak __attribute__((__weak__))

struct proto {
    int id;
    const char *name;
};

struct rq;
extern __attribute__((section(".data..percpu..shared_aligned"))) struct rq runqueues
    __attribute__((__aligned__(64)));

const struct proto lookup_proto weak;
weak const struct proto table_proto;
void weak (*power_off)(void);
int counters[2][1] weak;

struct proto banner_proto weak;
const char banner[] weak;
struct proto banner_proto = {7, "banner"};
const char banner[] = "Linux version";

int main(void) {
    counters[1][0] = 5;
    printf("lookup %d %d\n", lookup_proto.id, lookup_proto.name == NULL);
    printf("table %d\n", table_proto.id);
    printf("power %d\n", power_off == NULL);
    printf("counters %d %d\n", counters[0][0], counters[1][0]);
    printf("banner %d %s %s\n", banner_proto.id, banner_proto.name, banner);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(linkage)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut runqueues: rq;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct proto {
// CHECK-NEXT:     id: i32,
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct rq {}
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut lookup_proto: proto = unsafe { core::mem::zeroed() };
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table_proto: proto = unsafe { core::mem::zeroed() };
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut power_off: Option<unsafe extern "C-unwind" fn()> = unsafe {
// CHECK-NEXT:     core::mem::zeroed()
// CHECK-NEXT: };
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut counters: {{\[\[}}i32; 1]; 2] = unsafe { core::mem::zeroed() };
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut banner_proto: proto = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: proto = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].id = 7i32;
// CHECK-NEXT:     [[T_0]].name = (b"banner\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[linkage = "weak"]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut banner: [i8; 14] = [
// CHECK-NEXT:     76, 105, 110, 117, 120, 32, 118, 101, 114, 115, 105, 111, 110, 0,
// CHECK-NEXT: ];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counters[1][0] = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "lookup {} {}", unsafe { lookup_proto.id }, unsafe { lookup_proto.name }
// CHECK-NEXT:         .is_null() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("table {}", unsafe { table_proto.id });
// CHECK-NEXT:     println!("power {}", ((unsafe { power_off }) == None) as i32);
// CHECK-NEXT:     println!("counters {} {}", unsafe { counters[0] [0] }, unsafe { counters[1] [0] });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"banner %d %s %s\n".as_ptr(),
// CHECK-NEXT:             banner_proto.id,
// CHECK-NEXT:             banner_proto.name,
// CHECK-NEXT:             core::ptr::addr_of!(banner) as *const i8,
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
