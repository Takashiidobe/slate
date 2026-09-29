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
// DEFAULT-NEXT:     type @type5 stat = struct incomplete;
// DEFAULT-NEXT:     type @type6 = struct {
// DEFAULT-NEXT:         field0 gl_pathc: u64;
// DEFAULT-NEXT:         field1 gl_pathv: ptr<ptr<i8>>;
// DEFAULT-NEXT:         field2 gl_offs: u64;
// DEFAULT-NEXT:         field3 gl_flags: i32;
// DEFAULT-NEXT:         field4 gl_closedir: ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:         field5 gl_readdir: ptr<fn(ptr<void>) -> ptr<@type7>>;
// DEFAULT-NEXT:         field6 gl_opendir: ptr<fn(ptr<const i8>) -> ptr<void>>;
// DEFAULT-NEXT:         field7 gl_lstat: ptr<fn(ptr<const i8>, ptr<@type5>) -> i32>;
// DEFAULT-NEXT:         field8 gl_stat: ptr<fn(ptr<const i8>, ptr<@type5>) -> i32>;
// DEFAULT-NEXT:     } [size=72, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64]];
// DEFAULT-NEXT:     type @type7 dirent = struct incomplete;
// DEFAULT-NEXT:     type @type8 glob_t = @type6;
// DEFAULT-NEXT:     type @type9 __uint32_t = u32;
// DEFAULT-NEXT:     type @type10 __pid_t = i32;
// DEFAULT-NEXT:     type @type11 __time_t = i64;
// DEFAULT-NEXT:     type @type12 pid_t = i32;
// DEFAULT-NEXT:     type @type13 time_t = i64;
// DEFAULT-NEXT:     type @type14 __re_size_t = u32;
// DEFAULT-NEXT:     type @type15 __re_long_size_t = u64;
// DEFAULT-NEXT:     type @type16 reg_syntax_t = u64;
// DEFAULT-NEXT:     type @type17 re_pattern_buffer = struct {
// DEFAULT-NEXT:         field0 buffer: ptr<@type18>;
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
// DEFAULT-NEXT:     type @type18 re_dfa_t = struct incomplete;
// DEFAULT-NEXT:     type @type19 regex_t = @type17;
// DEFAULT-NEXT:     type @type20 regoff_t = i32;
// DEFAULT-NEXT:     type @type21 re_registers = struct {
// DEFAULT-NEXT:         field0 num_regs: u32;
// DEFAULT-NEXT:         field1 start: ptr<i32>;
// DEFAULT-NEXT:         field2 end: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type22 tm = struct {
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
// DEFAULT-NEXT:     type @type23 = enum : u32 {
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
// DEFAULT-NEXT:     extern %6 program_invocation_name: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %7 program_invocation_short_name: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %402 .str402: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %403 .str403: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %404 .str404: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %405 .str405: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 101, 97, 100, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %406 .str406: array<i8, 21> [storage=static] = code_units<array<i8, 21>>([83, 76, 65, 84, 69, 95, 71, 78, 85, 95, 76, 73, 66, 67, 95, 86, 65, 76, 85, 69, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %407 .str407: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([46, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %408 .str408: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %411 .str411: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %412 .str412: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([115, 108, 97, 116, 101, 45, 116, 114, 117, 110, 99, 97, 116, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %413 .str413: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %414 .str414: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([102, 105, 108, 101, 45, 43, 40, 111, 110, 101, 124, 116, 119, 111, 41, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %415 .str415: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 105, 108, 101, 45, 116, 119, 111, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %416 .str416: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([115, 108, 40, 97, 124, 101, 41, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %417 .str417: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 108, 97, 116, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %418 .str418: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([47, 100, 101, 118, 47, 123, 110, 117, 108, 108, 44, 122, 101, 114, 111, 125, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %419 .str419: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 110, 117, 108, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %420 .str420: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([47, 100, 101, 118, 47, 122, 101, 114, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %421 .str421: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %5 @dladdr(%352 __address: ptr<const void>, %353 __info: ptr<@type1>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @backtrace(%354 __array: ptr<ptr<void>>, %355 __size: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @fnmatch(%356 __pattern: ptr<const i8>, %357 __name: ptr<const i8>, %358 __flags: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %24 @glob(%359 __pattern: ptr<const i8> [restrict], %360 __flags: i32, %361 __errfunc: ptr<fn(ptr<const i8>, i32) -> i32>, %362 __pglob: ptr<@type6> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @globfree(%363 __pglob: ptr<@type6>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %27 @gnu_get_libc_version() -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %42 @re_set_syntax(%364 __syntax: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %46 @re_compile_pattern(%365 __pattern: ptr<const i8>, %366 __length: u64, %367 __buffer: ptr<@type17>) -> ptr<const i8> [linkage=external];
// DEFAULT-NEXT:     fn %52 @re_match(%368 __buffer: ptr<@type17>, %369 __String: ptr<const i8>, %370 __length: i32, %371 __start: i32, %372 __regs: ptr<@type21>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %54 @regfree(%373 __preg: ptr<@type17>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %56 @printf(%374 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %58 @arc4random_uniform(%375 __upper_bound: u32) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %60 @free(%376 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %62 @secure_getenv(%377 __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %66 @setenv(%378 __name: ptr<const i8>, %379 __value: ptr<const i8>, %380 __replace: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %68 @unsetenv(%381 __name: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %70 @canonicalize_file_name(%382 __name: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %74 @memcpy(%383 __dest: ptr<void> [restrict], %384 __src: ptr<const void> [restrict], %385 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %77 @strcmp(%386 __s1: ptr<const i8>, %387 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %79 @strlen(%388 __s: ptr<const i8>) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %82 @strnlen(%389 __string: ptr<const i8>, %390 __maxlen: u64) -> u64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %84 @getauxval(%391 __type: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %87 @getentropy(%392 __buffer: ptr<void>, %393 __length: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %88 @get_nprocs() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %89 @get_phys_pages() -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %92 @timegm(%394 __tp: ptr<@type22>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %94 @timelocal(%395 __tp: ptr<@type22>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %97 @getcwd(%396 __buf: ptr<i8>, %397 __size: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %98 @get_current_dir_name() -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %318 @sysconf(%398 __name: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %320 @syscall(%399 __sysno: i64, ...) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %323 @gettid() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %410 @__builtin_alloca(%409 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %324 @gnu_environment_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %325 directory: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %326 canonical: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %327 current: array<i8, 4096> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %328 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %422: i32 [synthetic] = read<i32>(%328);
// DEFAULT-NEXT:         let %423: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%422), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%66, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%402)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%403)), const<i32>(1)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%328, read<i32>(%423));
// DEFAULT-NEXT:         let %424: i32 [synthetic] = read<i32>(%328);
// DEFAULT-NEXT:         let %425: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%424), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%77, pointer_cast<ptr<const i8>, reason=arg>(call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%62, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%404)))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%405))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%328, read<i32>(%425));
// DEFAULT-NEXT:         let %426: i32 [synthetic] = read<i32>(%328);
// DEFAULT-NEXT:         let %427: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%426), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%68, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(21)>(%406))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%328, read<i32>(%427));
// DEFAULT-NEXT:         write<ptr<i8>>(%325, call<ptr<i8>, signature=fn() -> ptr<i8>>(%98));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn() -> ptr<i8>>(%98);
// DEFAULT-NEXT:         write<ptr<i8>>(%326, call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%407))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%407)));
// DEFAULT-NEXT:         let %428: i32 [synthetic] = read<i32>(%328);
// DEFAULT-NEXT:         let %429: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%428), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, u64) -> ptr<i8>>(%97, array_decay<ptr<i8>, length=Some(4096)>(%327), const<u64>(4096)), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%328, read<i32>(%429));
// DEFAULT-NEXT:         let %430: i32 [synthetic] = read<i32>(%328);
// DEFAULT-NEXT:         let %431: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%430), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%325), null<ptr<i8>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%77, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%325)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4096)>(%327))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%328, read<i32>(%431));
// DEFAULT-NEXT:         let %432: i32 [synthetic] = read<i32>(%328);
// DEFAULT-NEXT:         let %433: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%432), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%326), null<ptr<i8>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%77, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%326)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4096)>(%327))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%328, read<i32>(%433));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%60, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%325)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%60, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%326)));
// DEFAULT-NEXT:         let %434: i32 [synthetic] = read<i32>(%328);
// DEFAULT-NEXT:         let %435: ptr<i8> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %329 __old: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%408));
// DEFAULT-NEXT:             let %330 __len: u64 [storage=automatic] = add<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%79, read<ptr<const i8>>(%329)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:             let %331 __new: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%410, read<u64>(%330)));
// DEFAULT-NEXT:             write<ptr<i8>>(%435, pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%74, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%331)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%329)), read<u64>(%330))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %436: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%434), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%77, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%435)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%411))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%328, read<i32>(%436));
// DEFAULT-NEXT:         let %437: i32 [synthetic] = read<i32>(%328);
// DEFAULT-NEXT:         let %438: ptr<i8> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %332 __old: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(16)>(%412));
// DEFAULT-NEXT:             let %333 __len: u64 [storage=automatic] = call<u64, signature=fn(ptr<const i8>, u64) -> u64>(%82, read<ptr<const i8>>(%332), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:             let %334 __new: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%410, add<u64, overflow=wrap>(read<u64>(%333), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%334), read<u64>(%333))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             write<ptr<i8>>(%438, pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%74, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%334)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%332)), read<u64>(%333))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %439: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%437), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%77, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%438)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%413))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%328, read<i32>(%439));
// DEFAULT-NEXT:         return read<i32>(%328);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %335 @gnu_time_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %336 epoch: @type22 [storage=automatic] = aggregate<@type22, zero_fill=true>();
// DEFAULT-NEXT:         let %337 local: @type22 [storage=automatic] = aggregate<@type22, zero_fill=true>();
// DEFAULT-NEXT:         let %338 timestamp: i64 [storage=automatic];
// DEFAULT-NEXT:         let %339 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32>(field5(%336), const<i32>(70));
// DEFAULT-NEXT:         write<i32>(field4(%336), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field3(%336), const<i32>(1));
// DEFAULT-NEXT:         write<i64>(%338, call<i64, signature=fn(ptr<@type22>) -> i64>(%92, addr_of<ptr<@type22>>(%336)));
// DEFAULT-NEXT:         call<i64, signature=fn(ptr<@type22>) -> i64>(%92, addr_of<ptr<@type22>>(%336));
// DEFAULT-NEXT:         let %440: i32 [synthetic] = read<i32>(%339);
// DEFAULT-NEXT:         let %441: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%440), from_bool<i32, reason=promotion>(eq<i64>(read<i64>(%338), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%339, read<i32>(%441));
// DEFAULT-NEXT:         write<i32>(field5(%337), const<i32>(70));
// DEFAULT-NEXT:         write<i32>(field4(%337), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field3(%337), const<i32>(2));
// DEFAULT-NEXT:         let %442: i32 [synthetic] = read<i32>(%339);
// DEFAULT-NEXT:         let %443: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%442), from_bool<i32, reason=promotion>(ne<i64>(call<i64, signature=fn(ptr<@type22>) -> i64>(%94, addr_of<ptr<@type22>>(%337)), widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         write<i32>(%339, read<i32>(%443));
// DEFAULT-NEXT:         return read<i32>(%339);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %340 @gnu_pattern_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %341 expression: @type17 [storage=automatic] = aggregate<@type17, zero_fill=true>();
// DEFAULT-NEXT:         let %342 paths: @type6 [storage=automatic] = aggregate<@type6, zero_fill=true>();
// DEFAULT-NEXT:         let %343 error: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %344 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %444: i32 [synthetic] = read<i32>(%344);
// DEFAULT-NEXT:         let %445: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%444), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, i32) -> i32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%414)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%415)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%344, read<i32>(%445));
// DEFAULT-NEXT:         call<u64, signature=fn(u64) -> u64>(%42, or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))), shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         write<ptr<const i8>>(%343, call<ptr<const i8>, signature=fn(ptr<const i8>, u64, ptr<@type17>) -> ptr<const i8>>(%46, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%416)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))), addr_of<ptr<@type17>>(%341)));
// DEFAULT-NEXT:         call<ptr<const i8>, signature=fn(ptr<const i8>, u64, ptr<@type17>) -> ptr<const i8>>(%46, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%416)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9))), addr_of<ptr<@type17>>(%341));
// DEFAULT-NEXT:         let %446: i32 [synthetic] = read<i32>(%344);
// DEFAULT-NEXT:         let %447: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%446), from_bool<i32, reason=promotion>(eq<ptr<const i8>>(read<ptr<const i8>>(%343), null<ptr<const i8>>)));
// DEFAULT-NEXT:         write<i32>(%344, read<i32>(%447));
// DEFAULT-NEXT:         let %448: i32 [synthetic] = read<i32>(%344);
// DEFAULT-NEXT:         let %449: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%448), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type17>, ptr<const i8>, i32, i32, ptr<@type21>) -> i32>(%52, addr_of<ptr<@type17>>(%341), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%417)), const<i32>(5), const<i32>(0), null<ptr<@type21>>), const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(%344, read<i32>(%449));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>) -> void>(%54, addr_of<ptr<@type17>>(%341));
// DEFAULT-NEXT:         let %450: i32 [synthetic] = read<i32>(%344);
// DEFAULT-NEXT:         let %451: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%450), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, i32, ptr<fn(ptr<const i8>, i32) -> i32>, ptr<@type6>) -> i32>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%418)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(10)), null<ptr<fn(ptr<const i8>, i32) -> i32>>, addr_of<ptr<@type6>>(%342)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%344, read<i32>(%451));
// DEFAULT-NEXT:         let %452: i32 [synthetic] = read<i32>(%344);
// DEFAULT-NEXT:         let %453: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%452), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(field0(%342)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         write<i32>(%344, read<i32>(%453));
// DEFAULT-NEXT:         let %454: i32 [synthetic] = read<i32>(%344);
// DEFAULT-NEXT:         let %455: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%454), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%77, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(field1(%342)), const<i32>(0))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%419))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%344, read<i32>(%455));
// DEFAULT-NEXT:         let %456: i32 [synthetic] = read<i32>(%344);
// DEFAULT-NEXT:         let %457: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%456), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%77, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(field1(%342)), const<i32>(1))))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%420))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%344, read<i32>(%457));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>) -> void>(%26, addr_of<ptr<@type6>>(%342));
// DEFAULT-NEXT:         return read<i32>(%344);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %345 @gnu_runtime_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %346 random_bytes: array<u8, 8> [storage=automatic];
// DEFAULT-NEXT:         let %347 frames: array<ptr<void>, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %348 information: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         let %349 page_size: i64 [storage=automatic] = call<i64, signature=fn(i32) -> i64>(%318, const<i32>(30));
// DEFAULT-NEXT:         let %350 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %458: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %459: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%458), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(u64) -> u64>(%84, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%349)))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%459));
// DEFAULT-NEXT:         let %460: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %461: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%460), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn() -> i32>(%323), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64, ...) -> i64>(%320, widen<i64, reason=arg>(const<i32>(186)))))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%461));
// DEFAULT-NEXT:         let %462: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %463: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%462), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<void>, u64) -> i32>(%87, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(8)>(%346)), const<u64>(8)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%463));
// DEFAULT-NEXT:         let %464: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %465: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%464), from_bool<i32, reason=promotion>(eq<u32>(call<u32, signature=fn(u32) -> u32>(%58, reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%465));
// DEFAULT-NEXT:         let %466: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %467: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%466), from_bool<i32, reason=promotion>(gt<i32>(call<i32, signature=fn() -> i32>(%88), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%467));
// DEFAULT-NEXT:         let %468: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %469: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%468), from_bool<i32, reason=promotion>(gt<i64>(call<i64, signature=fn() -> i64>(%89), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%469));
// DEFAULT-NEXT:         let %470: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %471: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%470), from_bool<i32, reason=promotion>(gt<i32>(call<i32, signature=fn(ptr<ptr<void>>, i32) -> i32>(%10, array_decay<ptr<ptr<void>>, length=Some(8)>(%347), const<i32>(8)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%471));
// DEFAULT-NEXT:         let %472: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %473: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%472), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<@type1>) -> i32>(%5, pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<fn() -> i32>>(%345))), addr_of<ptr<@type1>>(%348)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%473));
// DEFAULT-NEXT:         let %474: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %475: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%474), from_bool<i32, reason=promotion>(ne<ptr<const i8>>(read<ptr<const i8>>(field0(%348)), null<ptr<const i8>>)));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%475));
// DEFAULT-NEXT:         let %476: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %477: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%476), from_bool<i32, reason=promotion>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(call<ptr<const i8>, signature=fn() -> ptr<const i8>>(%27), const<i32>(0))))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%477));
// DEFAULT-NEXT:         let %478: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %479: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%478), from_bool<i32, reason=promotion>(ne<ptr<i8>>(read<ptr<i8>>(%6), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%479));
// DEFAULT-NEXT:         let %480: i32 [synthetic] = read<i32>(%350);
// DEFAULT-NEXT:         let %481: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%480), from_bool<i32, reason=promotion>(ne<ptr<i8>>(read<ptr<i8>>(%7), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%350, read<i32>(%481));
// DEFAULT-NEXT:         return read<i32>(%350);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %351 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%56, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%421)), call<i32, signature=fn() -> i32>(%324), call<i32, signature=fn() -> i32>(%335), add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%340), call<i32, signature=fn() -> i32>(%345)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
