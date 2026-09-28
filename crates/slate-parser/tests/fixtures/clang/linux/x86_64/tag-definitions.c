struct { int y; } g1, g2;

struct outer {
  struct inner { int x; } nested;
  enum { RED, GREEN = RED + 2 } color;
};

typedef struct {
  int a;
  int b;
} pair;

int local_tags(void) {
  struct local { int z; } t, u;
  enum mode { OFF, ON } m = ON;
  t.z = u.z = m;
  return t.z + sizeof(struct { char c; });
}

void takes(struct inner *p, pair q);

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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 outer = struct {
// DEFAULT-NEXT:         field0 nested: @type2;
// DEFAULT-NEXT:         field1 color: @type3;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 inner = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 = enum : u32 {
// DEFAULT-NEXT:         %0 RED = const<i32>(0);
// DEFAULT-NEXT:         %1 GREEN = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type5 pair = @type4;
// DEFAULT-NEXT:     type @type6 local = struct {
// DEFAULT-NEXT:         field0 z: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type7 mode = enum : u32 {
// DEFAULT-NEXT:         %0 OFF = const<i32>(0);
// DEFAULT-NEXT:         %1 ON = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type8 = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %1 g1: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 g2: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %10 @local_tags() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 t: @type6 [storage=automatic];
// DEFAULT-NEXT:         let %13 u: @type6 [storage=automatic];
// DEFAULT-NEXT:         let %17 m: @type7 [storage=automatic] = int_to_enum<@type7, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field0(%13), reinterpret<i32, reason=assign, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type7>(%17))));
// DEFAULT-NEXT:         write<i32>(field0(%12), reinterpret<i32, reason=assign, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type7>(%17))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field0(%12)))), const<u64>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @takes(%22 p: ptr<@type2>, %23 q: @type4) -> void [linkage=external] [abi=sysv64(scalar, native_c) -> void];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
