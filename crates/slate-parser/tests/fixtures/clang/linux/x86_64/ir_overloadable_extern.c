// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

struct point { int x; };
union value { int i; float f; };
enum color { RED };

int area(int w) __attribute__((overloadable));
int area(int w, int h) __attribute__((overloadable));
double area(double r) __attribute__((overloadable));
int area(const char *s) __attribute__((overloadable));
int area(const char *s, const char *t) __attribute__((overloadable));
int area(struct point *p, struct point *q) __attribute__((overloadable));
int area(const union value *v, enum color c) __attribute__((overloadable));
int area(int (*fn)(int, int), int xs[4], int ys[]) __attribute__((overloadable));
int area(void) __attribute__((overloadable));
int area(long double, ...) __attribute__((overloadable));
int area(_Complex double z) __attribute__((overloadable));
int area(unsigned long long a, long long b, unsigned char c, signed char d, short e)
    __attribute__((overloadable));

int use(struct point *p) {
  return area(1) + area(1, 2) + (int)area(1.0) + area("a") + area("a", "b") +
         area(p, p);
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
// IR-NEXT:     type @type[[TYPE_point:[0-9]+]] point = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_value:[0-9]+]] value = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_color:[0-9]+]] color = enum : u32 {
// IR-NEXT:         %[[VALUE_RED:[0-9]+]] RED = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// IR-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// IR-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_area:[0-9]+]] @area(%[[VALUE_w:[0-9]+]] w: i32) -> i32 [linkage=external] [asm_name="_Z4areai"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_2:[0-9]+]] @area(%[[VALUE_w_2:[0-9]+]] w: i32, %[[VALUE_h:[0-9]+]] h: i32) -> i32 [linkage=external] [asm_name="_Z4areaii"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_3:[0-9]+]] @area(%[[VALUE_r:[0-9]+]] r: f64) -> f64 [linkage=external] [asm_name="_Z4aread"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_4:[0-9]+]] @area(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=external] [asm_name="_Z4areaPKc"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_5:[0-9]+]] @area(%[[VALUE_s_2:[0-9]+]] s: ptr<const i8>, %[[VALUE_t:[0-9]+]] t: ptr<const i8>) -> i32 [linkage=external] [asm_name="_Z4areaPKcS0_"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_6:[0-9]+]] @area(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_point]]>, %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_point]]>) -> i32 [linkage=external] [asm_name="_Z4areaP5pointS0_"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_7:[0-9]+]] @area(%[[VALUE_v:[0-9]+]] v: ptr<const @type[[TYPE_value]]>, %[[VALUE_c:[0-9]+]] c: @type[[TYPE_color]]) -> i32 [linkage=external] [asm_name="_Z4areaPK5value5color"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_8:[0-9]+]] @area(%[[VALUE_fn:[0-9]+]] fn: ptr<fn(i32, i32) -> i32>, %[[VALUE_xs:[0-9]+]] xs: ptr<i32> [array=4], %[[VALUE_ys:[0-9]+]] ys: ptr<i32>) -> i32 [linkage=external] [asm_name="_Z4areaPFiiiEPiS1_"] [overloadable] [c="int(int (*)(int, int), int[4], int[])"];
// IR-NEXT:     fn %[[VALUE_area_9:[0-9]+]] @area() -> i32 [linkage=external] [asm_name="_Z4areav"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_10:[0-9]+]] @area(%[[VALUE0:[0-9]+]] <unnamed>: f80, ...) -> i32 [linkage=external] [asm_name="_Z4areaez"] [overloadable];
// IR-NEXT:     fn %[[VALUE_area_11:[0-9]+]] @area(%[[VALUE_z:[0-9]+]] z: complex<f64>) -> i32 [linkage=external] [asm_name="_Z4areaCd"] [overloadable] [abi=sysv64(native_c) -> scalar];
// IR-NEXT:     fn %[[VALUE_area_12:[0-9]+]] @area(%[[VALUE_a:[0-9]+]] a: u64, %[[VALUE_b:[0-9]+]] b: i64, %[[VALUE_c_2:[0-9]+]] c: u8, %[[VALUE_d:[0-9]+]] d: i8, %[[VALUE_e:[0-9]+]] e: i16) -> i32 [linkage=external] [asm_name="_Z4areayxhas"] [overloadable];
// IR-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_point]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_area]], const<i32>(1)), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_area_2]], const<i32>(1), const<i32>(2))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_area_3]], const<f64>(1.0)))), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_area_4]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])))), call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_area_5]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]])))), call<i32, signature=fn(ptr<@type[[TYPE_point]]>, ptr<@type[[TYPE_point]]>) -> i32>(%[[VALUE_area_6]], read<ptr<@type[[TYPE_point]]>>(%[[VALUE_p_2]]), read<ptr<@type[[TYPE_point]]>>(%[[VALUE_p_2]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
