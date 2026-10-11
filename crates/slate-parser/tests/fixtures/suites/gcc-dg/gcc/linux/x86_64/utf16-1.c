/* Contributed by Kris Van Hees <kris.van.hees@oracle.com> */
/* Test the support for char16_t character constants. */
/* { dg-do run } */
/* { dg-options "-std=gnu99 -Wall -Werror" } */

typedef __CHAR16_TYPE__ char16_t;

extern void abort (void);

char16_t	c0 = u'a';
char16_t	c1 = u'\0';
char16_t	c2 = u'\u0024';
char16_t	c3 = u'\u2029';
char16_t	c4 = u'\u8010';

char16_t	c5 = 'a';
char16_t	c6 = U'a';
char16_t	c7 = U'\u2029';
char16_t	c8 = U'\u8010';
char16_t	c9 = L'a';
char16_t	ca = L'\u2029';
char16_t	cb = L'\u8010';

#define A	0x0061
#define D	0x0024
#define X	0x2029
#define Y	0x8010

int main ()
{
    if (sizeof (u'a') != sizeof (char16_t))
	abort ();
    if (sizeof (u'\0') != sizeof (char16_t))
	abort ();
    if (sizeof (u'\u0024') != sizeof (char16_t))
	abort ();
    if (sizeof (u'\u2029') != sizeof (char16_t))
	abort ();
    if (sizeof (u'\u8010') != sizeof (char16_t))
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

    if (c5 != A)
	abort ();
    if (c6 != A)
	abort ();
    if (c7 != X)
	abort ();
    if (c8 != Y)
	abort ();
    if (c9 != A)
	abort ();
    if (ca != X)
	abort ();
    if (cb != Y)
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
// DEFAULT-NEXT:     type @type[[TYPE_char16_t:[0-9]+]] char16_t = u16;
// DEFAULT-NEXT:     global %[[VALUE_c0:[0-9]+]] c0: u16 [storage=static] = const<u16>(97) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c1:[0-9]+]] c1: u16 [storage=static] = const<u16>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c2:[0-9]+]] c2: u16 [storage=static] = const<u16>(36) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c3:[0-9]+]] c3: u16 [storage=static] = const<u16>(8233) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c4:[0-9]+]] c4: u16 [storage=static] = const<u16>(32784) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c5:[0-9]+]] c5: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(97))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c6:[0-9]+]] c6: u16 [storage=static] = truncate<u16, reason=assign, fits=always>(const<u32>(97)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c7:[0-9]+]] c7: u16 [storage=static] = truncate<u16, reason=assign, fits=always>(const<u32>(8233)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c8:[0-9]+]] c8: u16 [storage=static] = truncate<u16, reason=assign, fits=always>(const<u32>(32784)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c9:[0-9]+]] c9: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(97))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ca:[0-9]+]] ca: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(8233))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cb:[0-9]+]] cb: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(32784))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c0]]))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c1]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c2]]))), const<i32>(36))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c3]]))), const<i32>(8233))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c4]]))), const<i32>(32784))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c5]]))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c6]]))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c7]]))), const<i32>(8233))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c8]]))), const<i32>(32784))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c9]]))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_ca]]))), const<i32>(8233))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_cb]]))), const<i32>(32784))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
