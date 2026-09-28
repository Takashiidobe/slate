#include <stdio.h>

#define NEXT_COUNTER()     __COUNTER__
#define COUNTER_PAIR(a, b) ((a) * 10 + (b))

static int direct_zero = __COUNTER__;
static int macro_one   = NEXT_COUNTER();

#if 0
static int ignored = __COUNTER__;
#endif

static int pair = COUNTER_PAIR(__COUNTER__, NEXT_COUNTER());

#define DROP_ARG(x)      1
#define STRINGIZE_ARG(x) #x
#define PASTE_ARG(x)     x##_tail

static int dropped = DROP_ARG(__COUNTER__);
static const char *stringized = STRINGIZE_ARG(__COUNTER__);
static int PASTE_ARG(__COUNTER__);

int main(void) {
  int local_four = __COUNTER__;
  printf("%d %d %d %d %d %s %d\n", direct_zero, macro_one, pair, local_four, dropped,
         stringized, __COUNTER___tail);
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
// DEFAULT-NEXT:     global %1 direct_zero: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %2 macro_one: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %3 pair: i32 [storage=static] = add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(10)), const<i32>(3)) [linkage=internal];
// DEFAULT-NEXT:     global %4 dropped: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([95, 95, 67, 79, 85, 78, 84, 69, 82, 95, 95, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 stringized: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%10)) [linkage=internal];
// DEFAULT-NEXT:     global %6 __COUNTER___tail: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 115, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 local_four: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%11)), read<i32>(%1), read<i32>(%2), read<i32>(%3), read<i32>(%8), read<i32>(%4), read<ptr<const i8>>(%5), read<i32>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
