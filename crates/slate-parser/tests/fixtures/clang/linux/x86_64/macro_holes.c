// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-macros

#define ADD_ONE(x) ((x) + 1)
#define SET(p, v) do { *(p) = (v); } while (0)
#define ASSIGN(l, v) ((l) = (v))
#define TEMP(x) do { int temp = 10; sink(x); } while (0)
#define INNER(x) sink(x)
#define OUTER(x) do { INNER(x); INNER((x) + 1); } while (0)
#define TIMES2(x) x * 2
#define TWO(a, b) sink(a); sink(b)
#define FIELD(s, f) sink((s).f)
#define BITS(s, v) ((s).bits = (v))

void sink(long);
struct S { int bits : 3; };

void conversions_stay_outside_the_hole(unsigned char c, int i, long l) {
    ADD_ONE(c);
    ADD_ONE(i);
    ADD_ONE(l);
}

void places_and_values(int i, long l) {
    long r;
    SET(&r, i);
    ASSIGN(r, l);
    if (i) ASSIGN(r, i++);
}

void argument_reads_a_body_local(int a) {
    int temp = 2;
    TEMP(temp + a);
    TEMP(a);
}

void nested(int a) {
    OUTER(a);
}

void several_statements(int i, int a) {
    TWO(i, a);
}

void not_one_subtree(int i, int a) {
    TIMES2(i + a);
}

void name_argument_stays_inline(struct S s) {
    FIELD(s, bits);
}

void bit_field_through_a_place_hole(struct S s, int i) {
    BITS(s, i);
}

void value_position_has_no_region_yet(int i) {
    int k = ADD_ONE(i);
    sink(k);
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
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 bits: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE0:[0-9]+]] <unnamed>: i64) -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_conversions_stay_outside_the_hole:[0-9]+]] @conversions_stay_outside_the_hole(%[[VALUE_c:[0-9]+]] c: u8, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_l:[0-9]+]] l: i64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         macro_body #[[#NODE0:]] {
// IR-NEXT:             add<i32>(reinterpret<i32>(widen<u32>(macro_arg<u8>(#[[#NODE0]], 0, read<u8>(%[[VALUE_c]]) [macro=ADD_ONE#0 arg0]) [macro=ADD_ONE#0 arg0]) [macro=ADD_ONE#0 arg0]) [macro=ADD_ONE#0 arg0], const<i32>(1) [macro=ADD_ONE#0 body]) [macro=ADD_ONE#0 body];
// IR-NEXT:         }
// IR-NEXT:         macro_body #[[#NODE1:]] {
// IR-NEXT:             add<i32>(macro_arg<i32>(#[[#NODE1]], 0, read<i32>(%[[VALUE_i]]) [macro=ADD_ONE#1 arg0]) [macro=ADD_ONE#1 arg0], const<i32>(1) [macro=ADD_ONE#1 body]) [macro=ADD_ONE#1 body];
// IR-NEXT:         }
// IR-NEXT:         macro_body #[[#NODE2:]] {
// IR-NEXT:             add<i64>(macro_arg<i64>(#[[#NODE2]], 0, read<i64>(%[[VALUE_l]]) [macro=ADD_ONE#2 arg0]) [macro=ADD_ONE#2 arg0], widen<i64>(const<i32>(1) [macro=ADD_ONE#2 body]) [macro=ADD_ONE#2 body]) [macro=ADD_ONE#2 body];
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_places_and_values:[0-9]+]] @places_and_values(%[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_l_2:[0-9]+]] l: i64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: i64 [storage=automatic];
// IR-NEXT:         macro_body #[[#NODE3:]] {
// IR-NEXT:             do %[[VALUE1:[0-9]+]]
// IR-NEXT:                 { [macro=SET#3 body]
// IR-NEXT:                     write<i64>(deref(macro_arg<ptr<i64>>(#[[#NODE3]], 0, addr_of<ptr<i64>>(%[[VALUE_r]]))), widen<i64>(macro_arg<i32>(#[[#NODE3]], 1, read<i32>(%[[VALUE_i_2]]) [macro=SET#3 arg1]) [macro=SET#3 arg1]) [macro=SET#3 arg1]) [macro=SET#3 body];
// IR-NEXT:                 }
// IR-NEXT:             while ne<i32>(const<i32>(0) [macro=SET#3 body], const<i32>(0) [macro=SET#3 body]) [macro=SET#3 body];
// IR-NEXT:         }
// IR-NEXT:         macro_body #[[#NODE4:]] {
// IR-NEXT:             write<i64>(macro_arg(#[[#NODE4]], 0, %[[VALUE_r]]), macro_arg<i64>(#[[#NODE4]], 1, read<i64>(%[[VALUE_l_2]]) [macro=ASSIGN#4 arg1]) [macro=ASSIGN#4 arg1]) [macro=ASSIGN#4 body];
// IR-NEXT:         }
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0))
// IR-NEXT:             macro_body #[[#NODE5:]] {
// IR-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]) [macro=ASSIGN#5 arg1] [macro=ASSIGN#5 arg1];
// IR-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE2]]) [macro=ASSIGN#5 arg1], const<i32>(1) [macro=ASSIGN#5 arg1]) [macro=ASSIGN#5 arg1] [macro=ASSIGN#5 arg1];
// IR-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE3]]) [macro=ASSIGN#5 arg1]) [macro=ASSIGN#5 arg1];
// IR-NEXT:                 write<i64>(macro_arg(#[[#NODE5]], 0, %[[VALUE_r]]), widen<i64>(macro_arg<i32>(#[[#NODE5]], 1, read<i32>(%[[VALUE2]]) [macro=ASSIGN#5 arg1]) [macro=ASSIGN#5 arg1]) [macro=ASSIGN#5 arg1]) [macro=ASSIGN#5 body];
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_argument_reads_a_body_local:[0-9]+]] @argument_reads_a_body_local(%[[VALUE_a:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_temp:[0-9]+]] temp: i32 [storage=automatic] = const<i32>(2);
// IR-NEXT:         do %[[VALUE4:[0-9]+]]
// IR-NEXT:             { [macro=TEMP#6 body]
// IR-NEXT:                 let %[[VALUE_temp_2:[0-9]+]] temp: i32 [storage=automatic] = const<i32>(10) [macro=TEMP#6 body] [macro=TEMP#6 body];
// IR-NEXT:                 call<void>(%[[VALUE_sink]], widen<i64>(add<i32>(read<i32>(%[[VALUE_temp_2]]) [macro=TEMP#6 arg0], read<i32>(%[[VALUE_a]]) [macro=TEMP#6 arg0]) [macro=TEMP#6 arg0]) [macro=TEMP#6 arg0]) [macro=TEMP#6 body] [macro=TEMP#6 body];
// IR-NEXT:             }
// IR-NEXT:         while ne<i32>(const<i32>(0) [macro=TEMP#6 body], const<i32>(0) [macro=TEMP#6 body]) [macro=TEMP#6 body];
// IR-NEXT:         macro_body #[[#NODE6:]] {
// IR-NEXT:             do %[[VALUE5:[0-9]+]]
// IR-NEXT:                 { [macro=TEMP#7 body]
// IR-NEXT:                     let %[[VALUE_temp_3:[0-9]+]] temp: i32 [storage=automatic] = const<i32>(10) [macro=TEMP#7 body] [macro=TEMP#7 body];
// IR-NEXT:                     call<void>(%[[VALUE_sink]], widen<i64>(macro_arg<i32>(#[[#NODE6]], 0, read<i32>(%[[VALUE_a]]) [macro=TEMP#7 arg0]) [macro=TEMP#7 arg0]) [macro=TEMP#7 arg0]) [macro=TEMP#7 body] [macro=TEMP#7 body];
// IR-NEXT:                 }
// IR-NEXT:             while ne<i32>(const<i32>(0) [macro=TEMP#7 body], const<i32>(0) [macro=TEMP#7 body]) [macro=TEMP#7 body];
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_a_2:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         macro_body #[[#NODE7:]] {
// IR-NEXT:             do %[[VALUE6:[0-9]+]]
// IR-NEXT:                 { [macro=OUTER#8 body]
// IR-NEXT:                     macro_body #[[#NODE8:]] { [macro=OUTER#8 body]
// IR-NEXT:                         call<void>(%[[VALUE_sink]], widen<i64>(macro_arg<i32>(#[[#NODE8]], 0, macro_arg<i32>(#[[#NODE7]], 0, read<i32>(%[[VALUE_a_2]]) [macro=INNER#9 arg0] [macro=OUTER#8 arg0]) [macro=INNER#9 arg0] [macro=OUTER#8 arg0]) [macro=INNER#9 arg0] [macro=OUTER#8 arg0]) [macro=INNER#9 arg0] [macro=OUTER#8 arg0]) [macro=INNER#9 body] [macro=OUTER#8 body] [macro=OUTER#8 body];
// IR-NEXT:                     }
// IR-NEXT:                     macro_body #[[#NODE9:]] { [macro=OUTER#8 body]
// IR-NEXT:                         call<void>(%[[VALUE_sink]], widen<i64>(macro_arg<i32>(#[[#NODE9]], 0, add<i32>(macro_arg<i32>(#[[#NODE7]], 0, read<i32>(%[[VALUE_a_2]]) [macro=INNER#10 arg0] [macro=OUTER#8 arg0]) [macro=INNER#10 arg0] [macro=OUTER#8 arg0], const<i32>(1) [macro=INNER#10 arg0] [macro=OUTER#8 body]) [macro=INNER#10 arg0] [macro=OUTER#8 body]) [macro=INNER#10 arg0] [macro=OUTER#8 body]) [macro=INNER#10 arg0] [macro=OUTER#8 body]) [macro=INNER#10 body] [macro=OUTER#8 body] [macro=OUTER#8 body];
// IR-NEXT:                     }
// IR-NEXT:                 }
// IR-NEXT:             while ne<i32>(const<i32>(0) [macro=OUTER#8 body], const<i32>(0) [macro=OUTER#8 body]) [macro=OUTER#8 body];
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_several_statements:[0-9]+]] @several_statements(%[[VALUE_i_3:[0-9]+]] i: i32, %[[VALUE_a_3:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         macro_body #[[#NODE10:]] { [macro=TWO#11 body]
// IR-NEXT:             call<void>(%[[VALUE_sink]], widen<i64>(macro_arg<i32>(#[[#NODE10]], 0, read<i32>(%[[VALUE_i_3]]) [macro=TWO#11 arg0]) [macro=TWO#11 arg0]) [macro=TWO#11 arg0]) [macro=TWO#11 body] [macro=TWO#11 body];
// IR-NEXT:             call<void>(%[[VALUE_sink]], widen<i64>(macro_arg<i32>(#[[#NODE10]], 1, read<i32>(%[[VALUE_a_3]]) [macro=TWO#11 arg1]) [macro=TWO#11 arg1]) [macro=TWO#11 arg1]) [macro=TWO#11 body];
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_not_one_subtree:[0-9]+]] @not_one_subtree(%[[VALUE_i_4:[0-9]+]] i: i32, %[[VALUE_a_4:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         add<i32>(read<i32>(%[[VALUE_i_4]]) [macro=TIMES2#12 arg0], mul<i32>(read<i32>(%[[VALUE_a_4]]) [macro=TIMES2#12 arg0], const<i32>(2) [macro=TIMES2#12 body]) [macro=TIMES2#12 body]) [macro=TIMES2#12 body];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_name_argument_stays_inline:[0-9]+]] @name_argument_stays_inline(%[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]]) -> void [linkage=external] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         macro_body #[[#NODE11:]] {
// IR-NEXT:             call<void>(%[[VALUE_sink]], widen<i64>(read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(macro_arg(#[[#NODE11]], 0, %[[VALUE_s]]))) [macro=FIELD#13 body]) [macro=FIELD#13 body]) [macro=FIELD#13 body];
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_bit_field_through_a_place_hole:[0-9]+]] @bit_field_through_a_place_hole(%[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]], %[[VALUE_i_5:[0-9]+]] i: i32) -> void [linkage=external] [abi=sysv64(native_c, scalar) -> void] [fallthrough=ret_void] {
// IR-NEXT:         macro_body #[[#NODE12:]] {
// IR-NEXT:             write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(macro_arg(#[[#NODE12]], 0, %[[VALUE_s_2]])), macro_arg<i32>(#[[#NODE12]], 1, read<i32>(%[[VALUE_i_5]]) [macro=BITS#14 arg1]) [macro=BITS#14 arg1]) [macro=BITS#14 body];
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_value_position_has_no_region_yet:[0-9]+]] @value_position_has_no_region_yet(%[[VALUE_i_6:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = macro_value<i32>(#[[#NODE13:]], statements=0, add<i32>(macro_arg<i32>(#{{[0-9]+}}, 0, read<i32>(%[[VALUE_i_6]]) [macro=ADD_ONE#15 arg0]) [macro=ADD_ONE#15 arg0], const<i32>(1) [macro=ADD_ONE#15 body]) [macro=ADD_ONE#15 body]) [macro=ADD_ONE#15 body];
// IR-NEXT:         call<void>(%[[VALUE_sink]], widen<i64>(read<i32>(%[[VALUE_k]])));
// IR-NEXT:     }
// IR-NEXT:     macro_definition #[[#NODE0]] ADD_ONE(x) [defined=7:1] = "( ( x ) + 1 )";
// IR-NEXT:     macro_definition #[[#NODE1]] SET(p, v) [defined=7:2] = "do { * ( p ) = ( v ) ; } while ( 0 )";
// IR-NEXT:     macro_definition #[[#NODE2]] ASSIGN(l, v) [defined=7:3] = "( ( l ) = ( v ) )";
// IR-NEXT:     macro_definition #[[#NODE3]] TEMP(x) [defined=7:4] = "do { int temp = 10 ; sink ( x ) ; } while ( 0 )";
// IR-NEXT:     macro_definition #[[#NODE4]] OUTER(x) [defined=7:6] = "do { INNER ( x ) ; INNER ( ( x ) + 1 ) ; } while ( 0 )";
// IR-NEXT:     macro_definition #[[#NODE5]] INNER(x) [defined=7:5] = "sink ( x )";
// IR-NEXT:     macro_definition #[[#NODE14:]] TWO(a, b) [defined=7:8] = "sink ( a ) ; sink ( b )";
// IR-NEXT:     macro_definition #[[#NODE6]] TIMES2(x) [defined=7:7] = "x * 2";
// IR-NEXT:     macro_definition #[[#NODE7]] FIELD(s, f) [defined=7:9] = "sink ( ( s ) . f )";
// IR-NEXT:     macro_definition #[[#NODE8]] BITS(s, v) [defined=7:10] = "( ( s ) . bits = ( v ) )";
// IR-NEXT:     macro_expansion #[[#NODE0]] ADD_ONE [definition=#[[#NODE0]]] [site=7:503+10] [arguments=511+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE1]] ADD_ONE [definition=#[[#NODE0]]] [site=7:519+10] [arguments=527+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE2]] ADD_ONE [definition=#[[#NODE0]]] [site=7:535+10] [arguments=543+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE3]] SET [definition=#[[#NODE1]]] [site=7:606+10] [arguments=610+2, 614+1] [covers=statements] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE4]] ASSIGN [definition=#[[#NODE2]]] [site=7:622+12] [arguments=629+1, 632+1] [covers=expr] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE5]] ASSIGN [definition=#[[#NODE2]]] [site=7:647+14] [arguments=654+1, 657+3] [covers=expr] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE14]] TEMP [definition=#[[#NODE3]]] [site=7:730+14] [arguments=735+8] [covers=statements] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE6]] TEMP [definition=#[[#NODE3]]] [site=7:750+7] [arguments=755+1] [covers=statements] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE7]] OUTER [definition=#[[#NODE4]]] [site=7:787+8] [arguments=793+1] [covers=statements] [arg0=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE8]] INNER [definition=#[[#NODE5]]] [parent=#[[#NODE7]]] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE9]] INNER [definition=#[[#NODE5]]] [parent=#[[#NODE7]]] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE10]] TWO [definition=#[[#NODE14]]] [site=7:844+9] [arguments=848+1, 851+1] [covers=statements] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE15:]] TIMES2 [definition=#[[#NODE6]]] [site=7:899+13] [arguments=906+5] [covers=expr] [arg0=split];
// IR-NEXT:     macro_expansion #[[#NODE11]] FIELD [definition=#[[#NODE7]]] [site=7:967+14] [arguments=973+1, 976+4] [covers=expr] [arg0=expr] [arg1=name];
// IR-NEXT:     macro_expansion #[[#NODE12]] BITS [definition=#[[#NODE8]]] [site=7:1047+10] [arguments=1052+1, 1055+1] [covers=expr] [arg0=expr] [arg1=expr];
// IR-NEXT:     macro_expansion #[[#NODE13]] ADD_ONE [definition=#[[#NODE0]]] [site=7:1121+10] [arguments=1129+1] [covers=expr] [arg0=expr];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
