typedef int T, A[sizeof(T)];

int own_initializer(void) {
  int T = sizeof(T);
  return T;
}

int next_declarator(void) {
  int T = 2, x = sizeof(T);
  return x;
}

int own_bound(void) {
  int T[sizeof(T)], x = sizeof(T);
  return x;
}

int parameters(int T, int a[sizeof(T)]) {
  return sizeof(T);
}

int prototype(int T, int a[sizeof(T)]);
T after_prototype;
int nested(int (*callback)(int T, int a[sizeof(T)]), T value);
int nested_shadow(int T, int (*callback)(int a[sizeof(T)]), int b[sizeof(T)]);
T after_nested;
int parameter_bound(int T[sizeof(T)], int a[sizeof(T)]) {
  return sizeof(T);
}

int enumeration(void) {
  enum { T = sizeof(T), U = sizeof(T) };
  return sizeof(T) + U;
}

int multiline_enumeration(void) {
  enum local {
    T = 2,
    U = sizeof(T)
  };
  return sizeof(T) + U;
}

int parameter_enum(enum { T = 2 } value, int a[sizeof(T)]) {
  return sizeof(T);
}
T after_parameter_enum;
int prototype_enum(enum { T = 2 } value, int a[sizeof(T)]);
T after_prototype_enum;

int nested_enum(int (*callback)(enum { T = 2 } value), T value) {
  return sizeof(T);
}

int (*return_callback(enum { T = 2 } value))(int argument) {
  if (sizeof(T)) return 0;
  return 0;
}
T after_return_callback;

int field_enum(void) {
  struct { enum { T = 2 } value; int a[sizeof(T)]; } object;
  return sizeof(T) + sizeof(object);
}

int loops(void) {
  for (int T = sizeof(T); sizeof(T); T += sizeof(T)) {
    for (int T[sizeof(T)]; sizeof(T); )
      return sizeof(T);
    return sizeof(T);
  }
  for (int T = 0; sizeof(T); ++T)
    T += sizeof(T);
  return sizeof(T);
}

int members(void) {
  struct { int T; int a[sizeof(T)]; } object;
  return sizeof(T) + sizeof(object);
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
// DEFAULT-NEXT:     type @type1 A = array<i32, 4>;
// DEFAULT-NEXT:     type @type2 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(4);
// DEFAULT-NEXT:         %1 U = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type3 local = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:         %1 U = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type5 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type6 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type7 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type8 = struct {
// DEFAULT-NEXT:         field0 value: @type9;
// DEFAULT-NEXT:         field1 a: array<i32, 4>;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type9 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type10 = struct {
// DEFAULT-NEXT:         field0 T: i32;
// DEFAULT-NEXT:         field1 a: array<i32, 4>;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %16 after_prototype: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %27 after_nested: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %44 after_parameter_enum: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %50 after_prototype_enum: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %61 after_return_callback: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @own_initializer() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 T: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @next_declarator() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %6 x: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @own_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 T: array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %9 x: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(16)));
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @parameters(%11 T: i32, %12 a: ptr<i32> [array=4]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @prototype(%74 T: i32, %75 a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @nested(%76 callback: ptr<fn(i32, ptr<i32>) -> i32>, %77 value: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @nested_shadow(%78 T: i32, %79 callback: ptr<fn(ptr<i32>) -> i32>, %80 b: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %28 @parameter_bound(%29 T: ptr<i32> [array=4], %30 a: ptr<i32> [array=8]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @enumeration() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @multiline_enumeration() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @parameter_enum(%42 value: @type4, %43 a: ptr<i32> [array=4]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @prototype_enum(%81 value: @type5, %82 a: ptr<i32> [array=4]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %51 @nested_enum(%55 callback: ptr<fn(@type6) -> i32>, %56 value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @return_callback(%60 value: @type7) -> ptr<fn(i32) -> i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(0))
// DEFAULT-NEXT:             return null<ptr<fn(i32) -> i32>>;
// DEFAULT-NEXT:         return null<ptr<fn(i32) -> i32>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @field_enum() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %66 object: @type8 [storage=automatic];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), const<u64>(20))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %67 @loops() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %83
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %68 T: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:             condition: ne<u64>(const<u64>(4), const<u64>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %86: i32 [synthetic] = read<i32>(%68);
// DEFAULT-NEXT:                 let %87: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%86))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%68, read<i32>(%87));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %84
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %69 T: array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:                         condition: ne<u64>(const<u64>(16), const<u64>(0))
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(16)));
// DEFAULT-NEXT:                     return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %85
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %70 T: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: ne<u64>(const<u64>(4), const<u64>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %88: i32 [synthetic] = read<i32>(%70);
// DEFAULT-NEXT:                 let %89: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%88), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%70, read<i32>(%89));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %90: i32 [synthetic] = read<i32>(%70);
// DEFAULT-NEXT:                 let %91: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%90))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%70, read<i32>(%91));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @members() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %73 object: @type10 [storage=automatic];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), const<u64>(20))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
