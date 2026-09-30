typedef int T;

int nested_scopes(int x) {
  T outer = 0;
  {
    int T = 1;
    x += sizeof(T);
    {
      typedef long T;
      x += sizeof(T);
    }
    x += sizeof(T);
  }
  x += sizeof(T);
  for (int T = 0; T < sizeof(T); T++) {
    x += sizeof(T);
  }
  return x + sizeof(T) + outer;
}

int parameter_scope(int T) {
  return sizeof(T);
}

T after_function;

int statement_expression(void) {
  return ({ int T = 1; sizeof(T); }) + sizeof(T);
}

struct T { int T; };
T after_tags_and_members;

int prototype(int (*callback)(int T), T value);
T after_prototype;

_BitInt(sizeof(T) * 8) bits;
int aligned_type __attribute__((aligned(sizeof(T))));
_Alignas(sizeof(T)) int aligned_expression;

void attribute_scope(void) {
  int T;
  int aligned_object __attribute__((aligned(sizeof(T))));
  _BitInt(sizeof(T) * 8) local_bits;
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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_2:[0-9]+]] T = i64;
// DEFAULT-NEXT:     type @type[[TYPE_T_3:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 T: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_after_function:[0-9]+]] after_function: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after_tags_and_members:[0-9]+]] after_tags_and_members: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after_prototype:[0-9]+]] after_prototype: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bits:[0-9]+]] bits: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_aligned_type:[0-9]+]] aligned_type: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_aligned_expression:[0-9]+]] aligned_expression: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested_scopes:[0-9]+]] @nested_scopes(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_outer:[0-9]+]] outer: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_T:[0-9]+]] T: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE0]]))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE2]]))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE4]]))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE6]]))), const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_T_2:[0-9]+]] T: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_T_2]]))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_T_2]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_T_2]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                     let %[[VALUE12:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE11]]))), const<u64>(4))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_x]]))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_outer]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parameter_scope:[0-9]+]] @parameter_scope(%[[VALUE_T_3:[0-9]+]] T: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_statement_expression:[0-9]+]] @statement_expression() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u64 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_T_4:[0-9]+]] T: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             write<u64>(%[[VALUE13]], const<u64>(4));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(read<u64>(%[[VALUE13]]), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_prototype:[0-9]+]] @prototype(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(i32) -> i32>, %[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_attribute_scope:[0-9]+]] @attribute_scope() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_T_5:[0-9]+]] T: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_aligned_object:[0-9]+]] aligned_object: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_local_bits:[0-9]+]] local_bits: i32b [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
