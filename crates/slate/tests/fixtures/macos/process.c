#include <dlfcn.h>
#include <fnmatch.h>
#include <poll.h>
#include <pwd.h>
#include <spawn.h>
#include <stddef.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

_Static_assert(sizeof(struct termios) == 72, "termios size");
_Static_assert(sizeof(struct passwd) == 72, "passwd size");
_Static_assert(sizeof(posix_spawnattr_t) == 8, "posix_spawnattr_t size");
_Static_assert(P_ALL == 0, "P_ALL");
_Static_assert(P_PID == 1, "P_PID");
_Static_assert(FNM_NOESCAPE == 0x01, "FNM_NOESCAPE");
_Static_assert(_SC_PAGESIZE == 29, "_SC_PAGESIZE");
_Static_assert(_SC_NPROCESSORS_ONLN == 58, "_SC_NPROCESSORS_ONLN");

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_spawn_and_wait(pid_t *child, const char *path, char *const argv[],
                         char *const envp[]) {
  int status = 0;
  int result = posix_spawn(child, path, NULL, NULL, argv, envp);
  if (result != 0)
    return result;
  return waitpid(*child, &status, 0) < 0 ? -1 : status;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
long slate_terminal_page_size(int fd, struct termios *state) {
  if (tcgetattr(fd, state) != 0)
    return -1;
  return sysconf(_SC_PAGESIZE);
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int slate_lookup_user_shell(uid_t uid, char *buffer, size_t size) {
  struct passwd *entry = getpwuid(uid);
  if (entry == NULL)
    return -1;
  size_t length = 0;
  while (entry->pw_shell[length] != '\0' && length + 1 < size)
    length++;
  __builtin_memcpy(buffer, entry->pw_shell, length);
  buffer[length] = '\0';
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS-DAG: unsafe fn slate_spawn_and_wait(
// LOWERING-MACOS-DAG:     {{arg[0-9]+}}: *mut i32,
// LOWERING-MACOS-DAG:     {{arg[0-9]+}}: *mut i8,
// LOWERING-MACOS-DAG:     {{arg[0-9]+}}: *mut *mut i8,
// LOWERING-MACOS-DAG:     {{arg[0-9]+}}: *mut *mut i8,
// LOWERING-MACOS-DAG: ) -> i32 {
// LOWERING-MACOS-DAG:     let mut __retval: i32 = 0;
// LOWERING-MACOS-DAG:     let mut status: i32 = 0;
// LOWERING-MACOS-DAG:     let mut result: i32 = 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:     status = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         posix_spawn(
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *mut i32,
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *const *mut core::ffi::c_void,
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *const *mut core::ffi::c_void,
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-MACOS-DAG:             {{arg[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     result = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = result;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = result;
// LOWERING-MACOS-DAG:             __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-MACOS-DAG:             return {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-MACOS-DAG:         waitpid(
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as i32,
// LOWERING-MACOS-DAG:             std::ptr::addr_of_mut!(status) as *mut i32,
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as i32,
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = -1;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     } else {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = status;
// LOWERING-MACOS-DAG:         {{_v[0-9]+}}
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: fn slate_terminal_page_size({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut termios) -> i64 {
// LOWERING-MACOS-DAG:     let mut fd: i32 = 0;
// LOWERING-MACOS-DAG:     let mut state: *mut termios = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let mut __retval: i64 = 0;
// LOWERING-MACOS-DAG:     fd = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     state = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = fd;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut termios = state;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = unsafe { tcgetattr({{_v[0-9]+}} as i32, {{_v[0-9]+}} as *mut termios) };
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i64 = -1;
// LOWERING-MACOS-DAG:             __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i64 = __retval;
// LOWERING-MACOS-DAG:             return {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 29;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i64 = unsafe { sysconf({{_v[0-9]+}} as i32) };
// LOWERING-MACOS-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i64 = __retval;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// LOWERING-MACOS-DAG: unsafe fn slate_lookup_user_shell({{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-MACOS-DAG:     let mut size: u64 = 0;
// LOWERING-MACOS-DAG:     let mut __retval: i32 = 0;
// LOWERING-MACOS-DAG:     let mut entry: *mut passwd = std::ptr::null_mut();
// LOWERING-MACOS-DAG:     let mut length: u64 = 0;
// LOWERING-MACOS-DAG:     size = {{arg[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut passwd = unsafe { getpwuid({{arg[0-9]+}} as u32) };
// LOWERING-MACOS-DAG:     entry = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut passwd = entry;
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: *mut passwd = std::ptr::null_mut();
// LOWERING-MACOS-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = -1;
// LOWERING-MACOS-DAG:             __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-MACOS-DAG:             return {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-MACOS-DAG:     length = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     {
// LOWERING-MACOS-DAG:         loop {
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: u64 = length;
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: *mut passwd = entry;
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: *mut i8 = unsafe { (*{{_v[0-9]+}}).pw_shell };
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-MACOS-DAG:                 let {{_v[0-9]+}}: u64 = length;
// LOWERING-MACOS-DAG:                 let {{_v[0-9]+}}: u64 = 1;
// LOWERING-MACOS-DAG:                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-MACOS-DAG:                 let {{_v[0-9]+}}: u64 = size;
// LOWERING-MACOS-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-MACOS-DAG:                 {{_v[0-9]+}}
// LOWERING-MACOS-DAG:             } else {
// LOWERING-MACOS-DAG:                 let {{_v[0-9]+}}: bool = false;
// LOWERING-MACOS-DAG:                 {{_v[0-9]+}}
// LOWERING-MACOS-DAG:             };
// LOWERING-MACOS-DAG:             if !{{_v[0-9]+}} {
// LOWERING-MACOS-DAG:                 break;
// LOWERING-MACOS-DAG:             }
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: u64 = length;
// LOWERING-MACOS-DAG:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-MACOS-DAG:             length = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:         }
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut passwd = entry;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = unsafe { (*{{_v[0-9]+}}).pw_shell };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-MACOS-DAG:         memcpy(
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-MACOS-DAG:             {{_v[0-9]+}} as u64,
// LOWERING-MACOS-DAG:         )
// LOWERING-MACOS-DAG:     };
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i8 = 0;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{arg[0-9]+}}.add({{_v[0-9]+}} as usize) };
// LOWERING-MACOS-DAG:     unsafe {
// LOWERING-MACOS-DAG:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     }
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-DAG:     __retval = {{_v[0-9]+}};
// LOWERING-MACOS-DAG:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-MACOS-DAG:     return {{_v[0-9]+}};
// LOWERING-MACOS-DAG: }
// SLATE-FILECHECK-END lowering-macos

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS-DAG: unsafe fn slate_spawn_and_wait(
// REWRITES-MACOS-DAG:     {{arg[0-9]+}}: *mut i32,
// REWRITES-MACOS-DAG:     {{arg[0-9]+}}: *mut i8,
// REWRITES-MACOS-DAG:     {{arg[0-9]+}}: *mut *mut i8,
// REWRITES-MACOS-DAG:     {{arg[0-9]+}}: *mut *mut i8,
// REWRITES-MACOS-DAG: ) -> i32 {
// REWRITES-MACOS-DAG:     let mut status: i32 = 0;
// REWRITES-MACOS-DAG:     let mut result: i32 = 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-MACOS-DAG:     result = unsafe {
// REWRITES-MACOS-DAG:         posix_spawn(
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *mut i32,
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *const core::ffi::c_char,
// REWRITES-MACOS-DAG:             {{_v[0-9]+}} as *const *mut core::ffi::c_void,
// REWRITES-MACOS-DAG:             {{_v[0-9]+}} as *const *mut core::ffi::c_void,
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *mut *mut core::ffi::c_char,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = result != 0;
// REWRITES-MACOS-DAG:     if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         return result;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-MACOS-DAG:         waitpid(
// REWRITES-MACOS-DAG:             (unsafe { *{{arg[0-9]+}} }) as i32,
// REWRITES-MACOS-DAG:             std::ptr::addr_of_mut!(status) as *mut i32,
// REWRITES-MACOS-DAG:             0 as i32,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = -1;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     } else {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: i32 = status;
// REWRITES-MACOS-DAG:         {{_v[0-9]+}}
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     {{_v[0-9]+}}
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: fn slate_terminal_page_size(mut {{_v[0-9]+}}: i32, mut {{_v[0-9]+}}: *mut termios) -> i64 {
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i32 = unsafe { tcgetattr({{_v[0-9]+}} as i32, {{_v[0-9]+}} as *mut termios) };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-MACOS-DAG:     if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         return -1;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     unsafe { sysconf(29 as i32) }
// REWRITES-MACOS-DAG: }
// REWRITES-MACOS-DAG: unsafe fn slate_lookup_user_shell({{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut i8, mut size: u64) -> i32 {
// REWRITES-MACOS-DAG:     let mut entry: *mut passwd = unsafe { getpwuid({{arg[0-9]+}} as u32) };
// REWRITES-MACOS-DAG:     let mut length: u64 = 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: bool = entry == std::ptr::null_mut();
// REWRITES-MACOS-DAG:     if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:         return -1;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     loop {
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: *mut i8 = unsafe { (*entry).pw_shell };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(length as usize) };
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = ((unsafe { *{{_v[0-9]+}} }) as i32) != 0;
// REWRITES-MACOS-DAG:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-MACOS-DAG:             let {{_v[0-9]+}}: bool = length + 1 < size;
// REWRITES-MACOS-DAG:             {{_v[0-9]+}}
// REWRITES-MACOS-DAG:         } else {
// REWRITES-MACOS-DAG:             let {{_v[0-9]+}}: bool = false;
// REWRITES-MACOS-DAG:             {{_v[0-9]+}}
// REWRITES-MACOS-DAG:         };
// REWRITES-MACOS-DAG:         if !{{_v[0-9]+}} {
// REWRITES-MACOS-DAG:             break;
// REWRITES-MACOS-DAG:         }
// REWRITES-MACOS-DAG:         length += 1;
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         std::ptr::copy_nonoverlapping(
// REWRITES-MACOS-DAG:             ((unsafe { (*entry).pw_shell }) as *mut core::ffi::c_void) as *const u8,
// REWRITES-MACOS-DAG:             {{arg[0-9]+}} as *mut u8,
// REWRITES-MACOS-DAG:             (length as u64) as usize,
// REWRITES-MACOS-DAG:         )
// REWRITES-MACOS-DAG:     };
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: i8 = 0;
// REWRITES-MACOS-DAG:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{arg[0-9]+}}.add(length as usize) };
// REWRITES-MACOS-DAG:     unsafe {
// REWRITES-MACOS-DAG:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-MACOS-DAG:     }
// REWRITES-MACOS-DAG:     0
// REWRITES-MACOS-DAG: }
// SLATE-FILECHECK-END rewrites-macos
