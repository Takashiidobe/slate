#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <execinfo.h>
#include <fnmatch.h>
#include <glob.h>
#include <gnu/libc-version.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/random.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

static int gnu_environment_extensions(void) {
  char *directory;
  char *canonical;
  char  current[4096];
  int   total = 0;

  total += setenv("SLATE_GNU_LIBC_VALUE", "ready", 1) == 0;
  total += strcmp(secure_getenv("SLATE_GNU_LIBC_VALUE"), "ready") == 0;
  total += unsetenv("SLATE_GNU_LIBC_VALUE") == 0;

  directory  = get_current_dir_name();
  canonical  = canonicalize_file_name(".");
  total     += getcwd(current, sizeof(current)) != NULL;
  total     += directory != NULL && strcmp(directory, current) == 0;
  total     += canonical != NULL && strcmp(canonical, current) == 0;
  free(directory);
  free(canonical);

  total += strcmp(strdupa("slate"), "slate") == 0;
  total += strcmp(strndupa("slate-truncated", 5), "slate") == 0;
  return total;
}

static int gnu_time_extensions(void) {
  struct tm epoch = {};
  struct tm local = {};
  time_t    timestamp;
  int       total = 0;

  epoch.tm_year  = 70;
  epoch.tm_mon   = 0;
  epoch.tm_mday  = 1;
  timestamp      = timegm(&epoch);
  total         += timestamp == 0;

  local.tm_year  = 70;
  local.tm_mon   = 0;
  local.tm_mday  = 2;
  total         += timelocal(&local) != (time_t)-1;
  return total;
}

static int gnu_pattern_extensions(void) {
  regex_t     expression = {};
  glob_t      paths      = {};
  const char *error;
  int         total = 0;

  total += fnmatch("file-+(one|two).c", "file-two.c", FNM_EXTMATCH) == 0;
  re_set_syntax(RE_SYNTAX_POSIX_EXTENDED);
  error  = re_compile_pattern("sl(a|e)te", 9, &expression);
  total += error == NULL;
  total += re_match(&expression, "slate", 5, 0, NULL) == 5;
  regfree(&expression);

  total += glob("/dev/{null,zero}", GLOB_BRACE, NULL, &paths) == 0;
  total += paths.gl_pathc == 2;
  total += strcmp(paths.gl_pathv[0], "/dev/null") == 0;
  total += strcmp(paths.gl_pathv[1], "/dev/zero") == 0;
  globfree(&paths);
  return total;
}

static int gnu_runtime_extensions(void) {
  unsigned char random_bytes[8];
  void         *frames[8];
  Dl_info       information = {};
  long          page_size   = sysconf(_SC_PAGESIZE);
  int           total       = 0;

  total += getauxval(AT_PAGESZ) == (unsigned long)page_size;
  total += gettid() == (pid_t)syscall(SYS_gettid);
  total += getentropy(random_bytes, sizeof(random_bytes)) == 0;
  total += arc4random_uniform(1) == 0;
  total += get_nprocs() > 0;
  total += get_phys_pages() > 0;
  total += backtrace(frames, 8) > 0;
  total += dladdr((void *)&gnu_runtime_extensions, &information) != 0;
  total += information.dli_fname != NULL;
  total += gnu_get_libc_version()[0] != '\0';
  total += program_invocation_name != NULL;
  total += program_invocation_short_name != NULL;
  return total;
}

int main(void) {
  printf("%d %d %d\n", gnu_environment_extensions(), gnu_time_extensions(),
         gnu_pattern_extensions() + gnu_runtime_extensions());
  return 0;
}




// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 dli_fname: ptr<const i8>;
// DEFAULT-NEXT:         field1 dli_fbase: ptr<void>;
// DEFAULT-NEXT:         field2 dli_sname: ptr<const i8>;
// DEFAULT-NEXT:         field3 dli_saddr: ptr<void>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_Dl_info:[0-9]+]] Dl_info = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE___size_t:[0-9]+]] __size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_size_t_2:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_stat:[0-9]+]] stat = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 gl_pathc: u64;
// DEFAULT-NEXT:         field1 gl_pathv: ptr<ptr<i8>>;
// DEFAULT-NEXT:         field2 gl_offs: u64;
// DEFAULT-NEXT:         field3 gl_flags: i32;
// DEFAULT-NEXT:         field4 gl_closedir: ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:         field5 gl_readdir: ptr<fn(ptr<void>) -> ptr<@type[[TYPE_dirent:[0-9]+]]>>;
// DEFAULT-NEXT:         field6 gl_opendir: ptr<fn(ptr<const i8>) -> ptr<void>>;
// DEFAULT-NEXT:         field7 gl_lstat: ptr<fn(ptr<const i8>, ptr<@type[[TYPE_stat]]>) -> i32>;
// DEFAULT-NEXT:         field8 gl_stat: ptr<fn(ptr<const i8>, ptr<@type[[TYPE_stat]]>) -> i32>;
// DEFAULT-NEXT:     } [size=72, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64]];
// DEFAULT-NEXT:     type @type[[TYPE_dirent]] dirent = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_glob_t:[0-9]+]] glob_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___pid_t:[0-9]+]] __pid_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___time_t:[0-9]+]] __time_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_pid_t:[0-9]+]] pid_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_time_t:[0-9]+]] time_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___re_size_t:[0-9]+]] __re_size_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___re_long_size_t:[0-9]+]] __re_long_size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_reg_syntax_t:[0-9]+]] reg_syntax_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_re_pattern_buffer:[0-9]+]] re_pattern_buffer = struct {
// DEFAULT-NEXT:         field0 buffer: ptr<@type[[TYPE_re_dfa_t:[0-9]+]]>;
// DEFAULT-NEXT:         field1 allocated: u64;
// DEFAULT-NEXT:         field2 used: u64;
// DEFAULT-NEXT:         field3 syntax: u64;
// DEFAULT-NEXT:         field4 fastmap: ptr<i8>;
// DEFAULT-NEXT:         field5 translate: ptr<u8>;
// DEFAULT-NEXT:         field6 re_nsub: u64;
// DEFAULT-NEXT:         field7 can_be_null: u32 : 1;
// DEFAULT-NEXT:         field8 regs_allocated: u32 : 2;
// DEFAULT-NEXT:         field9 fastmap_accurate: u32 : 1;
// DEFAULT-NEXT:         field10 no_sub: u32 : 1;
// DEFAULT-NEXT:         field11 not_bol: u32 : 1;
// DEFAULT-NEXT:         field12 not_eol: u32 : 1;
// DEFAULT-NEXT:         field13 newline_anchor: u32 : 1;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 56, 56, 56, 56, 56, 56], bit_offsets=[None, None, None, None, None, None, None, Some(448), Some(449), Some(451), Some(452), Some(453), Some(454), Some(455)], bit_units=[(56, 1)], field_units=[None, None, None, None, None, None, None, Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_re_dfa_t]] re_dfa_t = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_regex_t:[0-9]+]] regex_t = @type[[TYPE_re_pattern_buffer]];
// DEFAULT-NEXT:     type @type[[TYPE_regoff_t:[0-9]+]] regoff_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_re_registers:[0-9]+]] re_registers = struct {
// DEFAULT-NEXT:         field0 num_regs: u32;
// DEFAULT-NEXT:         field1 start: ptr<i32>;
// DEFAULT-NEXT:         field2 end: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_tm:[0-9]+]] tm = struct {
// DEFAULT-NEXT:         field0 tm_sec: i32;
// DEFAULT-NEXT:         field1 tm_min: i32;
// DEFAULT-NEXT:         field2 tm_hour: i32;
// DEFAULT-NEXT:         field3 tm_mday: i32;
// DEFAULT-NEXT:         field4 tm_mon: i32;
// DEFAULT-NEXT:         field5 tm_year: i32;
// DEFAULT-NEXT:         field6 tm_wday: i32;
// DEFAULT-NEXT:         field7 tm_yday: i32;
// DEFAULT-NEXT:         field8 tm_isdst: i32;
// DEFAULT-NEXT:         field9 tm_gmtoff: i64;
// DEFAULT-NEXT:         field10 tm_zone: ptr<const i8>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 4, 8, 12, 16, 20, 24, 28, 32, 40, 48]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE__SC_ARG_MAX:[0-9]+]] _SC_ARG_MAX = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE__SC_CHILD_MAX:[0-9]+]] _SC_CHILD_MAX = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE__SC_CLK_TCK:[0-9]+]] _SC_CLK_TCK = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE__SC_NGROUPS_MAX:[0-9]+]] _SC_NGROUPS_MAX = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE__SC_OPEN_MAX:[0-9]+]] _SC_OPEN_MAX = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE__SC_STREAM_MAX:[0-9]+]] _SC_STREAM_MAX = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE__SC_TZNAME_MAX:[0-9]+]] _SC_TZNAME_MAX = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE__SC_JOB_CONTROL:[0-9]+]] _SC_JOB_CONTROL = const<i32>(7);
// DEFAULT-NEXT:         %[[VALUE__SC_SAVED_IDS:[0-9]+]] _SC_SAVED_IDS = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE__SC_REALTIME_SIGNALS:[0-9]+]] _SC_REALTIME_SIGNALS = const<i32>(9);
// DEFAULT-NEXT:         %[[VALUE__SC_PRIORITY_SCHEDULING:[0-9]+]] _SC_PRIORITY_SCHEDULING = const<i32>(10);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMERS:[0-9]+]] _SC_TIMERS = const<i32>(11);
// DEFAULT-NEXT:         %[[VALUE__SC_ASYNCHRONOUS_IO:[0-9]+]] _SC_ASYNCHRONOUS_IO = const<i32>(12);
// DEFAULT-NEXT:         %[[VALUE__SC_PRIORITIZED_IO:[0-9]+]] _SC_PRIORITIZED_IO = const<i32>(13);
// DEFAULT-NEXT:         %[[VALUE__SC_SYNCHRONIZED_IO:[0-9]+]] _SC_SYNCHRONIZED_IO = const<i32>(14);
// DEFAULT-NEXT:         %[[VALUE__SC_FSYNC:[0-9]+]] _SC_FSYNC = const<i32>(15);
// DEFAULT-NEXT:         %[[VALUE__SC_MAPPED_FILES:[0-9]+]] _SC_MAPPED_FILES = const<i32>(16);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMLOCK:[0-9]+]] _SC_MEMLOCK = const<i32>(17);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMLOCK_RANGE:[0-9]+]] _SC_MEMLOCK_RANGE = const<i32>(18);
// DEFAULT-NEXT:         %[[VALUE__SC_MEMORY_PROTECTION:[0-9]+]] _SC_MEMORY_PROTECTION = const<i32>(19);
// DEFAULT-NEXT:         %[[VALUE__SC_MESSAGE_PASSING:[0-9]+]] _SC_MESSAGE_PASSING = const<i32>(20);
// DEFAULT-NEXT:         %[[VALUE__SC_SEMAPHORES:[0-9]+]] _SC_SEMAPHORES = const<i32>(21);
// DEFAULT-NEXT:         %[[VALUE__SC_SHARED_MEMORY_OBJECTS:[0-9]+]] _SC_SHARED_MEMORY_OBJECTS = const<i32>(22);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_LISTIO_MAX:[0-9]+]] _SC_AIO_LISTIO_MAX = const<i32>(23);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_MAX:[0-9]+]] _SC_AIO_MAX = const<i32>(24);
// DEFAULT-NEXT:         %[[VALUE__SC_AIO_PRIO_DELTA_MAX:[0-9]+]] _SC_AIO_PRIO_DELTA_MAX = const<i32>(25);
// DEFAULT-NEXT:         %[[VALUE__SC_DELAYTIMER_MAX:[0-9]+]] _SC_DELAYTIMER_MAX = const<i32>(26);
// DEFAULT-NEXT:         %[[VALUE__SC_MQ_OPEN_MAX:[0-9]+]] _SC_MQ_OPEN_MAX = const<i32>(27);
// DEFAULT-NEXT:         %[[VALUE__SC_MQ_PRIO_MAX:[0-9]+]] _SC_MQ_PRIO_MAX = const<i32>(28);
// DEFAULT-NEXT:         %[[VALUE__SC_VERSION:[0-9]+]] _SC_VERSION = const<i32>(29);
// DEFAULT-NEXT:         %[[VALUE__SC_PAGESIZE:[0-9]+]] _SC_PAGESIZE = const<i32>(30);
// DEFAULT-NEXT:         %[[VALUE__SC_RTSIG_MAX:[0-9]+]] _SC_RTSIG_MAX = const<i32>(31);
// DEFAULT-NEXT:         %[[VALUE__SC_SEM_NSEMS_MAX:[0-9]+]] _SC_SEM_NSEMS_MAX = const<i32>(32);
// DEFAULT-NEXT:         %[[VALUE__SC_SEM_VALUE_MAX:[0-9]+]] _SC_SEM_VALUE_MAX = const<i32>(33);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGQUEUE_MAX:[0-9]+]] _SC_SIGQUEUE_MAX = const<i32>(34);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMER_MAX:[0-9]+]] _SC_TIMER_MAX = const<i32>(35);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_BASE_MAX:[0-9]+]] _SC_BC_BASE_MAX = const<i32>(36);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_DIM_MAX:[0-9]+]] _SC_BC_DIM_MAX = const<i32>(37);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_SCALE_MAX:[0-9]+]] _SC_BC_SCALE_MAX = const<i32>(38);
// DEFAULT-NEXT:         %[[VALUE__SC_BC_STRING_MAX:[0-9]+]] _SC_BC_STRING_MAX = const<i32>(39);
// DEFAULT-NEXT:         %[[VALUE__SC_COLL_WEIGHTS_MAX:[0-9]+]] _SC_COLL_WEIGHTS_MAX = const<i32>(40);
// DEFAULT-NEXT:         %[[VALUE__SC_EQUIV_CLASS_MAX:[0-9]+]] _SC_EQUIV_CLASS_MAX = const<i32>(41);
// DEFAULT-NEXT:         %[[VALUE__SC_EXPR_NEST_MAX:[0-9]+]] _SC_EXPR_NEST_MAX = const<i32>(42);
// DEFAULT-NEXT:         %[[VALUE__SC_LINE_MAX:[0-9]+]] _SC_LINE_MAX = const<i32>(43);
// DEFAULT-NEXT:         %[[VALUE__SC_RE_DUP_MAX:[0-9]+]] _SC_RE_DUP_MAX = const<i32>(44);
// DEFAULT-NEXT:         %[[VALUE__SC_CHARCLASS_NAME_MAX:[0-9]+]] _SC_CHARCLASS_NAME_MAX = const<i32>(45);
// DEFAULT-NEXT:         %[[VALUE__SC_2_VERSION:[0-9]+]] _SC_2_VERSION = const<i32>(46);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_BIND:[0-9]+]] _SC_2_C_BIND = const<i32>(47);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_DEV:[0-9]+]] _SC_2_C_DEV = const<i32>(48);
// DEFAULT-NEXT:         %[[VALUE__SC_2_FORT_DEV:[0-9]+]] _SC_2_FORT_DEV = const<i32>(49);
// DEFAULT-NEXT:         %[[VALUE__SC_2_FORT_RUN:[0-9]+]] _SC_2_FORT_RUN = const<i32>(50);
// DEFAULT-NEXT:         %[[VALUE__SC_2_SW_DEV:[0-9]+]] _SC_2_SW_DEV = const<i32>(51);
// DEFAULT-NEXT:         %[[VALUE__SC_2_LOCALEDEF:[0-9]+]] _SC_2_LOCALEDEF = const<i32>(52);
// DEFAULT-NEXT:         %[[VALUE__SC_PII:[0-9]+]] _SC_PII = const<i32>(53);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_XTI:[0-9]+]] _SC_PII_XTI = const<i32>(54);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_SOCKET:[0-9]+]] _SC_PII_SOCKET = const<i32>(55);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET:[0-9]+]] _SC_PII_INTERNET = const<i32>(56);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI:[0-9]+]] _SC_PII_OSI = const<i32>(57);
// DEFAULT-NEXT:         %[[VALUE__SC_POLL:[0-9]+]] _SC_POLL = const<i32>(58);
// DEFAULT-NEXT:         %[[VALUE__SC_SELECT:[0-9]+]] _SC_SELECT = const<i32>(59);
// DEFAULT-NEXT:         %[[VALUE__SC_UIO_MAXIOV:[0-9]+]] _SC_UIO_MAXIOV = const<i32>(60);
// DEFAULT-NEXT:         %[[VALUE__SC_IOV_MAX:[0-9]+]] _SC_IOV_MAX = const<i32>(60);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET_STREAM:[0-9]+]] _SC_PII_INTERNET_STREAM = const<i32>(61);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_INTERNET_DGRAM:[0-9]+]] _SC_PII_INTERNET_DGRAM = const<i32>(62);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_COTS:[0-9]+]] _SC_PII_OSI_COTS = const<i32>(63);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_CLTS:[0-9]+]] _SC_PII_OSI_CLTS = const<i32>(64);
// DEFAULT-NEXT:         %[[VALUE__SC_PII_OSI_M:[0-9]+]] _SC_PII_OSI_M = const<i32>(65);
// DEFAULT-NEXT:         %[[VALUE__SC_T_IOV_MAX:[0-9]+]] _SC_T_IOV_MAX = const<i32>(66);
// DEFAULT-NEXT:         %[[VALUE__SC_THREADS:[0-9]+]] _SC_THREADS = const<i32>(67);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_SAFE_FUNCTIONS:[0-9]+]] _SC_THREAD_SAFE_FUNCTIONS = const<i32>(68);
// DEFAULT-NEXT:         %[[VALUE__SC_GETGR_R_SIZE_MAX:[0-9]+]] _SC_GETGR_R_SIZE_MAX = const<i32>(69);
// DEFAULT-NEXT:         %[[VALUE__SC_GETPW_R_SIZE_MAX:[0-9]+]] _SC_GETPW_R_SIZE_MAX = const<i32>(70);
// DEFAULT-NEXT:         %[[VALUE__SC_LOGIN_NAME_MAX:[0-9]+]] _SC_LOGIN_NAME_MAX = const<i32>(71);
// DEFAULT-NEXT:         %[[VALUE__SC_TTY_NAME_MAX:[0-9]+]] _SC_TTY_NAME_MAX = const<i32>(72);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_DESTRUCTOR_ITERATIONS:[0-9]+]] _SC_THREAD_DESTRUCTOR_ITERATIONS = const<i32>(73);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_KEYS_MAX:[0-9]+]] _SC_THREAD_KEYS_MAX = const<i32>(74);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_STACK_MIN:[0-9]+]] _SC_THREAD_STACK_MIN = const<i32>(75);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_THREADS_MAX:[0-9]+]] _SC_THREAD_THREADS_MAX = const<i32>(76);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ATTR_STACKADDR:[0-9]+]] _SC_THREAD_ATTR_STACKADDR = const<i32>(77);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ATTR_STACKSIZE:[0-9]+]] _SC_THREAD_ATTR_STACKSIZE = const<i32>(78);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIORITY_SCHEDULING:[0-9]+]] _SC_THREAD_PRIORITY_SCHEDULING = const<i32>(79);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIO_INHERIT:[0-9]+]] _SC_THREAD_PRIO_INHERIT = const<i32>(80);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PRIO_PROTECT:[0-9]+]] _SC_THREAD_PRIO_PROTECT = const<i32>(81);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_PROCESS_SHARED:[0-9]+]] _SC_THREAD_PROCESS_SHARED = const<i32>(82);
// DEFAULT-NEXT:         %[[VALUE__SC_NPROCESSORS_CONF:[0-9]+]] _SC_NPROCESSORS_CONF = const<i32>(83);
// DEFAULT-NEXT:         %[[VALUE__SC_NPROCESSORS_ONLN:[0-9]+]] _SC_NPROCESSORS_ONLN = const<i32>(84);
// DEFAULT-NEXT:         %[[VALUE__SC_PHYS_PAGES:[0-9]+]] _SC_PHYS_PAGES = const<i32>(85);
// DEFAULT-NEXT:         %[[VALUE__SC_AVPHYS_PAGES:[0-9]+]] _SC_AVPHYS_PAGES = const<i32>(86);
// DEFAULT-NEXT:         %[[VALUE__SC_ATEXIT_MAX:[0-9]+]] _SC_ATEXIT_MAX = const<i32>(87);
// DEFAULT-NEXT:         %[[VALUE__SC_PASS_MAX:[0-9]+]] _SC_PASS_MAX = const<i32>(88);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_VERSION:[0-9]+]] _SC_XOPEN_VERSION = const<i32>(89);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XCU_VERSION:[0-9]+]] _SC_XOPEN_XCU_VERSION = const<i32>(90);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_UNIX:[0-9]+]] _SC_XOPEN_UNIX = const<i32>(91);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_CRYPT:[0-9]+]] _SC_XOPEN_CRYPT = const<i32>(92);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_ENH_I18N:[0-9]+]] _SC_XOPEN_ENH_I18N = const<i32>(93);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_SHM:[0-9]+]] _SC_XOPEN_SHM = const<i32>(94);
// DEFAULT-NEXT:         %[[VALUE__SC_2_CHAR_TERM:[0-9]+]] _SC_2_CHAR_TERM = const<i32>(95);
// DEFAULT-NEXT:         %[[VALUE__SC_2_C_VERSION:[0-9]+]] _SC_2_C_VERSION = const<i32>(96);
// DEFAULT-NEXT:         %[[VALUE__SC_2_UPE:[0-9]+]] _SC_2_UPE = const<i32>(97);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG2:[0-9]+]] _SC_XOPEN_XPG2 = const<i32>(98);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG3:[0-9]+]] _SC_XOPEN_XPG3 = const<i32>(99);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_XPG4:[0-9]+]] _SC_XOPEN_XPG4 = const<i32>(100);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_BIT:[0-9]+]] _SC_CHAR_BIT = const<i32>(101);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_MAX:[0-9]+]] _SC_CHAR_MAX = const<i32>(102);
// DEFAULT-NEXT:         %[[VALUE__SC_CHAR_MIN:[0-9]+]] _SC_CHAR_MIN = const<i32>(103);
// DEFAULT-NEXT:         %[[VALUE__SC_INT_MAX:[0-9]+]] _SC_INT_MAX = const<i32>(104);
// DEFAULT-NEXT:         %[[VALUE__SC_INT_MIN:[0-9]+]] _SC_INT_MIN = const<i32>(105);
// DEFAULT-NEXT:         %[[VALUE__SC_LONG_BIT:[0-9]+]] _SC_LONG_BIT = const<i32>(106);
// DEFAULT-NEXT:         %[[VALUE__SC_WORD_BIT:[0-9]+]] _SC_WORD_BIT = const<i32>(107);
// DEFAULT-NEXT:         %[[VALUE__SC_MB_LEN_MAX:[0-9]+]] _SC_MB_LEN_MAX = const<i32>(108);
// DEFAULT-NEXT:         %[[VALUE__SC_NZERO:[0-9]+]] _SC_NZERO = const<i32>(109);
// DEFAULT-NEXT:         %[[VALUE__SC_SSIZE_MAX:[0-9]+]] _SC_SSIZE_MAX = const<i32>(110);
// DEFAULT-NEXT:         %[[VALUE__SC_SCHAR_MAX:[0-9]+]] _SC_SCHAR_MAX = const<i32>(111);
// DEFAULT-NEXT:         %[[VALUE__SC_SCHAR_MIN:[0-9]+]] _SC_SCHAR_MIN = const<i32>(112);
// DEFAULT-NEXT:         %[[VALUE__SC_SHRT_MAX:[0-9]+]] _SC_SHRT_MAX = const<i32>(113);
// DEFAULT-NEXT:         %[[VALUE__SC_SHRT_MIN:[0-9]+]] _SC_SHRT_MIN = const<i32>(114);
// DEFAULT-NEXT:         %[[VALUE__SC_UCHAR_MAX:[0-9]+]] _SC_UCHAR_MAX = const<i32>(115);
// DEFAULT-NEXT:         %[[VALUE__SC_UINT_MAX:[0-9]+]] _SC_UINT_MAX = const<i32>(116);
// DEFAULT-NEXT:         %[[VALUE__SC_ULONG_MAX:[0-9]+]] _SC_ULONG_MAX = const<i32>(117);
// DEFAULT-NEXT:         %[[VALUE__SC_USHRT_MAX:[0-9]+]] _SC_USHRT_MAX = const<i32>(118);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_ARGMAX:[0-9]+]] _SC_NL_ARGMAX = const<i32>(119);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_LANGMAX:[0-9]+]] _SC_NL_LANGMAX = const<i32>(120);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_MSGMAX:[0-9]+]] _SC_NL_MSGMAX = const<i32>(121);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_NMAX:[0-9]+]] _SC_NL_NMAX = const<i32>(122);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_SETMAX:[0-9]+]] _SC_NL_SETMAX = const<i32>(123);
// DEFAULT-NEXT:         %[[VALUE__SC_NL_TEXTMAX:[0-9]+]] _SC_NL_TEXTMAX = const<i32>(124);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_ILP32_OFF32:[0-9]+]] _SC_XBS5_ILP32_OFF32 = const<i32>(125);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_ILP32_OFFBIG:[0-9]+]] _SC_XBS5_ILP32_OFFBIG = const<i32>(126);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_LP64_OFF64:[0-9]+]] _SC_XBS5_LP64_OFF64 = const<i32>(127);
// DEFAULT-NEXT:         %[[VALUE__SC_XBS5_LPBIG_OFFBIG:[0-9]+]] _SC_XBS5_LPBIG_OFFBIG = const<i32>(128);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_LEGACY:[0-9]+]] _SC_XOPEN_LEGACY = const<i32>(129);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_REALTIME:[0-9]+]] _SC_XOPEN_REALTIME = const<i32>(130);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_REALTIME_THREADS:[0-9]+]] _SC_XOPEN_REALTIME_THREADS = const<i32>(131);
// DEFAULT-NEXT:         %[[VALUE__SC_ADVISORY_INFO:[0-9]+]] _SC_ADVISORY_INFO = const<i32>(132);
// DEFAULT-NEXT:         %[[VALUE__SC_BARRIERS:[0-9]+]] _SC_BARRIERS = const<i32>(133);
// DEFAULT-NEXT:         %[[VALUE__SC_BASE:[0-9]+]] _SC_BASE = const<i32>(134);
// DEFAULT-NEXT:         %[[VALUE__SC_C_LANG_SUPPORT:[0-9]+]] _SC_C_LANG_SUPPORT = const<i32>(135);
// DEFAULT-NEXT:         %[[VALUE__SC_C_LANG_SUPPORT_R:[0-9]+]] _SC_C_LANG_SUPPORT_R = const<i32>(136);
// DEFAULT-NEXT:         %[[VALUE__SC_CLOCK_SELECTION:[0-9]+]] _SC_CLOCK_SELECTION = const<i32>(137);
// DEFAULT-NEXT:         %[[VALUE__SC_CPUTIME:[0-9]+]] _SC_CPUTIME = const<i32>(138);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_CPUTIME:[0-9]+]] _SC_THREAD_CPUTIME = const<i32>(139);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_IO:[0-9]+]] _SC_DEVICE_IO = const<i32>(140);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_SPECIFIC:[0-9]+]] _SC_DEVICE_SPECIFIC = const<i32>(141);
// DEFAULT-NEXT:         %[[VALUE__SC_DEVICE_SPECIFIC_R:[0-9]+]] _SC_DEVICE_SPECIFIC_R = const<i32>(142);
// DEFAULT-NEXT:         %[[VALUE__SC_FD_MGMT:[0-9]+]] _SC_FD_MGMT = const<i32>(143);
// DEFAULT-NEXT:         %[[VALUE__SC_FIFO:[0-9]+]] _SC_FIFO = const<i32>(144);
// DEFAULT-NEXT:         %[[VALUE__SC_PIPE:[0-9]+]] _SC_PIPE = const<i32>(145);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_ATTRIBUTES:[0-9]+]] _SC_FILE_ATTRIBUTES = const<i32>(146);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_LOCKING:[0-9]+]] _SC_FILE_LOCKING = const<i32>(147);
// DEFAULT-NEXT:         %[[VALUE__SC_FILE_SYSTEM:[0-9]+]] _SC_FILE_SYSTEM = const<i32>(148);
// DEFAULT-NEXT:         %[[VALUE__SC_MONOTONIC_CLOCK:[0-9]+]] _SC_MONOTONIC_CLOCK = const<i32>(149);
// DEFAULT-NEXT:         %[[VALUE__SC_MULTI_PROCESS:[0-9]+]] _SC_MULTI_PROCESS = const<i32>(150);
// DEFAULT-NEXT:         %[[VALUE__SC_SINGLE_PROCESS:[0-9]+]] _SC_SINGLE_PROCESS = const<i32>(151);
// DEFAULT-NEXT:         %[[VALUE__SC_NETWORKING:[0-9]+]] _SC_NETWORKING = const<i32>(152);
// DEFAULT-NEXT:         %[[VALUE__SC_READER_WRITER_LOCKS:[0-9]+]] _SC_READER_WRITER_LOCKS = const<i32>(153);
// DEFAULT-NEXT:         %[[VALUE__SC_SPIN_LOCKS:[0-9]+]] _SC_SPIN_LOCKS = const<i32>(154);
// DEFAULT-NEXT:         %[[VALUE__SC_REGEXP:[0-9]+]] _SC_REGEXP = const<i32>(155);
// DEFAULT-NEXT:         %[[VALUE__SC_REGEX_VERSION:[0-9]+]] _SC_REGEX_VERSION = const<i32>(156);
// DEFAULT-NEXT:         %[[VALUE__SC_SHELL:[0-9]+]] _SC_SHELL = const<i32>(157);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGNALS:[0-9]+]] _SC_SIGNALS = const<i32>(158);
// DEFAULT-NEXT:         %[[VALUE__SC_SPAWN:[0-9]+]] _SC_SPAWN = const<i32>(159);
// DEFAULT-NEXT:         %[[VALUE__SC_SPORADIC_SERVER:[0-9]+]] _SC_SPORADIC_SERVER = const<i32>(160);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_SPORADIC_SERVER:[0-9]+]] _SC_THREAD_SPORADIC_SERVER = const<i32>(161);
// DEFAULT-NEXT:         %[[VALUE__SC_SYSTEM_DATABASE:[0-9]+]] _SC_SYSTEM_DATABASE = const<i32>(162);
// DEFAULT-NEXT:         %[[VALUE__SC_SYSTEM_DATABASE_R:[0-9]+]] _SC_SYSTEM_DATABASE_R = const<i32>(163);
// DEFAULT-NEXT:         %[[VALUE__SC_TIMEOUTS:[0-9]+]] _SC_TIMEOUTS = const<i32>(164);
// DEFAULT-NEXT:         %[[VALUE__SC_TYPED_MEMORY_OBJECTS:[0-9]+]] _SC_TYPED_MEMORY_OBJECTS = const<i32>(165);
// DEFAULT-NEXT:         %[[VALUE__SC_USER_GROUPS:[0-9]+]] _SC_USER_GROUPS = const<i32>(166);
// DEFAULT-NEXT:         %[[VALUE__SC_USER_GROUPS_R:[0-9]+]] _SC_USER_GROUPS_R = const<i32>(167);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS:[0-9]+]] _SC_2_PBS = const<i32>(168);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_ACCOUNTING:[0-9]+]] _SC_2_PBS_ACCOUNTING = const<i32>(169);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_LOCATE:[0-9]+]] _SC_2_PBS_LOCATE = const<i32>(170);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_MESSAGE:[0-9]+]] _SC_2_PBS_MESSAGE = const<i32>(171);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_TRACK:[0-9]+]] _SC_2_PBS_TRACK = const<i32>(172);
// DEFAULT-NEXT:         %[[VALUE__SC_SYMLOOP_MAX:[0-9]+]] _SC_SYMLOOP_MAX = const<i32>(173);
// DEFAULT-NEXT:         %[[VALUE__SC_STREAMS:[0-9]+]] _SC_STREAMS = const<i32>(174);
// DEFAULT-NEXT:         %[[VALUE__SC_2_PBS_CHECKPOINT:[0-9]+]] _SC_2_PBS_CHECKPOINT = const<i32>(175);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_ILP32_OFF32:[0-9]+]] _SC_V6_ILP32_OFF32 = const<i32>(176);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_ILP32_OFFBIG:[0-9]+]] _SC_V6_ILP32_OFFBIG = const<i32>(177);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_LP64_OFF64:[0-9]+]] _SC_V6_LP64_OFF64 = const<i32>(178);
// DEFAULT-NEXT:         %[[VALUE__SC_V6_LPBIG_OFFBIG:[0-9]+]] _SC_V6_LPBIG_OFFBIG = const<i32>(179);
// DEFAULT-NEXT:         %[[VALUE__SC_HOST_NAME_MAX:[0-9]+]] _SC_HOST_NAME_MAX = const<i32>(180);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE:[0-9]+]] _SC_TRACE = const<i32>(181);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_EVENT_FILTER:[0-9]+]] _SC_TRACE_EVENT_FILTER = const<i32>(182);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_INHERIT:[0-9]+]] _SC_TRACE_INHERIT = const<i32>(183);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_LOG:[0-9]+]] _SC_TRACE_LOG = const<i32>(184);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_SIZE:[0-9]+]] _SC_LEVEL1_ICACHE_SIZE = const<i32>(185);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_ASSOC:[0-9]+]] _SC_LEVEL1_ICACHE_ASSOC = const<i32>(186);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_ICACHE_LINESIZE:[0-9]+]] _SC_LEVEL1_ICACHE_LINESIZE = const<i32>(187);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_SIZE:[0-9]+]] _SC_LEVEL1_DCACHE_SIZE = const<i32>(188);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_ASSOC:[0-9]+]] _SC_LEVEL1_DCACHE_ASSOC = const<i32>(189);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL1_DCACHE_LINESIZE:[0-9]+]] _SC_LEVEL1_DCACHE_LINESIZE = const<i32>(190);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_SIZE:[0-9]+]] _SC_LEVEL2_CACHE_SIZE = const<i32>(191);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_ASSOC:[0-9]+]] _SC_LEVEL2_CACHE_ASSOC = const<i32>(192);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL2_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL2_CACHE_LINESIZE = const<i32>(193);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_SIZE:[0-9]+]] _SC_LEVEL3_CACHE_SIZE = const<i32>(194);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_ASSOC:[0-9]+]] _SC_LEVEL3_CACHE_ASSOC = const<i32>(195);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL3_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL3_CACHE_LINESIZE = const<i32>(196);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_SIZE:[0-9]+]] _SC_LEVEL4_CACHE_SIZE = const<i32>(197);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_ASSOC:[0-9]+]] _SC_LEVEL4_CACHE_ASSOC = const<i32>(198);
// DEFAULT-NEXT:         %[[VALUE__SC_LEVEL4_CACHE_LINESIZE:[0-9]+]] _SC_LEVEL4_CACHE_LINESIZE = const<i32>(199);
// DEFAULT-NEXT:         %[[VALUE__SC_IPV6:[0-9]+]] _SC_IPV6 = const<i32>(235);
// DEFAULT-NEXT:         %[[VALUE__SC_RAW_SOCKETS:[0-9]+]] _SC_RAW_SOCKETS = const<i32>(236);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_ILP32_OFF32:[0-9]+]] _SC_V7_ILP32_OFF32 = const<i32>(237);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_ILP32_OFFBIG:[0-9]+]] _SC_V7_ILP32_OFFBIG = const<i32>(238);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_LP64_OFF64:[0-9]+]] _SC_V7_LP64_OFF64 = const<i32>(239);
// DEFAULT-NEXT:         %[[VALUE__SC_V7_LPBIG_OFFBIG:[0-9]+]] _SC_V7_LPBIG_OFFBIG = const<i32>(240);
// DEFAULT-NEXT:         %[[VALUE__SC_SS_REPL_MAX:[0-9]+]] _SC_SS_REPL_MAX = const<i32>(241);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_EVENT_NAME_MAX:[0-9]+]] _SC_TRACE_EVENT_NAME_MAX = const<i32>(242);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_NAME_MAX:[0-9]+]] _SC_TRACE_NAME_MAX = const<i32>(243);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_SYS_MAX:[0-9]+]] _SC_TRACE_SYS_MAX = const<i32>(244);
// DEFAULT-NEXT:         %[[VALUE__SC_TRACE_USER_EVENT_MAX:[0-9]+]] _SC_TRACE_USER_EVENT_MAX = const<i32>(245);
// DEFAULT-NEXT:         %[[VALUE__SC_XOPEN_STREAMS:[0-9]+]] _SC_XOPEN_STREAMS = const<i32>(246);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ROBUST_PRIO_INHERIT:[0-9]+]] _SC_THREAD_ROBUST_PRIO_INHERIT = const<i32>(247);
// DEFAULT-NEXT:         %[[VALUE__SC_THREAD_ROBUST_PRIO_PROTECT:[0-9]+]] _SC_THREAD_ROBUST_PRIO_PROTECT = const<i32>(248);
// DEFAULT-NEXT:         %[[VALUE__SC_MINSIGSTKSZ:[0-9]+]] _SC_MINSIGSTKSZ = const<i32>(249);
// DEFAULT-NEXT:         %[[VALUE__SC_SIGSTKSZ:[0-9]+]] _SC_SIGSTKSZ = const<i32>(250);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     extern %[[VALUE__SC_TZNAME_MAX]] program_invocation_name: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE__SC_JOB_CONTROL]] program_invocation_short_name: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([46, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([115, 108, 97, 116, 101, 45, 116, 114, 117, 110, 99, 97, 116, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([102, 105, 108, 101, 45, 43, 40, 111, 110, 101, 124, 116, 119, 111, 41, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 105, 108, 101, 45, 116, 119, 111, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([115, 108, 40, 97, 124, 101, 41, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([47, 100, 101, 118, 47, 123, 110, 117, 108, 108, 44, 122, 101, 114, 111, 125, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 110, 117, 108, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 122, 101, 114, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__SC_STREAM_MAX]] @dladdr(%[[VALUE___address:[0-9]+]] __address: ptr<const void>, %[[VALUE___info:[0-9]+]] __info: ptr<@type[[TYPE0]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PRIORITY_SCHEDULING]] @backtrace(%[[VALUE___array:[0-9]+]] __array: ptr<ptr<void>>, %[[VALUE___size:[0-9]+]] __size: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_SYNCHRONIZED_IO]] @fnmatch(%[[VALUE___pattern:[0-9]+]] __pattern: ptr<const i8>, %[[VALUE___name:[0-9]+]] __name: ptr<const i8>, %[[VALUE___flags:[0-9]+]] __flags: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_AIO_MAX]] @glob(%[[VALUE___pattern_2:[0-9]+]] __pattern: ptr<const i8> [restrict], %[[VALUE___flags_2:[0-9]+]] __flags: i32, %[[VALUE___errfunc:[0-9]+]] __errfunc: ptr<fn(ptr<const i8>, i32) -> i32>, %[[VALUE___pglob:[0-9]+]] __pglob: ptr<@type[[TYPE1]]> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_DELAYTIMER_MAX]] @globfree(%[[VALUE___pglob_2:[0-9]+]] __pglob: ptr<@type[[TYPE1]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_MQ_OPEN_MAX]] @gnu_get_libc_version() -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_EXPR_NEST_MAX]] @re_set_syntax(%[[VALUE___syntax:[0-9]+]] __syntax: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_2_VERSION]] @re_compile_pattern(%[[VALUE___pattern_3:[0-9]+]] __pattern: ptr<const i8>, %[[VALUE___length:[0-9]+]] __length: u64, %[[VALUE___buffer:[0-9]+]] __buffer: ptr<@type[[TYPE_re_pattern_buffer]]>) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_2_LOCALEDEF]] @re_match(%[[VALUE___buffer_2:[0-9]+]] __buffer: ptr<@type[[TYPE_re_pattern_buffer]]>, %[[VALUE___String:[0-9]+]] __String: ptr<const i8>, %[[VALUE___length_2:[0-9]+]] __length: i32, %[[VALUE___start:[0-9]+]] __start: i32, %[[VALUE___regs:[0-9]+]] __regs: ptr<@type[[TYPE_re_registers]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PII_XTI]] @regfree(%[[VALUE___preg:[0-9]+]] __preg: ptr<@type[[TYPE_re_pattern_buffer]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PII_INTERNET]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_POLL]] @arc4random_uniform(%[[VALUE___upper_bound:[0-9]+]] __upper_bound: u32) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_UIO_MAXIOV]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PII_INTERNET_STREAM]] @secure_getenv(%[[VALUE___name_2:[0-9]+]] __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PII_OSI_M]] @setenv(%[[VALUE___name_3:[0-9]+]] __name: ptr<const i8>, %[[VALUE___value:[0-9]+]] __value: ptr<const i8>, %[[VALUE___replace:[0-9]+]] __replace: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_THREADS]] @unsetenv(%[[VALUE___name_4:[0-9]+]] __name: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_GETGR_R_SIZE_MAX]] @canonicalize_file_name(%[[VALUE___name_5:[0-9]+]] __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_THREAD_DESTRUCTOR_ITERATIONS]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_THREAD_THREADS_MAX]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE__SC_THREAD_ATTR_STACKSIZE]] @strlen(%[[VALUE___s:[0-9]+]] __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE__SC_THREAD_PRIO_PROTECT]] @strnlen(%[[VALUE___string:[0-9]+]] __string: ptr<const i8>, %[[VALUE___maxlen:[0-9]+]] __maxlen: u64) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE__SC_NPROCESSORS_CONF]] @getauxval(%[[VALUE___type:[0-9]+]] __type: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_AVPHYS_PAGES]] @getentropy(%[[VALUE___buffer_3:[0-9]+]] __buffer: ptr<void>, %[[VALUE___length_3:[0-9]+]] __length: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_ATEXIT_MAX]] @get_nprocs() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_PASS_MAX]] @get_phys_pages() -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_XOPEN_UNIX]] @timegm(%[[VALUE___tp:[0-9]+]] __tp: ptr<@type[[TYPE_tm]]>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_XOPEN_ENH_I18N]] @timelocal(%[[VALUE___tp_2:[0-9]+]] __tp: ptr<@type[[TYPE_tm]]>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_2_C_VERSION]] @getcwd(%[[VALUE___buf:[0-9]+]] __buf: ptr<i8>, %[[VALUE___size_2:[0-9]+]] __size: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__SC_2_UPE]] @get_current_dir_name() -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sysconf:[0-9]+]] @sysconf(%[[VALUE___name_6:[0-9]+]] __name: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_syscall:[0-9]+]] @syscall(%[[VALUE___sysno:[0-9]+]] __sysno: i64, ...) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gettid:[0-9]+]] @gettid() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_environment_extensions:[0-9]+]] @gnu_environment_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_directory:[0-9]+]] directory: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_canonical:[0-9]+]] canonical: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_current:[0-9]+]] current: array<i8, 4096> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%[[VALUE__SC_PII_OSI_M]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_2]])), const<i32>(1)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_THREAD_THREADS_MAX]], pointer_cast<ptr<const i8>, reason=arg>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE__SC_PII_INTERNET_STREAM]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_3]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_4]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE__SC_THREADS]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%[[VALUE_str_5]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_directory]], call<ptr<i8>, signature=fn() -> ptr<i8>>(%[[VALUE__SC_2_UPE]]));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn() -> ptr<i8>>(%[[VALUE__SC_2_UPE]]);
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_canonical]], call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE__SC_GETGR_R_SIZE_MAX]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_6]]))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE__SC_GETGR_R_SIZE_MAX]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_6]])));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, u64) -> ptr<i8>>(%[[VALUE__SC_2_C_VERSION]], array_decay<ptr<i8>, length=Some(4096)>(%[[VALUE_current]]), const<u64>(4096)), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_directory]]), null<ptr<i8>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_THREAD_THREADS_MAX]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_directory]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4096)>(%[[VALUE_current]]))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_canonical]]), null<ptr<i8>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_THREAD_THREADS_MAX]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_canonical]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4096)>(%[[VALUE_current]]))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE__SC_UIO_MAXIOV]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_directory]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE__SC_UIO_MAXIOV]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_canonical]])));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: ptr<i8> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___old:[0-9]+]] __old: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_7]]));
// DEFAULT-NEXT:             let %[[VALUE___len:[0-9]+]] __len: u64 [storage=automatic] = add<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE__SC_THREAD_ATTR_STACKSIZE]], read<ptr<const i8>>(%[[VALUE___old]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %[[VALUE___new:[0-9]+]] __new: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], read<u64>(%[[VALUE___len]])));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE14]], pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE__SC_THREAD_DESTRUCTOR_ITERATIONS]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE___new]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE___old]])), read<u64>(%[[VALUE___len]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_THREAD_THREADS_MAX]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE14]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_8]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: ptr<i8> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___old_2:[0-9]+]] __old: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_9]]));
// DEFAULT-NEXT:             let %[[VALUE___len_2:[0-9]+]] __len: u64 [storage=automatic] = call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%[[VALUE__SC_THREAD_PRIO_PROTECT]], read<ptr<const i8>>(%[[VALUE___old_2]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:             let %[[VALUE___new_2:[0-9]+]] __new: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], add<u64, overflow=wrap>(read<u64>(%[[VALUE___len_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE___new_2]]), read<u64>(%[[VALUE___len_2]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE17]], pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE__SC_THREAD_DESTRUCTOR_ITERATIONS]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE___new_2]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE___old_2]])), read<u64>(%[[VALUE___len_2]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_THREAD_THREADS_MAX]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE17]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_10]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_time_extensions:[0-9]+]] @gnu_time_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_epoch:[0-9]+]] epoch: @type[[TYPE_tm]] [storage=automatic] = aggregate<@type[[TYPE_tm]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: @type[[TYPE_tm]] [storage=automatic] = aggregate<@type[[TYPE_tm]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_timestamp:[0-9]+]] timestamp: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_total_2:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32>(field5(%[[VALUE_epoch]]), const<i32>(70));
// DEFAULT-NEXT:         write<i32>(field4(%[[VALUE_epoch]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field3(%[[VALUE_epoch]]), const<i32>(1));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_timestamp]], call<i64, signature=fn(ptr<@type[[TYPE_tm]]>) -> i64>(%[[VALUE__SC_XOPEN_UNIX]], addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_epoch]])));
// DEFAULT-NEXT:         call<i64, signature=fn(ptr<@type[[TYPE_tm]]>) -> i64>(%[[VALUE__SC_XOPEN_UNIX]], addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_epoch]]));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), from_bool<i32, reason=promotion>(eq<i64>(read<i64>(%[[VALUE_timestamp]]), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:         write<i32>(field5(%[[VALUE_local]]), const<i32>(70));
// DEFAULT-NEXT:         write<i32>(field4(%[[VALUE_local]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field3(%[[VALUE_local]]), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), from_bool<i32, reason=promotion>(ne<i64>(call<i64, signature=fn(ptr<@type[[TYPE_tm]]>) -> i64>(%[[VALUE__SC_XOPEN_ENH_I18N]], addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_local]])), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_pattern_extensions:[0-9]+]] @gnu_pattern_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_expression:[0-9]+]] expression: @type[[TYPE_re_pattern_buffer]] [storage=automatic] = aggregate<@type[[TYPE_re_pattern_buffer]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_paths:[0-9]+]] paths: @type[[TYPE1]] [storage=automatic] = aggregate<@type[[TYPE1]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_error:[0-9]+]] error: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_total_3:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%[[VALUE__SC_SYNCHRONIZED_IO]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str_11]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_12]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:         call<u64, signature=fn(u64) -> u64>(%[[VALUE__SC_EXPR_NEST_MAX]], or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_error]], call<ptr<const i8>, signature=fn(ptr<const i8>, u64, ptr<@type[[TYPE_re_pattern_buffer]]>) -> ptr<const i8>>(%[[VALUE__SC_2_VERSION]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_13]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))), addr_of<ptr<@type[[TYPE_re_pattern_buffer]]>>(%[[VALUE_expression]])));
// DEFAULT-NEXT:         call<ptr<const i8>, signature=fn(ptr<const i8>, u64, ptr<@type[[TYPE_re_pattern_buffer]]>) -> ptr<const i8>>(%[[VALUE__SC_2_VERSION]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_13]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))), addr_of<ptr<@type[[TYPE_re_pattern_buffer]]>>(%[[VALUE_expression]]));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE25]]), from_bool<i32, reason=promotion>(eq<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_error]]), null<ptr<const i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE26]]));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE_re_pattern_buffer]]>, ptr<const i8>, i32, i32, ptr<@type[[TYPE_re_registers]]>) -> i32>(%[[VALUE__SC_2_LOCALEDEF]], addr_of<ptr<@type[[TYPE_re_pattern_buffer]]>>(%[[VALUE_expression]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_14]])), const<i32>(5), const<i32>(0), null<ptr<@type[[TYPE_re_registers]]>>), const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_re_pattern_buffer]]>) -> void>(%[[VALUE__SC_PII_XTI]], addr_of<ptr<@type[[TYPE_re_pattern_buffer]]>>(%[[VALUE_expression]]));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, i32, ptr<fn(ptr<const i8>, i32) -> i32>, ptr<@type[[TYPE1]]>) -> i32>(%[[VALUE__SC_AIO_MAX]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_15]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(10)), null<ptr<fn(ptr<const i8>, i32) -> i32>>, addr_of<ptr<@type[[TYPE1]]>>(%[[VALUE_paths]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(field0(%[[VALUE_paths]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE32]]));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE33]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_THREAD_THREADS_MAX]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(field1(%[[VALUE_paths]])), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_16]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE34]]));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE35]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE__SC_THREAD_THREADS_MAX]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(field1(%[[VALUE_paths]])), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_17]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE1]]>) -> void>(%[[VALUE__SC_DELAYTIMER_MAX]], addr_of<ptr<@type[[TYPE1]]>>(%[[VALUE_paths]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_runtime_extensions:[0-9]+]] @gnu_runtime_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_random_bytes:[0-9]+]] random_bytes: array<u8, 8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_frames:[0-9]+]] frames: array<ptr<void>, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_information:[0-9]+]] information: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_page_size:[0-9]+]] page_size: i64 [storage=automatic] = call<i64, signature=fn(i32) -> i64>(%[[VALUE_sysconf]], const<i32>(30));
// DEFAULT-NEXT:         let %[[VALUE_total_4:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE37]]), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE__SC_NPROCESSORS_CONF]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_page_size]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE38]]));
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE39]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_gettid]]), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64, ...) -> i64>(%[[VALUE_syscall]], widen<i64, reason=arg>(const<i32>(186)))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE40]]));
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE41]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<void>, u64) -> i32>(%[[VALUE__SC_AVPHYS_PAGES]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%[[VALUE_random_bytes]])), const<u64>(8)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE42]]));
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE43]]), from_bool<i32, reason=promotion>(eq<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE__SC_POLL]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE44]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE45]]), from_bool<i32, reason=promotion>(gt<i32>(call<i32, signature=fn() -> i32>(%[[VALUE__SC_ATEXIT_MAX]]), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE46]]));
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE47]]), from_bool<i32, reason=promotion>(gt<i64>(call<i64, signature=fn() -> i64>(%[[VALUE__SC_PASS_MAX]]), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE48]]));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE49]]), from_bool<i32, reason=promotion>(gt<i32>(call<i32, signature=fn(ptr<ptr<void>>, i32) -> i32>(%[[VALUE__SC_PRIORITY_SCHEDULING]], array_decay<ptr<ptr<void>>, length=Some(8)>(%[[VALUE_frames]]), const<i32>(8)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE51]]), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<@type[[TYPE0]]>) -> i32>(%[[VALUE__SC_STREAM_MAX]], pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<fn() -> i32>>(%[[VALUE_gnu_runtime_extensions]]))), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_information]])), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE52]]));
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE53]]), from_bool<i32, reason=promotion>(ne<ptr<const i8>>(read<ptr<const i8>>(field0(%[[VALUE_information]])), null<ptr<const i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE54]]));
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE55]]), from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(call<ptr<const i8>, signature=fn() -> ptr<const i8>>(%[[VALUE__SC_MQ_OPEN_MAX]]), const<i32>(0))))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE56]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE57]]), from_bool<i32, reason=promotion>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE__SC_TZNAME_MAX]]), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE58]]));
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), from_bool<i32, reason=promotion>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE__SC_JOB_CONTROL]]), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE60]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE__SC_PII_INTERNET]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_18]])), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_environment_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_time_extensions]]), add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_gnu_pattern_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_runtime_extensions]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
