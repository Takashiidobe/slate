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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 dli_fname: ptr<const i8>;
// DEFAULT-NEXT:         field1 dli_fbase: ptr<void>;
// DEFAULT-NEXT:         field2 dli_sname: ptr<const i8>;
// DEFAULT-NEXT:         field3 dli_saddr: ptr<void>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type2 Dl_info = @type1;
// DEFAULT-NEXT:     type @type3 __size_t = u64;
// DEFAULT-NEXT:     type @type4 size_t = u64;
// DEFAULT-NEXT:     type @type5 = struct {
// DEFAULT-NEXT:         field0 gl_pathc: u64;
// DEFAULT-NEXT:         field1 gl_pathv: ptr<ptr<i8>>;
// DEFAULT-NEXT:         field2 gl_offs: u64;
// DEFAULT-NEXT:         field3 gl_flags: i32;
// DEFAULT-NEXT:         field4 gl_closedir: ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:         field5 gl_readdir: ptr<fn(ptr<void>) -> ptr<@type6>>;
// DEFAULT-NEXT:         field6 gl_opendir: ptr<fn(ptr<const i8>) -> ptr<void>>;
// DEFAULT-NEXT:         field7 gl_lstat: ptr<fn(ptr<const i8>, ptr<@type7>) -> i32>;
// DEFAULT-NEXT:         field8 gl_stat: ptr<fn(ptr<const i8>, ptr<@type8>) -> i32>;
// DEFAULT-NEXT:     } [size=72, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64]];
// DEFAULT-NEXT:     type @type6 dirent = struct incomplete;
// DEFAULT-NEXT:     type @type7 stat = struct incomplete;
// DEFAULT-NEXT:     type @type8 stat = struct incomplete;
// DEFAULT-NEXT:     type @type9 glob_t = @type5;
// DEFAULT-NEXT:     type @type10 __uint32_t = u32;
// DEFAULT-NEXT:     type @type11 __pid_t = i32;
// DEFAULT-NEXT:     type @type12 __time_t = i64;
// DEFAULT-NEXT:     type @type13 pid_t = i32;
// DEFAULT-NEXT:     type @type14 time_t = i64;
// DEFAULT-NEXT:     type @type15 __re_size_t = u32;
// DEFAULT-NEXT:     type @type16 __re_long_size_t = u64;
// DEFAULT-NEXT:     type @type17 reg_syntax_t = u64;
// DEFAULT-NEXT:     type @type18 re_pattern_buffer = struct {
// DEFAULT-NEXT:         field0 buffer: ptr<@type19>;
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
// DEFAULT-NEXT:     type @type19 re_dfa_t = struct incomplete;
// DEFAULT-NEXT:     type @type20 regex_t = @type18;
// DEFAULT-NEXT:     type @type21 regoff_t = i32;
// DEFAULT-NEXT:     type @type22 re_registers = struct {
// DEFAULT-NEXT:         field0 num_regs: u32;
// DEFAULT-NEXT:         field1 start: ptr<i32>;
// DEFAULT-NEXT:         field2 end: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type23 tm = struct {
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
// DEFAULT-NEXT:     type @type24 = enum : u32 {
// DEFAULT-NEXT:         %0 _SC_ARG_MAX = const<i32>(0);
// DEFAULT-NEXT:         %1 _SC_CHILD_MAX = const<i32>(1);
// DEFAULT-NEXT:         %2 _SC_CLK_TCK = const<i32>(2);
// DEFAULT-NEXT:         %3 _SC_NGROUPS_MAX = const<i32>(3);
// DEFAULT-NEXT:         %4 _SC_OPEN_MAX = const<i32>(4);
// DEFAULT-NEXT:         %5 _SC_STREAM_MAX = const<i32>(5);
// DEFAULT-NEXT:         %6 _SC_TZNAME_MAX = const<i32>(6);
// DEFAULT-NEXT:         %7 _SC_JOB_CONTROL = const<i32>(7);
// DEFAULT-NEXT:         %8 _SC_SAVED_IDS = const<i32>(8);
// DEFAULT-NEXT:         %9 _SC_REALTIME_SIGNALS = const<i32>(9);
// DEFAULT-NEXT:         %10 _SC_PRIORITY_SCHEDULING = const<i32>(10);
// DEFAULT-NEXT:         %11 _SC_TIMERS = const<i32>(11);
// DEFAULT-NEXT:         %12 _SC_ASYNCHRONOUS_IO = const<i32>(12);
// DEFAULT-NEXT:         %13 _SC_PRIORITIZED_IO = const<i32>(13);
// DEFAULT-NEXT:         %14 _SC_SYNCHRONIZED_IO = const<i32>(14);
// DEFAULT-NEXT:         %15 _SC_FSYNC = const<i32>(15);
// DEFAULT-NEXT:         %16 _SC_MAPPED_FILES = const<i32>(16);
// DEFAULT-NEXT:         %17 _SC_MEMLOCK = const<i32>(17);
// DEFAULT-NEXT:         %18 _SC_MEMLOCK_RANGE = const<i32>(18);
// DEFAULT-NEXT:         %19 _SC_MEMORY_PROTECTION = const<i32>(19);
// DEFAULT-NEXT:         %20 _SC_MESSAGE_PASSING = const<i32>(20);
// DEFAULT-NEXT:         %21 _SC_SEMAPHORES = const<i32>(21);
// DEFAULT-NEXT:         %22 _SC_SHARED_MEMORY_OBJECTS = const<i32>(22);
// DEFAULT-NEXT:         %23 _SC_AIO_LISTIO_MAX = const<i32>(23);
// DEFAULT-NEXT:         %24 _SC_AIO_MAX = const<i32>(24);
// DEFAULT-NEXT:         %25 _SC_AIO_PRIO_DELTA_MAX = const<i32>(25);
// DEFAULT-NEXT:         %26 _SC_DELAYTIMER_MAX = const<i32>(26);
// DEFAULT-NEXT:         %27 _SC_MQ_OPEN_MAX = const<i32>(27);
// DEFAULT-NEXT:         %28 _SC_MQ_PRIO_MAX = const<i32>(28);
// DEFAULT-NEXT:         %29 _SC_VERSION = const<i32>(29);
// DEFAULT-NEXT:         %30 _SC_PAGESIZE = const<i32>(30);
// DEFAULT-NEXT:         %31 _SC_RTSIG_MAX = const<i32>(31);
// DEFAULT-NEXT:         %32 _SC_SEM_NSEMS_MAX = const<i32>(32);
// DEFAULT-NEXT:         %33 _SC_SEM_VALUE_MAX = const<i32>(33);
// DEFAULT-NEXT:         %34 _SC_SIGQUEUE_MAX = const<i32>(34);
// DEFAULT-NEXT:         %35 _SC_TIMER_MAX = const<i32>(35);
// DEFAULT-NEXT:         %36 _SC_BC_BASE_MAX = const<i32>(36);
// DEFAULT-NEXT:         %37 _SC_BC_DIM_MAX = const<i32>(37);
// DEFAULT-NEXT:         %38 _SC_BC_SCALE_MAX = const<i32>(38);
// DEFAULT-NEXT:         %39 _SC_BC_STRING_MAX = const<i32>(39);
// DEFAULT-NEXT:         %40 _SC_COLL_WEIGHTS_MAX = const<i32>(40);
// DEFAULT-NEXT:         %41 _SC_EQUIV_CLASS_MAX = const<i32>(41);
// DEFAULT-NEXT:         %42 _SC_EXPR_NEST_MAX = const<i32>(42);
// DEFAULT-NEXT:         %43 _SC_LINE_MAX = const<i32>(43);
// DEFAULT-NEXT:         %44 _SC_RE_DUP_MAX = const<i32>(44);
// DEFAULT-NEXT:         %45 _SC_CHARCLASS_NAME_MAX = const<i32>(45);
// DEFAULT-NEXT:         %46 _SC_2_VERSION = const<i32>(46);
// DEFAULT-NEXT:         %47 _SC_2_C_BIND = const<i32>(47);
// DEFAULT-NEXT:         %48 _SC_2_C_DEV = const<i32>(48);
// DEFAULT-NEXT:         %49 _SC_2_FORT_DEV = const<i32>(49);
// DEFAULT-NEXT:         %50 _SC_2_FORT_RUN = const<i32>(50);
// DEFAULT-NEXT:         %51 _SC_2_SW_DEV = const<i32>(51);
// DEFAULT-NEXT:         %52 _SC_2_LOCALEDEF = const<i32>(52);
// DEFAULT-NEXT:         %53 _SC_PII = const<i32>(53);
// DEFAULT-NEXT:         %54 _SC_PII_XTI = const<i32>(54);
// DEFAULT-NEXT:         %55 _SC_PII_SOCKET = const<i32>(55);
// DEFAULT-NEXT:         %56 _SC_PII_INTERNET = const<i32>(56);
// DEFAULT-NEXT:         %57 _SC_PII_OSI = const<i32>(57);
// DEFAULT-NEXT:         %58 _SC_POLL = const<i32>(58);
// DEFAULT-NEXT:         %59 _SC_SELECT = const<i32>(59);
// DEFAULT-NEXT:         %60 _SC_UIO_MAXIOV = const<i32>(60);
// DEFAULT-NEXT:         %61 _SC_IOV_MAX = const<i32>(60);
// DEFAULT-NEXT:         %62 _SC_PII_INTERNET_STREAM = const<i32>(61);
// DEFAULT-NEXT:         %63 _SC_PII_INTERNET_DGRAM = const<i32>(62);
// DEFAULT-NEXT:         %64 _SC_PII_OSI_COTS = const<i32>(63);
// DEFAULT-NEXT:         %65 _SC_PII_OSI_CLTS = const<i32>(64);
// DEFAULT-NEXT:         %66 _SC_PII_OSI_M = const<i32>(65);
// DEFAULT-NEXT:         %67 _SC_T_IOV_MAX = const<i32>(66);
// DEFAULT-NEXT:         %68 _SC_THREADS = const<i32>(67);
// DEFAULT-NEXT:         %69 _SC_THREAD_SAFE_FUNCTIONS = const<i32>(68);
// DEFAULT-NEXT:         %70 _SC_GETGR_R_SIZE_MAX = const<i32>(69);
// DEFAULT-NEXT:         %71 _SC_GETPW_R_SIZE_MAX = const<i32>(70);
// DEFAULT-NEXT:         %72 _SC_LOGIN_NAME_MAX = const<i32>(71);
// DEFAULT-NEXT:         %73 _SC_TTY_NAME_MAX = const<i32>(72);
// DEFAULT-NEXT:         %74 _SC_THREAD_DESTRUCTOR_ITERATIONS = const<i32>(73);
// DEFAULT-NEXT:         %75 _SC_THREAD_KEYS_MAX = const<i32>(74);
// DEFAULT-NEXT:         %76 _SC_THREAD_STACK_MIN = const<i32>(75);
// DEFAULT-NEXT:         %77 _SC_THREAD_THREADS_MAX = const<i32>(76);
// DEFAULT-NEXT:         %78 _SC_THREAD_ATTR_STACKADDR = const<i32>(77);
// DEFAULT-NEXT:         %79 _SC_THREAD_ATTR_STACKSIZE = const<i32>(78);
// DEFAULT-NEXT:         %80 _SC_THREAD_PRIORITY_SCHEDULING = const<i32>(79);
// DEFAULT-NEXT:         %81 _SC_THREAD_PRIO_INHERIT = const<i32>(80);
// DEFAULT-NEXT:         %82 _SC_THREAD_PRIO_PROTECT = const<i32>(81);
// DEFAULT-NEXT:         %83 _SC_THREAD_PROCESS_SHARED = const<i32>(82);
// DEFAULT-NEXT:         %84 _SC_NPROCESSORS_CONF = const<i32>(83);
// DEFAULT-NEXT:         %85 _SC_NPROCESSORS_ONLN = const<i32>(84);
// DEFAULT-NEXT:         %86 _SC_PHYS_PAGES = const<i32>(85);
// DEFAULT-NEXT:         %87 _SC_AVPHYS_PAGES = const<i32>(86);
// DEFAULT-NEXT:         %88 _SC_ATEXIT_MAX = const<i32>(87);
// DEFAULT-NEXT:         %89 _SC_PASS_MAX = const<i32>(88);
// DEFAULT-NEXT:         %90 _SC_XOPEN_VERSION = const<i32>(89);
// DEFAULT-NEXT:         %91 _SC_XOPEN_XCU_VERSION = const<i32>(90);
// DEFAULT-NEXT:         %92 _SC_XOPEN_UNIX = const<i32>(91);
// DEFAULT-NEXT:         %93 _SC_XOPEN_CRYPT = const<i32>(92);
// DEFAULT-NEXT:         %94 _SC_XOPEN_ENH_I18N = const<i32>(93);
// DEFAULT-NEXT:         %95 _SC_XOPEN_SHM = const<i32>(94);
// DEFAULT-NEXT:         %96 _SC_2_CHAR_TERM = const<i32>(95);
// DEFAULT-NEXT:         %97 _SC_2_C_VERSION = const<i32>(96);
// DEFAULT-NEXT:         %98 _SC_2_UPE = const<i32>(97);
// DEFAULT-NEXT:         %99 _SC_XOPEN_XPG2 = const<i32>(98);
// DEFAULT-NEXT:         %100 _SC_XOPEN_XPG3 = const<i32>(99);
// DEFAULT-NEXT:         %101 _SC_XOPEN_XPG4 = const<i32>(100);
// DEFAULT-NEXT:         %102 _SC_CHAR_BIT = const<i32>(101);
// DEFAULT-NEXT:         %103 _SC_CHAR_MAX = const<i32>(102);
// DEFAULT-NEXT:         %104 _SC_CHAR_MIN = const<i32>(103);
// DEFAULT-NEXT:         %105 _SC_INT_MAX = const<i32>(104);
// DEFAULT-NEXT:         %106 _SC_INT_MIN = const<i32>(105);
// DEFAULT-NEXT:         %107 _SC_LONG_BIT = const<i32>(106);
// DEFAULT-NEXT:         %108 _SC_WORD_BIT = const<i32>(107);
// DEFAULT-NEXT:         %109 _SC_MB_LEN_MAX = const<i32>(108);
// DEFAULT-NEXT:         %110 _SC_NZERO = const<i32>(109);
// DEFAULT-NEXT:         %111 _SC_SSIZE_MAX = const<i32>(110);
// DEFAULT-NEXT:         %112 _SC_SCHAR_MAX = const<i32>(111);
// DEFAULT-NEXT:         %113 _SC_SCHAR_MIN = const<i32>(112);
// DEFAULT-NEXT:         %114 _SC_SHRT_MAX = const<i32>(113);
// DEFAULT-NEXT:         %115 _SC_SHRT_MIN = const<i32>(114);
// DEFAULT-NEXT:         %116 _SC_UCHAR_MAX = const<i32>(115);
// DEFAULT-NEXT:         %117 _SC_UINT_MAX = const<i32>(116);
// DEFAULT-NEXT:         %118 _SC_ULONG_MAX = const<i32>(117);
// DEFAULT-NEXT:         %119 _SC_USHRT_MAX = const<i32>(118);
// DEFAULT-NEXT:         %120 _SC_NL_ARGMAX = const<i32>(119);
// DEFAULT-NEXT:         %121 _SC_NL_LANGMAX = const<i32>(120);
// DEFAULT-NEXT:         %122 _SC_NL_MSGMAX = const<i32>(121);
// DEFAULT-NEXT:         %123 _SC_NL_NMAX = const<i32>(122);
// DEFAULT-NEXT:         %124 _SC_NL_SETMAX = const<i32>(123);
// DEFAULT-NEXT:         %125 _SC_NL_TEXTMAX = const<i32>(124);
// DEFAULT-NEXT:         %126 _SC_XBS5_ILP32_OFF32 = const<i32>(125);
// DEFAULT-NEXT:         %127 _SC_XBS5_ILP32_OFFBIG = const<i32>(126);
// DEFAULT-NEXT:         %128 _SC_XBS5_LP64_OFF64 = const<i32>(127);
// DEFAULT-NEXT:         %129 _SC_XBS5_LPBIG_OFFBIG = const<i32>(128);
// DEFAULT-NEXT:         %130 _SC_XOPEN_LEGACY = const<i32>(129);
// DEFAULT-NEXT:         %131 _SC_XOPEN_REALTIME = const<i32>(130);
// DEFAULT-NEXT:         %132 _SC_XOPEN_REALTIME_THREADS = const<i32>(131);
// DEFAULT-NEXT:         %133 _SC_ADVISORY_INFO = const<i32>(132);
// DEFAULT-NEXT:         %134 _SC_BARRIERS = const<i32>(133);
// DEFAULT-NEXT:         %135 _SC_BASE = const<i32>(134);
// DEFAULT-NEXT:         %136 _SC_C_LANG_SUPPORT = const<i32>(135);
// DEFAULT-NEXT:         %137 _SC_C_LANG_SUPPORT_R = const<i32>(136);
// DEFAULT-NEXT:         %138 _SC_CLOCK_SELECTION = const<i32>(137);
// DEFAULT-NEXT:         %139 _SC_CPUTIME = const<i32>(138);
// DEFAULT-NEXT:         %140 _SC_THREAD_CPUTIME = const<i32>(139);
// DEFAULT-NEXT:         %141 _SC_DEVICE_IO = const<i32>(140);
// DEFAULT-NEXT:         %142 _SC_DEVICE_SPECIFIC = const<i32>(141);
// DEFAULT-NEXT:         %143 _SC_DEVICE_SPECIFIC_R = const<i32>(142);
// DEFAULT-NEXT:         %144 _SC_FD_MGMT = const<i32>(143);
// DEFAULT-NEXT:         %145 _SC_FIFO = const<i32>(144);
// DEFAULT-NEXT:         %146 _SC_PIPE = const<i32>(145);
// DEFAULT-NEXT:         %147 _SC_FILE_ATTRIBUTES = const<i32>(146);
// DEFAULT-NEXT:         %148 _SC_FILE_LOCKING = const<i32>(147);
// DEFAULT-NEXT:         %149 _SC_FILE_SYSTEM = const<i32>(148);
// DEFAULT-NEXT:         %150 _SC_MONOTONIC_CLOCK = const<i32>(149);
// DEFAULT-NEXT:         %151 _SC_MULTI_PROCESS = const<i32>(150);
// DEFAULT-NEXT:         %152 _SC_SINGLE_PROCESS = const<i32>(151);
// DEFAULT-NEXT:         %153 _SC_NETWORKING = const<i32>(152);
// DEFAULT-NEXT:         %154 _SC_READER_WRITER_LOCKS = const<i32>(153);
// DEFAULT-NEXT:         %155 _SC_SPIN_LOCKS = const<i32>(154);
// DEFAULT-NEXT:         %156 _SC_REGEXP = const<i32>(155);
// DEFAULT-NEXT:         %157 _SC_REGEX_VERSION = const<i32>(156);
// DEFAULT-NEXT:         %158 _SC_SHELL = const<i32>(157);
// DEFAULT-NEXT:         %159 _SC_SIGNALS = const<i32>(158);
// DEFAULT-NEXT:         %160 _SC_SPAWN = const<i32>(159);
// DEFAULT-NEXT:         %161 _SC_SPORADIC_SERVER = const<i32>(160);
// DEFAULT-NEXT:         %162 _SC_THREAD_SPORADIC_SERVER = const<i32>(161);
// DEFAULT-NEXT:         %163 _SC_SYSTEM_DATABASE = const<i32>(162);
// DEFAULT-NEXT:         %164 _SC_SYSTEM_DATABASE_R = const<i32>(163);
// DEFAULT-NEXT:         %165 _SC_TIMEOUTS = const<i32>(164);
// DEFAULT-NEXT:         %166 _SC_TYPED_MEMORY_OBJECTS = const<i32>(165);
// DEFAULT-NEXT:         %167 _SC_USER_GROUPS = const<i32>(166);
// DEFAULT-NEXT:         %168 _SC_USER_GROUPS_R = const<i32>(167);
// DEFAULT-NEXT:         %169 _SC_2_PBS = const<i32>(168);
// DEFAULT-NEXT:         %170 _SC_2_PBS_ACCOUNTING = const<i32>(169);
// DEFAULT-NEXT:         %171 _SC_2_PBS_LOCATE = const<i32>(170);
// DEFAULT-NEXT:         %172 _SC_2_PBS_MESSAGE = const<i32>(171);
// DEFAULT-NEXT:         %173 _SC_2_PBS_TRACK = const<i32>(172);
// DEFAULT-NEXT:         %174 _SC_SYMLOOP_MAX = const<i32>(173);
// DEFAULT-NEXT:         %175 _SC_STREAMS = const<i32>(174);
// DEFAULT-NEXT:         %176 _SC_2_PBS_CHECKPOINT = const<i32>(175);
// DEFAULT-NEXT:         %177 _SC_V6_ILP32_OFF32 = const<i32>(176);
// DEFAULT-NEXT:         %178 _SC_V6_ILP32_OFFBIG = const<i32>(177);
// DEFAULT-NEXT:         %179 _SC_V6_LP64_OFF64 = const<i32>(178);
// DEFAULT-NEXT:         %180 _SC_V6_LPBIG_OFFBIG = const<i32>(179);
// DEFAULT-NEXT:         %181 _SC_HOST_NAME_MAX = const<i32>(180);
// DEFAULT-NEXT:         %182 _SC_TRACE = const<i32>(181);
// DEFAULT-NEXT:         %183 _SC_TRACE_EVENT_FILTER = const<i32>(182);
// DEFAULT-NEXT:         %184 _SC_TRACE_INHERIT = const<i32>(183);
// DEFAULT-NEXT:         %185 _SC_TRACE_LOG = const<i32>(184);
// DEFAULT-NEXT:         %186 _SC_LEVEL1_ICACHE_SIZE = const<i32>(185);
// DEFAULT-NEXT:         %187 _SC_LEVEL1_ICACHE_ASSOC = const<i32>(186);
// DEFAULT-NEXT:         %188 _SC_LEVEL1_ICACHE_LINESIZE = const<i32>(187);
// DEFAULT-NEXT:         %189 _SC_LEVEL1_DCACHE_SIZE = const<i32>(188);
// DEFAULT-NEXT:         %190 _SC_LEVEL1_DCACHE_ASSOC = const<i32>(189);
// DEFAULT-NEXT:         %191 _SC_LEVEL1_DCACHE_LINESIZE = const<i32>(190);
// DEFAULT-NEXT:         %192 _SC_LEVEL2_CACHE_SIZE = const<i32>(191);
// DEFAULT-NEXT:         %193 _SC_LEVEL2_CACHE_ASSOC = const<i32>(192);
// DEFAULT-NEXT:         %194 _SC_LEVEL2_CACHE_LINESIZE = const<i32>(193);
// DEFAULT-NEXT:         %195 _SC_LEVEL3_CACHE_SIZE = const<i32>(194);
// DEFAULT-NEXT:         %196 _SC_LEVEL3_CACHE_ASSOC = const<i32>(195);
// DEFAULT-NEXT:         %197 _SC_LEVEL3_CACHE_LINESIZE = const<i32>(196);
// DEFAULT-NEXT:         %198 _SC_LEVEL4_CACHE_SIZE = const<i32>(197);
// DEFAULT-NEXT:         %199 _SC_LEVEL4_CACHE_ASSOC = const<i32>(198);
// DEFAULT-NEXT:         %200 _SC_LEVEL4_CACHE_LINESIZE = const<i32>(199);
// DEFAULT-NEXT:         %201 _SC_IPV6 = const<i32>(235);
// DEFAULT-NEXT:         %202 _SC_RAW_SOCKETS = const<i32>(236);
// DEFAULT-NEXT:         %203 _SC_V7_ILP32_OFF32 = const<i32>(237);
// DEFAULT-NEXT:         %204 _SC_V7_ILP32_OFFBIG = const<i32>(238);
// DEFAULT-NEXT:         %205 _SC_V7_LP64_OFF64 = const<i32>(239);
// DEFAULT-NEXT:         %206 _SC_V7_LPBIG_OFFBIG = const<i32>(240);
// DEFAULT-NEXT:         %207 _SC_SS_REPL_MAX = const<i32>(241);
// DEFAULT-NEXT:         %208 _SC_TRACE_EVENT_NAME_MAX = const<i32>(242);
// DEFAULT-NEXT:         %209 _SC_TRACE_NAME_MAX = const<i32>(243);
// DEFAULT-NEXT:         %210 _SC_TRACE_SYS_MAX = const<i32>(244);
// DEFAULT-NEXT:         %211 _SC_TRACE_USER_EVENT_MAX = const<i32>(245);
// DEFAULT-NEXT:         %212 _SC_XOPEN_STREAMS = const<i32>(246);
// DEFAULT-NEXT:         %213 _SC_THREAD_ROBUST_PRIO_INHERIT = const<i32>(247);
// DEFAULT-NEXT:         %214 _SC_THREAD_ROBUST_PRIO_PROTECT = const<i32>(248);
// DEFAULT-NEXT:         %215 _SC_MINSIGSTKSZ = const<i32>(249);
// DEFAULT-NEXT:         %216 _SC_SIGSTKSZ = const<i32>(250);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     extern %4 program_invocation_name: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %5 program_invocation_short_name: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %353 .str353: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %354 .str354: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %355 .str355: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %356 .str356: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %357 .str357: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %358 .str358: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([46, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %359 .str359: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %362 .str362: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %363 .str363: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([115, 108, 97, 116, 101, 45, 116, 114, 117, 110, 99, 97, 116, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %364 .str364: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %365 .str365: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([102, 105, 108, 101, 45, 43, 40, 111, 110, 101, 124, 116, 119, 111, 41, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %366 .str366: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 105, 108, 101, 45, 116, 119, 111, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %367 .str367: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([115, 108, 40, 97, 124, 101, 41, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %368 .str368: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %369 .str369: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([47, 100, 101, 118, 47, 123, 110, 117, 108, 108, 44, 122, 101, 114, 111, 125, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %370 .str370: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 110, 117, 108, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %371 .str371: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 122, 101, 114, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %372 .str372: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @dladdr(%303 __address: ptr<const void>, %304 __info: ptr<@type1>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @backtrace(%305 __array: ptr<ptr<void>>, %306 __size: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @fnmatch(%307 __pattern: ptr<const i8>, %308 __name: ptr<const i8>, %309 __flags: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @glob(%310 __pattern: ptr<const i8> [restrict], %311 __flags: i32, %312 __errfunc: ptr<fn(ptr<const i8>, i32) -> i32>, %313 __pglob: ptr<@type5> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @globfree(%314 __pglob: ptr<@type5>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @gnu_get_libc_version() -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %30 @re_set_syntax(%315 __syntax: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %31 @re_compile_pattern(%316 __pattern: ptr<const i8>, %317 __length: u64, %318 __buffer: ptr<@type18>) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %32 @re_match(%319 __buffer: ptr<@type18>, %320 __String: ptr<const i8>, %321 __length: i32, %322 __start: i32, %323 __regs: ptr<@type22>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %33 @regfree(%324 __preg: ptr<@type18>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %34 @printf(%325 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %35 @arc4random_uniform(%326 __upper_bound: u32) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %36 @free(%327 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %37 @secure_getenv(%328 __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %38 @setenv(%329 __name: ptr<const i8>, %330 __value: ptr<const i8>, %331 __replace: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %39 @unsetenv(%332 __name: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %40 @canonicalize_file_name(%333 __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %41 @memcpy(%334 __dest: ptr<void> [restrict], %335 __src: ptr<const void> [restrict], %336 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %42 @strcmp(%337 __s1: ptr<const i8>, %338 __s2: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %43 @strlen(%339 __s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %44 @strnlen(%340 __string: ptr<const i8>, %341 __maxlen: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %45 @getauxval(%342 __type: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %46 @getentropy(%343 __buffer: ptr<void>, %344 __length: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %47 @get_nprocs() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %48 @get_phys_pages() -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %50 @timegm(%345 __tp: ptr<@type23>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %51 @timelocal(%346 __tp: ptr<@type23>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %52 @getcwd(%347 __buf: ptr<i8>, %348 __size: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %53 @get_current_dir_name() -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %272 @sysconf(%349 __name: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %273 @syscall(%350 __sysno: i64, ...) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %274 @gettid() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %361 @__builtin_alloca(%360 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %275 @gnu_environment_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %276 directory: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %277 canonical: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %278 current: array<i8, 4096> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %279 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %373: i32 [synthetic] = read<i32>(%279);
// DEFAULT-NEXT:         let %374: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%373), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%38, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%353)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%354)), const<i32>(1)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%279, read<i32>(%374));
// DEFAULT-NEXT:         let %375: i32 [synthetic] = read<i32>(%279);
// DEFAULT-NEXT:         let %376: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%375), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%42, pointer_cast<ptr<const i8>, reason=arg>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%37, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%355)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%356))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%279, read<i32>(%376));
// DEFAULT-NEXT:         let %377: i32 [synthetic] = read<i32>(%279);
// DEFAULT-NEXT:         let %378: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%377), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%39, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%357))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%279, read<i32>(%378));
// DEFAULT-NEXT:         write<ptr<i8>>(%276, call<ptr<i8>, signature=fn() -> ptr<i8>>(%53));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn() -> ptr<i8>>(%53);
// DEFAULT-NEXT:         write<ptr<i8>>(%277, call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%358))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%40, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%358)));
// DEFAULT-NEXT:         let %379: i32 [synthetic] = read<i32>(%279);
// DEFAULT-NEXT:         let %380: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%379), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, u64) -> ptr<i8>>(%52, array_decay<ptr<i8>, length=Some(4096)>(%278), const<u64>(4096)), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%279, read<i32>(%380));
// DEFAULT-NEXT:         let %381: i32 [synthetic] = read<i32>(%279);
// DEFAULT-NEXT:         let %382: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%276), null<ptr<i8>>)
// DEFAULT-NEXT:             write<bool>(%382, eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%42, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%276)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4096)>(%278))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%382, const<bool>(false));
// DEFAULT-NEXT:         let %383: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%381), from_bool<i32, reason=promotion>(read<bool>(%382)));
// DEFAULT-NEXT:         write<i32>(%279, read<i32>(%383));
// DEFAULT-NEXT:         let %384: i32 [synthetic] = read<i32>(%279);
// DEFAULT-NEXT:         let %385: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%277), null<ptr<i8>>)
// DEFAULT-NEXT:             write<bool>(%385, eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%42, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%277)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4096)>(%278))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%385, const<bool>(false));
// DEFAULT-NEXT:         let %386: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%384), from_bool<i32, reason=promotion>(read<bool>(%385)));
// DEFAULT-NEXT:         write<i32>(%279, read<i32>(%386));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%36, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%276)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%36, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%277)));
// DEFAULT-NEXT:         let %387: i32 [synthetic] = read<i32>(%279);
// DEFAULT-NEXT:         let %388: ptr<i8> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %280 __old: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%359));
// DEFAULT-NEXT:             let %281 __len: u64 [storage=automatic] = add<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%43, read<ptr<const i8>>(%280)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %282 __new: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%361, read<u64>(%281)));
// DEFAULT-NEXT:             write<ptr<i8>>(%388, pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%41, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%282)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%280)), read<u64>(%281))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %389: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%387), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%42, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%388)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%362))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%279, read<i32>(%389));
// DEFAULT-NEXT:         let %390: i32 [synthetic] = read<i32>(%279);
// DEFAULT-NEXT:         let %391: ptr<i8> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %283 __old: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(16)>(%363));
// DEFAULT-NEXT:             let %284 __len: u64 [storage=automatic] = call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%44, read<ptr<const i8>>(%283), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:             let %285 __new: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%361, add<u64, overflow=wrap>(read<u64>(%284), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%285), read<u64>(%284))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<i8>>(%391, pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%41, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%285)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%283)), read<u64>(%284))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %392: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%390), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%42, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%391)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%364))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%279, read<i32>(%392));
// DEFAULT-NEXT:         return read<i32>(%279);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %286 @gnu_time_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %287 epoch: @type23 [storage=automatic] = aggregate<@type23, zero_fill=true>();
// DEFAULT-NEXT:         let %288 local: @type23 [storage=automatic] = aggregate<@type23, zero_fill=true>();
// DEFAULT-NEXT:         let %289 timestamp: i64 [storage=automatic];
// DEFAULT-NEXT:         let %290 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32>(field5(%287), const<i32>(70));
// DEFAULT-NEXT:         write<i32>(field4(%287), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field3(%287), const<i32>(1));
// DEFAULT-NEXT:         write<i64>(%289, call<i64, signature=fn(ptr<@type23>) -> i64>(%50, addr_of<ptr<@type23>>(%287)));
// DEFAULT-NEXT:         call<i64, signature=fn(ptr<@type23>) -> i64>(%50, addr_of<ptr<@type23>>(%287));
// DEFAULT-NEXT:         let %393: i32 [synthetic] = read<i32>(%290);
// DEFAULT-NEXT:         let %394: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%393), from_bool<i32, reason=promotion>(eq<i64>(read<i64>(%289), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%290, read<i32>(%394));
// DEFAULT-NEXT:         write<i32>(field5(%288), const<i32>(70));
// DEFAULT-NEXT:         write<i32>(field4(%288), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field3(%288), const<i32>(2));
// DEFAULT-NEXT:         let %395: i32 [synthetic] = read<i32>(%290);
// DEFAULT-NEXT:         let %396: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%395), from_bool<i32, reason=promotion>(ne<i64>(call<i64, signature=fn(ptr<@type23>) -> i64>(%51, addr_of<ptr<@type23>>(%288)), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         write<i32>(%290, read<i32>(%396));
// DEFAULT-NEXT:         return read<i32>(%290);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %291 @gnu_pattern_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %292 expression: @type18 [storage=automatic] = aggregate<@type18, zero_fill=true>();
// DEFAULT-NEXT:         let %293 paths: @type5 [storage=automatic] = aggregate<@type5, zero_fill=true>();
// DEFAULT-NEXT:         let %294 error: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %295 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %397: i32 [synthetic] = read<i32>(%295);
// DEFAULT-NEXT:         let %398: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%397), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%365)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%366)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%295, read<i32>(%398));
// DEFAULT-NEXT:         call<u64, signature=fn(u64) -> u64>(%30, or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         write<ptr<const i8>>(%294, call<ptr<const i8>, signature=fn(ptr<const i8>, u64, ptr<@type18>) -> ptr<const i8>>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%367)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))), addr_of<ptr<@type18>>(%292)));
// DEFAULT-NEXT:         call<ptr<const i8>, signature=fn(ptr<const i8>, u64, ptr<@type18>) -> ptr<const i8>>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%367)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))), addr_of<ptr<@type18>>(%292));
// DEFAULT-NEXT:         let %399: i32 [synthetic] = read<i32>(%295);
// DEFAULT-NEXT:         let %400: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%399), from_bool<i32, reason=promotion>(eq<ptr<const i8>>(read<ptr<const i8>>(%294), null<ptr<const i8>>)));
// DEFAULT-NEXT:         write<i32>(%295, read<i32>(%400));
// DEFAULT-NEXT:         let %401: i32 [synthetic] = read<i32>(%295);
// DEFAULT-NEXT:         let %402: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%401), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type18>, ptr<const i8>, i32, i32, ptr<@type22>) -> i32>(%32, addr_of<ptr<@type18>>(%292), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%368)), const<i32>(5), const<i32>(0), null<ptr<@type22>>), const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(%295, read<i32>(%402));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>) -> void>(%33, addr_of<ptr<@type18>>(%292));
// DEFAULT-NEXT:         let %403: i32 [synthetic] = read<i32>(%295);
// DEFAULT-NEXT:         let %404: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%403), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, i32, ptr<fn(ptr<const i8>, i32) -> i32>, ptr<@type5>) -> i32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%369)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(10)), null<ptr<fn(ptr<const i8>, i32) -> i32>>, addr_of<ptr<@type5>>(%293)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%295, read<i32>(%404));
// DEFAULT-NEXT:         let %405: i32 [synthetic] = read<i32>(%295);
// DEFAULT-NEXT:         let %406: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%405), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(field0(%293)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         write<i32>(%295, read<i32>(%406));
// DEFAULT-NEXT:         let %407: i32 [synthetic] = read<i32>(%295);
// DEFAULT-NEXT:         let %408: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%407), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%42, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(field1(%293)), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%370))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%295, read<i32>(%408));
// DEFAULT-NEXT:         let %409: i32 [synthetic] = read<i32>(%295);
// DEFAULT-NEXT:         let %410: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%409), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%42, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(field1(%293)), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%371))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%295, read<i32>(%410));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>) -> void>(%15, addr_of<ptr<@type5>>(%293));
// DEFAULT-NEXT:         return read<i32>(%295);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %296 @gnu_runtime_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %297 random_bytes: array<u8, 8> [storage=automatic];
// DEFAULT-NEXT:         let %298 frames: array<ptr<void>, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %299 information: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         let %300 page_size: i64 [storage=automatic] = call<i64, signature=fn(i32) -> i64>(%272, const<i32>(30));
// DEFAULT-NEXT:         let %301 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %411: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %412: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%411), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(u64) -> u64>(%45, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%300)))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%412));
// DEFAULT-NEXT:         let %413: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %414: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%413), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn() -> i32>(%274), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64, ...) -> i64>(%273, widen<i64, reason=arg>(const<i32>(186)))))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%414));
// DEFAULT-NEXT:         let %415: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %416: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%415), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<void>, u64) -> i32>(%46, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%297)), const<u64>(8)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%416));
// DEFAULT-NEXT:         let %417: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %418: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%417), from_bool<i32, reason=promotion>(eq<u32>(call<u32, signature=fn(u32) -> u32>(%35, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%418));
// DEFAULT-NEXT:         let %419: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %420: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%419), from_bool<i32, reason=promotion>(gt<i32>(call<i32, signature=fn() -> i32>(%47), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%420));
// DEFAULT-NEXT:         let %421: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %422: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%421), from_bool<i32, reason=promotion>(gt<i64>(call<i64, signature=fn() -> i64>(%48), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%422));
// DEFAULT-NEXT:         let %423: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %424: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%423), from_bool<i32, reason=promotion>(gt<i32>(call<i32, signature=fn(ptr<ptr<void>>, i32) -> i32>(%6, array_decay<ptr<ptr<void>>, length=Some(8)>(%298), const<i32>(8)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%424));
// DEFAULT-NEXT:         let %425: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %426: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%425), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<@type1>) -> i32>(%3, pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<fn() -> i32>>(%296))), addr_of<ptr<@type1>>(%299)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%426));
// DEFAULT-NEXT:         let %427: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %428: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%427), from_bool<i32, reason=promotion>(ne<ptr<const i8>>(read<ptr<const i8>>(field0(%299)), null<ptr<const i8>>)));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%428));
// DEFAULT-NEXT:         let %429: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %430: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%429), from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(call<ptr<const i8>, signature=fn() -> ptr<const i8>>(%16), const<i32>(0))))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%430));
// DEFAULT-NEXT:         let %431: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %432: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%431), from_bool<i32, reason=promotion>(ne<ptr<i8>>(read<ptr<i8>>(%4), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%432));
// DEFAULT-NEXT:         let %433: i32 [synthetic] = read<i32>(%301);
// DEFAULT-NEXT:         let %434: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%433), from_bool<i32, reason=promotion>(ne<ptr<i8>>(read<ptr<i8>>(%5), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%301, read<i32>(%434));
// DEFAULT-NEXT:         return read<i32>(%301);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %302 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%34, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%372)), call<i32, signature=fn() -> i32>(%275), call<i32, signature=fn() -> i32>(%286), add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%291), call<i32, signature=fn() -> i32>(%296)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
