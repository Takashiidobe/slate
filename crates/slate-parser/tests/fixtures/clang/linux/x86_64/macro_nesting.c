// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-macros

void sink(int value);

#define LEAF(x) { sink(x); }
#define MIDDLE(a) { LEAF((a) + 1); LEAF(2); sink(a); }
#define OUTER(a, b) MIDDLE((a) + (b))
#define NAME_FROM_ARGUMENT(f, x) f(x)
#define OBJECT LEAF(3)

int use(int value) {
    LEAF(value);
    OUTER(value, 4);
    NAME_FROM_ARGUMENT(LEAF, value);
    OBJECT;
    return value;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE_value:[0-9]+]] value: i32) -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_value_2:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         { [macro=LEAF#0 body]
// IR-NEXT:             call<void>(%[[VALUE_sink]], read<i32>(%[[VALUE_value_2]]) [macro=LEAF#0 arg0]) [macro=LEAF#0 body] [macro=LEAF#0 body];
// IR-NEXT:         }
// IR-NEXT:         ;
// IR-NEXT:         { [macro=MIDDLE#2 body] [macro=OUTER#1 body]
// IR-NEXT:             { [macro=LEAF#3 body] [macro=MIDDLE#2 body] [macro=OUTER#1 body]
// IR-NEXT:                 call<void>(%[[VALUE_sink]], add<i32>(add<i32>(read<i32>(%[[VALUE_value_2]]) [macro=LEAF#3 arg0] [macro=MIDDLE#2 arg0] [macro=OUTER#1 arg0], const<i32>(4) [macro=LEAF#3 arg0] [macro=MIDDLE#2 arg0] [macro=OUTER#1 arg1]) [macro=LEAF#3 arg0] [macro=MIDDLE#2 arg0] [macro=OUTER#1 body], const<i32>(1) [macro=LEAF#3 arg0] [macro=MIDDLE#2 body] [macro=OUTER#1 body]) [macro=LEAF#3 arg0] [macro=MIDDLE#2 body] [macro=OUTER#1 body]) [macro=LEAF#3 body] [macro=MIDDLE#2 body] [macro=OUTER#1 body] [macro=LEAF#3 body] [macro=MIDDLE#2 body] [macro=OUTER#1 body];
// IR-NEXT:             }
// IR-NEXT:              [macro=MIDDLE#2 body] [macro=OUTER#1 body];
// IR-NEXT:             { [macro=LEAF#4 body] [macro=MIDDLE#2 body] [macro=OUTER#1 body]
// IR-NEXT:                 call<void>(%[[VALUE_sink]], const<i32>(2) [macro=LEAF#4 arg0] [macro=MIDDLE#2 body] [macro=OUTER#1 body]) [macro=LEAF#4 body] [macro=MIDDLE#2 body] [macro=OUTER#1 body] [macro=LEAF#4 body] [macro=MIDDLE#2 body] [macro=OUTER#1 body];
// IR-NEXT:             }
// IR-NEXT:              [macro=MIDDLE#2 body] [macro=OUTER#1 body];
// IR-NEXT:             call<void>(%[[VALUE_sink]], add<i32>(read<i32>(%[[VALUE_value_2]]) [macro=MIDDLE#2 arg0] [macro=OUTER#1 arg0], const<i32>(4) [macro=MIDDLE#2 arg0] [macro=OUTER#1 arg1]) [macro=MIDDLE#2 arg0] [macro=OUTER#1 body]) [macro=MIDDLE#2 body] [macro=OUTER#1 body] [macro=MIDDLE#2 body] [macro=OUTER#1 body];
// IR-NEXT:         }
// IR-NEXT:         ;
// IR-NEXT:         { [macro=NAME_FROM_ARGUMENT#5 body]
// IR-NEXT:             call<void>(%[[VALUE_sink]], read<i32>(%[[VALUE_value_2]]) [macro=NAME_FROM_ARGUMENT#5 arg1]) [macro=NAME_FROM_ARGUMENT#5 body] [macro=NAME_FROM_ARGUMENT#5 body];
// IR-NEXT:         }
// IR-NEXT:         ;
// IR-NEXT:         { [macro=LEAF#7 body] [macro=OBJECT#6 body]
// IR-NEXT:             call<void>(%[[VALUE_sink]], const<i32>(3) [macro=LEAF#7 arg0] [macro=OBJECT#6 body]) [macro=LEAF#7 body] [macro=OBJECT#6 body] [macro=LEAF#7 body] [macro=OBJECT#6 body];
// IR-NEXT:         }
// IR-NEXT:         ;
// IR-NEXT:         return read<i32>(%[[VALUE_value_2]]);
// IR-NEXT:     }
// IR-NEXT:     macro_definition #[[#NODE0:]] LEAF(x) [defined=7:3] = "{ sink ( x ) ; }";
// IR-NEXT:     macro_definition #[[#NODE1:]] OUTER(a, b) [defined=7:5] = "MIDDLE ( ( a ) + ( b ) )";
// IR-NEXT:     macro_definition #[[#NODE2:]] MIDDLE(a) [defined=7:4] = "{ LEAF ( ( a ) + 1 ) ; LEAF ( 2 ) ; sink ( a ) ; }";
// IR-NEXT:     macro_definition #[[#NODE3:]] NAME_FROM_ARGUMENT(f, x) [defined=7:6] = "f ( x )";
// IR-NEXT:     macro_definition #[[#NODE4:]] OBJECT [defined=7:7] = "LEAF ( 3 )";
// IR-NEXT:     macro_expansion #[[#NODE0]] LEAF [definition=#[[#NODE0]]] [site=7:233+11] [arguments=238+5] [covers=statements] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE1]] OUTER [definition=#[[#NODE1]]] [site=7:250+15] [arguments=256+5, 263+1] [covers=statements] [arg0=expr] [arg0=expr] [arg1=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE2]] MIDDLE [definition=#[[#NODE2]]] [parent=#[[#NODE1]]] [covers=statements] [arg0=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE3]] LEAF [definition=#[[#NODE0]]] [parent=#[[#NODE2]]] [covers=statements] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE4]] LEAF [definition=#[[#NODE0]]] [parent=#[[#NODE2]]] [covers=statements] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE5:]] NAME_FROM_ARGUMENT [definition=#[[#NODE3]]] [site=7:271+31] [arguments=290+4, 296+5] [covers=statements] [arg0=split] [arg0=split] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE6:]] OBJECT [definition=#[[#NODE4]]] [site=7:308+6] [covers=statements];
// IR-NEXT:     macro_expansion #[[#NODE7:]] LEAF [definition=#[[#NODE0]]] [parent=#[[#NODE6]]] [covers=statements] [arg0=expr];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
