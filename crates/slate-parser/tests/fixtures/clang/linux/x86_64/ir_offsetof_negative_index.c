// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu17

struct s { int a[4]; int b[2][3]; struct { int x; int y; } c[2]; };

unsigned long before_array(void) { return __builtin_offsetof(struct s, a[-1]); }
unsigned long inner_index(void) { return __builtin_offsetof(struct s, b[1][-2]); }
unsigned long then_member(void) { return __builtin_offsetof(struct s, c[-1].y); }

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
// IR-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// IR-NEXT:         field0 a: array<i32, 4>;
// IR-NEXT:         field1 b: array<array<i32, 3>, 2>;
// IR-NEXT:         field2 c: array<@type[[TYPE0:[0-9]+]], 2>;
// IR-NEXT:     } [size=56, align=4, offsets=[0, 16, 40]];
// IR-NEXT:     type @type[[TYPE0]] = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %[[VALUE_before_array:[0-9]+]] @before_array() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(18446744073709551612);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_inner_index:[0-9]+]] @inner_index() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(20);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_then_member:[0-9]+]] @then_member() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(36);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
