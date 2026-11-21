// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-macros
typedef unsigned char uch;

#define LONG(x) ((long)(x))
#define SAME(x) ((int)(x))
#define BYTE(x) ((uch)(x))
#define OUTER(x) ((long)(int)(x))

struct box {
  unsigned short value;
};

long casts(int i, unsigned u, struct box *b) {
  long widened = LONG(i);
  int same = SAME(i);
  int reinterpreted = SAME(u);
  uch narrowed = BYTE(i);
  uch from_place = BYTE(b->value);
  long nested = OUTER(u);
  return widened + same + reinterpreted + narrowed + from_place + nested;
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
// IR-NEXT:     type @type[[TYPE_uch:[0-9]+]] uch = u8;
// IR-NEXT:     type @type[[TYPE_box:[0-9]+]] box = struct {
// IR-NEXT:         field0 value: u16;
// IR-NEXT:     } [size=2, align=2, offsets=[0]];
// IR-NEXT:     fn %[[VALUE_casts:[0-9]+]] @casts(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_u:[0-9]+]] u: u32, %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_box]]>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_widened:[0-9]+]] widened: i64 [storage=automatic] = widen<i64>(read<i32>(%[[VALUE_i]]) [macro_arg="#[[#NODE0:]].0"] [macro=LONG#0 arg0]) [macro_cast="i64"] [macro_value="#{{[0-9]+}}"] [macro=LONG#0 body];
// IR-NEXT:         let %[[VALUE_same:[0-9]+]] same: i32 [storage=automatic] = read<i32>(%[[VALUE_i]]) [macro_cast="i32"] [macro_arg="#[[#NODE1:]].0"] [macro_value="#{{[0-9]+}}"] [macro=SAME#1 arg0];
// IR-NEXT:         let %[[VALUE_reinterpreted:[0-9]+]] reinterpreted: i32 [storage=automatic] = reinterpret<i32>(read<u32>(%[[VALUE_u]]) [macro_arg="#[[#NODE2:]].0"] [macro=SAME#2 arg0]) [macro_cast="i32"] [macro_value="#{{[0-9]+}}"] [macro=SAME#2 body];
// IR-NEXT:         let %[[VALUE_narrowed:[0-9]+]] narrowed: u8 [storage=automatic] = reinterpret<u8>(truncate<i8>(read<i32>(%[[VALUE_i]]) [macro_arg="#[[#NODE3:]].0"] [macro=BYTE#3 arg0]) [macro=BYTE#3 body]) [c="uch"] [macro_cast="u8"] [macro_value="#{{[0-9]+}}"] [macro=BYTE#3 body] [c="uch"];
// IR-NEXT:         let %[[VALUE_from_place:[0-9]+]] from_place: u8 [storage=automatic] = truncate<u8>(read<u16>(field0(deref(read<ptr<@type[[TYPE_box]]>>(%[[VALUE_b]])))) [macro_arg="#[[#NODE4:]].0"] [macro=BYTE#4 arg0]) [c="uch"] [macro_cast="u8"] [macro_value="#{{[0-9]+}}"] [macro=BYTE#4 body] [c="uch"];
// IR-NEXT:         let %[[VALUE_nested:[0-9]+]] nested: i64 [storage=automatic] = widen<i64>(reinterpret<i32>(read<u32>(%[[VALUE_u]]) [macro_arg="#[[#NODE5:]].0"] [macro=OUTER#5 arg0]) [macro_cast="i32"] [macro=OUTER#5 body]) [macro_value="#{{[0-9]+}}"] [macro=OUTER#5 body];
// IR-NEXT:         return add<i64>(add<i64>(add<i64>(add<i64>(add<i64>(read<i64>(%[[VALUE_widened]]), widen<i64>(read<i32>(%[[VALUE_same]]))), widen<i64>(read<i32>(%[[VALUE_reinterpreted]]))), widen<i64>(reinterpret<i32>(widen<u32>(read<u8>(%[[VALUE_narrowed]]) [c="uch"])))), widen<i64>(reinterpret<i32>(widen<u32>(read<u8>(%[[VALUE_from_place]]) [c="uch"])))), read<i64>(%[[VALUE_nested]]));
// IR-NEXT:     }
// IR-NEXT:     macro_definition #[[#NODE0]] LONG(x) [defined=7:2] = "( ( long ) ( x ) )";
// IR-NEXT:     macro_definition #[[#NODE1]] SAME(x) [defined=7:3] = "( ( int ) ( x ) )";
// IR-NEXT:     macro_definition #[[#NODE2]] BYTE(x) [defined=7:4] = "( ( uch ) ( x ) )";
// IR-NEXT:     macro_definition #[[#NODE3]] OUTER(x) [defined=7:5] = "( ( long ) ( int ) ( x ) )";
// IR-NEXT:     macro_expansion #[[#NODE0]] LONG [definition=#[[#NODE0]]] [site=7:250+7] [arguments=255+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE1]] SAME [definition=#[[#NODE1]]] [site=7:272+7] [arguments=277+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE2]] SAME [definition=#[[#NODE1]]] [site=7:303+7] [arguments=308+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE3]] BYTE [definition=#[[#NODE2]]] [site=7:329+7] [arguments=334+1] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE4]] BYTE [definition=#[[#NODE2]]] [site=7:357+14] [arguments=362+8] [covers=expr] [arg0=expr];
// IR-NEXT:     macro_expansion #[[#NODE5]] OUTER [definition=#[[#NODE3]]] [site=7:389+8] [arguments=395+1] [covers=expr] [arg0=expr];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
