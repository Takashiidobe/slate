/* PR c/52085 */
/* { dg-do compile } */
/* { dg-options "" } */

#define SA(X) _Static_assert((X),#X)

enum e1;
enum e1 { A } __attribute__ ((__packed__));
enum e2 { B } __attribute__ ((__packed__));
SA (sizeof (enum e1) == sizeof (enum e2));
SA (_Alignof (enum e1) == _Alignof (enum e2));

enum e3;
enum e3 { C = 256 } __attribute__ ((__packed__));
enum e4 { D = 256 } __attribute__ ((__packed__));
SA (sizeof (enum e3) == sizeof (enum e4));
SA (_Alignof (enum e3) == _Alignof (enum e4));

enum e5;
enum e5 { E = __INT_MAX__ } __attribute__ ((__packed__));
enum e6 { F = __INT_MAX__ } __attribute__ ((__packed__));
SA (sizeof (enum e5) == sizeof (enum e6));
SA (_Alignof (enum e5) == _Alignof (enum e6));

enum e7;
enum e7 { G } __attribute__ ((__mode__(__byte__)));
enum e8 { H } __attribute__ ((__mode__(__byte__)));
SA (sizeof (enum e7) == sizeof (enum e8));
SA (_Alignof (enum e7) == _Alignof (enum e8));

enum e9;
enum e9 { I } __attribute__ ((__packed__, __mode__(__byte__)));
enum e10 { J } __attribute__ ((__packed__, __mode__(__byte__)));
SA (sizeof (enum e9) == sizeof (enum e10));
SA (_Alignof (enum e9) == _Alignof (enum e10));

enum e11;
enum e11 { K } __attribute__ ((__mode__(__word__)));
enum e12 { L } __attribute__ ((__mode__(__word__)));
SA (sizeof (enum e11) == sizeof (enum e12));
SA (_Alignof (enum e11) == _Alignof (enum e12));

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_e1:[0-9]+]] e1 = enum : u8 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(0);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     type @type[[TYPE_e2:[0-9]+]] e2 = enum : u8 {
// DEFAULT-NEXT:         %[[VALUE_A]] B = const<i32>(0);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     type @type[[TYPE_e3:[0-9]+]] e3 = enum : u16 {
// DEFAULT-NEXT:         %[[VALUE_A]] C = const<i32>(256);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type[[TYPE_e4:[0-9]+]] e4 = enum : u16 {
// DEFAULT-NEXT:         %[[VALUE_A]] D = const<i32>(256);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type[[TYPE_e5:[0-9]+]] e5 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] E = const<i32>(2147483647);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_e6:[0-9]+]] e6 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] F = const<i32>(2147483647);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_e7:[0-9]+]] e7 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] G = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_e8:[0-9]+]] e8 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] H = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_e9:[0-9]+]] e9 = enum : u8 {
// DEFAULT-NEXT:         %[[VALUE_A]] I = const<i32>(0);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     type @type[[TYPE_e10:[0-9]+]] e10 = enum : u8 {
// DEFAULT-NEXT:         %[[VALUE_A]] J = const<i32>(0);
// DEFAULT-NEXT:     } [size=1, align=1];
// DEFAULT-NEXT:     type @type[[TYPE_e11:[0-9]+]] e11 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] K = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_e12:[0-9]+]] e12 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] L = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
