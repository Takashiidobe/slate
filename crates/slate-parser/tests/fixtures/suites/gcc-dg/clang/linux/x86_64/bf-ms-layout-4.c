/* PR target/52991 */
/* { dg-do compile { target i?86-*-* x86_64-*-* } } */

#define CHECK(expr) extern char c[(expr) ? 1 : -1]
#define offsetof(x, y) __builtin_offsetof (x, y)

struct test_sp1 {
    int a;
    short b;
    int c;
    char d;
} __attribute__((packed,ms_struct));

CHECK (sizeof (struct test_sp1) == 11);
CHECK (offsetof (struct test_sp1, a) == 0);
CHECK (offsetof (struct test_sp1, b) == 4);
CHECK (offsetof (struct test_sp1, c) == 6);
CHECK (offsetof (struct test_sp1, d) == 10);

struct test_sp3 {
    int a;
    short b __attribute__((aligned(8)));
    int c;
    char d;
} __attribute__((packed,ms_struct));

CHECK (sizeof (struct test_sp3) == 16);
CHECK (offsetof (struct test_sp3, a) == 0);
CHECK (offsetof (struct test_sp3, b) == 8);
CHECK (offsetof (struct test_sp3, c) == 10);
CHECK (offsetof (struct test_sp3, d) == 14);

struct test_s4 {
    int a;
    short b;
    int c:15;
    char d;
} __attribute__((ms_struct));

CHECK (sizeof (struct test_s4) == 16);
CHECK (offsetof (struct test_s4, a) == 0);
CHECK (offsetof (struct test_s4, b) == 4);
CHECK (offsetof (struct test_s4, d) == 12);

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type0 test_sp1 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i8;
// DEFAULT-NEXT:     } [size=11, align=1, offsets=[0, 4, 6, 10]];
// DEFAULT-NEXT:     type @type1 test_sp3 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i8;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 10, 14]];
// DEFAULT-NEXT:     type @type2 test_s4 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i16;
// DEFAULT-NEXT:         field2 c: i32 : 15;
// DEFAULT-NEXT:         field3 d: i8;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12], bit_offsets=[None, None, Some(64), None], bit_units=[(8, 4)], field_units=[None, None, Some(0), None]];
// DEFAULT-NEXT:     extern %1 c: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
