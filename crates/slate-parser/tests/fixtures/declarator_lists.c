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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 G = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 S = struct {
// DEFAULT-NEXT:         field0 z: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 F = struct {
// DEFAULT-NEXT:         field0 w: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 t: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type5 T = @type4;
// DEFAULT-NEXT:     type @type6 PT = ptr<@type4>;
// DEFAULT-NEXT:     type @type7 E = enum : u32 {
// DEFAULT-NEXT:         %0 E1 = const<i32>(0);
// DEFAULT-NEXT:         %1 E2 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type8 I = i32;
// DEFAULT-NEXT:     type @type9 IP = ptr<i32>;
// DEFAULT-NEXT:     type @type10 Bits = struct {
// DEFAULT-NEXT:         field0 lo: u32 : 3;
// DEFAULT-NEXT:         field1 hi: u32 : 5;
// DEFAULT-NEXT:         field2 x: i32;
// DEFAULT-NEXT:         field3 y: ptr<i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 4, 8], bit_offsets=[Some(0), Some(3), None, None], bit_units=[(0, 1)], field_units=[Some(0), Some(0), None, None]];
// DEFAULT-NEXT:     global %1 g1: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 g2: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 g: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %5 h: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %7 s: @type2 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 fp: ptr<fn() -> @type3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 e1: @type7 [storage=static] = int_to_enum<@type7, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %17 e2: @type7 [storage=static] = int_to_enum<@type7, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %18 a1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 b1: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%18) [linkage=external];
// DEFAULT-NEXT:     fn %23 @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24 a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %25 b: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %26 p: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%24);
// DEFAULT-NEXT:         for %32
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %27 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 let %28 j: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%27), read<i32>(%28))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%27, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %35: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:                     let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), read<i32>(%27));
// DEFAULT-NEXT:                     write<i32>(%25, read<i32>(%36));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%24), read<i32>(%25)), read<i32>(deref(read<ptr<i32>>(%26))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %30 pt: ptr<@type4> [storage=automatic] = null<ptr<@type4>>;
// DEFAULT-NEXT:         let %31 t: @type4 [storage=automatic] = aggregate<@type4, zero_fill=false>(field0 = const<i32>(0));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%23), read<i32>(field0(%1))), read<i32>(field0(%2))), read<i32>(field0(%4))), read<i32>(field0(%7))), read<i32>(field0(%31))), from_bool<i32, reason=promotion>(ne<ptr<@type4>>(read<ptr<@type4>>(%30), null<ptr<@type4>>)))), enum_to_int<u32, reason=promotion>(read<@type7>(%16))), enum_to_int<u32, reason=promotion>(read<@type7>(%17))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(ne<ptr<fn() -> @type3>>(read<ptr<fn() -> @type3>>(%9), null<ptr<fn() -> @type3>>)))), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(deref(read<ptr<i32>>(%19))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
