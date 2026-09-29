struct { int x; } g1, g2;
struct G { int y; } g = {1}, h = {2};
static struct S { int z; } s;
struct F { int w; } (*fp)(void);
typedef struct { int t; } T, *PT;
enum E { E1, E2 } e1 = E1, e2 = {E2};
int a1, *b1 = &a1;
typedef int I, *IP;

struct Bits {
  unsigned lo : 3, hi : 5;
  int x, *y;
};

int f(void) {
  int a = 1, b = 2;
  IP p = &a;
  for (int i = 0, j = 1; i < j; i++) {
    b += i;
  }
  return a + b + *p;
}

int main(void) {
  PT pt = 0;
  T t = {0};
  return f() + g1.x + g2.x + g.y + s.z + t.t + (pt != 0) + e1 + e2 + (fp != 0) + *b1 - 4;
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_G:[0-9]+]] G = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 z: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = struct {
// DEFAULT-NEXT:         field0 w: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 t: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE_PT:[0-9]+]] PT = ptr<@type[[TYPE1]]>;
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1:[0-9]+]] E1 = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_E2:[0-9]+]] E2 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_I:[0-9]+]] I = i32;
// DEFAULT-NEXT:     type @type[[TYPE_IP:[0-9]+]] IP = ptr<i32>;
// DEFAULT-NEXT:     type @type[[TYPE_Bits:[0-9]+]] Bits = struct {
// DEFAULT-NEXT:         field0 lo: u32 : 3;
// DEFAULT-NEXT:         field1 hi: u32 : 5;
// DEFAULT-NEXT:         field2 x: i32;
// DEFAULT-NEXT:         field3 y: ptr<i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 4, 8], bit_offsets=[Some(0), Some(3), None, None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None, None]];
// DEFAULT-NEXT:     global %[[VALUE_E2]] g1: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2:[0-9]+]] g2: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: @type[[TYPE_G]] [storage=static] = aggregate<@type[[TYPE_G]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: @type[[TYPE_G]] [storage=static] = aggregate<@type[[TYPE_G]], zero_fill=false>(field0 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_fp:[0-9]+]] fp: ptr<fn() -> @type[[TYPE_F]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e1:[0-9]+]] e1: @type[[TYPE_E]] [storage=static] = int_to_enum<@type[[TYPE_E]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e2:[0-9]+]] e2: @type[[TYPE_E]] [storage=static] = int_to_enum<@type[[TYPE_E]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a1:[0-9]+]] a1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b1:[0-9]+]] b1: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_a1]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_pt:[0-9]+]] pt: ptr<@type[[TYPE1]]> [storage=automatic] = null<ptr<@type[[TYPE1]]>>;
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE1]] [storage=automatic] = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = const<i32>(0));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_f]]), read<i32>(field0(%[[VALUE_E2]]))), read<i32>(field0(%[[VALUE_g2]]))), read<i32>(field0(%[[VALUE_g]]))), read<i32>(field0(%[[VALUE_s]]))), read<i32>(field0(%[[VALUE_t]]))), from_bool<i32, reason=promotion>(ne<ptr<@type[[TYPE1]]>>(read<ptr<@type[[TYPE1]]>>(%[[VALUE_pt]]), null<ptr<@type[[TYPE1]]>>)))), enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e1]]))), enum_to_int<u32, reason=promotion>(read<@type[[TYPE_E]]>(%[[VALUE_e2]]))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(ne<ptr<fn() -> @type[[TYPE_F]]>>(read<ptr<fn() -> @type[[TYPE_F]]>>(%[[VALUE_fp]]), null<ptr<fn() -> @type[[TYPE_F]]>>)))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_b1]]))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
