// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c23
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

_Noreturn void stop(void);
void stop(void) { for (;;) {} }
[[noreturn]] void halt(void) { stop(); }
__attribute__((noreturn, cold)) void aborting(void);
__attribute__((warn_unused_result, pure)) int inspect(int x);
int inspect(int x) { return x; }
constexpr int constant = 5;
constexpr int *null_pointer = 0;
int read_constant(void) {
    constexpr int local = 3;
    return constant + local;
}

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
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %5 constant: i32 [storage=static] [const] [constexpr] = const<i32>(5) [linkage=internal] [c="const int"] [c_const="true"];
// DEFAULT-NEXT:     global %6 null_pointer: ptr<i32> [storage=static] [const] [constexpr] = null<ptr<i32>> [linkage=internal] [c="int *const"] [c_const="true"];
// DEFAULT-NEXT:     fn %0 @stop() -> void [linkage=external] [noreturn] [fallthrough=ub] [c="void(void)"] {
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @halt() -> void [linkage=external] [noreturn] [fallthrough=ub] [c_storage="none"] [c_return="void"] [c="void(void)"] [c_attributes="[NoReturn]"] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @aborting() -> void [linkage=external] [noreturn] [c="void(void)"] [c_attributes="[NoReturn, Cold]"];
// DEFAULT-NEXT:     fn %3 @inspect(%4 x: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(int)"] [c_attributes="[WarnUnusedResult, Pure]"] {
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @read_constant() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// DEFAULT-NEXT:         let %8 local: i32 [storage=automatic] [const] [constexpr] = const<i32>(3) [c="const int"] [c_const="true"];
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%5), read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
