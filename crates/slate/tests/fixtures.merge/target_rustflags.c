#include <stdio.h>

struct pair { int a; int b; };

struct pair make_pair(int a, int b) {
    struct pair p = {a, b};
    return p;
}

int main(void) {
    struct pair p = make_pair(3, 4);
    printf("%d %d\n", p.a, p.b);
    return 0;
}

// merge-second-target: linux-i686 i686-unknown-linux-gnu gcc
// merge-linux-args: -fstack-protector-all
// merge-target-args: linux-i686 -freg-struct-return -mregparm=3

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-i686: 1 compiles
// CHECK-NEXT:   target_rustflags.c: [] 1 compiled
// CHECK-NEXT: target linux-x86_64: 1 compiles
// CHECK-NEXT:   target_rustflags.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit target_rustflags: 6 items, 0 gated, 0 duplicated functions
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 6 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(all(target_arch = "x86", target_os = "linux", target_env = "gnu"), not(slate_profile_i686_unknown_linux_gnu)))]
// CHECK-NEXT: compile_error!("build i686-unknown-linux-gnu with --profile i686-unknown-linux-gnu");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"), not(slate_profile_default)))]
// CHECK-NEXT: compile_error!("build x86_64-unknown-linux-gnu with the dev or release profile");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(all(any(target_arch = "x86", target_arch = "x86_64"), target_os = "linux", target_env = "gnu")))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn make_pair(a: i32, b: i32) -> pair {
// CHECK-NEXT:     let p: pair = pair { a: a, b: b };
// CHECK-NEXT:     return p;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // merge-second-target: linux-i686 i686-unknown-linux-gnu gcc
// CHECK-NEXT: //
// CHECK-NEXT: // merge-linux-args: -fstack-protector-all
// CHECK-NEXT: //
// CHECK-NEXT: // merge-target-args: linux-i686 -freg-struct-return -mregparm=3
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: pair = make_pair(3i32, 4i32);
// CHECK-NEXT:     println!("{} {}", p.a, p.b);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: file Cargo.toml profiles
// CHECK-NEXT: [profile.dev.package."<crate>"]
// CHECK-NEXT: rustflags = ["-Zstack-protector=all", "-Clink-arg=-Wl,-z,nostart-stop-gc", "--cfg", "slate_profile_default", "--check-cfg=cfg(slate_profile_default, slate_profile_i686_unknown_linux_gnu)"]
// CHECK-NEXT: [profile.i686-unknown-linux-gnu]
// CHECK-NEXT: inherits = "release"
// CHECK-EMPTY:
// CHECK-NEXT: [profile.i686-unknown-linux-gnu.package."<crate>"]
// CHECK-NEXT: rustflags = ["-Zreg-struct-return", "-Zregparm=3", "-Cunsafe-allow-abi-mismatch=reg-struct-return,regparm", "-Clink-arg=-Wl,-z,nostart-stop-gc", "--cfg", "slate_profile_i686_unknown_linux_gnu", "--check-cfg=cfg(slate_profile_default, slate_profile_i686_unknown_linux_gnu)"]
// CHECK-NEXT: [profile.release.package."<crate>"]
// CHECK-NEXT: rustflags = ["-Zstack-protector=all", "-Clink-arg=-Wl,-z,nostart-stop-gc", "--cfg", "slate_profile_default", "--check-cfg=cfg(slate_profile_default, slate_profile_i686_unknown_linux_gnu)"]
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
