#include <unistd.h>

extern int slate_oracle__Fork(void);
extern int slate_oracle___getpgid(int);
extern void slate_oracle__exit(int) __attribute__((noreturn));
extern int slate_oracle_access(const char *, int);
extern int slate_oracle_acct(const char *);
extern unsigned int slate_oracle_alarm(unsigned int);
extern int slate_oracle_brk(void *);
extern int slate_oracle_chdir(const char *);
extern int slate_oracle_chown(const char *, unsigned int, unsigned int);
extern int slate_oracle_chroot(const char *);
extern int slate_oracle_close(int);
extern int slate_oracle_close_range(unsigned int, unsigned int, int);
extern void slate_oracle_closefrom(int);
extern unsigned long slate_oracle_confstr(int, char *, unsigned long);
extern long slate_oracle_copy_file_range(int, long *, int, long *, unsigned long, unsigned int);
extern char * slate_oracle_crypt(const char *, const char *);
extern int slate_oracle_daemon(int, int);
extern int slate_oracle_dup(int);
extern int slate_oracle_dup2(int, int);
extern int slate_oracle_dup3(int, int, int);
extern int slate_oracle_eaccess(const char *, int);
extern void slate_oracle_endusershell(void);
extern int slate_oracle_euidaccess(const char *, int);
extern int slate_oracle_execl(const char *, const char *, ...);
extern int slate_oracle_execle(const char *, const char *, ...);
extern int slate_oracle_execlp(const char *, const char *, ...);
extern int slate_oracle_execv(const char *, char *const *);
extern int slate_oracle_execve(const char *, char *const *, char *const *);
extern int slate_oracle_execveat(int, const char *, char *const *, char *const *, int);
extern int slate_oracle_execvp(const char *, char *const *);
extern int slate_oracle_execvpe(const char *, char *const *, char *const *);
extern int slate_oracle_faccessat(int, const char *, int, int);
extern int slate_oracle_fchdir(int);
extern int slate_oracle_fchown(int, unsigned int, unsigned int);
extern int slate_oracle_fchownat(int, const char *, unsigned int, unsigned int, int);
extern int slate_oracle_fdatasync(int);
extern int slate_oracle_fexecve(int, char *const *, char *const *);
extern int slate_oracle_fork(void);
extern long slate_oracle_fpathconf(int, int);
extern int slate_oracle_fsync(int);
extern int slate_oracle_ftruncate(int, long);
extern int slate_oracle_ftruncate64(int, long);
extern char * slate_oracle_get_current_dir_name(void);
extern char * slate_oracle_getcwd(char *, unsigned long);
extern int slate_oracle_getdomainname(char *, unsigned long);
extern int slate_oracle_getdtablesize(void);
extern unsigned int slate_oracle_getegid(void);
extern int slate_oracle_getentropy(void *, unsigned long);
extern unsigned int slate_oracle_geteuid(void);
extern unsigned int slate_oracle_getgid(void);
extern int slate_oracle_getgroups(int, unsigned int *);
extern long slate_oracle_gethostid(void);
extern int slate_oracle_gethostname(char *, unsigned long);
extern char * slate_oracle_getlogin(void);
extern int slate_oracle_getlogin_r(char *, unsigned long);
extern int slate_oracle_getopt(int, char *const *, const char *);
extern int slate_oracle_getpagesize(void);
extern char * slate_oracle_getpass(const char *);
extern int slate_oracle_getpgid(int);
extern int slate_oracle_getpgrp(void);
extern int slate_oracle_getpid(void);
extern int slate_oracle_getppid(void);
extern int slate_oracle_getresgid(unsigned int *, unsigned int *, unsigned int *);
extern int slate_oracle_getresuid(unsigned int *, unsigned int *, unsigned int *);
extern int slate_oracle_getsid(int);
extern int slate_oracle_gettid(void);
extern unsigned int slate_oracle_getuid(void);
extern char * slate_oracle_getusershell(void);
extern char * slate_oracle_getwd(char *);
extern int slate_oracle_group_member(unsigned int);
extern int slate_oracle_isatty(int);
extern int slate_oracle_lchown(const char *, unsigned int, unsigned int);
extern int slate_oracle_link(const char *, const char *);
extern int slate_oracle_linkat(int, const char *, int, const char *, int);
extern int slate_oracle_lockf(int, int, long);
extern int slate_oracle_lockf64(int, int, long);
extern long slate_oracle_lseek(int, long, int);
extern long slate_oracle_lseek64(int, long, int);
extern int slate_oracle_nice(int);
extern long slate_oracle_pathconf(const char *, int);
extern int slate_oracle_pause(void);
extern int slate_oracle_pipe(int *);
extern int slate_oracle_pipe2(int *, int);
extern long slate_oracle_pread(int, void *, unsigned long, long);
extern long slate_oracle_pread64(int, void *, unsigned long, long);
extern int slate_oracle_profil(unsigned short *, unsigned long, unsigned long, unsigned int);
extern long slate_oracle_pwrite(int, const void *, unsigned long, long);
extern long slate_oracle_pwrite64(int, const void *, unsigned long, long);
extern long slate_oracle_read(int, void *, unsigned long);
extern long slate_oracle_readlink(const char *restrict, char *restrict, unsigned long);
extern long slate_oracle_readlinkat(int, const char *restrict, char *restrict, unsigned long);
extern int slate_oracle_revoke(const char *);
extern int slate_oracle_rmdir(const char *);
extern void * slate_oracle_sbrk(long);
extern int slate_oracle_setdomainname(const char *, unsigned long);
extern int slate_oracle_setegid(unsigned int);
extern int slate_oracle_seteuid(unsigned int);
extern int slate_oracle_setgid(unsigned int);
extern int slate_oracle_sethostid(long);
extern int slate_oracle_sethostname(const char *, unsigned long);
extern int slate_oracle_setlogin(const char *);
extern int slate_oracle_setpgid(int, int);
extern int slate_oracle_setpgrp(void);
extern int slate_oracle_setregid(unsigned int, unsigned int);
extern int slate_oracle_setresgid(unsigned int, unsigned int, unsigned int);
extern int slate_oracle_setresuid(unsigned int, unsigned int, unsigned int);
extern int slate_oracle_setreuid(unsigned int, unsigned int);
extern int slate_oracle_setsid(void);
extern int slate_oracle_setuid(unsigned int);
extern void slate_oracle_setusershell(void);
extern unsigned int slate_oracle_sleep(unsigned int);
extern void slate_oracle_swab(const void *restrict, void *restrict, long);
extern int slate_oracle_symlink(const char *, const char *);
extern int slate_oracle_symlinkat(const char *, int, const char *);
extern void slate_oracle_sync(void);
extern int slate_oracle_syncfs(int);
extern long slate_oracle_syscall(long, ...);
extern long slate_oracle_sysconf(int);
extern int slate_oracle_tcgetpgrp(int);
extern int slate_oracle_tcsetpgrp(int, int);
extern int slate_oracle_truncate(const char *, long);
extern int slate_oracle_truncate64(const char *, long);
extern char * slate_oracle_ttyname(int);
extern int slate_oracle_ttyname_r(int, char *, unsigned long);
extern int slate_oracle_ttyslot(void);
extern unsigned int slate_oracle_ualarm(unsigned int, unsigned int);
extern int slate_oracle_unlink(const char *);
extern int slate_oracle_unlinkat(int, const char *, int);
extern int slate_oracle_usleep(unsigned int);
extern int slate_oracle_vfork(void);
extern int slate_oracle_vhangup(void);
extern long slate_oracle_write(int, const void *, unsigned long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__Fork), __typeof__(_Fork)),
    "unistd.h:_Fork declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___getpgid), __typeof__(__getpgid)),
    "unistd.h:__getpgid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__exit), __typeof__(_exit)),
    "unistd.h:_exit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_access), __typeof__(access)),
    "unistd.h:access declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acct), __typeof__(acct)),
    "unistd.h:acct declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_alarm), __typeof__(alarm)),
    "unistd.h:alarm declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_brk), __typeof__(brk)),
    "unistd.h:brk declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_chdir), __typeof__(chdir)),
    "unistd.h:chdir declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_chown), __typeof__(chown)),
    "unistd.h:chown declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_chroot), __typeof__(chroot)),
    "unistd.h:chroot declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_close), __typeof__(close)),
    "unistd.h:close declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_close_range), __typeof__(close_range)),
    "unistd.h:close_range declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_closefrom), __typeof__(closefrom)),
    "unistd.h:closefrom declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_confstr), __typeof__(confstr)),
    "unistd.h:confstr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_copy_file_range), __typeof__(copy_file_range)),
    "unistd.h:copy_file_range declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_crypt), __typeof__(crypt)),
    "unistd.h:crypt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_daemon), __typeof__(daemon)),
    "unistd.h:daemon declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_dup), __typeof__(dup)),
    "unistd.h:dup declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_dup2), __typeof__(dup2)),
    "unistd.h:dup2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_dup3), __typeof__(dup3)),
    "unistd.h:dup3 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_eaccess), __typeof__(eaccess)),
    "unistd.h:eaccess declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_endusershell), __typeof__(endusershell)),
    "unistd.h:endusershell declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_euidaccess), __typeof__(euidaccess)),
    "unistd.h:euidaccess declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execl), __typeof__(execl)),
    "unistd.h:execl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execle), __typeof__(execle)),
    "unistd.h:execle declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execlp), __typeof__(execlp)),
    "unistd.h:execlp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execv), __typeof__(execv)),
    "unistd.h:execv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execve), __typeof__(execve)),
    "unistd.h:execve declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execveat), __typeof__(execveat)),
    "unistd.h:execveat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execvp), __typeof__(execvp)),
    "unistd.h:execvp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_execvpe), __typeof__(execvpe)),
    "unistd.h:execvpe declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_faccessat), __typeof__(faccessat)),
    "unistd.h:faccessat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fchdir), __typeof__(fchdir)),
    "unistd.h:fchdir declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fchown), __typeof__(fchown)),
    "unistd.h:fchown declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fchownat), __typeof__(fchownat)),
    "unistd.h:fchownat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdatasync), __typeof__(fdatasync)),
    "unistd.h:fdatasync declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fexecve), __typeof__(fexecve)),
    "unistd.h:fexecve declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fork), __typeof__(fork)),
    "unistd.h:fork declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fpathconf), __typeof__(fpathconf)),
    "unistd.h:fpathconf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fsync), __typeof__(fsync)),
    "unistd.h:fsync declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ftruncate), __typeof__(ftruncate)),
    "unistd.h:ftruncate declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ftruncate64), __typeof__(ftruncate64)),
    "unistd.h:ftruncate64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_get_current_dir_name), __typeof__(get_current_dir_name)),
    "unistd.h:get_current_dir_name declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getcwd), __typeof__(getcwd)),
    "unistd.h:getcwd declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getdomainname), __typeof__(getdomainname)),
    "unistd.h:getdomainname declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getdtablesize), __typeof__(getdtablesize)),
    "unistd.h:getdtablesize declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getegid), __typeof__(getegid)),
    "unistd.h:getegid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getentropy), __typeof__(getentropy)),
    "unistd.h:getentropy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_geteuid), __typeof__(geteuid)),
    "unistd.h:geteuid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getgid), __typeof__(getgid)),
    "unistd.h:getgid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getgroups), __typeof__(getgroups)),
    "unistd.h:getgroups declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gethostid), __typeof__(gethostid)),
    "unistd.h:gethostid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gethostname), __typeof__(gethostname)),
    "unistd.h:gethostname declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getlogin), __typeof__(getlogin)),
    "unistd.h:getlogin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getlogin_r), __typeof__(getlogin_r)),
    "unistd.h:getlogin_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getopt), __typeof__(getopt)),
    "unistd.h:getopt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getpagesize), __typeof__(getpagesize)),
    "unistd.h:getpagesize declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getpass), __typeof__(getpass)),
    "unistd.h:getpass declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getpgid), __typeof__(getpgid)),
    "unistd.h:getpgid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getpgrp), __typeof__(getpgrp)),
    "unistd.h:getpgrp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getpid), __typeof__(getpid)),
    "unistd.h:getpid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getppid), __typeof__(getppid)),
    "unistd.h:getppid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getresgid), __typeof__(getresgid)),
    "unistd.h:getresgid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getresuid), __typeof__(getresuid)),
    "unistd.h:getresuid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getsid), __typeof__(getsid)),
    "unistd.h:getsid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gettid), __typeof__(gettid)),
    "unistd.h:gettid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getuid), __typeof__(getuid)),
    "unistd.h:getuid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getusershell), __typeof__(getusershell)),
    "unistd.h:getusershell declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getwd), __typeof__(getwd)),
    "unistd.h:getwd declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_group_member), __typeof__(group_member)),
    "unistd.h:group_member declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isatty), __typeof__(isatty)),
    "unistd.h:isatty declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lchown), __typeof__(lchown)),
    "unistd.h:lchown declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_link), __typeof__(link)),
    "unistd.h:link declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_linkat), __typeof__(linkat)),
    "unistd.h:linkat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lockf), __typeof__(lockf)),
    "unistd.h:lockf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lockf64), __typeof__(lockf64)),
    "unistd.h:lockf64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lseek), __typeof__(lseek)),
    "unistd.h:lseek declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lseek64), __typeof__(lseek64)),
    "unistd.h:lseek64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nice), __typeof__(nice)),
    "unistd.h:nice declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pathconf), __typeof__(pathconf)),
    "unistd.h:pathconf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pause), __typeof__(pause)),
    "unistd.h:pause declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pipe), __typeof__(pipe)),
    "unistd.h:pipe declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pipe2), __typeof__(pipe2)),
    "unistd.h:pipe2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pread), __typeof__(pread)),
    "unistd.h:pread declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pread64), __typeof__(pread64)),
    "unistd.h:pread64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_profil), __typeof__(profil)),
    "unistd.h:profil declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pwrite), __typeof__(pwrite)),
    "unistd.h:pwrite declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pwrite64), __typeof__(pwrite64)),
    "unistd.h:pwrite64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_read), __typeof__(read)),
    "unistd.h:read declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_readlink), __typeof__(readlink)),
    "unistd.h:readlink declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_readlinkat), __typeof__(readlinkat)),
    "unistd.h:readlinkat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_revoke), __typeof__(revoke)),
    "unistd.h:revoke declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rmdir), __typeof__(rmdir)),
    "unistd.h:rmdir declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sbrk), __typeof__(sbrk)),
    "unistd.h:sbrk declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setdomainname), __typeof__(setdomainname)),
    "unistd.h:setdomainname declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setegid), __typeof__(setegid)),
    "unistd.h:setegid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_seteuid), __typeof__(seteuid)),
    "unistd.h:seteuid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setgid), __typeof__(setgid)),
    "unistd.h:setgid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sethostid), __typeof__(sethostid)),
    "unistd.h:sethostid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sethostname), __typeof__(sethostname)),
    "unistd.h:sethostname declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setlogin), __typeof__(setlogin)),
    "unistd.h:setlogin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setpgid), __typeof__(setpgid)),
    "unistd.h:setpgid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setpgrp), __typeof__(setpgrp)),
    "unistd.h:setpgrp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setregid), __typeof__(setregid)),
    "unistd.h:setregid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setresgid), __typeof__(setresgid)),
    "unistd.h:setresgid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setresuid), __typeof__(setresuid)),
    "unistd.h:setresuid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setreuid), __typeof__(setreuid)),
    "unistd.h:setreuid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setsid), __typeof__(setsid)),
    "unistd.h:setsid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setuid), __typeof__(setuid)),
    "unistd.h:setuid declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setusershell), __typeof__(setusershell)),
    "unistd.h:setusershell declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sleep), __typeof__(sleep)),
    "unistd.h:sleep declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_swab), __typeof__(swab)),
    "unistd.h:swab declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_symlink), __typeof__(symlink)),
    "unistd.h:symlink declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_symlinkat), __typeof__(symlinkat)),
    "unistd.h:symlinkat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sync), __typeof__(sync)),
    "unistd.h:sync declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_syncfs), __typeof__(syncfs)),
    "unistd.h:syncfs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_syscall), __typeof__(syscall)),
    "unistd.h:syscall declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sysconf), __typeof__(sysconf)),
    "unistd.h:sysconf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tcgetpgrp), __typeof__(tcgetpgrp)),
    "unistd.h:tcgetpgrp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tcsetpgrp), __typeof__(tcsetpgrp)),
    "unistd.h:tcsetpgrp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_truncate), __typeof__(truncate)),
    "unistd.h:truncate declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_truncate64), __typeof__(truncate64)),
    "unistd.h:truncate64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ttyname), __typeof__(ttyname)),
    "unistd.h:ttyname declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ttyname_r), __typeof__(ttyname_r)),
    "unistd.h:ttyname_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ttyslot), __typeof__(ttyslot)),
    "unistd.h:ttyslot declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ualarm), __typeof__(ualarm)),
    "unistd.h:ualarm declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_unlink), __typeof__(unlink)),
    "unistd.h:unlink declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_unlinkat), __typeof__(unlinkat)),
    "unistd.h:unlinkat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_usleep), __typeof__(usleep)),
    "unistd.h:usleep declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vfork), __typeof__(vfork)),
    "unistd.h:vfork declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vhangup), __typeof__(vhangup)),
    "unistd.h:vhangup declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_write), __typeof__(write)),
    "unistd.h:write declaration differs from oracle");

static __typeof__(_Fork) *const slate_reference__Fork = &_Fork;
static __typeof__(__getpgid) *const slate_reference___getpgid = &__getpgid;
static __typeof__(_exit) *const slate_reference__exit = &_exit;
static __typeof__(access) *const slate_reference_access = &access;
static __typeof__(acct) *const slate_reference_acct = &acct;
static __typeof__(alarm) *const slate_reference_alarm = &alarm;
static __typeof__(brk) *const slate_reference_brk = &brk;
static __typeof__(chdir) *const slate_reference_chdir = &chdir;
static __typeof__(chown) *const slate_reference_chown = &chown;
static __typeof__(chroot) *const slate_reference_chroot = &chroot;
static __typeof__(close) *const slate_reference_close = &close;
static __typeof__(close_range) *const slate_reference_close_range = &close_range;
static __typeof__(closefrom) *const slate_reference_closefrom = &closefrom;
static __typeof__(confstr) *const slate_reference_confstr = &confstr;
static __typeof__(copy_file_range) *const slate_reference_copy_file_range = &copy_file_range;
static __typeof__(crypt) *const slate_reference_crypt = &crypt;
static __typeof__(daemon) *const slate_reference_daemon = &daemon;
static __typeof__(dup) *const slate_reference_dup = &dup;
static __typeof__(dup2) *const slate_reference_dup2 = &dup2;
static __typeof__(dup3) *const slate_reference_dup3 = &dup3;
static __typeof__(eaccess) *const slate_reference_eaccess = &eaccess;
static __typeof__(endusershell) *const slate_reference_endusershell = &endusershell;
static __typeof__(euidaccess) *const slate_reference_euidaccess = &euidaccess;
static __typeof__(execl) *const slate_reference_execl = &execl;
static __typeof__(execle) *const slate_reference_execle = &execle;
static __typeof__(execlp) *const slate_reference_execlp = &execlp;
static __typeof__(execv) *const slate_reference_execv = &execv;
static __typeof__(execve) *const slate_reference_execve = &execve;
static __typeof__(execveat) *const slate_reference_execveat = &execveat;
static __typeof__(execvp) *const slate_reference_execvp = &execvp;
static __typeof__(execvpe) *const slate_reference_execvpe = &execvpe;
static __typeof__(faccessat) *const slate_reference_faccessat = &faccessat;
static __typeof__(fchdir) *const slate_reference_fchdir = &fchdir;
static __typeof__(fchown) *const slate_reference_fchown = &fchown;
static __typeof__(fchownat) *const slate_reference_fchownat = &fchownat;
static __typeof__(fdatasync) *const slate_reference_fdatasync = &fdatasync;
static __typeof__(fexecve) *const slate_reference_fexecve = &fexecve;
static __typeof__(fork) *const slate_reference_fork = &fork;
static __typeof__(fpathconf) *const slate_reference_fpathconf = &fpathconf;
static __typeof__(fsync) *const slate_reference_fsync = &fsync;
static __typeof__(ftruncate) *const slate_reference_ftruncate = &ftruncate;
static __typeof__(ftruncate64) *const slate_reference_ftruncate64 = &ftruncate64;
static __typeof__(get_current_dir_name) *const slate_reference_get_current_dir_name = &get_current_dir_name;
static __typeof__(getcwd) *const slate_reference_getcwd = &getcwd;
static __typeof__(getdomainname) *const slate_reference_getdomainname = &getdomainname;
static __typeof__(getdtablesize) *const slate_reference_getdtablesize = &getdtablesize;
static __typeof__(getegid) *const slate_reference_getegid = &getegid;
static __typeof__(getentropy) *const slate_reference_getentropy = &getentropy;
static __typeof__(geteuid) *const slate_reference_geteuid = &geteuid;
static __typeof__(getgid) *const slate_reference_getgid = &getgid;
static __typeof__(getgroups) *const slate_reference_getgroups = &getgroups;
static __typeof__(gethostid) *const slate_reference_gethostid = &gethostid;
static __typeof__(gethostname) *const slate_reference_gethostname = &gethostname;
static __typeof__(getlogin) *const slate_reference_getlogin = &getlogin;
static __typeof__(getlogin_r) *const slate_reference_getlogin_r = &getlogin_r;
static __typeof__(getopt) *const slate_reference_getopt = &getopt;
static __typeof__(getpagesize) *const slate_reference_getpagesize = &getpagesize;
static __typeof__(getpass) *const slate_reference_getpass = &getpass;
static __typeof__(getpgid) *const slate_reference_getpgid = &getpgid;
static __typeof__(getpgrp) *const slate_reference_getpgrp = &getpgrp;
static __typeof__(getpid) *const slate_reference_getpid = &getpid;
static __typeof__(getppid) *const slate_reference_getppid = &getppid;
static __typeof__(getresgid) *const slate_reference_getresgid = &getresgid;
static __typeof__(getresuid) *const slate_reference_getresuid = &getresuid;
static __typeof__(getsid) *const slate_reference_getsid = &getsid;
static __typeof__(gettid) *const slate_reference_gettid = &gettid;
static __typeof__(getuid) *const slate_reference_getuid = &getuid;
static __typeof__(getusershell) *const slate_reference_getusershell = &getusershell;
static __typeof__(getwd) *const slate_reference_getwd = &getwd;
static __typeof__(group_member) *const slate_reference_group_member = &group_member;
static __typeof__(isatty) *const slate_reference_isatty = &isatty;
static __typeof__(lchown) *const slate_reference_lchown = &lchown;
static __typeof__(link) *const slate_reference_link = &link;
static __typeof__(linkat) *const slate_reference_linkat = &linkat;
static __typeof__(lockf) *const slate_reference_lockf = &lockf;
static __typeof__(lockf64) *const slate_reference_lockf64 = &lockf64;
static __typeof__(lseek) *const slate_reference_lseek = &lseek;
static __typeof__(lseek64) *const slate_reference_lseek64 = &lseek64;
static __typeof__(nice) *const slate_reference_nice = &nice;
static __typeof__(pathconf) *const slate_reference_pathconf = &pathconf;
static __typeof__(pause) *const slate_reference_pause = &pause;
static __typeof__(pipe) *const slate_reference_pipe = &pipe;
static __typeof__(pipe2) *const slate_reference_pipe2 = &pipe2;
static __typeof__(pread) *const slate_reference_pread = &pread;
static __typeof__(pread64) *const slate_reference_pread64 = &pread64;
static __typeof__(profil) *const slate_reference_profil = &profil;
static __typeof__(pwrite) *const slate_reference_pwrite = &pwrite;
static __typeof__(pwrite64) *const slate_reference_pwrite64 = &pwrite64;
static __typeof__(read) *const slate_reference_read = &read;
static __typeof__(readlink) *const slate_reference_readlink = &readlink;
static __typeof__(readlinkat) *const slate_reference_readlinkat = &readlinkat;
static __typeof__(revoke) *const slate_reference_revoke = &revoke;
static __typeof__(rmdir) *const slate_reference_rmdir = &rmdir;
static __typeof__(sbrk) *const slate_reference_sbrk = &sbrk;
static __typeof__(setdomainname) *const slate_reference_setdomainname = &setdomainname;
static __typeof__(setegid) *const slate_reference_setegid = &setegid;
static __typeof__(seteuid) *const slate_reference_seteuid = &seteuid;
static __typeof__(setgid) *const slate_reference_setgid = &setgid;
static __typeof__(sethostid) *const slate_reference_sethostid = &sethostid;
static __typeof__(sethostname) *const slate_reference_sethostname = &sethostname;
static __typeof__(setlogin) *const slate_reference_setlogin = &setlogin;
static __typeof__(setpgid) *const slate_reference_setpgid = &setpgid;
static __typeof__(setpgrp) *const slate_reference_setpgrp = &setpgrp;
static __typeof__(setregid) *const slate_reference_setregid = &setregid;
static __typeof__(setresgid) *const slate_reference_setresgid = &setresgid;
static __typeof__(setresuid) *const slate_reference_setresuid = &setresuid;
static __typeof__(setreuid) *const slate_reference_setreuid = &setreuid;
static __typeof__(setsid) *const slate_reference_setsid = &setsid;
static __typeof__(setuid) *const slate_reference_setuid = &setuid;
static __typeof__(setusershell) *const slate_reference_setusershell = &setusershell;
static __typeof__(sleep) *const slate_reference_sleep = &sleep;
static __typeof__(swab) *const slate_reference_swab = &swab;
static __typeof__(symlink) *const slate_reference_symlink = &symlink;
static __typeof__(symlinkat) *const slate_reference_symlinkat = &symlinkat;
static __typeof__(sync) *const slate_reference_sync = &sync;
static __typeof__(syncfs) *const slate_reference_syncfs = &syncfs;
static __typeof__(syscall) *const slate_reference_syscall = &syscall;
static __typeof__(sysconf) *const slate_reference_sysconf = &sysconf;
static __typeof__(tcgetpgrp) *const slate_reference_tcgetpgrp = &tcgetpgrp;
static __typeof__(tcsetpgrp) *const slate_reference_tcsetpgrp = &tcsetpgrp;
static __typeof__(truncate) *const slate_reference_truncate = &truncate;
static __typeof__(truncate64) *const slate_reference_truncate64 = &truncate64;
static __typeof__(ttyname) *const slate_reference_ttyname = &ttyname;
static __typeof__(ttyname_r) *const slate_reference_ttyname_r = &ttyname_r;
static __typeof__(ttyslot) *const slate_reference_ttyslot = &ttyslot;
static __typeof__(ualarm) *const slate_reference_ualarm = &ualarm;
static __typeof__(unlink) *const slate_reference_unlink = &unlink;
static __typeof__(unlinkat) *const slate_reference_unlinkat = &unlinkat;
static __typeof__(usleep) *const slate_reference_usleep = &usleep;
static __typeof__(vfork) *const slate_reference_vfork = &vfork;
static __typeof__(vhangup) *const slate_reference_vhangup = &vhangup;
static __typeof__(write) *const slate_reference_write = &write;

extern char ** slate_oracle_environ;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_environ), __typeof__(environ)), "environ object type differs from oracle");

static __typeof__(environ) *const slate_reference_environ = &environ;

extern char * slate_oracle_optarg;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_optarg), __typeof__(optarg)), "optarg object type differs from oracle");

static __typeof__(optarg) *const slate_reference_optarg = &optarg;

extern int slate_oracle_opterr;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_opterr), __typeof__(opterr)), "opterr object type differs from oracle");

static __typeof__(opterr) *const slate_reference_opterr = &opterr;

extern int slate_oracle_optind;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_optind), __typeof__(optind)), "optind object type differs from oracle");

static __typeof__(optind) *const slate_reference_optind = &optind;

extern int slate_oracle_optopt;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_optopt), __typeof__(optopt)), "optopt object type differs from oracle");

static __typeof__(optopt) *const slate_reference_optopt = &optopt;

typedef unsigned int slate_oracle_typedef_gid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_gid_t, gid_t), "typedef gid_t differs from oracle");

typedef long slate_oracle_typedef_intptr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_intptr_t, intptr_t), "typedef intptr_t differs from oracle");

typedef long slate_oracle_typedef_off64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off64_t, off64_t), "typedef off64_t differs from oracle");

typedef long slate_oracle_typedef_off_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off_t, off_t), "typedef off_t differs from oracle");

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

typedef unsigned long slate_oracle_typedef_size_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_size_t, size_t), "typedef size_t differs from oracle");

typedef unsigned int slate_oracle_typedef_socklen_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_socklen_t, socklen_t), "typedef socklen_t differs from oracle");

typedef long slate_oracle_typedef_ssize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ssize_t, ssize_t), "typedef ssize_t differs from oracle");

typedef unsigned int slate_oracle_typedef_uid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uid_t, uid_t), "typedef uid_t differs from oracle");

typedef unsigned int slate_oracle_typedef_useconds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_useconds_t, useconds_t), "typedef useconds_t differs from oracle");

#ifndef CLOSE_RANGE_CLOEXEC
#error "unistd.h:CLOSE_RANGE_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef CLOSE_RANGE_UNSHARE
#error "unistd.h:CLOSE_RANGE_UNSHARE macro is missing from libc-shim"
#endif

#ifndef F_LOCK
#error "unistd.h:F_LOCK macro is missing from libc-shim"
#endif

#ifndef F_OK
#error "unistd.h:F_OK macro is missing from libc-shim"
#endif

#ifndef F_TEST
#error "unistd.h:F_TEST macro is missing from libc-shim"
#endif

#ifndef F_TLOCK
#error "unistd.h:F_TLOCK macro is missing from libc-shim"
#endif

#ifndef F_ULOCK
#error "unistd.h:F_ULOCK macro is missing from libc-shim"
#endif

#ifndef L_INCR
#error "unistd.h:L_INCR macro is missing from libc-shim"
#endif

#ifndef L_SET
#error "unistd.h:L_SET macro is missing from libc-shim"
#endif

#ifndef L_XTND
#error "unistd.h:L_XTND macro is missing from libc-shim"
#endif

#ifndef NULL
#error "unistd.h:NULL macro is missing from libc-shim"
#endif

#ifndef R_OK
#error "unistd.h:R_OK macro is missing from libc-shim"
#endif

#ifndef SEEK_CUR
#error "unistd.h:SEEK_CUR macro is missing from libc-shim"
#endif

#ifndef SEEK_DATA
#error "unistd.h:SEEK_DATA macro is missing from libc-shim"
#endif

#ifndef SEEK_END
#error "unistd.h:SEEK_END macro is missing from libc-shim"
#endif

#ifndef SEEK_HOLE
#error "unistd.h:SEEK_HOLE macro is missing from libc-shim"
#endif

#ifndef SEEK_SET
#error "unistd.h:SEEK_SET macro is missing from libc-shim"
#endif

#ifndef STDERR_FILENO
#error "unistd.h:STDERR_FILENO macro is missing from libc-shim"
#endif

#ifndef STDIN_FILENO
#error "unistd.h:STDIN_FILENO macro is missing from libc-shim"
#endif

#ifndef STDOUT_FILENO
#error "unistd.h:STDOUT_FILENO macro is missing from libc-shim"
#endif

#ifndef TEMP_FAILURE_RETRY
#error "unistd.h:TEMP_FAILURE_RETRY macro is missing from libc-shim"
#endif

#ifndef W_OK
#error "unistd.h:W_OK macro is missing from libc-shim"
#endif

#ifndef X_OK
#error "unistd.h:X_OK macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "unistd.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _BITS_POSIX_OPT_H
#error "unistd.h:_BITS_POSIX_OPT_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TIME64_H
#error "unistd.h:_BITS_TIME64_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPESIZES_H
#error "unistd.h:_BITS_TYPESIZES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES_H
#error "unistd.h:_BITS_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _CS_GNU_LIBC_VERSION
#error "unistd.h:_CS_GNU_LIBC_VERSION macro is missing from libc-shim"
#endif

#ifndef _CS_GNU_LIBPTHREAD_VERSION
#error "unistd.h:_CS_GNU_LIBPTHREAD_VERSION macro is missing from libc-shim"
#endif

#ifndef _CS_LFS64_CFLAGS
#error "unistd.h:_CS_LFS64_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_LFS64_LDFLAGS
#error "unistd.h:_CS_LFS64_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_LFS64_LIBS
#error "unistd.h:_CS_LFS64_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_LFS64_LINTFLAGS
#error "unistd.h:_CS_LFS64_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_LFS_CFLAGS
#error "unistd.h:_CS_LFS_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_LFS_LDFLAGS
#error "unistd.h:_CS_LFS_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_LFS_LIBS
#error "unistd.h:_CS_LFS_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_LFS_LINTFLAGS
#error "unistd.h:_CS_LFS_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_PATH
#error "unistd.h:_CS_PATH macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V5_WIDTH_RESTRICTED_ENVS
#error "unistd.h:_CS_POSIX_V5_WIDTH_RESTRICTED_ENVS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_ILP32_OFF32_CFLAGS
#error "unistd.h:_CS_POSIX_V6_ILP32_OFF32_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_ILP32_OFF32_LDFLAGS
#error "unistd.h:_CS_POSIX_V6_ILP32_OFF32_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_ILP32_OFF32_LIBS
#error "unistd.h:_CS_POSIX_V6_ILP32_OFF32_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS
#error "unistd.h:_CS_POSIX_V6_ILP32_OFF32_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS
#error "unistd.h:_CS_POSIX_V6_ILP32_OFFBIG_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS
#error "unistd.h:_CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_ILP32_OFFBIG_LIBS
#error "unistd.h:_CS_POSIX_V6_ILP32_OFFBIG_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS
#error "unistd.h:_CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_LP64_OFF64_CFLAGS
#error "unistd.h:_CS_POSIX_V6_LP64_OFF64_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_LP64_OFF64_LDFLAGS
#error "unistd.h:_CS_POSIX_V6_LP64_OFF64_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_LP64_OFF64_LIBS
#error "unistd.h:_CS_POSIX_V6_LP64_OFF64_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_LP64_OFF64_LINTFLAGS
#error "unistd.h:_CS_POSIX_V6_LP64_OFF64_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS
#error "unistd.h:_CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS
#error "unistd.h:_CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_LPBIG_OFFBIG_LIBS
#error "unistd.h:_CS_POSIX_V6_LPBIG_OFFBIG_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS
#error "unistd.h:_CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V6_WIDTH_RESTRICTED_ENVS
#error "unistd.h:_CS_POSIX_V6_WIDTH_RESTRICTED_ENVS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_ILP32_OFF32_CFLAGS
#error "unistd.h:_CS_POSIX_V7_ILP32_OFF32_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_ILP32_OFF32_LDFLAGS
#error "unistd.h:_CS_POSIX_V7_ILP32_OFF32_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_ILP32_OFF32_LIBS
#error "unistd.h:_CS_POSIX_V7_ILP32_OFF32_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_ILP32_OFF32_LINTFLAGS
#error "unistd.h:_CS_POSIX_V7_ILP32_OFF32_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS
#error "unistd.h:_CS_POSIX_V7_ILP32_OFFBIG_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS
#error "unistd.h:_CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_ILP32_OFFBIG_LIBS
#error "unistd.h:_CS_POSIX_V7_ILP32_OFFBIG_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS
#error "unistd.h:_CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_LP64_OFF64_CFLAGS
#error "unistd.h:_CS_POSIX_V7_LP64_OFF64_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_LP64_OFF64_LDFLAGS
#error "unistd.h:_CS_POSIX_V7_LP64_OFF64_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_LP64_OFF64_LIBS
#error "unistd.h:_CS_POSIX_V7_LP64_OFF64_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_LP64_OFF64_LINTFLAGS
#error "unistd.h:_CS_POSIX_V7_LP64_OFF64_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS
#error "unistd.h:_CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS
#error "unistd.h:_CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_LPBIG_OFFBIG_LIBS
#error "unistd.h:_CS_POSIX_V7_LPBIG_OFFBIG_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS
#error "unistd.h:_CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS
#error "unistd.h:_CS_POSIX_V7_WIDTH_RESTRICTED_ENVS macro is missing from libc-shim"
#endif

#ifndef _CS_V5_WIDTH_RESTRICTED_ENVS
#error "unistd.h:_CS_V5_WIDTH_RESTRICTED_ENVS macro is missing from libc-shim"
#endif

#ifndef _CS_V6_ENV
#error "unistd.h:_CS_V6_ENV macro is missing from libc-shim"
#endif

#ifndef _CS_V6_WIDTH_RESTRICTED_ENVS
#error "unistd.h:_CS_V6_WIDTH_RESTRICTED_ENVS macro is missing from libc-shim"
#endif

#ifndef _CS_V7_ENV
#error "unistd.h:_CS_V7_ENV macro is missing from libc-shim"
#endif

#ifndef _CS_V7_WIDTH_RESTRICTED_ENVS
#error "unistd.h:_CS_V7_WIDTH_RESTRICTED_ENVS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_ILP32_OFF32_CFLAGS
#error "unistd.h:_CS_XBS5_ILP32_OFF32_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_ILP32_OFF32_LDFLAGS
#error "unistd.h:_CS_XBS5_ILP32_OFF32_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_ILP32_OFF32_LIBS
#error "unistd.h:_CS_XBS5_ILP32_OFF32_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_ILP32_OFF32_LINTFLAGS
#error "unistd.h:_CS_XBS5_ILP32_OFF32_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_ILP32_OFFBIG_CFLAGS
#error "unistd.h:_CS_XBS5_ILP32_OFFBIG_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_ILP32_OFFBIG_LDFLAGS
#error "unistd.h:_CS_XBS5_ILP32_OFFBIG_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_ILP32_OFFBIG_LIBS
#error "unistd.h:_CS_XBS5_ILP32_OFFBIG_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_ILP32_OFFBIG_LINTFLAGS
#error "unistd.h:_CS_XBS5_ILP32_OFFBIG_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_LP64_OFF64_CFLAGS
#error "unistd.h:_CS_XBS5_LP64_OFF64_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_LP64_OFF64_LDFLAGS
#error "unistd.h:_CS_XBS5_LP64_OFF64_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_LP64_OFF64_LIBS
#error "unistd.h:_CS_XBS5_LP64_OFF64_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_LP64_OFF64_LINTFLAGS
#error "unistd.h:_CS_XBS5_LP64_OFF64_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_LPBIG_OFFBIG_CFLAGS
#error "unistd.h:_CS_XBS5_LPBIG_OFFBIG_CFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_LPBIG_OFFBIG_LDFLAGS
#error "unistd.h:_CS_XBS5_LPBIG_OFFBIG_LDFLAGS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_LPBIG_OFFBIG_LIBS
#error "unistd.h:_CS_XBS5_LPBIG_OFFBIG_LIBS macro is missing from libc-shim"
#endif

#ifndef _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS
#error "unistd.h:_CS_XBS5_LPBIG_OFFBIG_LINTFLAGS macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "unistd.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "unistd.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "unistd.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _GETOPT_CORE_H
#error "unistd.h:_GETOPT_CORE_H macro is missing from libc-shim"
#endif

#ifndef _GETOPT_POSIX_H
#error "unistd.h:_GETOPT_POSIX_H macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "unistd.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "unistd.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "unistd.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "unistd.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "unistd.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "unistd.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "unistd.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LFS64_ASYNCHRONOUS_IO
#error "unistd.h:_LFS64_ASYNCHRONOUS_IO macro is missing from libc-shim"
#endif

#ifndef _LFS64_LARGEFILE
#error "unistd.h:_LFS64_LARGEFILE macro is missing from libc-shim"
#endif

#ifndef _LFS64_STDIO
#error "unistd.h:_LFS64_STDIO macro is missing from libc-shim"
#endif

#ifndef _LFS_ASYNCHRONOUS_IO
#error "unistd.h:_LFS_ASYNCHRONOUS_IO macro is missing from libc-shim"
#endif

#ifndef _LFS_LARGEFILE
#error "unistd.h:_LFS_LARGEFILE macro is missing from libc-shim"
#endif

#ifndef _LINUX_CLOSE_RANGE_H
#error "unistd.h:_LINUX_CLOSE_RANGE_H macro is missing from libc-shim"
#endif

#ifndef _PC_2_SYMLINKS
#error "unistd.h:_PC_2_SYMLINKS macro is missing from libc-shim"
#endif

#ifndef _PC_ALLOC_SIZE_MIN
#error "unistd.h:_PC_ALLOC_SIZE_MIN macro is missing from libc-shim"
#endif

#ifndef _PC_ASYNC_IO
#error "unistd.h:_PC_ASYNC_IO macro is missing from libc-shim"
#endif

#ifndef _PC_CHOWN_RESTRICTED
#error "unistd.h:_PC_CHOWN_RESTRICTED macro is missing from libc-shim"
#endif

#ifndef _PC_FILESIZEBITS
#error "unistd.h:_PC_FILESIZEBITS macro is missing from libc-shim"
#endif

#ifndef _PC_LINK_MAX
#error "unistd.h:_PC_LINK_MAX macro is missing from libc-shim"
#endif

#ifndef _PC_MAX_CANON
#error "unistd.h:_PC_MAX_CANON macro is missing from libc-shim"
#endif

#ifndef _PC_MAX_INPUT
#error "unistd.h:_PC_MAX_INPUT macro is missing from libc-shim"
#endif

#ifndef _PC_NAME_MAX
#error "unistd.h:_PC_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef _PC_NO_TRUNC
#error "unistd.h:_PC_NO_TRUNC macro is missing from libc-shim"
#endif

#ifndef _PC_PATH_MAX
#error "unistd.h:_PC_PATH_MAX macro is missing from libc-shim"
#endif

#ifndef _PC_PIPE_BUF
#error "unistd.h:_PC_PIPE_BUF macro is missing from libc-shim"
#endif

#ifndef _PC_PRIO_IO
#error "unistd.h:_PC_PRIO_IO macro is missing from libc-shim"
#endif

#ifndef _PC_REC_INCR_XFER_SIZE
#error "unistd.h:_PC_REC_INCR_XFER_SIZE macro is missing from libc-shim"
#endif

#ifndef _PC_REC_MAX_XFER_SIZE
#error "unistd.h:_PC_REC_MAX_XFER_SIZE macro is missing from libc-shim"
#endif

#ifndef _PC_REC_MIN_XFER_SIZE
#error "unistd.h:_PC_REC_MIN_XFER_SIZE macro is missing from libc-shim"
#endif

#ifndef _PC_REC_XFER_ALIGN
#error "unistd.h:_PC_REC_XFER_ALIGN macro is missing from libc-shim"
#endif

#ifndef _PC_SOCK_MAXBUF
#error "unistd.h:_PC_SOCK_MAXBUF macro is missing from libc-shim"
#endif

#ifndef _PC_SYMLINK_MAX
#error "unistd.h:_PC_SYMLINK_MAX macro is missing from libc-shim"
#endif

#ifndef _PC_SYNC_IO
#error "unistd.h:_PC_SYNC_IO macro is missing from libc-shim"
#endif

#ifndef _PC_VDISABLE
#error "unistd.h:_PC_VDISABLE macro is missing from libc-shim"
#endif

#ifndef _POSIX2_CHAR_TERM
#error "unistd.h:_POSIX2_CHAR_TERM macro is missing from libc-shim"
#endif

#ifndef _POSIX2_C_BIND
#error "unistd.h:_POSIX2_C_BIND macro is missing from libc-shim"
#endif

#ifndef _POSIX2_C_DEV
#error "unistd.h:_POSIX2_C_DEV macro is missing from libc-shim"
#endif

#ifndef _POSIX2_C_VERSION
#error "unistd.h:_POSIX2_C_VERSION macro is missing from libc-shim"
#endif

#ifndef _POSIX2_LOCALEDEF
#error "unistd.h:_POSIX2_LOCALEDEF macro is missing from libc-shim"
#endif

#ifndef _POSIX2_SW_DEV
#error "unistd.h:_POSIX2_SW_DEV macro is missing from libc-shim"
#endif

#ifndef _POSIX2_VERSION
#error "unistd.h:_POSIX2_VERSION macro is missing from libc-shim"
#endif

#ifndef _POSIX_ADVISORY_INFO
#error "unistd.h:_POSIX_ADVISORY_INFO macro is missing from libc-shim"
#endif

#ifndef _POSIX_ASYNCHRONOUS_IO
#error "unistd.h:_POSIX_ASYNCHRONOUS_IO macro is missing from libc-shim"
#endif

#ifndef _POSIX_ASYNC_IO
#error "unistd.h:_POSIX_ASYNC_IO macro is missing from libc-shim"
#endif

#ifndef _POSIX_BARRIERS
#error "unistd.h:_POSIX_BARRIERS macro is missing from libc-shim"
#endif

#ifndef _POSIX_CHOWN_RESTRICTED
#error "unistd.h:_POSIX_CHOWN_RESTRICTED macro is missing from libc-shim"
#endif

#ifndef _POSIX_CLOCK_SELECTION
#error "unistd.h:_POSIX_CLOCK_SELECTION macro is missing from libc-shim"
#endif

#ifndef _POSIX_CPUTIME
#error "unistd.h:_POSIX_CPUTIME macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "unistd.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_FSYNC
#error "unistd.h:_POSIX_FSYNC macro is missing from libc-shim"
#endif

#ifndef _POSIX_IPV6
#error "unistd.h:_POSIX_IPV6 macro is missing from libc-shim"
#endif

#ifndef _POSIX_JOB_CONTROL
#error "unistd.h:_POSIX_JOB_CONTROL macro is missing from libc-shim"
#endif

#ifndef _POSIX_MAPPED_FILES
#error "unistd.h:_POSIX_MAPPED_FILES macro is missing from libc-shim"
#endif

#ifndef _POSIX_MEMLOCK
#error "unistd.h:_POSIX_MEMLOCK macro is missing from libc-shim"
#endif

#ifndef _POSIX_MEMLOCK_RANGE
#error "unistd.h:_POSIX_MEMLOCK_RANGE macro is missing from libc-shim"
#endif

#ifndef _POSIX_MEMORY_PROTECTION
#error "unistd.h:_POSIX_MEMORY_PROTECTION macro is missing from libc-shim"
#endif

#ifndef _POSIX_MESSAGE_PASSING
#error "unistd.h:_POSIX_MESSAGE_PASSING macro is missing from libc-shim"
#endif

#ifndef _POSIX_MONOTONIC_CLOCK
#error "unistd.h:_POSIX_MONOTONIC_CLOCK macro is missing from libc-shim"
#endif

#ifndef _POSIX_NO_TRUNC
#error "unistd.h:_POSIX_NO_TRUNC macro is missing from libc-shim"
#endif

#ifndef _POSIX_PRIORITIZED_IO
#error "unistd.h:_POSIX_PRIORITIZED_IO macro is missing from libc-shim"
#endif

#ifndef _POSIX_PRIORITY_SCHEDULING
#error "unistd.h:_POSIX_PRIORITY_SCHEDULING macro is missing from libc-shim"
#endif

#ifndef _POSIX_RAW_SOCKETS
#error "unistd.h:_POSIX_RAW_SOCKETS macro is missing from libc-shim"
#endif

#ifndef _POSIX_READER_WRITER_LOCKS
#error "unistd.h:_POSIX_READER_WRITER_LOCKS macro is missing from libc-shim"
#endif

#ifndef _POSIX_REALTIME_SIGNALS
#error "unistd.h:_POSIX_REALTIME_SIGNALS macro is missing from libc-shim"
#endif

#ifndef _POSIX_REENTRANT_FUNCTIONS
#error "unistd.h:_POSIX_REENTRANT_FUNCTIONS macro is missing from libc-shim"
#endif

#ifndef _POSIX_REGEXP
#error "unistd.h:_POSIX_REGEXP macro is missing from libc-shim"
#endif

#ifndef _POSIX_SAVED_IDS
#error "unistd.h:_POSIX_SAVED_IDS macro is missing from libc-shim"
#endif

#ifndef _POSIX_SEMAPHORES
#error "unistd.h:_POSIX_SEMAPHORES macro is missing from libc-shim"
#endif

#ifndef _POSIX_SHARED_MEMORY_OBJECTS
#error "unistd.h:_POSIX_SHARED_MEMORY_OBJECTS macro is missing from libc-shim"
#endif

#ifndef _POSIX_SHELL
#error "unistd.h:_POSIX_SHELL macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "unistd.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SPAWN
#error "unistd.h:_POSIX_SPAWN macro is missing from libc-shim"
#endif

#ifndef _POSIX_SPIN_LOCKS
#error "unistd.h:_POSIX_SPIN_LOCKS macro is missing from libc-shim"
#endif

#ifndef _POSIX_SPORADIC_SERVER
#error "unistd.h:_POSIX_SPORADIC_SERVER macro is missing from libc-shim"
#endif

#ifndef _POSIX_SYNCHRONIZED_IO
#error "unistd.h:_POSIX_SYNCHRONIZED_IO macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREADS
#error "unistd.h:_POSIX_THREADS macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_ATTR_STACKADDR
#error "unistd.h:_POSIX_THREAD_ATTR_STACKADDR macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_ATTR_STACKSIZE
#error "unistd.h:_POSIX_THREAD_ATTR_STACKSIZE macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_CPUTIME
#error "unistd.h:_POSIX_THREAD_CPUTIME macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_PRIORITY_SCHEDULING
#error "unistd.h:_POSIX_THREAD_PRIORITY_SCHEDULING macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_PRIO_INHERIT
#error "unistd.h:_POSIX_THREAD_PRIO_INHERIT macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_PRIO_PROTECT
#error "unistd.h:_POSIX_THREAD_PRIO_PROTECT macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_PROCESS_SHARED
#error "unistd.h:_POSIX_THREAD_PROCESS_SHARED macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_ROBUST_PRIO_INHERIT
#error "unistd.h:_POSIX_THREAD_ROBUST_PRIO_INHERIT macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_ROBUST_PRIO_PROTECT
#error "unistd.h:_POSIX_THREAD_ROBUST_PRIO_PROTECT macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_SAFE_FUNCTIONS
#error "unistd.h:_POSIX_THREAD_SAFE_FUNCTIONS macro is missing from libc-shim"
#endif

#ifndef _POSIX_THREAD_SPORADIC_SERVER
#error "unistd.h:_POSIX_THREAD_SPORADIC_SERVER macro is missing from libc-shim"
#endif

#ifndef _POSIX_TIMEOUTS
#error "unistd.h:_POSIX_TIMEOUTS macro is missing from libc-shim"
#endif

#ifndef _POSIX_TIMERS
#error "unistd.h:_POSIX_TIMERS macro is missing from libc-shim"
#endif

#ifndef _POSIX_TRACE
#error "unistd.h:_POSIX_TRACE macro is missing from libc-shim"
#endif

#ifndef _POSIX_TRACE_EVENT_FILTER
#error "unistd.h:_POSIX_TRACE_EVENT_FILTER macro is missing from libc-shim"
#endif

#ifndef _POSIX_TRACE_INHERIT
#error "unistd.h:_POSIX_TRACE_INHERIT macro is missing from libc-shim"
#endif

#ifndef _POSIX_TRACE_LOG
#error "unistd.h:_POSIX_TRACE_LOG macro is missing from libc-shim"
#endif

#ifndef _POSIX_TYPED_MEMORY_OBJECTS
#error "unistd.h:_POSIX_TYPED_MEMORY_OBJECTS macro is missing from libc-shim"
#endif

#ifndef _POSIX_V6_LP64_OFF64
#error "unistd.h:_POSIX_V6_LP64_OFF64 macro is missing from libc-shim"
#endif

#ifndef _POSIX_V6_LPBIG_OFFBIG
#error "unistd.h:_POSIX_V6_LPBIG_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _POSIX_V7_LP64_OFF64
#error "unistd.h:_POSIX_V7_LP64_OFF64 macro is missing from libc-shim"
#endif

#ifndef _POSIX_V7_LPBIG_OFFBIG
#error "unistd.h:_POSIX_V7_LPBIG_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _POSIX_VDISABLE
#error "unistd.h:_POSIX_VDISABLE macro is missing from libc-shim"
#endif

#ifndef _POSIX_VERSION
#error "unistd.h:_POSIX_VERSION macro is missing from libc-shim"
#endif

#ifndef _SC_2_CHAR_TERM
#error "unistd.h:_SC_2_CHAR_TERM macro is missing from libc-shim"
#endif

#ifndef _SC_2_C_BIND
#error "unistd.h:_SC_2_C_BIND macro is missing from libc-shim"
#endif

#ifndef _SC_2_C_DEV
#error "unistd.h:_SC_2_C_DEV macro is missing from libc-shim"
#endif

#ifndef _SC_2_C_VERSION
#error "unistd.h:_SC_2_C_VERSION macro is missing from libc-shim"
#endif

#ifndef _SC_2_FORT_DEV
#error "unistd.h:_SC_2_FORT_DEV macro is missing from libc-shim"
#endif

#ifndef _SC_2_FORT_RUN
#error "unistd.h:_SC_2_FORT_RUN macro is missing from libc-shim"
#endif

#ifndef _SC_2_LOCALEDEF
#error "unistd.h:_SC_2_LOCALEDEF macro is missing from libc-shim"
#endif

#ifndef _SC_2_PBS
#error "unistd.h:_SC_2_PBS macro is missing from libc-shim"
#endif

#ifndef _SC_2_PBS_ACCOUNTING
#error "unistd.h:_SC_2_PBS_ACCOUNTING macro is missing from libc-shim"
#endif

#ifndef _SC_2_PBS_CHECKPOINT
#error "unistd.h:_SC_2_PBS_CHECKPOINT macro is missing from libc-shim"
#endif

#ifndef _SC_2_PBS_LOCATE
#error "unistd.h:_SC_2_PBS_LOCATE macro is missing from libc-shim"
#endif

#ifndef _SC_2_PBS_MESSAGE
#error "unistd.h:_SC_2_PBS_MESSAGE macro is missing from libc-shim"
#endif

#ifndef _SC_2_PBS_TRACK
#error "unistd.h:_SC_2_PBS_TRACK macro is missing from libc-shim"
#endif

#ifndef _SC_2_SW_DEV
#error "unistd.h:_SC_2_SW_DEV macro is missing from libc-shim"
#endif

#ifndef _SC_2_UPE
#error "unistd.h:_SC_2_UPE macro is missing from libc-shim"
#endif

#ifndef _SC_2_VERSION
#error "unistd.h:_SC_2_VERSION macro is missing from libc-shim"
#endif

#ifndef _SC_ADVISORY_INFO
#error "unistd.h:_SC_ADVISORY_INFO macro is missing from libc-shim"
#endif

#ifndef _SC_AIO_LISTIO_MAX
#error "unistd.h:_SC_AIO_LISTIO_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_AIO_MAX
#error "unistd.h:_SC_AIO_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_AIO_PRIO_DELTA_MAX
#error "unistd.h:_SC_AIO_PRIO_DELTA_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_ARG_MAX
#error "unistd.h:_SC_ARG_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_ASYNCHRONOUS_IO
#error "unistd.h:_SC_ASYNCHRONOUS_IO macro is missing from libc-shim"
#endif

#ifndef _SC_ATEXIT_MAX
#error "unistd.h:_SC_ATEXIT_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_AVPHYS_PAGES
#error "unistd.h:_SC_AVPHYS_PAGES macro is missing from libc-shim"
#endif

#ifndef _SC_BARRIERS
#error "unistd.h:_SC_BARRIERS macro is missing from libc-shim"
#endif

#ifndef _SC_BASE
#error "unistd.h:_SC_BASE macro is missing from libc-shim"
#endif

#ifndef _SC_BC_BASE_MAX
#error "unistd.h:_SC_BC_BASE_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_BC_DIM_MAX
#error "unistd.h:_SC_BC_DIM_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_BC_SCALE_MAX
#error "unistd.h:_SC_BC_SCALE_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_BC_STRING_MAX
#error "unistd.h:_SC_BC_STRING_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_CHARCLASS_NAME_MAX
#error "unistd.h:_SC_CHARCLASS_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_CHAR_BIT
#error "unistd.h:_SC_CHAR_BIT macro is missing from libc-shim"
#endif

#ifndef _SC_CHAR_MAX
#error "unistd.h:_SC_CHAR_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_CHAR_MIN
#error "unistd.h:_SC_CHAR_MIN macro is missing from libc-shim"
#endif

#ifndef _SC_CHILD_MAX
#error "unistd.h:_SC_CHILD_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_CLK_TCK
#error "unistd.h:_SC_CLK_TCK macro is missing from libc-shim"
#endif

#ifndef _SC_CLOCK_SELECTION
#error "unistd.h:_SC_CLOCK_SELECTION macro is missing from libc-shim"
#endif

#ifndef _SC_COLL_WEIGHTS_MAX
#error "unistd.h:_SC_COLL_WEIGHTS_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_CPUTIME
#error "unistd.h:_SC_CPUTIME macro is missing from libc-shim"
#endif

#ifndef _SC_C_LANG_SUPPORT
#error "unistd.h:_SC_C_LANG_SUPPORT macro is missing from libc-shim"
#endif

#ifndef _SC_C_LANG_SUPPORT_R
#error "unistd.h:_SC_C_LANG_SUPPORT_R macro is missing from libc-shim"
#endif

#ifndef _SC_DELAYTIMER_MAX
#error "unistd.h:_SC_DELAYTIMER_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_DEVICE_IO
#error "unistd.h:_SC_DEVICE_IO macro is missing from libc-shim"
#endif

#ifndef _SC_DEVICE_SPECIFIC
#error "unistd.h:_SC_DEVICE_SPECIFIC macro is missing from libc-shim"
#endif

#ifndef _SC_DEVICE_SPECIFIC_R
#error "unistd.h:_SC_DEVICE_SPECIFIC_R macro is missing from libc-shim"
#endif

#ifndef _SC_EQUIV_CLASS_MAX
#error "unistd.h:_SC_EQUIV_CLASS_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_EXPR_NEST_MAX
#error "unistd.h:_SC_EXPR_NEST_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_FD_MGMT
#error "unistd.h:_SC_FD_MGMT macro is missing from libc-shim"
#endif

#ifndef _SC_FIFO
#error "unistd.h:_SC_FIFO macro is missing from libc-shim"
#endif

#ifndef _SC_FILE_ATTRIBUTES
#error "unistd.h:_SC_FILE_ATTRIBUTES macro is missing from libc-shim"
#endif

#ifndef _SC_FILE_LOCKING
#error "unistd.h:_SC_FILE_LOCKING macro is missing from libc-shim"
#endif

#ifndef _SC_FILE_SYSTEM
#error "unistd.h:_SC_FILE_SYSTEM macro is missing from libc-shim"
#endif

#ifndef _SC_FSYNC
#error "unistd.h:_SC_FSYNC macro is missing from libc-shim"
#endif

#ifndef _SC_GETGR_R_SIZE_MAX
#error "unistd.h:_SC_GETGR_R_SIZE_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_GETPW_R_SIZE_MAX
#error "unistd.h:_SC_GETPW_R_SIZE_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_HOST_NAME_MAX
#error "unistd.h:_SC_HOST_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_INT_MAX
#error "unistd.h:_SC_INT_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_INT_MIN
#error "unistd.h:_SC_INT_MIN macro is missing from libc-shim"
#endif

#ifndef _SC_IOV_MAX
#error "unistd.h:_SC_IOV_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_IPV6
#error "unistd.h:_SC_IPV6 macro is missing from libc-shim"
#endif

#ifndef _SC_JOB_CONTROL
#error "unistd.h:_SC_JOB_CONTROL macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL1_DCACHE_ASSOC
#error "unistd.h:_SC_LEVEL1_DCACHE_ASSOC macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL1_DCACHE_LINESIZE
#error "unistd.h:_SC_LEVEL1_DCACHE_LINESIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL1_DCACHE_SIZE
#error "unistd.h:_SC_LEVEL1_DCACHE_SIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL1_ICACHE_ASSOC
#error "unistd.h:_SC_LEVEL1_ICACHE_ASSOC macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL1_ICACHE_LINESIZE
#error "unistd.h:_SC_LEVEL1_ICACHE_LINESIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL1_ICACHE_SIZE
#error "unistd.h:_SC_LEVEL1_ICACHE_SIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL2_CACHE_ASSOC
#error "unistd.h:_SC_LEVEL2_CACHE_ASSOC macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL2_CACHE_LINESIZE
#error "unistd.h:_SC_LEVEL2_CACHE_LINESIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL2_CACHE_SIZE
#error "unistd.h:_SC_LEVEL2_CACHE_SIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL3_CACHE_ASSOC
#error "unistd.h:_SC_LEVEL3_CACHE_ASSOC macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL3_CACHE_LINESIZE
#error "unistd.h:_SC_LEVEL3_CACHE_LINESIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL3_CACHE_SIZE
#error "unistd.h:_SC_LEVEL3_CACHE_SIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL4_CACHE_ASSOC
#error "unistd.h:_SC_LEVEL4_CACHE_ASSOC macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL4_CACHE_LINESIZE
#error "unistd.h:_SC_LEVEL4_CACHE_LINESIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LEVEL4_CACHE_SIZE
#error "unistd.h:_SC_LEVEL4_CACHE_SIZE macro is missing from libc-shim"
#endif

#ifndef _SC_LINE_MAX
#error "unistd.h:_SC_LINE_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_LOGIN_NAME_MAX
#error "unistd.h:_SC_LOGIN_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_LONG_BIT
#error "unistd.h:_SC_LONG_BIT macro is missing from libc-shim"
#endif

#ifndef _SC_MAPPED_FILES
#error "unistd.h:_SC_MAPPED_FILES macro is missing from libc-shim"
#endif

#ifndef _SC_MB_LEN_MAX
#error "unistd.h:_SC_MB_LEN_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_MEMLOCK
#error "unistd.h:_SC_MEMLOCK macro is missing from libc-shim"
#endif

#ifndef _SC_MEMLOCK_RANGE
#error "unistd.h:_SC_MEMLOCK_RANGE macro is missing from libc-shim"
#endif

#ifndef _SC_MEMORY_PROTECTION
#error "unistd.h:_SC_MEMORY_PROTECTION macro is missing from libc-shim"
#endif

#ifndef _SC_MESSAGE_PASSING
#error "unistd.h:_SC_MESSAGE_PASSING macro is missing from libc-shim"
#endif

#ifndef _SC_MINSIGSTKSZ
#error "unistd.h:_SC_MINSIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef _SC_MONOTONIC_CLOCK
#error "unistd.h:_SC_MONOTONIC_CLOCK macro is missing from libc-shim"
#endif

#ifndef _SC_MQ_OPEN_MAX
#error "unistd.h:_SC_MQ_OPEN_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_MQ_PRIO_MAX
#error "unistd.h:_SC_MQ_PRIO_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_MULTI_PROCESS
#error "unistd.h:_SC_MULTI_PROCESS macro is missing from libc-shim"
#endif

#ifndef _SC_NETWORKING
#error "unistd.h:_SC_NETWORKING macro is missing from libc-shim"
#endif

#ifndef _SC_NGROUPS_MAX
#error "unistd.h:_SC_NGROUPS_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_NL_ARGMAX
#error "unistd.h:_SC_NL_ARGMAX macro is missing from libc-shim"
#endif

#ifndef _SC_NL_LANGMAX
#error "unistd.h:_SC_NL_LANGMAX macro is missing from libc-shim"
#endif

#ifndef _SC_NL_MSGMAX
#error "unistd.h:_SC_NL_MSGMAX macro is missing from libc-shim"
#endif

#ifndef _SC_NL_NMAX
#error "unistd.h:_SC_NL_NMAX macro is missing from libc-shim"
#endif

#ifndef _SC_NL_SETMAX
#error "unistd.h:_SC_NL_SETMAX macro is missing from libc-shim"
#endif

#ifndef _SC_NL_TEXTMAX
#error "unistd.h:_SC_NL_TEXTMAX macro is missing from libc-shim"
#endif

#ifndef _SC_NPROCESSORS_CONF
#error "unistd.h:_SC_NPROCESSORS_CONF macro is missing from libc-shim"
#endif

#ifndef _SC_NPROCESSORS_ONLN
#error "unistd.h:_SC_NPROCESSORS_ONLN macro is missing from libc-shim"
#endif

#ifndef _SC_NZERO
#error "unistd.h:_SC_NZERO macro is missing from libc-shim"
#endif

#ifndef _SC_OPEN_MAX
#error "unistd.h:_SC_OPEN_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_PAGESIZE
#error "unistd.h:_SC_PAGESIZE macro is missing from libc-shim"
#endif

#ifndef _SC_PAGE_SIZE
#error "unistd.h:_SC_PAGE_SIZE macro is missing from libc-shim"
#endif

#ifndef _SC_PASS_MAX
#error "unistd.h:_SC_PASS_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_PHYS_PAGES
#error "unistd.h:_SC_PHYS_PAGES macro is missing from libc-shim"
#endif

#ifndef _SC_PII
#error "unistd.h:_SC_PII macro is missing from libc-shim"
#endif

#ifndef _SC_PII_INTERNET
#error "unistd.h:_SC_PII_INTERNET macro is missing from libc-shim"
#endif

#ifndef _SC_PII_INTERNET_DGRAM
#error "unistd.h:_SC_PII_INTERNET_DGRAM macro is missing from libc-shim"
#endif

#ifndef _SC_PII_INTERNET_STREAM
#error "unistd.h:_SC_PII_INTERNET_STREAM macro is missing from libc-shim"
#endif

#ifndef _SC_PII_OSI
#error "unistd.h:_SC_PII_OSI macro is missing from libc-shim"
#endif

#ifndef _SC_PII_OSI_CLTS
#error "unistd.h:_SC_PII_OSI_CLTS macro is missing from libc-shim"
#endif

#ifndef _SC_PII_OSI_COTS
#error "unistd.h:_SC_PII_OSI_COTS macro is missing from libc-shim"
#endif

#ifndef _SC_PII_OSI_M
#error "unistd.h:_SC_PII_OSI_M macro is missing from libc-shim"
#endif

#ifndef _SC_PII_SOCKET
#error "unistd.h:_SC_PII_SOCKET macro is missing from libc-shim"
#endif

#ifndef _SC_PII_XTI
#error "unistd.h:_SC_PII_XTI macro is missing from libc-shim"
#endif

#ifndef _SC_PIPE
#error "unistd.h:_SC_PIPE macro is missing from libc-shim"
#endif

#ifndef _SC_POLL
#error "unistd.h:_SC_POLL macro is missing from libc-shim"
#endif

#ifndef _SC_PRIORITIZED_IO
#error "unistd.h:_SC_PRIORITIZED_IO macro is missing from libc-shim"
#endif

#ifndef _SC_PRIORITY_SCHEDULING
#error "unistd.h:_SC_PRIORITY_SCHEDULING macro is missing from libc-shim"
#endif

#ifndef _SC_RAW_SOCKETS
#error "unistd.h:_SC_RAW_SOCKETS macro is missing from libc-shim"
#endif

#ifndef _SC_READER_WRITER_LOCKS
#error "unistd.h:_SC_READER_WRITER_LOCKS macro is missing from libc-shim"
#endif

#ifndef _SC_REALTIME_SIGNALS
#error "unistd.h:_SC_REALTIME_SIGNALS macro is missing from libc-shim"
#endif

#ifndef _SC_REGEXP
#error "unistd.h:_SC_REGEXP macro is missing from libc-shim"
#endif

#ifndef _SC_REGEX_VERSION
#error "unistd.h:_SC_REGEX_VERSION macro is missing from libc-shim"
#endif

#ifndef _SC_RE_DUP_MAX
#error "unistd.h:_SC_RE_DUP_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_RTSIG_MAX
#error "unistd.h:_SC_RTSIG_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SAVED_IDS
#error "unistd.h:_SC_SAVED_IDS macro is missing from libc-shim"
#endif

#ifndef _SC_SCHAR_MAX
#error "unistd.h:_SC_SCHAR_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SCHAR_MIN
#error "unistd.h:_SC_SCHAR_MIN macro is missing from libc-shim"
#endif

#ifndef _SC_SELECT
#error "unistd.h:_SC_SELECT macro is missing from libc-shim"
#endif

#ifndef _SC_SEMAPHORES
#error "unistd.h:_SC_SEMAPHORES macro is missing from libc-shim"
#endif

#ifndef _SC_SEM_NSEMS_MAX
#error "unistd.h:_SC_SEM_NSEMS_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SEM_VALUE_MAX
#error "unistd.h:_SC_SEM_VALUE_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SHARED_MEMORY_OBJECTS
#error "unistd.h:_SC_SHARED_MEMORY_OBJECTS macro is missing from libc-shim"
#endif

#ifndef _SC_SHELL
#error "unistd.h:_SC_SHELL macro is missing from libc-shim"
#endif

#ifndef _SC_SHRT_MAX
#error "unistd.h:_SC_SHRT_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SHRT_MIN
#error "unistd.h:_SC_SHRT_MIN macro is missing from libc-shim"
#endif

#ifndef _SC_SIGNALS
#error "unistd.h:_SC_SIGNALS macro is missing from libc-shim"
#endif

#ifndef _SC_SIGQUEUE_MAX
#error "unistd.h:_SC_SIGQUEUE_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SIGSTKSZ
#error "unistd.h:_SC_SIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef _SC_SINGLE_PROCESS
#error "unistd.h:_SC_SINGLE_PROCESS macro is missing from libc-shim"
#endif

#ifndef _SC_SPAWN
#error "unistd.h:_SC_SPAWN macro is missing from libc-shim"
#endif

#ifndef _SC_SPIN_LOCKS
#error "unistd.h:_SC_SPIN_LOCKS macro is missing from libc-shim"
#endif

#ifndef _SC_SPORADIC_SERVER
#error "unistd.h:_SC_SPORADIC_SERVER macro is missing from libc-shim"
#endif

#ifndef _SC_SSIZE_MAX
#error "unistd.h:_SC_SSIZE_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SS_REPL_MAX
#error "unistd.h:_SC_SS_REPL_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_STREAMS
#error "unistd.h:_SC_STREAMS macro is missing from libc-shim"
#endif

#ifndef _SC_STREAM_MAX
#error "unistd.h:_SC_STREAM_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SYMLOOP_MAX
#error "unistd.h:_SC_SYMLOOP_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_SYNCHRONIZED_IO
#error "unistd.h:_SC_SYNCHRONIZED_IO macro is missing from libc-shim"
#endif

#ifndef _SC_SYSTEM_DATABASE
#error "unistd.h:_SC_SYSTEM_DATABASE macro is missing from libc-shim"
#endif

#ifndef _SC_SYSTEM_DATABASE_R
#error "unistd.h:_SC_SYSTEM_DATABASE_R macro is missing from libc-shim"
#endif

#ifndef _SC_THREADS
#error "unistd.h:_SC_THREADS macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_ATTR_STACKADDR
#error "unistd.h:_SC_THREAD_ATTR_STACKADDR macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_ATTR_STACKSIZE
#error "unistd.h:_SC_THREAD_ATTR_STACKSIZE macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_CPUTIME
#error "unistd.h:_SC_THREAD_CPUTIME macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_DESTRUCTOR_ITERATIONS
#error "unistd.h:_SC_THREAD_DESTRUCTOR_ITERATIONS macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_KEYS_MAX
#error "unistd.h:_SC_THREAD_KEYS_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_PRIORITY_SCHEDULING
#error "unistd.h:_SC_THREAD_PRIORITY_SCHEDULING macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_PRIO_INHERIT
#error "unistd.h:_SC_THREAD_PRIO_INHERIT macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_PRIO_PROTECT
#error "unistd.h:_SC_THREAD_PRIO_PROTECT macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_PROCESS_SHARED
#error "unistd.h:_SC_THREAD_PROCESS_SHARED macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_ROBUST_PRIO_INHERIT
#error "unistd.h:_SC_THREAD_ROBUST_PRIO_INHERIT macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_ROBUST_PRIO_PROTECT
#error "unistd.h:_SC_THREAD_ROBUST_PRIO_PROTECT macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_SAFE_FUNCTIONS
#error "unistd.h:_SC_THREAD_SAFE_FUNCTIONS macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_SPORADIC_SERVER
#error "unistd.h:_SC_THREAD_SPORADIC_SERVER macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_STACK_MIN
#error "unistd.h:_SC_THREAD_STACK_MIN macro is missing from libc-shim"
#endif

#ifndef _SC_THREAD_THREADS_MAX
#error "unistd.h:_SC_THREAD_THREADS_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_TIMEOUTS
#error "unistd.h:_SC_TIMEOUTS macro is missing from libc-shim"
#endif

#ifndef _SC_TIMERS
#error "unistd.h:_SC_TIMERS macro is missing from libc-shim"
#endif

#ifndef _SC_TIMER_MAX
#error "unistd.h:_SC_TIMER_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_TRACE
#error "unistd.h:_SC_TRACE macro is missing from libc-shim"
#endif

#ifndef _SC_TRACE_EVENT_FILTER
#error "unistd.h:_SC_TRACE_EVENT_FILTER macro is missing from libc-shim"
#endif

#ifndef _SC_TRACE_EVENT_NAME_MAX
#error "unistd.h:_SC_TRACE_EVENT_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_TRACE_INHERIT
#error "unistd.h:_SC_TRACE_INHERIT macro is missing from libc-shim"
#endif

#ifndef _SC_TRACE_LOG
#error "unistd.h:_SC_TRACE_LOG macro is missing from libc-shim"
#endif

#ifndef _SC_TRACE_NAME_MAX
#error "unistd.h:_SC_TRACE_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_TRACE_SYS_MAX
#error "unistd.h:_SC_TRACE_SYS_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_TRACE_USER_EVENT_MAX
#error "unistd.h:_SC_TRACE_USER_EVENT_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_TTY_NAME_MAX
#error "unistd.h:_SC_TTY_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_TYPED_MEMORY_OBJECTS
#error "unistd.h:_SC_TYPED_MEMORY_OBJECTS macro is missing from libc-shim"
#endif

#ifndef _SC_TZNAME_MAX
#error "unistd.h:_SC_TZNAME_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_T_IOV_MAX
#error "unistd.h:_SC_T_IOV_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_UCHAR_MAX
#error "unistd.h:_SC_UCHAR_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_UINT_MAX
#error "unistd.h:_SC_UINT_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_UIO_MAXIOV
#error "unistd.h:_SC_UIO_MAXIOV macro is missing from libc-shim"
#endif

#ifndef _SC_ULONG_MAX
#error "unistd.h:_SC_ULONG_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_USER_GROUPS
#error "unistd.h:_SC_USER_GROUPS macro is missing from libc-shim"
#endif

#ifndef _SC_USER_GROUPS_R
#error "unistd.h:_SC_USER_GROUPS_R macro is missing from libc-shim"
#endif

#ifndef _SC_USHRT_MAX
#error "unistd.h:_SC_USHRT_MAX macro is missing from libc-shim"
#endif

#ifndef _SC_V6_ILP32_OFF32
#error "unistd.h:_SC_V6_ILP32_OFF32 macro is missing from libc-shim"
#endif

#ifndef _SC_V6_ILP32_OFFBIG
#error "unistd.h:_SC_V6_ILP32_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _SC_V6_LP64_OFF64
#error "unistd.h:_SC_V6_LP64_OFF64 macro is missing from libc-shim"
#endif

#ifndef _SC_V6_LPBIG_OFFBIG
#error "unistd.h:_SC_V6_LPBIG_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _SC_V7_ILP32_OFF32
#error "unistd.h:_SC_V7_ILP32_OFF32 macro is missing from libc-shim"
#endif

#ifndef _SC_V7_ILP32_OFFBIG
#error "unistd.h:_SC_V7_ILP32_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _SC_V7_LP64_OFF64
#error "unistd.h:_SC_V7_LP64_OFF64 macro is missing from libc-shim"
#endif

#ifndef _SC_V7_LPBIG_OFFBIG
#error "unistd.h:_SC_V7_LPBIG_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _SC_VERSION
#error "unistd.h:_SC_VERSION macro is missing from libc-shim"
#endif

#ifndef _SC_WORD_BIT
#error "unistd.h:_SC_WORD_BIT macro is missing from libc-shim"
#endif

#ifndef _SC_XBS5_ILP32_OFF32
#error "unistd.h:_SC_XBS5_ILP32_OFF32 macro is missing from libc-shim"
#endif

#ifndef _SC_XBS5_ILP32_OFFBIG
#error "unistd.h:_SC_XBS5_ILP32_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _SC_XBS5_LP64_OFF64
#error "unistd.h:_SC_XBS5_LP64_OFF64 macro is missing from libc-shim"
#endif

#ifndef _SC_XBS5_LPBIG_OFFBIG
#error "unistd.h:_SC_XBS5_LPBIG_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_CRYPT
#error "unistd.h:_SC_XOPEN_CRYPT macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_ENH_I18N
#error "unistd.h:_SC_XOPEN_ENH_I18N macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_LEGACY
#error "unistd.h:_SC_XOPEN_LEGACY macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_REALTIME
#error "unistd.h:_SC_XOPEN_REALTIME macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_REALTIME_THREADS
#error "unistd.h:_SC_XOPEN_REALTIME_THREADS macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_SHM
#error "unistd.h:_SC_XOPEN_SHM macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_STREAMS
#error "unistd.h:_SC_XOPEN_STREAMS macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_UNIX
#error "unistd.h:_SC_XOPEN_UNIX macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_VERSION
#error "unistd.h:_SC_XOPEN_VERSION macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_XCU_VERSION
#error "unistd.h:_SC_XOPEN_XCU_VERSION macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_XPG2
#error "unistd.h:_SC_XOPEN_XPG2 macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_XPG3
#error "unistd.h:_SC_XOPEN_XPG3 macro is missing from libc-shim"
#endif

#ifndef _SC_XOPEN_XPG4
#error "unistd.h:_SC_XOPEN_XPG4 macro is missing from libc-shim"
#endif

#ifndef _SIZE_T
#error "unistd.h:_SIZE_T macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "unistd.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "unistd.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _UNISTD_H
#error "unistd.h:_UNISTD_H macro is missing from libc-shim"
#endif

#ifndef _XBS5_LP64_OFF64
#error "unistd.h:_XBS5_LP64_OFF64 macro is missing from libc-shim"
#endif

#ifndef _XBS5_LPBIG_OFFBIG
#error "unistd.h:_XBS5_LPBIG_OFFBIG macro is missing from libc-shim"
#endif

#ifndef _XOPEN_ENH_I18N
#error "unistd.h:_XOPEN_ENH_I18N macro is missing from libc-shim"
#endif

#ifndef _XOPEN_LEGACY
#error "unistd.h:_XOPEN_LEGACY macro is missing from libc-shim"
#endif

#ifndef _XOPEN_REALTIME
#error "unistd.h:_XOPEN_REALTIME macro is missing from libc-shim"
#endif

#ifndef _XOPEN_REALTIME_THREADS
#error "unistd.h:_XOPEN_REALTIME_THREADS macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SHM
#error "unistd.h:_XOPEN_SHM macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "unistd.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "unistd.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

#ifndef _XOPEN_UNIX
#error "unistd.h:_XOPEN_UNIX macro is missing from libc-shim"
#endif

#ifndef _XOPEN_VERSION
#error "unistd.h:_XOPEN_VERSION macro is missing from libc-shim"
#endif

#ifndef _XOPEN_XCU_VERSION
#error "unistd.h:_XOPEN_XCU_VERSION macro is missing from libc-shim"
#endif

#ifndef _XOPEN_XPG2
#error "unistd.h:_XOPEN_XPG2 macro is missing from libc-shim"
#endif

#ifndef _XOPEN_XPG3
#error "unistd.h:_XOPEN_XPG3 macro is missing from libc-shim"
#endif

#ifndef _XOPEN_XPG4
#error "unistd.h:_XOPEN_XPG4 macro is missing from libc-shim"
#endif

int main(void) { return 0; }
