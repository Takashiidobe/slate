__attribute__((target("rtm"))) void abort_transaction(void) {
  __builtin_ia32_xabort(3 + 4);
}

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(rtm_target_feature)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.xabort"]
// CHECK-NEXT:     fn __slate_intrinsic_75bfe883d88838c4([[ARG_0:arg[0-9]+]]: i8);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "rtm")]
// CHECK-NEXT: unsafe fn abort_transaction() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_75bfe883d88838c4(const { ((3 as i32) + (4 as i32)) as i8 })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
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
