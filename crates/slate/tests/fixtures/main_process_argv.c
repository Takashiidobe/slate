#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char **argv, char **envp) {
    printf("envp %d argc %d\n", envp == environ, argc);
    size_t len = strlen(argv[0]);
    memset(argv[0], 0, len);
    memcpy(argv[0], "renamed", len < 7 ? len : 7);
    char buf[64] = {0};
    int fd = open("/proc/self/cmdline", O_RDONLY);
    ssize_t n = read(fd, buf, sizeof buf - 1);
    close(fd);
    printf("%d %s\n", n > 0, buf);
    printf("environ after argv %d\n", environ[0] == NULL || environ[0] > argv[argc - 1]);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut environ: *mut *mut i8;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::open;
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: use slate_support::memset;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-NEXT: use slate_support::close;
// CHECK-NEXT: use slate_support::read;
// CHECK-NEXT: fn __slate_main(argc: i32, argv: *mut *mut i8, envp: *mut *mut i8) -> i32 {
// CHECK-NEXT:     println!("envp {} argc {}", (envp == unsafe { environ }) as i32, argc);
// CHECK-NEXT:     let len: u64 = unsafe {
// CHECK-NEXT:         strlen((*argv.add(0) as *const i8) as *const core::ffi::c_char) as u64
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             ((*argv.add(0) as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (0i32 as i32) as u8,
// CHECK-NEXT:             (len as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((b"renamed\0".as_ptr() as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((*argv.add(0) as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((if len < 7 { len } else { 7u64 }) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 64]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let fd: i32 = unsafe {
// CHECK-NEXT:         open(
// CHECK-NEXT:             c"/proc/self/cmdline".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             (slate_support::O_RDONLY as i32) as i32,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let n: i64 = unsafe {
// CHECK-NEXT:         read(
// CHECK-NEXT:             fd as i32,
// CHECK-NEXT:             (buf.0.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             (std::mem::size_of::<[i8; 64]>() as u64).wrapping_sub(1) as usize,
// CHECK-NEXT:         ) as i64
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { close(fd as i32) as i32 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %s\n".as_ptr(),
// CHECK-NEXT:             (n > 0) as i32,
// CHECK-NEXT:             buf.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "environ after argv {}", (unsafe { * unsafe { environ } .add(0) } .is_null() ||
// CHECK-NEXT:         (unsafe { * unsafe { environ } .add(0) }) > unsafe { * argv.offset((argc - 1) as
// CHECK-NEXT:         isize) }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _, envp as _) }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
