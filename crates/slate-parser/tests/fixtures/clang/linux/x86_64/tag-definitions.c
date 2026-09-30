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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// DEFAULT-NEXT:         field0 nested: @type[[TYPE_inner:[0-9]+]];
// DEFAULT-NEXT:         field1 color: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_inner]] inner = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_RED:[0-9]+]] RED = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_GREEN:[0-9]+]] GREEN = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE_local:[0-9]+]] local = struct {
// DEFAULT-NEXT:         field0 z: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_mode:[0-9]+]] mode = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_RED]] OFF = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_GREEN]] ON = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_GREEN]] g1: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2:[0-9]+]] g2: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_local_tags:[0-9]+]] @local_tags() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_local]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_local]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: @type[[TYPE_mode]] [storage=automatic] = int_to_enum<@type[[TYPE_mode]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_mode]]>(%[[VALUE_m]])));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_u]]), read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_t]]), read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field0(%[[VALUE_t]])))), const<u64>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_takes:[0-9]+]] @takes(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_inner]]>, %[[VALUE_q:[0-9]+]] q: @type[[TYPE2]]) -> void [linkage=external] [abi=sysv64(scalar, native_c) -> void];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
