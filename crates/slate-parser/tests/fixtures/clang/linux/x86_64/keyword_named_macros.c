#define const
const int x;
int *p = &x;

#define __int128 long
__int128 q;
_Static_assert(_Generic(q, long: 1, default: 0), "__int128 macro expands to long");

#ifndef const
#error const should be defined
#endif

#define S(x) #x
char spelled[] = S(__const __signed__ __inline sizeof _Alignof);

#define CAT(a, b) a##b
CAT(un, signed) pasted;

#define inline __inline__ __attribute__((always_inline))
static inline int hinted(void) { return (int)q; }

#if sizeof
#error keywords are identifiers in #if
#endif

#undef const
const int restored = 1;



// SLATE-FILECHECK-STD DEFAULT c17
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
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_x]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_spelled:[0-9]+]] spelled: array<i8, 44> [storage=static] [align=16] = code_units<array<i8, 44>>([95, 95, 99, 111, 110, 115, 116, 32, 95, 95, 115, 105, 103, 110, 101, 100, 95, 95, 32, 95, 95, 105, 110, 108, 105, 110, 101, 32, 115, 105, 122, 101, 111, 102, 32, 95, 65, 108, 105, 103, 110, 111, 102, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pasted:[0-9]+]] pasted: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_restored:[0-9]+]] restored: i32 [storage=static] [const] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hinted:[0-9]+]] @hinted() -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_q]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
