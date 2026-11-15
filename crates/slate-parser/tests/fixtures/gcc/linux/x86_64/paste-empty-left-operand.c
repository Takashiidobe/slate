#define STR(...) #__VA_ARGS__
#define XSTR(...) STR(__VA_ARGS__)
#define F(m, x) do { if (x) m##foo = 1; } while (0)
#define G(p, s) { &p##attr_##s##_freq.attr }
#define H(n) h(u32, n##control_freq)
#define I(a, b, c) i(x) a##b##c
#define J(a, b) [a##b]

int foo;
int prefixfoo;

const char empty_after_rparen[] = XSTR(F(, 1));
const char filled_after_rparen[] = XSTR(F(prefix, 1));
const char empty_after_ampersand[] = XSTR(G(, cur));
const char empty_after_comma[] = XSTR(H());
const char two_empty_then_token[] = XSTR(I(, , z));
const char all_empty[] = XSTR(I(, , ));
const char both_empty_after_lbracket[] = XSTR(J(, ));

int main(void) {
    F(, 1);
    F(prefix, 2);
    return foo + prefixfoo - 2;
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
// DEFAULT-NEXT:     global %[[VALUE_foo:[0-9]+]] foo: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_prefixfoo:[0-9]+]] prefixfoo: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_empty_after_rparen:[0-9]+]] empty_after_rparen: array<i8, 33> [storage=static] [const] [align=16] = code_units<array<i8, 33>>([100, 111, 32, 123, 32, 105, 102, 32, 40, 49, 41, 32, 102, 111, 111, 32, 61, 32, 49, 59, 32, 125, 32, 119, 104, 105, 108, 101, 32, 40, 48, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_filled_after_rparen:[0-9]+]] filled_after_rparen: array<i8, 39> [storage=static] [const] [align=16] = code_units<array<i8, 39>>([100, 111, 32, 123, 32, 105, 102, 32, 40, 49, 41, 32, 112, 114, 101, 102, 105, 120, 102, 111, 111, 32, 61, 32, 49, 59, 32, 125, 32, 119, 104, 105, 108, 101, 32, 40, 48, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_empty_after_ampersand:[0-9]+]] empty_after_ampersand: array<i8, 24> [storage=static] [const] [align=16] = code_units<array<i8, 24>>([123, 32, 38, 97, 116, 116, 114, 95, 99, 117, 114, 95, 102, 114, 101, 113, 46, 97, 116, 116, 114, 32, 125, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_empty_after_comma:[0-9]+]] empty_after_comma: array<i8, 21> [storage=static] [const] [align=16] = code_units<array<i8, 21>>([104, 40, 117, 51, 50, 44, 32, 99, 111, 110, 116, 114, 111, 108, 95, 102, 114, 101, 113, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_two_empty_then_token:[0-9]+]] two_empty_then_token: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([105, 40, 120, 41, 32, 122, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_all_empty:[0-9]+]] all_empty: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([105, 40, 120, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_both_empty_after_lbracket:[0-9]+]] both_empty_after_lbracket: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([91, 93, 0]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(const<i32>(1), const<i32>(0)) [folded=true]
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_foo]], const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(const<i32>(2), const<i32>(0)) [folded=true]
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_prefixfoo]], const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_foo]]), read<i32>(%[[VALUE_prefixfoo]])), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
