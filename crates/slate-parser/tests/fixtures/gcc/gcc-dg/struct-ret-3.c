/* PR middle-end/31309 */
/* Origin: Peeter Joot <peeterj@ca.ibm.com> */

/* { dg-do run { target *-*-linux* *-*-gnu* *-*-uclinux* } } */
/* { dg-add-options stack_size } */
 
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#if defined(STACK_SIZE) && (STACK_SIZE < 128*1024)
 #define CHUNK_SIZE 4096
#else
 #define CHUNK_SIZE 16384
#endif

unsigned long ossAlignX(unsigned long i, unsigned long X)
{
   return ((i + (X - 1)) & ~(unsigned long) (X - 1));
}

struct STRUCT_6_BYTES
{
   unsigned char slot[sizeof(unsigned short)];
   unsigned char page[sizeof(unsigned int)];
};

struct SQLU_DICT_INFO_0
{
   void *pBlah;
   char bSomeFlag1;
   char bSomeFlag2;
   struct STRUCT_6_BYTES dRID;
};

struct SQLU_DATAPART_0
{
   struct SQLU_DICT_INFO_0 *pDictRidderInfo;
};

struct XXX
{
   struct SQLU_DATAPART_0 *m_pDatapart;
};

struct STRUCT_6_BYTES INIT_6_BYTES_ZERO()
{
   struct STRUCT_6_BYTES ridOut = {{0,0}, {0,0,0,0}};
   return ridOut;
}

void Initialize(struct XXX *this, int iIndex)
{
   struct SQLU_DICT_INFO_0 *pDictRidderInfo
     = this->m_pDatapart[iIndex].pDictRidderInfo;
   pDictRidderInfo->bSomeFlag1 = 0;
   pDictRidderInfo->bSomeFlag2 = 0;
   pDictRidderInfo->dRID = INIT_6_BYTES_ZERO();
}

int main(void)
{
   int rc;

   struct stuff
   {
      char c0[CHUNK_SIZE-sizeof(struct XXX)];
      struct XXX o;
      char c1[CHUNK_SIZE*2-sizeof(struct SQLU_DATAPART_0)];
      struct SQLU_DATAPART_0 dp;
      char c2[CHUNK_SIZE*2-sizeof(struct SQLU_DICT_INFO_0)];
      struct SQLU_DICT_INFO_0 di;
      char c3[CHUNK_SIZE];
   };

   char buf[sizeof(struct stuff)+CHUNK_SIZE];
   struct stuff *u
     = (struct stuff *)ossAlignX((unsigned long)&buf[0], CHUNK_SIZE);

   /* This test assumes system memory page
      size of CHUNK_SIZE bytes or less.  */
   if (sysconf(_SC_PAGESIZE) > CHUNK_SIZE)
     return 0;

   memset(u, 1, sizeof(struct stuff));
   u->c1[0] = '\xAA';
   u->c2[0] = '\xBB';
   u->c3[0] = '\xCC';

   rc = mprotect(u->c1, CHUNK_SIZE, PROT_NONE);
   if (rc == -1)
      printf("mprotect:c1: %d: %d(%s)\n", rc, errno, strerror(errno));

   rc = mprotect(u->c2, CHUNK_SIZE, PROT_NONE);
   if (rc == -1)
      printf("mprotect:c2: %d: %d(%s)\n", rc, errno, strerror(errno));

   rc = mprotect(u->c3, CHUNK_SIZE, PROT_NONE);
   if (rc == -1)
      printf("mprotect:c3: %d: %d(%s)\n", rc, errno, strerror(errno));

   u->o.m_pDatapart = &u->dp;
   u->dp.pDictRidderInfo = &u->di;
   Initialize(&u->o, 0);

   mprotect(u->c1, CHUNK_SIZE, PROT_READ|PROT_WRITE);
   mprotect(u->c2, CHUNK_SIZE, PROT_READ|PROT_WRITE);
   mprotect(u->c3, CHUNK_SIZE, PROT_READ|PROT_WRITE);

   return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type1 = enum : u32 {
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
// DEFAULT-NEXT:     type @type2 STRUCT_6_BYTES = struct {
// DEFAULT-NEXT:         field0 slot: array<u8, 2>;
// DEFAULT-NEXT:         field1 page: array<u8, 4>;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type3 SQLU_DICT_INFO_0 = struct {
// DEFAULT-NEXT:         field0 pBlah: ptr<void>;
// DEFAULT-NEXT:         field1 bSomeFlag1: i8;
// DEFAULT-NEXT:         field2 bSomeFlag2: i8;
// DEFAULT-NEXT:         field3 dRID: @type2;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 9, 10]];
// DEFAULT-NEXT:     type @type4 SQLU_DATAPART_0 = struct {
// DEFAULT-NEXT:         field0 pDictRidderInfo: ptr<@type3>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type5 XXX = struct {
// DEFAULT-NEXT:         field0 m_pDatapart: ptr<@type4>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type6 stuff = struct {
// DEFAULT-NEXT:         field0 c0: array<i8, 16376>;
// DEFAULT-NEXT:         field1 o: @type5;
// DEFAULT-NEXT:         field2 c1: array<i8, 32760>;
// DEFAULT-NEXT:         field3 dp: @type4;
// DEFAULT-NEXT:         field4 c2: array<i8, 32752>;
// DEFAULT-NEXT:         field5 di: @type3;
// DEFAULT-NEXT:         field6 c3: array<i8, 16384>;
// DEFAULT-NEXT:     } [size=98304, align=8, offsets=[0, 16376, 16384, 49144, 49152, 81904, 81920]];
// DEFAULT-NEXT:     global %252 .str252: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([109, 112, 114, 111, 116, 101, 99, 116, 58, 99, 49, 58, 32, 37, 100, 58, 32, 37, 100, 40, 37, 115, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %253 .str253: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([109, 112, 114, 111, 116, 101, 99, 116, 58, 99, 50, 58, 32, 37, 100, 58, 32, 37, 100, 40, 37, 115, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %254 .str254: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([109, 112, 114, 111, 116, 101, 99, 116, 58, 99, 51, 58, 32, 37, 100, 58, 32, 37, 100, 40, 37, 115, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @mprotect(%243 __addr: ptr<void>, %244 __len: u64, %245 __prot: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @memset(%246 __s: ptr<void>, %247 __c: i32, %248 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @strerror(%249 __errnum: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %4 @printf(%250 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @__errno_location() -> ptr<i32> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %224 @sysconf(%251 __name: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %225 @ossAlignX(%226 i: u64, %227 X: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<u64>(add<u64, overflow=wrap>(read<u64>(%226), sub<u64, overflow=wrap>(read<u64>(%227), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), not<u64>(sub<u64, overflow=wrap>(read<u64>(%227), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %232 @INIT_6_BYTES_ZERO(unprototyped) -> @type2 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %233 ridOut: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = aggregate<array<u8, 2>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))), field1 = aggregate<array<u8, 4>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         return copy<@type2, reason=return>(read<@type2>(%233));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %234 @Initialize(%235 this: ptr<@type5>, %236 iIndex: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %237 pDictRidderInfo: ptr<@type3> [storage=automatic] = read<ptr<@type3>>(field0(deref(ptr_offset<ptr<@type4>, subtract=false, element=@type4, overflow=ub>(read<ptr<@type4>>(field0(deref(read<ptr<@type5>>(%235)))), read<i32>(%236)))));
// DEFAULT-NEXT:         write<i8>(field1(deref(read<ptr<@type3>>(%237))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(field2(deref(read<ptr<@type3>>(%237))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type2>(field3(deref(read<ptr<@type3>>(%237))), copy<@type2, reason=assign>(call<@type2, signature=fn(unprototyped) -> @type2, abi=sysv64() -> native_c>(%232)));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(unprototyped) -> @type2, abi=sysv64() -> native_c>(%232));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %238 @main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %239 rc: i32 [storage=automatic];
// DEFAULT-NEXT:         let %241 buf: array<i8, 114688> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %242 u: ptr<@type6> [storage=automatic] = int_to_ptr<ptr<@type6>, reason=explicit>(call<u64, signature=fn(u64, u64) -> u64>(%225, ptr_to_int<u64, reason=explicit>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(114688)>(%241), const<i32>(0))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384)))));
// DEFAULT-NEXT:         if gt<i64>(call<i64, signature=fn(i32) -> i64>(%224, const<i32>(30)), widen<i64, reason=usual_arith>(const<i32>(16384)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type6>>(%242)), const<i32>(1), const<u64>(98304));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32760)>(field2(deref(read<ptr<@type6>>(%242)))), const<i32>(0))), truncate<i8, reason=assign, fits=unknown>(const<i32>(-86)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32752)>(field4(deref(read<ptr<@type6>>(%242)))), const<i32>(0))), truncate<i8, reason=assign, fits=unknown>(const<i32>(-69)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16384)>(field6(deref(read<ptr<@type6>>(%242)))), const<i32>(0))), truncate<i8, reason=assign, fits=unknown>(const<i32>(-52)));
// DEFAULT-NEXT:         write<i32>(%239, call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32760)>(field2(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32760)>(field2(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%239), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%252)), read<i32>(%239), read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%5))), call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%3, read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%5)))));
// DEFAULT-NEXT:         write<i32>(%239, call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32752)>(field4(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32752)>(field4(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%239), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%253)), read<i32>(%239), read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%5))), call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%3, read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%5)))));
// DEFAULT-NEXT:         write<i32>(%239, call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16384)>(field6(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16384)>(field6(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%239), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%254)), read<i32>(%239), read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%5))), call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%3, read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%5)))));
// DEFAULT-NEXT:         write<ptr<@type4>>(field0(field1(deref(read<ptr<@type6>>(%242)))), addr_of<ptr<@type4>>(field3(deref(read<ptr<@type6>>(%242)))));
// DEFAULT-NEXT:         write<ptr<@type3>>(field0(field3(deref(read<ptr<@type6>>(%242)))), addr_of<ptr<@type3>>(field5(deref(read<ptr<@type6>>(%242)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%234, addr_of<ptr<@type5>>(field1(deref(read<ptr<@type6>>(%242)))), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32760)>(field2(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), or<i32>(const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(32752)>(field4(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), or<i32>(const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64, i32) -> i32>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(16384)>(field6(deref(read<ptr<@type6>>(%242))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16384))), or<i32>(const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
