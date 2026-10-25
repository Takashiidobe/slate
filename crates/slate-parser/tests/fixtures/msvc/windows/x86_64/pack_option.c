// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir /Zp2
#include <stddef.h>

struct mixed { char c; int i; short h; double d; };
_Static_assert(sizeof(struct mixed) == 16, "/Zp2 caps member alignment at 2");
_Static_assert(offsetof(struct mixed, i) == 2, "/Zp2 caps member alignment at 2");

#pragma pack(push, 4)
struct pushed { char c; double d; };
_Static_assert(sizeof(struct pushed) == 12, "#pragma pack overrides /Zp");
#pragma pack()
struct reset { char c; double d; };
_Static_assert(sizeof(struct reset) == 10, "#pragma pack() restores the /Zp value");
#pragma pack(pop)

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// IR-NEXT:     type @type[[TYPE_size_t_2:[0-9]+]] size_t = u64;
// IR-NEXT:     type @type[[TYPE_mixed:[0-9]+]] mixed = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 i: i32;
// IR-NEXT:         field2 h: i16;
// IR-NEXT:         field3 d: f64;
// IR-NEXT:     } [size=16, align=2, offsets=[0, 2, 6, 8]];
// IR-NEXT:     type @type[[TYPE_pushed:[0-9]+]] pushed = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 d: f64;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_reset:[0-9]+]] reset = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 d: f64;
// IR-NEXT:     } [size=10, align=2, offsets=[0, 2]];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
