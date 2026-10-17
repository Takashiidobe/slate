#include <stdarg.h>
struct a { int c; };
struct b { long x, y; };
struct c { double d; long l; };
struct d { float f[4]; };
struct e { long x, y, z; };
struct f { char c[3]; };
union u { double d; int i; };
void g(int n, ...) {
  va_list ap; va_start(ap, n);
  struct a a = va_arg(ap, struct a);
  struct b b = va_arg(ap, struct b);
  struct c c = va_arg(ap, struct c);
  struct d d = va_arg(ap, struct d);
  struct e e = va_arg(ap, struct e);
  struct f f = va_arg(ap, struct f);
  union u u = va_arg(ap, union u);
  int i = va_arg(ap, int);
  va_end(ap);
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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_a:[0-9]+]] a = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_b:[0-9]+]] b = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_c:[0-9]+]] c = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 l: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_d:[0-9]+]] d = struct {
// DEFAULT-NEXT:         field0 f: array<f32, 4>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:         field2 z: i64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_f:[0-9]+]] f = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 3>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_n:[0-9]+]] n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_a]] [storage=automatic] = copy<@type[[TYPE_a]], reason=assign>(va_arg<@type[[TYPE_a]], regs=[int]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_b]] [storage=automatic] = copy<@type[[TYPE_b]], reason=assign>(va_arg<@type[[TYPE_b]], regs=[int, int]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_c]] [storage=automatic] = copy<@type[[TYPE_c]], reason=assign>(va_arg<@type[[TYPE_c]], regs=[sse, int]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE_d]] [storage=automatic] = copy<@type[[TYPE_d]], reason=assign>(va_arg<@type[[TYPE_d]], regs=[sse, sse]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: @type[[TYPE_e]] [storage=automatic] = copy<@type[[TYPE_e]], reason=assign>(va_arg<@type[[TYPE_e]], memory>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: @type[[TYPE_f]] [storage=automatic] = copy<@type[[TYPE_f]], reason=assign>(va_arg<@type[[TYPE_f]], regs=[int]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_u]] [storage=automatic] = copy<@type[[TYPE_u]], reason=assign>(va_arg<@type[[TYPE_u]], regs=[int]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
