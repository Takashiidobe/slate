/* PR rtl-optimization/58365 */

extern void abort(void);

struct S {
  volatile int a;
  int          b, c, d, e;
} f;
static struct S g, h;
int             i = 1;

char foo(void) { return i; }

static struct S bar(void) {
  if (foo())
    return f;
  return g;
}

int main() {
  h   = bar();
  f.b = 1;
  if (h.b != 0)
    abort();
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: volatile i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:         field4 e: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     global %2 f: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 g: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %4 h: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %5 i: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @foo() -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(read<i32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar() -> @type0 [linkage=internal] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i8>(call<i8, signature=fn() -> i8>(%6), const<i8>(0))
// DEFAULT-NEXT:             return copy<@type0, reason=return>(read<@type0>(%2));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<@type0>(%4, copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%7)));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%7));
// DEFAULT-NEXT:         write<i32>(field1(%2), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field1(%4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
