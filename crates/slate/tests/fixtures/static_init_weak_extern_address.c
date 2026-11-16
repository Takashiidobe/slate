#include <stddef.h>
#include <stdio.h>

struct attribute {
    const char *name;
    int mode;
};

struct kobj_attribute {
    struct attribute attr;
    long (*show)(void);
};

extern __attribute__((weak)) struct kobj_attribute missing_vendor;
extern __attribute__((weak)) struct kobj_attribute missing_runtime;
extern __attribute__((weak)) int missing_counter;

static struct kobj_attribute present = {{"present", 0444}, NULL};

static struct attribute *attributes[] = {
    &present.attr,
    &missing_vendor.attr,
    &missing_runtime.attr,
    NULL,
};

static int *counter = &missing_counter;
static struct kobj_attribute *whole = &missing_runtime;

int main(void) {
    for (unsigned i = 0; i < 4; i++)
        printf("attribute %u null %d offset %zu\n", i, attributes[i] == NULL,
               attributes[i] == &present.attr ? 0 : (size_t)attributes[i]);
    printf("counter %d whole %d\n", counter == NULL, whole == NULL);
    printf("runtime %d %d\n", &missing_vendor == NULL, &missing_counter == NULL);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(linkage)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static missing_vendor: *mut kobj_attribute;
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static missing_runtime: *mut kobj_attribute;
// CHECK-NEXT:     #[linkage = "extern_weak"]
// CHECK-NEXT:     static missing_counter: *mut i32;
// CHECK-NEXT:     #[link_name = "missing_vendor"]
// CHECK-NEXT:     static mut __slate_weak_address_missing_vendor: kobj_attribute;
// CHECK-NEXT:     #[link_name = "missing_runtime"]
// CHECK-NEXT:     static mut __slate_weak_address_missing_runtime: kobj_attribute;
// CHECK-NEXT:     #[link_name = "missing_counter"]
// CHECK-NEXT:     static mut __slate_weak_address_missing_counter: i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct attribute {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     mode: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct kobj_attribute {
// CHECK-NEXT:     attr: attribute,
// CHECK-NEXT:     show: Option<unsafe extern "C-unwind" fn() -> i64>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: static mut present: kobj_attribute = kobj_attribute {
// CHECK-NEXT:     attr: {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: attribute = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].name = (b"present\0".as_ptr() as *mut i8) as *const i8;
// CHECK-NEXT:         [[T_0]].mode = 292i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT:     show: None,
// CHECK-NEXT: };
// CHECK-NEXT: static mut attributes: __SlateAlign16<[*mut attribute; 4]> = __SlateAlign16([
// CHECK-NEXT:     unsafe { core::ptr::addr_of_mut!(present.attr) },
// CHECK-NEXT:     unsafe { core::ptr::addr_of_mut!(__slate_weak_address_missing_vendor.attr) },
// CHECK-NEXT:     unsafe { core::ptr::addr_of_mut!(__slate_weak_address_missing_runtime.attr) },
// CHECK-NEXT:     core::ptr::null_mut::<attribute>(),
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut counter: *mut i32 = unsafe {
// CHECK-NEXT:     core::ptr::addr_of_mut!(__slate_weak_address_missing_counter)
// CHECK-NEXT: };
// CHECK-NEXT: static mut whole: *mut kobj_attribute = unsafe {
// CHECK-NEXT:     core::ptr::addr_of_mut!(__slate_weak_address_missing_runtime)
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0u32..4u32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"attribute %u null %d offset %zu\n".as_ptr(),
// CHECK-NEXT:                 i,
// CHECK-NEXT:                 unsafe { attributes.0[i as usize] }.is_null() as i32,
// CHECK-NEXT:                 if attributes.0[i as usize] == core::ptr::addr_of_mut!(present.attr) {
// CHECK-NEXT:                     0u64
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     attributes.0[i as usize] as u64
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "counter {} whole {}", unsafe { counter } .is_null() as i32, unsafe { whole }
// CHECK-NEXT:         .is_null() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "runtime {} {}", unsafe { core::ptr::addr_of_mut!(* missing_vendor) } .is_null()
// CHECK-NEXT:         as i32, unsafe { core::ptr::addr_of_mut!(* missing_counter) } .is_null() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: core::arch::global_asm!(".weak {}", sym __slate_weak_address_missing_vendor);
// CHECK-NEXT: core::arch::global_asm!(".weak {}", sym __slate_weak_address_missing_runtime);
// CHECK-NEXT: core::arch::global_asm!(".weak {}", sym __slate_weak_address_missing_counter);
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
