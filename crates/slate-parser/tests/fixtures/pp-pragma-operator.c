#define DO_PRAGMA(x) _Pragma(#x)
#define VALUE 7
DO_PRAGMA(push_macro("VALUE"))
#undef VALUE
#define VALUE 9
int before_pop = VALUE;
DO_PRAGMA(pop_macro("VALUE"))
int after_pop = VALUE;
int left; DO_PRAGMA(message("quoted text")) int right;
int same_line(void) { int x = 4; DO_PRAGMA(message("inside")) return x; }
int expression(void) {
  int x = 1;
  return DO_PRAGMA(clang diagnostic push) x + 2;
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
// DEFAULT-NEXT:     global %0 before_pop: i32 [storage=static] = const<i32>(9) [linkage=external];
// DEFAULT-NEXT:     global %1 after_pop: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %2 left: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 right: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @same_line() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 x: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @expression() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 x: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%7), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
