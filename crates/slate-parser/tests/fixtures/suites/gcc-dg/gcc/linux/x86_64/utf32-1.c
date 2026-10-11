/* Contributed by Kris Van Hees <kris.van.hees@oracle.com> */
/* Test the support for char32_t character constants. */
/* { dg-do run } */
/* { dg-options "-std=gnu99 -Wall -Werror" } */

typedef __CHAR32_TYPE__ char32_t;

extern void abort (void);

char32_t	c0 = U'a';
char32_t	c1 = U'\0';
char32_t	c2 = U'\u0024';
char32_t	c3 = U'\u2029';
char32_t	c4 = U'\U00064321';

#define A	0x00000061
#define D	0x00000024
#define X	0x00002029
#define Y	0x00064321

int main ()
{
    if (sizeof (U'a') != sizeof (char32_t))
	abort ();
    if (sizeof (U'\0') != sizeof (char32_t))
	abort ();
    if (sizeof (U'\u0024') != sizeof (char32_t))
	abort ();
    if (sizeof (U'\u2029') != sizeof (char32_t))
	abort ();
    if (sizeof (U'\U00064321') != sizeof (char32_t))
	abort ();

    if (c0 != A)
	abort ();
    if (c1 != 0x0000)
	abort ();
    if (c2 != D)
	abort ();
    if (c3 != X)
	abort ();
    if (c4 != Y)
	abort ();
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type[[TYPE_char32_t:[0-9]+]] char32_t = u32;
// DEFAULT-NEXT:     global %[[VALUE_c0:[0-9]+]] c0: u32 [storage=static] = const<u32>(97) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c1:[0-9]+]] c1: u32 [storage=static] = const<u32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c2:[0-9]+]] c2: u32 [storage=static] = const<u32>(36) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c3:[0-9]+]] c3: u32 [storage=static] = const<u32>(8233) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c4:[0-9]+]] c4: u32 [storage=static] = const<u32>(410401) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_c0]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(97)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_c1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_c2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(36)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_c3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8233)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_c4]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(410401)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
