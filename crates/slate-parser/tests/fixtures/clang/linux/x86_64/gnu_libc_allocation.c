#define _GNU_SOURCE
#define obstack_chunk_alloc malloc
#define obstack_chunk_free  free
#include <malloc.h>
#include <obstack.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int gnu_allocation_extensions(void) {
  int  *values    = reallocarray(NULL, 4, sizeof(*values));
  void *aligned   = memalign(64, 80);
  void *page      = valloc(1);
  void *rounded   = pvalloc(1);
  long  page_size = sysconf(_SC_PAGESIZE);
  int   total     = 0;

  for (int index = 0; index < 4; ++index) {
    values[index] = index + 1;
  }
  total += values[0] + values[1] + values[2] + values[3];
  total += malloc_usable_size(values) >= 4 * sizeof(*values);
  total += aligned != NULL && (uintptr_t)aligned % 64 == 0;
  total += page != NULL && (uintptr_t)page % (uintptr_t)page_size == 0;
  total += rounded != NULL && (uintptr_t)rounded % (uintptr_t)page_size == 0;
  total += malloc_usable_size(rounded) >= (size_t)page_size;
  total += mallopt(M_CHECK_ACTION, 1) != 0;

  free(values);
  free(aligned);
  free(page);
  free(rounded);
  return total;
}

static int gnu_obstack_extensions(void) {
  struct obstack storage;
  char          *first;
  char          *second;
  int            total = 0;

  obstack_init(&storage);
  first   = obstack_copy0(&storage, "gnu", 3);
  second  = obstack_copy0(&storage, "libc", 4);
  total  += strcmp(first, "gnu") == 0;
  total  += strcmp(second, "libc") == 0;
  total  += obstack_object_size(&storage) == 0;
  obstack_free(&storage, NULL);
  return total;
}

int main(void) {
  printf("%d %d\n", gnu_allocation_extensions(), gnu_obstack_extensions());
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
// DEFAULT-NEXT:     type @type1 _obstack_chunk = struct {
// DEFAULT-NEXT:         field0 limit: ptr<i8>;
// DEFAULT-NEXT:         field1 prev: ptr<@type1>;
// DEFAULT-NEXT:         field2 contents: array<i8, 4>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type2 obstack = struct {
// DEFAULT-NEXT:         field0 chunk_size: i64;
// DEFAULT-NEXT:         field1 chunk: ptr<@type1>;
// DEFAULT-NEXT:         field2 object_base: ptr<i8>;
// DEFAULT-NEXT:         field3 next_free: ptr<i8>;
// DEFAULT-NEXT:         field4 chunk_limit: ptr<i8>;
// DEFAULT-NEXT:         field5 temp: @type3;
// DEFAULT-NEXT:         field6 alignment_mask: i32;
// DEFAULT-NEXT:         field7 chunkfun: ptr<fn(ptr<void>, i64) -> ptr<@type1>>;
// DEFAULT-NEXT:         field8 freefun: ptr<fn(ptr<void>, ptr<@type1>) -> void>;
// DEFAULT-NEXT:         field9 extra_arg: ptr<void>;
// DEFAULT-NEXT:         field10 use_extra_arg: u32 : 1;
// DEFAULT-NEXT:         field11 maybe_empty_object: u32 : 1;
// DEFAULT-NEXT:         field12 alloc_failed: u32 : 1;
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 80, 80], bit_offsets=[None, None, None, None, None, None, None, None, None, None, Some(640), Some(641), Some(642)], bit_units=[(80, 1)], field_units=[None, None, None, None, None, None, None, None, None, None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type3 = union {
// DEFAULT-NEXT:         field0 tempint: i64;
// DEFAULT-NEXT:         field1 tempptr: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type4 uintptr_t = u64;
// DEFAULT-NEXT:     type @type5 = enum : u32 {
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
// DEFAULT-NEXT:     global %336 .str336: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %337 .str337: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 105, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %338 .str338: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([103, 110, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %339 .str339: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 105, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %340 .str340: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%296 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%297 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %8 @reallocarray(%298 __ptr: ptr<void>, %299 __nmemb: u64, %300 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %10 @free(%301 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @memalign(%302 __alignment: u64, %303 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @valloc(%304 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %17 @pvalloc(%305 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %20 @mallopt(%306 __param: i32, %307 __val: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %22 @malloc_usable_size(%308 __ptr: ptr<void>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %26 @memcpy(%309 __dest: ptr<void> [restrict], %310 __src: ptr<const void> [restrict], %311 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %29 @strcmp(%312 __s1: ptr<const i8>, %313 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %32 @index(%314 __s: ptr<const i8>, %315 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %36 @_obstack_newchunk(%316 <unnamed>: ptr<@type2>, %317 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %37 @_obstack_begin(%318 <unnamed>: ptr<@type2>, %319 <unnamed>: i32, %320 <unnamed>: i32, %321 <unnamed>: ptr<fn(i64) -> ptr<void>>, %322 <unnamed>: ptr<fn(ptr<void>) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %38 @obstack_free(%323 <unnamed>: ptr<@type2>, %324 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %268 @sysconf(%334 __name: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %269 @gnu_allocation_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %270 values: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>, u64, u64) -> ptr<void>>(%8, null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), const<u64>(4)));
// DEFAULT-NEXT:         let %271 aligned: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%13, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(80))));
// DEFAULT-NEXT:         let %272 page: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%15, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %273 rounded: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%17, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %274 page_size: i64 [storage=automatic] = call<i64, signature=fn(i32) -> i64>(%268, const<i32>(30));
// DEFAULT-NEXT:         let %275 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %335
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %276 index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%276), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %341: i32 [synthetic] = read<i32>(%276);
// DEFAULT-NEXT:                 let %342: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%341), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%276, read<i32>(%342));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%270), read<i32>(%276))), add<i32, overflow=ub>(read<i32>(%276), const<i32>(1)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %343: i32 [synthetic] = read<i32>(%275);
// DEFAULT-NEXT:         let %344: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%343), add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%270), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%270), const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%270), const<i32>(2))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%270), const<i32>(3))))));
// DEFAULT-NEXT:         write<i32>(%275, read<i32>(%344));
// DEFAULT-NEXT:         let %345: i32 [synthetic] = read<i32>(%275);
// DEFAULT-NEXT:         let %346: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%345), from_bool<i32, reason=promotion>(ge<u64>(call<u64, signature=fn(ptr<void>) -> u64>(%22, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%270))), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(4)))));
// DEFAULT-NEXT:         write<i32>(%275, read<i32>(%346));
// DEFAULT-NEXT:         let %347: i32 [synthetic] = read<i32>(%275);
// DEFAULT-NEXT:         let %348: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%347), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%271), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%271)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(64)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))));
// DEFAULT-NEXT:         write<i32>(%275, read<i32>(%348));
// DEFAULT-NEXT:         let %349: i32 [synthetic] = read<i32>(%275);
// DEFAULT-NEXT:         let %350: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%349), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%272), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%272)), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%274))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))));
// DEFAULT-NEXT:         write<i32>(%275, read<i32>(%350));
// DEFAULT-NEXT:         let %351: i32 [synthetic] = read<i32>(%275);
// DEFAULT-NEXT:         let %352: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%351), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%273), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%273)), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%274))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))));
// DEFAULT-NEXT:         write<i32>(%275, read<i32>(%352));
// DEFAULT-NEXT:         let %353: i32 [synthetic] = read<i32>(%275);
// DEFAULT-NEXT:         let %354: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%353), from_bool<i32, reason=promotion>(ge<u64>(call<u64, signature=fn(ptr<void>) -> u64>(%22, read<ptr<void>>(%273)), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%274)))));
// DEFAULT-NEXT:         write<i32>(%275, read<i32>(%354));
// DEFAULT-NEXT:         let %355: i32 [synthetic] = read<i32>(%275);
// DEFAULT-NEXT:         let %356: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%355), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%20, neg<i32, overflow=ub>(const<i32>(5)), const<i32>(1)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%275, read<i32>(%356));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%10, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%270)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%10, read<ptr<void>>(%271));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%10, read<ptr<void>>(%272));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%10, read<ptr<void>>(%273));
// DEFAULT-NEXT:         return read<i32>(%275);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %277 @gnu_obstack_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %278 storage: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %279 first: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %280 second: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %281 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type2>, i32, i32, ptr<fn(i64) -> ptr<void>>, ptr<fn(ptr<void>) -> void>) -> i32>(%37, addr_of<ptr<@type2>>(%278), const<i32>(0), const<i32>(0), pointer_cast<ptr<fn(i64) -> ptr<void>>, reason=explicit>(function_decay<ptr<fn(u64) -> ptr<void>>>(%4)), function_decay<ptr<fn(ptr<void>) -> void>>(%10));
// DEFAULT-NEXT:         let %357: ptr<void> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %282 __h: ptr<@type2> [storage=automatic] = addr_of<ptr<@type2>>(%278);
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %283 __o: ptr<@type2> [storage=automatic] = read<ptr<@type2>>(%282);
// DEFAULT-NEXT:                 let %284 __len: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:                 if gt<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%283)))), read<i32>(%284)), const<i32>(1)), read<ptr<i8>>(field4(deref(read<ptr<@type2>>(%283)))))
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type2>, i32) -> void>(%36, read<ptr<@type2>>(%283), add<i32, overflow=ub>(read<i32>(%284), const<i32>(1)));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%26, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%283))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%336)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%284))));
// DEFAULT-NEXT:                 let %358: ptr<@type2> [synthetic] = read<ptr<@type2>>(%283);
// DEFAULT-NEXT:                 let %359: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%358))));
// DEFAULT-NEXT:                 let %360: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%359), read<i32>(%284));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%358))), read<ptr<i8>>(%360));
// DEFAULT-NEXT:                 let %361: ptr<@type2> [synthetic] = read<ptr<@type2>>(%283);
// DEFAULT-NEXT:                 let %362: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%361))));
// DEFAULT-NEXT:                 let %363: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%362), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%361))), read<ptr<i8>>(%363));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%362)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %364: ptr<void> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %285 __o1: ptr<@type2> [storage=automatic] = read<ptr<@type2>>(%282);
// DEFAULT-NEXT:                 let %286 __value: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=explicit>(read<ptr<i8>>(field2(deref(read<ptr<@type2>>(%285)))));
// DEFAULT-NEXT:                 if eq<ptr<i8>>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%285)))), pointer_cast<ptr<i8>, reason=usual_arith>(read<ptr<void>>(%286)))
// DEFAULT-NEXT:                     write<u32>(bitfield11<unit=0, bytes=80..81, bits=1..2>(deref(read<ptr<@type2>>(%285))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%285))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(conditional<ptr<i8>>(lt<u64>(const<u64>(8), const<u64>(8)), read<ptr<i8>>(field2(deref(read<ptr<@type2>>(%285)))), null<ptr<i8>>), and<i64>(add<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%285)))), conditional<ptr<i8>>(lt<u64>(const<u64>(8), const<u64>(8)), read<ptr<i8>>(field2(deref(read<ptr<@type2>>(%285)))), null<ptr<i8>>)), widen<i64, reason=usual_arith>(read<i32>(field6(deref(read<ptr<@type2>>(%285)))))), widen<i64, reason=usual_arith>(not<i32>(read<i32>(field6(deref(read<ptr<@type2>>(%285)))))))));
// DEFAULT-NEXT:                 if gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%285)))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type1>>(field1(deref(read<ptr<@type2>>(%285)))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field4(deref(read<ptr<@type2>>(%285)))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type1>>(field1(deref(read<ptr<@type2>>(%285)))))))
// DEFAULT-NEXT:                     write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%285))), read<ptr<i8>>(field4(deref(read<ptr<@type2>>(%285)))));
// DEFAULT-NEXT:                 write<ptr<i8>>(field2(deref(read<ptr<@type2>>(%285))), read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%285)))));
// DEFAULT-NEXT:                 write<ptr<void>>(%364, read<ptr<void>>(%286));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<void>>(%357, read<ptr<void>>(%364));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<i8>>(%279, pointer_cast<ptr<i8>, reason=assign>(read<ptr<void>>(%357)));
// DEFAULT-NEXT:         let %365: ptr<void> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %287 __h: ptr<@type2> [storage=automatic] = addr_of<ptr<@type2>>(%278);
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %288 __o: ptr<@type2> [storage=automatic] = read<ptr<@type2>>(%287);
// DEFAULT-NEXT:                 let %289 __len: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:                 if gt<ptr<i8>>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%288)))), read<i32>(%289)), const<i32>(1)), read<ptr<i8>>(field4(deref(read<ptr<@type2>>(%288)))))
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type2>, i32) -> void>(%36, read<ptr<@type2>>(%288), add<i32, overflow=ub>(read<i32>(%289), const<i32>(1)));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%26, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%288))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%337)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%289))));
// DEFAULT-NEXT:                 let %366: ptr<@type2> [synthetic] = read<ptr<@type2>>(%288);
// DEFAULT-NEXT:                 let %367: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%366))));
// DEFAULT-NEXT:                 let %368: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%367), read<i32>(%289));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%366))), read<ptr<i8>>(%368));
// DEFAULT-NEXT:                 let %369: ptr<@type2> [synthetic] = read<ptr<@type2>>(%288);
// DEFAULT-NEXT:                 let %370: ptr<i8> [synthetic] = read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%369))));
// DEFAULT-NEXT:                 let %371: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%370), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%369))), read<ptr<i8>>(%371));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%370)), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %372: ptr<void> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %290 __o1: ptr<@type2> [storage=automatic] = read<ptr<@type2>>(%287);
// DEFAULT-NEXT:                 let %291 __value: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=explicit>(read<ptr<i8>>(field2(deref(read<ptr<@type2>>(%290)))));
// DEFAULT-NEXT:                 if eq<ptr<i8>>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%290)))), pointer_cast<ptr<i8>, reason=usual_arith>(read<ptr<void>>(%291)))
// DEFAULT-NEXT:                     write<u32>(bitfield11<unit=0, bytes=80..81, bits=1..2>(deref(read<ptr<@type2>>(%290))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%290))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(conditional<ptr<i8>>(lt<u64>(const<u64>(8), const<u64>(8)), read<ptr<i8>>(field2(deref(read<ptr<@type2>>(%290)))), null<ptr<i8>>), and<i64>(add<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%290)))), conditional<ptr<i8>>(lt<u64>(const<u64>(8), const<u64>(8)), read<ptr<i8>>(field2(deref(read<ptr<@type2>>(%290)))), null<ptr<i8>>)), widen<i64, reason=usual_arith>(read<i32>(field6(deref(read<ptr<@type2>>(%290)))))), widen<i64, reason=usual_arith>(not<i32>(read<i32>(field6(deref(read<ptr<@type2>>(%290)))))))));
// DEFAULT-NEXT:                 if gt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%290)))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type1>>(field1(deref(read<ptr<@type2>>(%290)))))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field4(deref(read<ptr<@type2>>(%290)))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type1>>(field1(deref(read<ptr<@type2>>(%290)))))))
// DEFAULT-NEXT:                     write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%290))), read<ptr<i8>>(field4(deref(read<ptr<@type2>>(%290)))));
// DEFAULT-NEXT:                 write<ptr<i8>>(field2(deref(read<ptr<@type2>>(%290))), read<ptr<i8>>(field3(deref(read<ptr<@type2>>(%290)))));
// DEFAULT-NEXT:                 write<ptr<void>>(%372, read<ptr<void>>(%291));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<void>>(%365, read<ptr<void>>(%372));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<i8>>(%280, pointer_cast<ptr<i8>, reason=assign>(read<ptr<void>>(%365)));
// DEFAULT-NEXT:         let %373: i32 [synthetic] = read<i32>(%281);
// DEFAULT-NEXT:         let %374: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%373), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%279)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%338))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%281, read<i32>(%374));
// DEFAULT-NEXT:         let %375: i32 [synthetic] = read<i32>(%281);
// DEFAULT-NEXT:         let %376: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%375), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%280)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%339))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%281, read<i32>(%376));
// DEFAULT-NEXT:         let %377: i32 [synthetic] = read<i32>(%281);
// DEFAULT-NEXT:         let %378: u32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %292 __o: ptr<const @type2> [storage=automatic] = pointer_cast<ptr<const @type2>, reason=assign>(addr_of<ptr<@type2>>(%278));
// DEFAULT-NEXT:             write<u32>(%378, reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(read<ptr<i8>>(field3(deref(read<ptr<const @type2>>(%292)))), read<ptr<i8>>(field2(deref(read<ptr<const @type2>>(%292))))))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %379: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%377), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%378), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%281, read<i32>(%379));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %293 __o: ptr<@type2> [storage=automatic] = addr_of<ptr<@type2>>(%278);
// DEFAULT-NEXT:             let %294 __obj: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:             if logical_and<bool>(gt<ptr<void>>(read<ptr<void>>(%294), pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type1>>(field1(deref(read<ptr<@type2>>(%293)))))), lt<ptr<void>>(read<ptr<void>>(%294), pointer_cast<ptr<void>, reason=explicit>(read<ptr<i8>>(field4(deref(read<ptr<@type2>>(%293)))))))
// DEFAULT-NEXT:                 write<ptr<i8>>(field2(deref(read<ptr<@type2>>(%293))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%294)));
// DEFAULT-NEXT:                 write<ptr<i8>>(field3(deref(read<ptr<@type2>>(%293))), pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%294)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type2>, ptr<void>) -> void>(%38, read<ptr<@type2>>(%293), read<ptr<void>>(%294));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%281);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %295 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%340)), call<i32, signature=fn() -> i32>(%269), call<i32, signature=fn() -> i32>(%277));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
