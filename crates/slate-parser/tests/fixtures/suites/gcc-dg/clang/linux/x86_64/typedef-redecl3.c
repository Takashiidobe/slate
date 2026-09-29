/* { dg-do compile } */
/* { dg-options "" } */

#define N 64

struct f { int x; };
typedef struct f T;
typedef struct f T __attribute__((aligned (N)));
typedef struct f T __attribute__((aligned (N * 2)));
typedef struct f T __attribute__((aligned (N)));
typedef struct f T;

_Static_assert (_Alignof (T) == N * 2, "N * 2");

enum g { A = 1 };
typedef enum g S;
typedef enum g S __attribute__((aligned (N)));
typedef enum g S __attribute__((aligned (N * 2)));
typedef enum g S __attribute__((aligned (N)));
typedef enum g S;

_Static_assert (_Alignof (S) == N * 2, "N * 2");

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
// DEFAULT-NEXT:     type @type[[TYPE_f:[0-9]+]] f = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = @type[[TYPE_f]];
// DEFAULT-NEXT:     type @type[[TYPE_T_2:[0-9]+]] T = @type[[TYPE_f]];
// DEFAULT-NEXT:     type @type[[TYPE_T_3:[0-9]+]] T = @type[[TYPE_f]];
// DEFAULT-NEXT:     type @type[[TYPE_T_4:[0-9]+]] T = @type[[TYPE_f]];
// DEFAULT-NEXT:     type @type[[TYPE_T_5:[0-9]+]] T = @type[[TYPE_f]];
// DEFAULT-NEXT:     type @type[[TYPE_g:[0-9]+]] g = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = @type[[TYPE_g]];
// DEFAULT-NEXT:     type @type[[TYPE_S_2:[0-9]+]] S = @type[[TYPE_g]];
// DEFAULT-NEXT:     type @type[[TYPE_S_3:[0-9]+]] S = @type[[TYPE_g]];
// DEFAULT-NEXT:     type @type[[TYPE_S_4:[0-9]+]] S = @type[[TYPE_g]];
// DEFAULT-NEXT:     type @type[[TYPE_S_5:[0-9]+]] S = @type[[TYPE_g]];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
