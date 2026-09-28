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
// DEFAULT-NEXT:     type @type0 ull = u64;
// DEFAULT-NEXT:     global %3 gvol: volatile array<i32, 32> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %4 gull: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%371 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @t1(%6 n: i32, %7 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %372 {
// DEFAULT-NEXT:             let %383: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:             let %384: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%383), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%6, read<i32>(%384));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%383), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %8 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %9 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %10 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %11 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %12 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %13 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %14 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %15 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %16 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %17 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %18 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %19 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %20 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %21 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %22 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %23 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %24 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %25 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %26 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %27 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %28 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %29 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %30 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %31 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %32 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %33 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %34 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %35 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %36 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %37 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%8, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%22, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%24, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%25, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%26, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%27, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%28, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%29, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%30, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%31, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%32, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%33, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%34, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%35, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%36, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%37, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%8));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%9));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%10));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%11));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%12));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%13));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%14));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%15));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%16));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%17));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%18));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%19));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%20));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%21));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%22));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%23));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%24));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%25));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%26));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%27));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%28));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%29));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%30));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%31));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%32));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%33));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%34));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%35));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%36));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%37));
// DEFAULT-NEXT:                 let %385: u64 [synthetic] = read<u64>(%7);
// DEFAULT-NEXT:                 let %386: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%385), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(2048)))));
// DEFAULT-NEXT:                 write<u64>(%7, read<u64>(%386));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @t2(%39 n: i32, %40 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %373 {
// DEFAULT-NEXT:             let %387: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:             let %388: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%387), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%39, read<i32>(%388));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%387), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %41 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %42 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %43 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %44 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %45 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %46 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %47 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %48 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %49 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %50 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %51 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %52 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %53 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %54 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %55 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %56 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %57 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %58 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %59 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %60 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %61 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %62 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %63 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %64 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %65 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %66 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %67 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %68 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %69 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %70 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%41, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%42, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%43, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%44, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%45, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%46, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%47, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%48, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%49, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%50, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%51, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%52, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%53, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%54, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%55, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%56, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%57, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%58, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%59, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%60, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%61, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%62, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%63, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%64, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%65, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%66, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%67, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%68, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%69, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%70, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%41));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%42));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%43));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%44));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%45));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%46));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%47));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%48));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%49));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%50));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%51));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%52));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%53));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%54));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%55));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%56));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%57));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%58));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%59));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%60));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%61));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%62));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%63));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%64));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%65));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%66));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%67));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%68));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%69));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%70));
// DEFAULT-NEXT:                 let %389: u64 [synthetic] = read<u64>(%40);
// DEFAULT-NEXT:                 let %390: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%389), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(513)))));
// DEFAULT-NEXT:                 write<u64>(%40, read<u64>(%390));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%40);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @t3(%72 n: i32, %73 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %374 {
// DEFAULT-NEXT:             let %391: i32 [synthetic] = read<i32>(%72);
// DEFAULT-NEXT:             let %392: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%391), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%72, read<i32>(%392));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%391), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %74 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %75 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %76 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %77 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %78 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %79 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %80 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %81 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %82 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %83 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %84 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %85 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %86 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %87 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %88 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %89 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %90 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %91 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %92 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %93 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %94 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %95 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %96 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %97 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %98 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %99 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %100 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %101 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %102 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %103 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%74, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%76, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%77, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%78, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%79, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%80, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%81, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%82, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%83, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%84, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%85, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%86, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%87, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%88, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%89, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%90, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%91, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%92, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%93, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%94, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%95, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%96, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%99, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%100, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%101, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%102, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%103, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%74));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%75));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%76));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%77));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%78));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%79));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%80));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%81));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%82));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%83));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%84));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%85));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%86));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%87));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%88));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%89));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%90));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%91));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%92));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%93));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%94));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%95));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%96));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%97));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%98));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%99));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%100));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%101));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%102));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%103));
// DEFAULT-NEXT:                 let %393: u64 [synthetic] = read<u64>(%73);
// DEFAULT-NEXT:                 let %394: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%393), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(512)))));
// DEFAULT-NEXT:                 write<u64>(%73, read<u64>(%394));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%73);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @t4(%105 n: i32, %106 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %375 {
// DEFAULT-NEXT:             let %395: i32 [synthetic] = read<i32>(%105);
// DEFAULT-NEXT:             let %396: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%395), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%105, read<i32>(%396));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%395), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %107 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %108 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %109 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %110 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %111 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %112 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %113 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %114 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %115 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %116 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %117 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %118 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %119 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %120 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %121 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %122 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %123 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %124 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %125 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %126 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %127 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %128 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %129 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %130 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %131 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %132 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %133 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %134 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %135 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %136 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%107, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%108, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%109, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%110, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%111, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%112, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%113, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%114, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%115, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%116, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%117, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%118, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%119, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%120, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%121, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%122, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%123, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%124, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%125, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%126, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%127, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%128, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%129, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%130, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%131, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%132, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%133, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%134, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%135, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%136, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%107));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%108));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%109));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%110));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%111));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%112));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%113));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%114));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%115));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%116));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%117));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%118));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%119));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%120));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%121));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%122));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%123));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%124));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%125));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%126));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%127));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%128));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%129));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%130));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%131));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%132));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%133));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%134));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%135));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%136));
// DEFAULT-NEXT:                 let %397: u64 [synthetic] = read<u64>(%106);
// DEFAULT-NEXT:                 let %398: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%397), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(511)))));
// DEFAULT-NEXT:                 write<u64>(%106, read<u64>(%398));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%106);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %137 @t5(%138 n: i32, %139 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %376 {
// DEFAULT-NEXT:             let %399: i32 [synthetic] = read<i32>(%138);
// DEFAULT-NEXT:             let %400: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%399), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%138, read<i32>(%400));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%399), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %140 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %141 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %142 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %143 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %144 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %145 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %146 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %147 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %148 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %149 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %150 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %151 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %152 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %153 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %154 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %155 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %156 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %157 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %158 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %159 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %160 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %161 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %162 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %163 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %164 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %165 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %166 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %167 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %168 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %169 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%140, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%141, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%142, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%143, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%144, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%145, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%146, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%147, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%148, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%149, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%150, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%151, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%152, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%153, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%154, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%155, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%156, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%157, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%158, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%159, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%160, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%161, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%162, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%163, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%164, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%165, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%166, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%167, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%168, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%169, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%140));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%141));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%142));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%143));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%144));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%145));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%146));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%147));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%148));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%149));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%150));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%151));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%152));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%153));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%154));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%155));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%156));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%157));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%158));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%159));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%160));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%161));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%162));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%163));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%164));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%165));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%166));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%167));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%168));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%169));
// DEFAULT-NEXT:                 let %401: u64 [synthetic] = read<u64>(%139);
// DEFAULT-NEXT:                 let %402: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%401), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:                 write<u64>(%139, read<u64>(%402));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%139);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @t6(%171 n: i32, %172 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %377 {
// DEFAULT-NEXT:             let %403: i32 [synthetic] = read<i32>(%171);
// DEFAULT-NEXT:             let %404: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%403), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%171, read<i32>(%404));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%403), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %173 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %174 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %175 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %176 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %177 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %178 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %179 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %180 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %181 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %182 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %183 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %184 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %185 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %186 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %187 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %188 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %189 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %190 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %191 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %192 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %193 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %194 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %195 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %196 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %197 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %198 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %199 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %200 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %201 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %202 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%173, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%174, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%175, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%176, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%177, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%178, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%179, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%180, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%181, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%182, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%183, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%184, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%185, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%186, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%187, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%188, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%189, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%190, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%191, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%192, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%193, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%194, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%195, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%196, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%197, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%198, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%199, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%200, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%201, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%202, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%173));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%174));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%175));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%176));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%177));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%178));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%179));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%180));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%181));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%182));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%183));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%184));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%185));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%186));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%187));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%188));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%189));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%190));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%191));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%192));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%193));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%194));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%195));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%196));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%197));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%198));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%199));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%200));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%201));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%202));
// DEFAULT-NEXT:                 let %405: u64 [synthetic] = read<u64>(%172);
// DEFAULT-NEXT:                 let %406: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%405), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%172, read<u64>(%406));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%172);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %203 @t7(%204 n: i32, %205 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %378 {
// DEFAULT-NEXT:             let %407: i32 [synthetic] = read<i32>(%204);
// DEFAULT-NEXT:             let %408: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%407), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%204, read<i32>(%408));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%407), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %206 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %207 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %208 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %209 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %210 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %211 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %212 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %213 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %214 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %215 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %216 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %217 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %218 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %219 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %220 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %221 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %222 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %223 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %224 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %225 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %226 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %227 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %228 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %229 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %230 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %231 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %232 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %233 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %234 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %235 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%206, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%207, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%208, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%209, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%210, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%211, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%212, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%213, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%214, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%215, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%216, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%217, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%218, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%219, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%220, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%221, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%222, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%223, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%224, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%225, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%226, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%227, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%228, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%229, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%230, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%231, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%232, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%233, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%234, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%235, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%206));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%207));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%208));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%209));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%210));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%211));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%212));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%213));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%214));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%215));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%216));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%217));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%218));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%219));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%220));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%221));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%222));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%223));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%224));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%225));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%226));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%227));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%228));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%229));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%230));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%231));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%232));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%233));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%234));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%235));
// DEFAULT-NEXT:                 let %409: u64 [synthetic] = read<u64>(%205);
// DEFAULT-NEXT:                 let %410: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%409), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(511))));
// DEFAULT-NEXT:                 write<u64>(%205, read<u64>(%410));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%205);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %236 @t8(%237 n: i32, %238 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %379 {
// DEFAULT-NEXT:             let %411: i32 [synthetic] = read<i32>(%237);
// DEFAULT-NEXT:             let %412: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%411), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%237, read<i32>(%412));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%411), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %239 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %240 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %241 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %242 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %243 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %244 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %245 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %246 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %247 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %248 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %249 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %250 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %251 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %252 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %253 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %254 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %255 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %256 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %257 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %258 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %259 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %260 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %261 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %262 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %263 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %264 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %265 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %266 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %267 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %268 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%239, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%240, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%241, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%242, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%243, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%244, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%245, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%246, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%247, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%248, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%249, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%250, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%251, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%252, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%253, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%254, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%255, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%256, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%257, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%258, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%259, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%260, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%261, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%262, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%263, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%264, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%265, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%266, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%267, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%268, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%239));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%240));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%241));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%242));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%243));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%244));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%245));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%246));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%247));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%248));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%249));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%250));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%251));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%252));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%253));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%254));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%255));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%256));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%257));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%258));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%259));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%260));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%261));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%262));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%263));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%264));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%265));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%266));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%267));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%268));
// DEFAULT-NEXT:                 let %413: u64 [synthetic] = read<u64>(%238);
// DEFAULT-NEXT:                 let %414: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%413), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(512))));
// DEFAULT-NEXT:                 write<u64>(%238, read<u64>(%414));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%238);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %269 @t9(%270 n: i32, %271 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %380 {
// DEFAULT-NEXT:             let %415: i32 [synthetic] = read<i32>(%270);
// DEFAULT-NEXT:             let %416: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%415), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%270, read<i32>(%416));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%415), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %272 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %273 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %274 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %275 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %276 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %277 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %278 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %279 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %280 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %281 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %282 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %283 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %284 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %285 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %286 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %287 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %288 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %289 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %290 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %291 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %292 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %293 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %294 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %295 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %296 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %297 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %298 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %299 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %300 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %301 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%272, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%273, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%274, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%275, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%276, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%277, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%278, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%279, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%280, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%281, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%282, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%283, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%284, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%285, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%286, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%287, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%288, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%289, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%290, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%291, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%292, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%293, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%294, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%295, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%296, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%297, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%298, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%299, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%300, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%301, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%272));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%273));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%274));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%275));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%276));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%277));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%278));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%279));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%280));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%281));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%282));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%283));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%284));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%285));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%286));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%287));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%288));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%289));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%290));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%291));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%292));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%293));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%294));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%295));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%296));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%297));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%298));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%299));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%300));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%301));
// DEFAULT-NEXT:                 let %417: u64 [synthetic] = read<u64>(%271);
// DEFAULT-NEXT:                 let %418: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%417), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(513))));
// DEFAULT-NEXT:                 write<u64>(%271, read<u64>(%418));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%271);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %302 @t10(%303 n: i32, %304 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %381 {
// DEFAULT-NEXT:             let %419: i32 [synthetic] = read<i32>(%303);
// DEFAULT-NEXT:             let %420: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%419), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%303, read<i32>(%420));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%419), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %305 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %306 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %307 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %308 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %309 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %310 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %311 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %312 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %313 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %314 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %315 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %316 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %317 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %318 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %319 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %320 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %321 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %322 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %323 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %324 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %325 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %326 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %327 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %328 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %329 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %330 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %331 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %332 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %333 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %334 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%305, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%306, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%307, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%308, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%309, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%310, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%311, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%312, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%313, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%314, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%315, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%316, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%317, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%318, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%319, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%320, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%321, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%322, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%323, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%324, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%325, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%326, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%327, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%328, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%329, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%330, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%331, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%332, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%333, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%334, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%305));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%306));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%307));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%308));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%309));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%310));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%311));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%312));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%313));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%314));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%315));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%316));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%317));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%318));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%319));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%320));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%321));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%322));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%323));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%324));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%325));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%326));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%327));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%328));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%329));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%330));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%331));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%332));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%333));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%334));
// DEFAULT-NEXT:                 let %421: u64 [synthetic] = read<u64>(%304);
// DEFAULT-NEXT:                 let %422: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%421), read<u64>(%4));
// DEFAULT-NEXT:                 write<u64>(%304, read<u64>(%422));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%304);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %335 @t11(%336 n: i32, %337 x: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %382 {
// DEFAULT-NEXT:             let %423: i32 [synthetic] = read<i32>(%336);
// DEFAULT-NEXT:             let %424: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%423), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%336, read<i32>(%424));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%423), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %338 x1: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %339 x2: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %340 x3: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %341 x4: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %342 x5: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %343 x6: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %344 x7: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %345 x8: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %346 x9: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %347 x10: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %348 x11: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %349 x12: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %350 x13: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %351 x14: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %352 x15: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %353 x16: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %354 x17: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %355 x18: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %356 x19: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %357 x20: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %358 x21: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %359 x22: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %360 x23: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %361 x24: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %362 x25: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %363 x26: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %364 x27: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %365 x28: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %366 x29: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %367 x30: i32 [storage=automatic];
// DEFAULT-NEXT:                 write<i32>(%338, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1)))));
// DEFAULT-NEXT:                 write<i32>(%339, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2)))));
// DEFAULT-NEXT:                 write<i32>(%340, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3)))));
// DEFAULT-NEXT:                 write<i32>(%341, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4)))));
// DEFAULT-NEXT:                 write<i32>(%342, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5)))));
// DEFAULT-NEXT:                 write<i32>(%343, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6)))));
// DEFAULT-NEXT:                 write<i32>(%344, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7)))));
// DEFAULT-NEXT:                 write<i32>(%345, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8)))));
// DEFAULT-NEXT:                 write<i32>(%346, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9)))));
// DEFAULT-NEXT:                 write<i32>(%347, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10)))));
// DEFAULT-NEXT:                 write<i32>(%348, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11)))));
// DEFAULT-NEXT:                 write<i32>(%349, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12)))));
// DEFAULT-NEXT:                 write<i32>(%350, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13)))));
// DEFAULT-NEXT:                 write<i32>(%351, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14)))));
// DEFAULT-NEXT:                 write<i32>(%352, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15)))));
// DEFAULT-NEXT:                 write<i32>(%353, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16)))));
// DEFAULT-NEXT:                 write<i32>(%354, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17)))));
// DEFAULT-NEXT:                 write<i32>(%355, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18)))));
// DEFAULT-NEXT:                 write<i32>(%356, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19)))));
// DEFAULT-NEXT:                 write<i32>(%357, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20)))));
// DEFAULT-NEXT:                 write<i32>(%358, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21)))));
// DEFAULT-NEXT:                 write<i32>(%359, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22)))));
// DEFAULT-NEXT:                 write<i32>(%360, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23)))));
// DEFAULT-NEXT:                 write<i32>(%361, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24)))));
// DEFAULT-NEXT:                 write<i32>(%362, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25)))));
// DEFAULT-NEXT:                 write<i32>(%363, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26)))));
// DEFAULT-NEXT:                 write<i32>(%364, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27)))));
// DEFAULT-NEXT:                 write<i32>(%365, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28)))));
// DEFAULT-NEXT:                 write<i32>(%366, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29)))));
// DEFAULT-NEXT:                 write<i32>(%367, read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30)))));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(1))), read<i32>(%338));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(2))), read<i32>(%339));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(3))), read<i32>(%340));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(4))), read<i32>(%341));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(5))), read<i32>(%342));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(6))), read<i32>(%343));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(7))), read<i32>(%344));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(8))), read<i32>(%345));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(9))), read<i32>(%346));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(10))), read<i32>(%347));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(11))), read<i32>(%348));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(12))), read<i32>(%349));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(13))), read<i32>(%350));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(14))), read<i32>(%351));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(15))), read<i32>(%352));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(16))), read<i32>(%353));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(17))), read<i32>(%354));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(18))), read<i32>(%355));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(19))), read<i32>(%356));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(20))), read<i32>(%357));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(21))), read<i32>(%358));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(22))), read<i32>(%359));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(23))), read<i32>(%360));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(24))), read<i32>(%361));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(25))), read<i32>(%362));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(26))), read<i32>(%363));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(27))), read<i32>(%364));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(28))), read<i32>(%365));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(29))), read<i32>(%366));
// DEFAULT-NEXT:                 write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(32)>(%3), const<i32>(30))), read<i32>(%367));
// DEFAULT-NEXT:                 let %425: u64 [synthetic] = read<u64>(%337);
// DEFAULT-NEXT:                 let %426: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%425), neg<u64, overflow=wrap>(read<u64>(%4)));
// DEFAULT-NEXT:                 write<u64>(%337, read<u64>(%426));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%337);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %368 @neg(%369 x: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<u64, overflow=wrap>(read<u64>(%369));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %370 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u64>(%4, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(100))));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%5, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2048)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%5, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2048)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%38, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(513)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%38, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(513)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%71, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(512)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%71, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(512)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%104, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(511)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%104, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(511)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%137, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%137, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%170, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(1), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%170, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(1), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%203, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(511), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%203, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(511), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%236, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(512), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%236, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(512), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%269, const<i32>(3), not<u64>(const<u64>(0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(513), const<i32>(3)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%269, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(const<i32>(513), const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%302, const<i32>(3), not<u64>(const<u64>(0))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%302, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%335, const<i32>(3), not<u64>(const<u64>(0))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(neg<u64, overflow=wrap>(read<u64>(%4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i32, u64) -> u64>(%335, const<i32>(3), const<u64>(4294967295)), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(neg<u64, overflow=wrap>(read<u64>(%4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), const<u64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%368, read<u64>(%4)), neg<u64, overflow=wrap>(const<u64>(100)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
