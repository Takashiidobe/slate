#include <stdio.h>

__attribute__((used)) static int lo12 = 3;
__attribute__((used)) static int helper_value = 7;
int stored;

__asm__(".text\n"
        ".globl store_helper\n"
        "store_helper:\n"
        "\tadrp x9, helper_value\n"
        "\tldr w10, [x9, :lo12:helper_value]\n"
        "\tadrp x9, lo12\n"
        "\tldr w11, [x9, :lo12:lo12]\n"
        "\tadd w10, w10, w11\n"
        "\tadrp x9, stored\n"
        "\tstr w10, [x9, :lo12:stored]\n"
        "\tret\n");

void store_helper(void);

int main(void) {
    store_helper();
    printf("%d\n", stored);
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(used_with_arg)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn store_helper();
// CHECK-NEXT: }
// CHECK-NEXT: #[used(compiler)]
// CHECK-NEXT: static mut lo12: i32 = 3;
// CHECK-NEXT: #[used(compiler)]
// CHECK-NEXT: static mut helper_value: i32 = 7;
// CHECK-NEXT: static mut stored: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".text\n.globl store_helper\nstore_helper:\n\tadrp x9, {0}\n\tldr w10, [x9, :lo12:{0}]\n\tadrp x9, {1}\n\tldr w11, [x9, :lo12:{1}]\n\tadd w10, w10, w11\n\tadrp x9, {2}\n\tstr w10, [x9, :lo12:{2}]\n\tret\n",
// CHECK-NEXT:     sym helper_value, sym lo12, sym stored
// CHECK-NEXT: );
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { store_helper() };
// CHECK-NEXT:     println!("{}", unsafe { stored });
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
