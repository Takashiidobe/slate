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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = array<i32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T:[0-9]+]] T = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_U:[0-9]+]] U = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_local:[0-9]+]] local = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_U]] U = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE4:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE5:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 value: @type[[TYPE6:[0-9]+]];
// DEFAULT-NEXT:         field1 a: array<i32, 4>;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE6]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE7:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 T: i32;
// DEFAULT-NEXT:         field1 a: array<i32, 4>;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_after_prototype:[0-9]+]] after_prototype: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after_nested:[0-9]+]] after_nested: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after_parameter_enum:[0-9]+]] after_parameter_enum: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after_prototype_enum:[0-9]+]] after_prototype_enum: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after_return_callback:[0-9]+]] after_return_callback: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_own_initializer:[0-9]+]] @own_initializer() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_T_2:[0-9]+]] T: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_T_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_next_declarator:[0-9]+]] @next_declarator() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_T_3:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_own_bound:[0-9]+]] @own_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_T_4:[0-9]+]] T: array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(16)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parameters:[0-9]+]] @parameters(%[[VALUE_T_5:[0-9]+]] T: i32, %[[VALUE_a:[0-9]+]] a: ptr<i32> [array=4]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_prototype:[0-9]+]] @prototype(%[[VALUE_T_6:[0-9]+]] T: i32, %[[VALUE_a_2:[0-9]+]] a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(i32, ptr<i32>) -> i32>, %[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested_shadow:[0-9]+]] @nested_shadow(%[[VALUE_T_7:[0-9]+]] T: i32, %[[VALUE_callback_2:[0-9]+]] callback: ptr<fn(ptr<i32>) -> i32>, %[[VALUE_b:[0-9]+]] b: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_parameter_bound:[0-9]+]] @parameter_bound(%[[VALUE_T_8:[0-9]+]] T: ptr<i32> [array=4], %[[VALUE_a_3:[0-9]+]] a: ptr<i32> [array=8]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_enumeration:[0-9]+]] @enumeration() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_multiline_enumeration:[0-9]+]] @multiline_enumeration() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parameter_enum:[0-9]+]] @parameter_enum(%[[VALUE_value_2:[0-9]+]] value: @type[[TYPE1]], %[[VALUE_a_4:[0-9]+]] a: ptr<i32> [array=4]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_prototype_enum:[0-9]+]] @prototype_enum(%[[VALUE_value_3:[0-9]+]] value: @type[[TYPE2]], %[[VALUE_a_5:[0-9]+]] a: ptr<i32> [array=4]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested_enum:[0-9]+]] @nested_enum(%[[VALUE_callback_3:[0-9]+]] callback: ptr<fn(@type[[TYPE3]]) -> i32>, %[[VALUE_value_4:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_return_callback:[0-9]+]] @return_callback(%[[VALUE_value_5:[0-9]+]] value: @type[[TYPE4]]) -> ptr<fn(i32) -> i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(0))
// DEFAULT-NEXT:             return null<ptr<fn(i32) -> i32>>;
// DEFAULT-NEXT:         return null<ptr<fn(i32) -> i32>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_field_enum:[0-9]+]] @field_enum() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_object:[0-9]+]] object: @type[[TYPE5]] [storage=automatic];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), const<u64>(20))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_loops:[0-9]+]] @loops() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_T_9:[0-9]+]] T: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:             condition: ne<u64>(const<u64>(4), const<u64>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_T_9]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE1]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_T_9]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_T_10:[0-9]+]] T: array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:                         condition: ne<u64>(const<u64>(16), const<u64>(0))
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(16)));
// DEFAULT-NEXT:                     return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_T_11:[0-9]+]] T: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: ne<u64>(const<u64>(4), const<u64>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_T_11]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_T_11]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_T_11]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE7]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_T_11]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_members:[0-9]+]] @members() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_object_2:[0-9]+]] object: @type[[TYPE7]] [storage=automatic];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), const<u64>(20))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
