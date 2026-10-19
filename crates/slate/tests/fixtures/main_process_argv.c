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
// CHECK-NEXT:     fn open(__file: *const i8, __oflag: i32, ...) -> i32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT:     fn close(__fd: i32) -> i32;
// CHECK-NEXT:     fn read(__fd: i32, __buf: *mut (), __nbytes: u64) -> i64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main(argc: i32, argv: *mut *mut i8, envp: *mut *mut i8) -> i32 {
// CHECK-NEXT:     println!("envp {} argc {}", (envp == unsafe { environ }) as i32, argc);
// CHECK-NEXT:     let len: u64 = unsafe { strlen(*argv.offset(0 as isize) as *const i8) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             (*argv.offset(0 as isize) as *mut ()) as *mut u8,
// CHECK-NEXT:             0u8,
// CHECK-NEXT:             len as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             c"renamed".as_ptr() as *const u8,
// CHECK-NEXT:             (*argv.offset(0 as isize) as *mut ()) as *mut u8,
// CHECK-NEXT:             (if len < 7 { len } else { 7u64 }) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 64]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let fd: i32 = unsafe { open(c"/proc/self/cmdline".as_ptr(), 0i32) };
// CHECK-NEXT:     let n: i64 = unsafe { read(fd, buf.0.as_mut_ptr() as *mut (), 63u64) };
// CHECK-NEXT:     unsafe { close(fd) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%d %s\n".as_ptr(), (n > 0) as i32, buf.0.as_mut_ptr() as *mut i8)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "environ after argv {}", (unsafe { * unsafe { environ } .offset(0 as isize) }
// CHECK-NEXT:         .is_null() || (unsafe { * unsafe { environ } .offset(0 as isize) }) > unsafe { *
// CHECK-NEXT:         argv.offset((argc - 1) as isize) }) as i32
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
