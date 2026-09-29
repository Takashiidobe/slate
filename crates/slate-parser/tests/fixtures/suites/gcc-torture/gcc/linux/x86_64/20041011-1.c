void abort(void);
void exit(int);

typedef unsigned long long ull;
volatile int               gvol[32];
ull                        gull;

#define MULTI(X)                                                               \
  X(1), X(2), X(3), X(4), X(5), X(6), X(7), X(8), X(9), X(10), X(11), X(12),   \
      X(13), X(14), X(15), X(16), X(17), X(18), X(19), X(20), X(21), X(22),    \
      X(23), X(24), X(25), X(26), X(27), X(28), X(29), X(30)

#define DECLARE(INDEX) x##INDEX
#define COPYIN(INDEX)  x##INDEX = gvol[INDEX]
#define COPYOUT(INDEX) gvol[INDEX] = x##INDEX

#define BUILD_TEST(NAME, N)                                                    \
  ull __attribute__((noinline)) NAME(int n, ull x) {                           \
    while (n--) {                                                              \
      int MULTI(DECLARE);                                                      \
      MULTI(COPYIN);                                                           \
      MULTI(COPYOUT);                                                          \
      x += N;                                                                  \
    }                                                                          \
    return x;                                                                  \
  }

#define RUN_TEST(NAME, N)                                                      \
  if (NAME(3, ~0ULL) != N * 3 - 1)                                             \
    abort();                                                                   \
  if (NAME(3, 0xffffffffULL) != N * 3 + 0xffffffffULL)                         \
    abort();

#define DO_TESTS(DO_TEST)                                                      \
  DO_TEST(t1, -2048)                                                           \
  DO_TEST(t2, -513)                                                            \
  DO_TEST(t3, -512)                                                            \
  DO_TEST(t4, -511)                                                            \
  DO_TEST(t5, -1)                                                              \
  DO_TEST(t6, 1)                                                               \
  DO_TEST(t7, 511)                                                             \
  DO_TEST(t8, 512)                                                             \
  DO_TEST(t9, 513)                                                             \
  DO_TEST(t10, gull)                                                           \
  DO_TEST(t11, -gull)

DO_TESTS(BUILD_TEST)

ull neg(ull x) { return -x; }

int main() {
  gull = 100;
  DO_TESTS(RUN_TEST)
  if (neg(gull) != -100ULL)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_ull:[0-9]+]] ull = u64;
// DEFAULT-NEXT:     global %[[VALUE_gvol:[0-9]+]] gvol: volatile array<i32, 32> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gull:[0-9]+]] gull: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_t1:[0-9]+]] @t1(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_x:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE2]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30]]));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(2048)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x]], read<u64>(%[[VALUE5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t2:[0-9]+]] @t2(%[[VALUE_n_2:[0-9]+]] n: i32, %[[VALUE_x_2:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE6:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_2]]);
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_2]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE7]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_2:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_2:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_2:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_2:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_2:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_2:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_2:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_2:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_2:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_2:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_2:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_2:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_2:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_2:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_2:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_2:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_2:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_2:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_2:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_2:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_2:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_2:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_2:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_2:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_2:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_2:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_2:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_2:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_2:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_2:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_2]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_2]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_2]]));
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE9]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(513)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_2]], read<u64>(%[[VALUE10]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t3:[0-9]+]] @t3(%[[VALUE_n_3:[0-9]+]] n: i32, %[[VALUE_x_3:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE11:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_3]]);
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_3]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE12]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_3:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_3:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_3:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_3:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_3:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_3:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_3:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_3:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_3:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_3:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_3:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_3:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_3:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_3:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_3:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_3:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_3:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_3:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_3:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_3:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_3:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_3:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_3:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_3:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_3:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_3:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_3:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_3:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_3:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_3:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_3]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_3]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_3]]));
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE14]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(512)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_3]], read<u64>(%[[VALUE15]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t4:[0-9]+]] @t4(%[[VALUE_n_4:[0-9]+]] n: i32, %[[VALUE_x_4:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE16:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_4]]);
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_4]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE17]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_4:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_4:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_4:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_4:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_4:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_4:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_4:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_4:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_4:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_4:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_4:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_4:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_4:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_4:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_4:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_4:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_4:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_4:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_4:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_4:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_4:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_4:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_4:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_4:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_4:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_4:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_4:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_4:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_4:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_4:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_4]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_4]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_4]]));
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE19]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(511)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_4]], read<u64>(%[[VALUE20]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t5:[0-9]+]] @t5(%[[VALUE_n_5:[0-9]+]] n: i32, %[[VALUE_x_5:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE21:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_5]]);
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_5]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE22]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_5:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_5:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_5:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_5:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_5:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_5:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_5:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_5:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_5:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_5:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_5:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_5:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_5:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_5:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_5:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_5:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_5:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_5:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_5:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_5:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_5:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_5:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_5:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_5:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_5:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_5:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_5:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_5:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_5:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_5:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_5]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_5]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_5]]));
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_5]]);
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE24]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_5]], read<u64>(%[[VALUE25]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t6:[0-9]+]] @t6(%[[VALUE_n_6:[0-9]+]] n: i32, %[[VALUE_x_6:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE26:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_6]]);
// DEFAULT-NEXT:             let %[[VALUE28:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_6]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE27]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_6:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_6:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_6:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_6:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_6:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_6:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_6:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_6:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_6:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_6:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_6:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_6:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_6:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_6:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_6:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_6:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_6:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_6:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_6:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_6:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_6:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_6:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_6:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_6:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_6:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_6:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_6:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_6:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_6:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_6:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_6]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_6]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_6]]));
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_6]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE29]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_6]], read<u64>(%[[VALUE30]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t7:[0-9]+]] @t7(%[[VALUE_n_7:[0-9]+]] n: i32, %[[VALUE_x_7:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE31:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_7]]);
// DEFAULT-NEXT:             let %[[VALUE33:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE32]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_7]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE32]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_7:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_7:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_7:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_7:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_7:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_7:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_7:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_7:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_7:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_7:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_7:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_7:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_7:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_7:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_7:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_7:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_7:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_7:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_7:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_7:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_7:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_7:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_7:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_7:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_7:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_7:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_7:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_7:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_7:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_7:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_7]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_7]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_7]]));
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_7]]);
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE34]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(511))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_7]], read<u64>(%[[VALUE35]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t8:[0-9]+]] @t8(%[[VALUE_n_8:[0-9]+]] n: i32, %[[VALUE_x_8:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE36:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_8]]);
// DEFAULT-NEXT:             let %[[VALUE38:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE37]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_8]], read<i32>(%[[VALUE38]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE37]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_8:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_8:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_8:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_8:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_8:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_8:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_8:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_8:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_8:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_8:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_8:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_8:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_8:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_8:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_8:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_8:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_8:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_8:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_8:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_8:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_8:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_8:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_8:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_8:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_8:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_8:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_8:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_8:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_8:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_8:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_8]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_8]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_8]]));
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_8]]);
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE39]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(512))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_8]], read<u64>(%[[VALUE40]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t9:[0-9]+]] @t9(%[[VALUE_n_9:[0-9]+]] n: i32, %[[VALUE_x_9:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE41:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_9]]);
// DEFAULT-NEXT:             let %[[VALUE43:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE42]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_9]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE42]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_9:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_9:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_9:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_9:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_9:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_9:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_9:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_9:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_9:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_9:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_9:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_9:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_9:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_9:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_9:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_9:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_9:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_9:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_9:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_9:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_9:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_9:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_9:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_9:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_9:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_9:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_9:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_9:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_9:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_9:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_9]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_9]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_9]]));
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_9]]);
// DEFAULT-NEXT:                 let %[[VALUE45:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE44]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(513))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_9]], read<u64>(%[[VALUE45]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t10:[0-9]+]] @t10(%[[VALUE_n_10:[0-9]+]] n: i32, %[[VALUE_x_10:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE46:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE47:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_10]]);
// DEFAULT-NEXT:             let %[[VALUE48:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE47]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_10]], read<i32>(%[[VALUE48]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE47]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_10:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_10:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_10:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_10:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_10:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_10:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_10:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_10:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_10:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_10:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_10:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_10:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_10:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_10:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_10:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_10:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_10:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_10:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_10:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_10:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_10:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_10:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_10:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_10:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_10:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_10:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_10:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_10:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_10:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_10:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_10]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_10]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_10]]));
// DEFAULT-NEXT:                 let %[[VALUE49:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_10]]);
// DEFAULT-NEXT:                 let %[[VALUE50:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE49]]), read<u64>(%[[VALUE_gull]]));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_10]], read<u64>(%[[VALUE50]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_10]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t11:[0-9]+]] @t11(%[[VALUE_n_11:[0-9]+]] n: i32, %[[VALUE_x_11:[0-9]+]] x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE51:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE52:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n_11]]);
// DEFAULT-NEXT:             let %[[VALUE53:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE52]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n_11]], read<i32>(%[[VALUE53]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE52]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x1_11:[0-9]+]] x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x2_11:[0-9]+]] x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x3_11:[0-9]+]] x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x4_11:[0-9]+]] x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x5_11:[0-9]+]] x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x6_11:[0-9]+]] x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x7_11:[0-9]+]] x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x8_11:[0-9]+]] x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x9_11:[0-9]+]] x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x10_11:[0-9]+]] x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x11_11:[0-9]+]] x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x12_11:[0-9]+]] x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x13_11:[0-9]+]] x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x14_11:[0-9]+]] x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x15_11:[0-9]+]] x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x16_11:[0-9]+]] x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x17_11:[0-9]+]] x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x18_11:[0-9]+]] x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x19_11:[0-9]+]] x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x20_11:[0-9]+]] x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x21_11:[0-9]+]] x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x22_11:[0-9]+]] x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x23_11:[0-9]+]] x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x24_11:[0-9]+]] x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x25_11:[0-9]+]] x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x26_11:[0-9]+]] x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x27_11:[0-9]+]] x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x28_11:[0-9]+]] x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x29_11:[0-9]+]] x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_x30_11:[0-9]+]] x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x1_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x2_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x3_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x4_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x5_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x6_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x7_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x8_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x9_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x10_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x11_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x12_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x13_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x14_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x15_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x16_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x17_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x18_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x19_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x20_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x21_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x22_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x23_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x24_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x25_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x26_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x27_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x28_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x29_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x30_11]], read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(1))), read<i32>(%[[VALUE_x1_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(2))), read<i32>(%[[VALUE_x2_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(3))), read<i32>(%[[VALUE_x3_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(4))), read<i32>(%[[VALUE_x4_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(5))), read<i32>(%[[VALUE_x5_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(6))), read<i32>(%[[VALUE_x6_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(7))), read<i32>(%[[VALUE_x7_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(8))), read<i32>(%[[VALUE_x8_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(9))), read<i32>(%[[VALUE_x9_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(10))), read<i32>(%[[VALUE_x10_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(11))), read<i32>(%[[VALUE_x11_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(12))), read<i32>(%[[VALUE_x12_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(13))), read<i32>(%[[VALUE_x13_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(14))), read<i32>(%[[VALUE_x14_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(15))), read<i32>(%[[VALUE_x15_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(16))), read<i32>(%[[VALUE_x16_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(17))), read<i32>(%[[VALUE_x17_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(18))), read<i32>(%[[VALUE_x18_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(19))), read<i32>(%[[VALUE_x19_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(20))), read<i32>(%[[VALUE_x20_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(21))), read<i32>(%[[VALUE_x21_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(22))), read<i32>(%[[VALUE_x22_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(23))), read<i32>(%[[VALUE_x23_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(24))), read<i32>(%[[VALUE_x24_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(25))), read<i32>(%[[VALUE_x25_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(26))), read<i32>(%[[VALUE_x26_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(27))), read<i32>(%[[VALUE_x27_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(28))), read<i32>(%[[VALUE_x28_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(29))), read<i32>(%[[VALUE_x29_11]]));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%[[VALUE_gvol]]), const<i32>(30))), read<i32>(%[[VALUE_x30_11]]));
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_x_11]]);
// DEFAULT-NEXT:                 let %[[VALUE55:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE54]]), neg<u64, overflow=wrap>(read<u64>(%[[VALUE_gull]])));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_x_11]], read<u64>(%[[VALUE55]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_x_11]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_neg:[0-9]+]] @neg(%[[VALUE_x_12:[0-9]+]] x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<u64, overflow=wrap>(read<u64>(%[[VALUE_x_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u64>(%[[VALUE_gull]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(100))));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t1]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2048)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t1]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2048)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t2]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(513)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t2]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(513)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t3]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(512)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t3]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(512)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t4]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(511)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t4]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(511)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t5]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t5]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t6]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(1), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t6]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(1), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t7]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(511), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t7]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(511), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t8]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(512), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t8]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(512), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t9]], const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(513), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t9]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(513), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t10]], const<i32>(3), not<u64>(const<u64>(0))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE_gull]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t10]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE_gull]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t11]], const<i32>(3), not<u64>(const<u64>(0))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(neg<u64, overflow=wrap>(read<u64>(%[[VALUE_gull]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%[[VALUE_t11]], const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(neg<u64, overflow=wrap>(read<u64>(%[[VALUE_gull]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE_neg]], read<u64>(%[[VALUE_gull]])), neg<u64, overflow=wrap>(const<u64>(100)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
