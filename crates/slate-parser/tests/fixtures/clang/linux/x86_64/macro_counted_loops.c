// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-macros

#define LIMIT 10
#define TWICE(x) ((x) * 2)
#define BELOW(i, n) ((i) < (n))

int sum(const int *a, int n) {
    int s = 0;
    for (int i = 0; i < LIMIT; i++)
        s += a[i];
    for (int i = 0; i < TWICE(n); i++)
        s += a[i];
    for (int i = 0; BELOW(i, n); i++)
        s += a[i];
    for (int i = 0; i <= TWICE(4); i++)
        s += a[i];
    return s;
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
// IR-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_a:[0-9]+]] a: ptr<const i32>, %[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_s:[0-9]+]] s: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         for %[[VALUE0:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), macro_value<i32>(#[[#NODE0:]], statements=0, const<i32>(10) [macro=LIMIT#0 body]) [macro=LIMIT#0 body])
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..macro_value<i32>(#[[#NODE0]], statements=0, const<i32>(10) [macro=LIMIT#0 body]) [macro=LIMIT#0 body]
// IR-NEXT:             body:
// IR-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]])))));
// IR-NEXT:                 write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE4]]));
// IR-NEXT:         for %[[VALUE5:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), macro_value<i32>(#[[#NODE1:]], statements=0, mul<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, read<i32>(%[[VALUE_n]]) [macro=TWICE#1 arg0]) [macro=TWICE#1 arg0], const<i32>(2) [macro=TWICE#1 body]) [macro=TWICE#1 body]) [macro=TWICE#1 body])
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// IR-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE6]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_i_2]] in const<i32>(0)..macro_value<i32>(#[[#NODE1]], statements=0, mul<i32>(macro_arg<i32>(#[[#NODE1]], 0, read<i32>(%[[VALUE_n]]) [macro=TWICE#1 arg0]) [macro=TWICE#1 arg0], const<i32>(2) [macro=TWICE#1 body]) [macro=TWICE#1 body]) [macro=TWICE#1 body]
// IR-NEXT:             body:
// IR-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE8]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_i_2]])))));
// IR-NEXT:                 write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE9]]));
// IR-NEXT:         for %[[VALUE10:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: macro_value<bool>(#[[#NODE2:]], statements=0, lt<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, read<i32>(%[[VALUE_i_3]]) [macro=BELOW#2 arg0]) [macro=BELOW#2 arg0], macro_arg<i32>(#{{[0-9]+}}, 1, read<i32>(%[[VALUE_n]]) [macro=BELOW#2 arg1]) [macro=BELOW#2 arg1]) [macro=BELOW#2 body]) [macro=BELOW#2 body]
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// IR-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE11]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE12]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_i_3]] in const<i32>(0)..macro_arg<i32>(#[[#NODE2]], 1, read<i32>(%[[VALUE_n]]) [macro=BELOW#2 arg1]) [macro=BELOW#2 arg1]
// IR-NEXT:             body:
// IR-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE13]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_i_3]])))));
// IR-NEXT:                 write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE14]]));
// IR-NEXT:         for %[[VALUE15:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i_4]]), macro_value<i32>(#[[#NODE3:]], statements=0, mul<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, const<i32>(4) [macro=TWICE#3 arg0]) [macro=TWICE#3 arg0], const<i32>(2) [macro=TWICE#3 body]) [macro=TWICE#3 body]) [macro=TWICE#3 body])
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// IR-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE16]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE17]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             counted: %[[VALUE_i_4]] in const<i32>(0)..=macro_value<i32>(#[[#NODE3]], statements=0, mul<i32>(macro_arg<i32>(#[[#NODE3]], 0, const<i32>(4) [macro=TWICE#3 arg0]) [macro=TWICE#3 arg0], const<i32>(2) [macro=TWICE#3 body]) [macro=TWICE#3 body]) [macro=TWICE#3 body]
// IR-NEXT:             body:
// IR-NEXT:                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_s]]);
// IR-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE18]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_i_4]])))));
// IR-NEXT:                 write<i32>(%[[VALUE_s]], read<i32>(%[[VALUE19]]));
// IR-NEXT:         return read<i32>(%[[VALUE_s]]);
// IR-NEXT:     }
// IR-NEXT:     macro_definition #[[#NODE0]] LIMIT [defined=7:1] = "10";
// IR-NEXT:     macro_definition #[[#NODE1]] TWICE(x) [defined=7:2] = "( ( x ) * 2 )";
// IR-NEXT:     macro_definition #[[#NODE2]] BELOW(i, n) [defined=7:3] = "( ( i ) < ( n ) )";
// IR-NEXT:     macro_expansion #[[#NODE0]] LIMIT [definition=#[[#NODE0]]] [site=7:148+5] [covers=expr];
// IR-NEXT:     macro_expansion #[[#NODE1]] TWICE [definition=#[[#NODE1]]] [site=7:203+8] [arguments=209+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE2]] BELOW [definition=#[[#NODE2]]] [site=7:257+11] [arguments=263+1, 266+1] [covers=expr] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE3]] TWICE [definition=#[[#NODE1]]] [site=7:319+8] [arguments=325+1] [covers=expr] [arg0=expr];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
