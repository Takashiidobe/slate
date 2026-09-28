// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-DEFINES NPOT NPOT
// SLATE-FILECHECK-IR-ERROR NPOT
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
// SLATE-FILECHECK-STD NPOT c23

#define MS __attribute__((ms_struct))
struct MS A { char c; long long x; double d[2]; };
struct MS B { char c; _Complex double z; };
enum __attribute__((packed)) P { Q };
struct MS C { char c; _BitInt(64) b; };
struct MS D { char c; long long b:3; };
struct MS E { char c; long long x __attribute__((packed)); };
#pragma pack(2)
struct MS F { char c; long long x; };
#pragma pack()
struct G { char c; long long x; };
struct MS H { struct G g; char c; };

#ifdef NPOT
struct MS L { char c; long double d; };
#endif

// SLATE-FILECHECK-BEGIN NPOT
// NPOT: Error:   × semantic analysis failed
// NPOT: Error:
// NPOT: × ms_struct layout of a fundamental type whose size is not a power of two
// NPOT: ╭─[tests/fixtures/clang/linux/i686/ms_struct_layout.c:16:1]
// NPOT: 15 │ #ifdef NPOT
// NPOT: 16 │ struct MS L { char c; long double d; };
// NPOT: · ───────────────────────────────────────
// NPOT: 17 │ #endif
// NPOT: ╰────
// SLATE-FILECHECK-END NPOT
// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 A = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 x: i64;
// IR-NEXT:         field2 d: array<f64, 2>;
// IR-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     type @type1 B = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 z: complex<f64>;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type2 P = enum : u32 {
// IR-NEXT:         %0 Q = const<i32>(0);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type3 C = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 b: i64b;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type4 D = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 b: i64 : 3;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 8)], field_units=[None, Some(0)]];
// IR-NEXT:     type @type5 E = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 x: i64;
// IR-NEXT:     } [size=9, align=1, offsets=[0, 1]];
// IR-NEXT:     type @type6 F = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 x: i64;
// IR-NEXT:     } [size=10, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type7 G = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 x: i64;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type8 H = struct {
// IR-NEXT:         field0 g: @type7;
// IR-NEXT:         field1 c: i8;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 12]];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
