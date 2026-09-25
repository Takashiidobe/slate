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
// DEFAULT-NEXT:     type @type0 T = i32;
// DEFAULT-NEXT:     type @type1 T = i64;
// DEFAULT-NEXT:     type @type2 T = struct {
// DEFAULT-NEXT:         field0 T: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %9 after_function: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 after_tags_and_members: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 after_prototype: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 bits: i32b [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 aligned_type: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 aligned_expression: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @nested_scopes(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 outer: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %4 T: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             let %26: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:             let %27: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%26))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%2, read<i32>(%27));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%28))), const<u64>(8))));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%29));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %30: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:             let %31: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%30))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%2, read<i32>(%31));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%32))), const<u64>(4))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%33));
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 T: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%6))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%35));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %36: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                     let %37: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%36))), const<u64>(4))));
// DEFAULT-NEXT:                     write<i32>(%2, read<i32>(%37));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%2))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%3))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @parameter_scope(%8 T: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @statement_expression() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %38: u64 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %11 T: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             write<u64>(%38, const<u64>(4));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(read<u64>(%38), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @prototype(%24 callback: ptr<fn(i32) -> i32>, %25 value: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @attribute_scope() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 T: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 aligned_object: i32 [storage=automatic];
// DEFAULT-NEXT:         let %22 local_bits: i32b [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
