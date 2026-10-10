// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C94
// SLATE-FILECHECK-STD C94 iso9899:199409
// SLATE-FILECHECK-DEFINES GNU89
// SLATE-FILECHECK-STD GNU89 gnu89

%:define STR(x) %:x
%:define XSTR(x) STR(x)
%:define GLUE(a, b) a %:%: b
%:if 1
int digraph_directive;
%:endif

int array <: 2 :> = <% 1, 2 %>;
const char stringified_bracket[] = STR(<:);
const char stringified_paste[] = STR(%:%:);
int GLUE(pasted_, identifier);
const char pasted_digraph[] = XSTR(GLUE(<, :));
const char pasted_hash_hash[] = XSTR(GLUE(%:, %:));

// SLATE-FILECHECK-BEGIN C99
// C99: module {
// C99-NEXT:     target "x86_64-unknown-linux-gnu" {
// C99-NEXT:         endian = little;
// C99-NEXT:         pointer [size=8, align=8];
// C99-NEXT:         stack_alignment = 16;
// C99-NEXT:         long_double = f80;
// C99-NEXT:         storage bool [size=1, align=1];
// C99-NEXT:         storage i8, u8 [size=1, align=1];
// C99-NEXT:         storage i16, u16 [size=2, align=2];
// C99-NEXT:         storage i32, u32 [size=4, align=4];
// C99-NEXT:         storage i64, u64 [size=8, align=8];
// C99-NEXT:         storage i128, u128 [size=16, align=16];
// C99-NEXT:         storage bf16 [size=2, align=2];
// C99-NEXT:         storage f16 [size=2, align=2];
// C99-NEXT:         storage f32 [size=4, align=4];
// C99-NEXT:         storage f64 [size=8, align=8];
// C99-NEXT:         storage f80 [size=16, align=16];
// C99-NEXT:         storage f128 [size=16, align=16];
// C99-NEXT:         storage d32 [size=4, align=4];
// C99-NEXT:         storage d64 [size=8, align=8];
// C99-NEXT:         storage d128 [size=16, align=16];
// C99-NEXT:     }
// C99-NEXT:     global %[[VALUE_digraph_directive:[0-9]+]] digraph_directive: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_array:[0-9]+]] array: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// C99-NEXT:     global %[[VALUE_stringified_bracket:[0-9]+]] stringified_bracket: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([60, 58, 0]) [linkage=external];
// C99-NEXT:     global %[[VALUE_stringified_paste:[0-9]+]] stringified_paste: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([37, 58, 37, 58, 0]) [linkage=external];
// C99-NEXT:     global %[[VALUE_pasted_identifier:[0-9]+]] pasted_identifier: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_pasted_digraph:[0-9]+]] pasted_digraph: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([60, 58, 0]) [linkage=external];
// C99-NEXT:     global %[[VALUE_pasted_hash_hash:[0-9]+]] pasted_hash_hash: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([37, 58, 37, 58, 0]) [linkage=external];
// C99-NEXT: }
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C94
// C94: module {
// C94-NEXT:     target "x86_64-unknown-linux-gnu" {
// C94-NEXT:         endian = little;
// C94-NEXT:         pointer [size=8, align=8];
// C94-NEXT:         stack_alignment = 16;
// C94-NEXT:         long_double = f80;
// C94-NEXT:         storage bool [size=1, align=1];
// C94-NEXT:         storage i8, u8 [size=1, align=1];
// C94-NEXT:         storage i16, u16 [size=2, align=2];
// C94-NEXT:         storage i32, u32 [size=4, align=4];
// C94-NEXT:         storage i64, u64 [size=8, align=8];
// C94-NEXT:         storage i128, u128 [size=16, align=16];
// C94-NEXT:         storage bf16 [size=2, align=2];
// C94-NEXT:         storage f16 [size=2, align=2];
// C94-NEXT:         storage f32 [size=4, align=4];
// C94-NEXT:         storage f64 [size=8, align=8];
// C94-NEXT:         storage f80 [size=16, align=16];
// C94-NEXT:         storage f128 [size=16, align=16];
// C94-NEXT:         storage d32 [size=4, align=4];
// C94-NEXT:         storage d64 [size=8, align=8];
// C94-NEXT:         storage d128 [size=16, align=16];
// C94-NEXT:     }
// C94-NEXT:     global %[[VALUE_digraph_directive:[0-9]+]] digraph_directive: i32 [storage=static] [linkage=external];
// C94-NEXT:     global %[[VALUE_array:[0-9]+]] array: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// C94-NEXT:     global %[[VALUE_stringified_bracket:[0-9]+]] stringified_bracket: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([60, 58, 0]) [linkage=external];
// C94-NEXT:     global %[[VALUE_stringified_paste:[0-9]+]] stringified_paste: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([37, 58, 37, 58, 0]) [linkage=external];
// C94-NEXT:     global %[[VALUE_pasted_identifier:[0-9]+]] pasted_identifier: i32 [storage=static] [linkage=external];
// C94-NEXT:     global %[[VALUE_pasted_digraph:[0-9]+]] pasted_digraph: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([60, 58, 0]) [linkage=external];
// C94-NEXT:     global %[[VALUE_pasted_hash_hash:[0-9]+]] pasted_hash_hash: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([37, 58, 37, 58, 0]) [linkage=external];
// C94-NEXT: }
// SLATE-FILECHECK-END C94
// SLATE-FILECHECK-BEGIN GNU89
// GNU89: module {
// GNU89-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU89-NEXT:         endian = little;
// GNU89-NEXT:         pointer [size=8, align=8];
// GNU89-NEXT:         stack_alignment = 16;
// GNU89-NEXT:         long_double = f80;
// GNU89-NEXT:         storage bool [size=1, align=1];
// GNU89-NEXT:         storage i8, u8 [size=1, align=1];
// GNU89-NEXT:         storage i16, u16 [size=2, align=2];
// GNU89-NEXT:         storage i32, u32 [size=4, align=4];
// GNU89-NEXT:         storage i64, u64 [size=8, align=8];
// GNU89-NEXT:         storage i128, u128 [size=16, align=16];
// GNU89-NEXT:         storage bf16 [size=2, align=2];
// GNU89-NEXT:         storage f16 [size=2, align=2];
// GNU89-NEXT:         storage f32 [size=4, align=4];
// GNU89-NEXT:         storage f64 [size=8, align=8];
// GNU89-NEXT:         storage f80 [size=16, align=16];
// GNU89-NEXT:         storage f128 [size=16, align=16];
// GNU89-NEXT:         storage d32 [size=4, align=4];
// GNU89-NEXT:         storage d64 [size=8, align=8];
// GNU89-NEXT:         storage d128 [size=16, align=16];
// GNU89-NEXT:     }
// GNU89-NEXT:     global %[[VALUE_digraph_directive:[0-9]+]] digraph_directive: i32 [storage=static] [linkage=external];
// GNU89-NEXT:     global %[[VALUE_array:[0-9]+]] array: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// GNU89-NEXT:     global %[[VALUE_stringified_bracket:[0-9]+]] stringified_bracket: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([60, 58, 0]) [linkage=external];
// GNU89-NEXT:     global %[[VALUE_stringified_paste:[0-9]+]] stringified_paste: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([37, 58, 37, 58, 0]) [linkage=external];
// GNU89-NEXT:     global %[[VALUE_pasted_identifier:[0-9]+]] pasted_identifier: i32 [storage=static] [linkage=external];
// GNU89-NEXT:     global %[[VALUE_pasted_digraph:[0-9]+]] pasted_digraph: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([60, 58, 0]) [linkage=external];
// GNU89-NEXT:     global %[[VALUE_pasted_hash_hash:[0-9]+]] pasted_hash_hash: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([37, 58, 37, 58, 0]) [linkage=external];
// GNU89-NEXT: }
// SLATE-FILECHECK-END GNU89
