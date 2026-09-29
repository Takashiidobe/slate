/* Test Unicode strings in C11.  Test valid code.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

/* More thorough tests are in c-c++-common/raw-string-*.c; this test
   verifies the particular subset (Unicode but not raw strings) that
   is in C11.  */

typedef __CHAR16_TYPE__ char16_t;
typedef __CHAR32_TYPE__ char32_t;
typedef __SIZE_TYPE__ size_t;

extern void abort (void);
extern void exit (int);
extern int memcmp (const void *, const void *, size_t);

#define R "(R)"
#define u8R "(u8R)"
#define uR "(uR)"
#define UR "(UR)"
#define LR "(LR)"
#define u8 randomu8
#define u randomu
#define U randomU

const char su8[] = u8"a\u010d";
const char su8a[] = "a\xc4\x8d";

const char16_t su16[] = u"\u0567";
const char16_t su16a[] = { 0x0567, 0 };

const char32_t su32[] = U"\u0123";
const char32_t su32a[] = { 0x0123, 0 };

const char tu[] = R"a";
const char tua[] = "(R)a";

const char tu8[] = u8R"b";
const char tu8a[] = "(u8R)b";

const char tu16[] = uR"c";
const char tu16a[] = "(uR)c";

const char tu32[] = UR"d";
const char tu32a[] = "(UR)d";

const char tl[] = LR"e";
const char tla[] = "(LR)e";

#define str(x) #x
const char ts[] = str(u"a" U"b" u8"c");
const char tsa[] = "u\"a\" U\"b\" u8\"c\"";

/* GCC always uses UTF-16 and UTF-32 for char16_t and char32_t.  */
#ifndef __STDC_UTF_16__
#error "__STDC_UTF_16__ not defined"
#endif
#ifndef __STDC_UTF_32__
#error "__STDC_UTF_32__ not defined"
#endif
#define xstr(x) str(x)
const char tm16[] = xstr(__STDC_UTF_16__);
const char tm16a[] = "1";
const char tm32[] = xstr(__STDC_UTF_32__);
const char tm32a[] = "1";

int
main (void)
{
  if (sizeof (su8) != sizeof (su8a)
      || memcmp (su8, su8a, sizeof (su8)) != 0)
    abort ();
  if (sizeof (su16) != sizeof (su16a)
      || memcmp (su16, su16a, sizeof (su16)) != 0)
    abort ();
  if (sizeof (su32) != sizeof (su32a)
      || memcmp (su32, su32a, sizeof (su32)) != 0)
    abort ();
  if (sizeof (tu) != sizeof (tua)
      || memcmp (tu, tua, sizeof (tu)) != 0)
    abort ();
  if (sizeof (tu8) != sizeof (tu8a)
      || memcmp (tu8, tu8a, sizeof (tu8)) != 0)
    abort ();
  if (sizeof (tu16) != sizeof (tu16a)
      || memcmp (tu16, tu16a, sizeof (tu16)) != 0)
    abort ();
  if (sizeof (tu32) != sizeof (tu32a)
      || memcmp (tu32, tu32a, sizeof (tu32)) != 0)
    abort ();
  if (sizeof (tl) != sizeof (tla)
      || memcmp (tl, tla, sizeof (tl)) != 0)
    abort ();
  if (sizeof (ts) != sizeof (tsa)
      || memcmp (ts, tsa, sizeof (ts)) != 0)
    abort ();
  if (sizeof (tm16) != sizeof (tm16a)
      || memcmp (tm16, tm16a, sizeof (tm16)) != 0)
    abort ();
  if (sizeof (tm32) != sizeof (tm32a)
      || memcmp (tm32, tm32a, sizeof (tm32)) != 0)
    abort ();
  if (u'\u0123' != 0x0123)
    abort ();
  if (U'\u0456' != 0x0456)
    abort ();
#undef u8
#define u8
  if (u8'a' != 'a')
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type[[TYPE_char32_t:[0-9]+]] char32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_su8:[0-9]+]] su8: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([97, 196, 141, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_su8a:[0-9]+]] su8a: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([97, 196, 141, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_su16:[0-9]+]] su16: array<u16, 2> [storage=static] [const] = code_units<array<u16, 2>>([1383, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_su16a:[0-9]+]] su16a: array<u16, 2> [storage=static] [const] = aggregate<array<u16, 2>, zero_fill=false>(index0 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(1383))), index1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_su32:[0-9]+]] su32: array<u32, 2> [storage=static] [const] = code_units<array<u32, 2>>([291, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_su32a:[0-9]+]] su32a: array<u32, 2> [storage=static] [const] = aggregate<array<u32, 2>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(291)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tu:[0-9]+]] tu: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([40, 82, 41, 97, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tua:[0-9]+]] tua: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([40, 82, 41, 97, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tu8:[0-9]+]] tu8: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([40, 117, 56, 82, 41, 98, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tu8a:[0-9]+]] tu8a: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([40, 117, 56, 82, 41, 98, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tu16:[0-9]+]] tu16: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([40, 117, 82, 41, 99, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tu16a:[0-9]+]] tu16a: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([40, 117, 82, 41, 99, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tu32:[0-9]+]] tu32: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([40, 85, 82, 41, 100, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tu32a:[0-9]+]] tu32a: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([40, 85, 82, 41, 100, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tl:[0-9]+]] tl: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([40, 76, 82, 41, 101, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tla:[0-9]+]] tla: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([40, 76, 82, 41, 101, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ts:[0-9]+]] ts: array<i8, 16> [storage=static] [const] [align=16] = code_units<array<i8, 16>>([117, 34, 97, 34, 32, 85, 34, 98, 34, 32, 117, 56, 34, 99, 34, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tsa:[0-9]+]] tsa: array<i8, 16> [storage=static] [const] [align=16] = code_units<array<i8, 16>>([117, 34, 97, 34, 32, 85, 34, 98, 34, 32, 117, 56, 34, 99, 34, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tm16:[0-9]+]] tm16: array<i8, 2> [storage=static] [const] = code_units<array<i8, 2>>([49, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tm16a:[0-9]+]] tm16a: array<i8, 2> [storage=static] [const] = code_units<array<i8, 2>>([49, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tm32:[0-9]+]] tm32: array<i8, 2> [storage=static] [const] = code_units<array<i8, 2>>([49, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tm32a:[0-9]+]] tm32a: array<i8, 2> [storage=static] [const] = code_units<array<i8, 2>>([49, 0]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_su8]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(4)>(%[[VALUE_su8a]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const u16>, length=Some(2)>(%[[VALUE_su16]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const u16>, length=Some(2)>(%[[VALUE_su16a]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const u32>, length=Some(2)>(%[[VALUE_su32]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const u32>, length=Some(2)>(%[[VALUE_su32a]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(5), const<u64>(5))
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(5)>(%[[VALUE_tu]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(5)>(%[[VALUE_tua]])), const<u64>(5)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(7), const<u64>(7))
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(7)>(%[[VALUE_tu8]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(7)>(%[[VALUE_tu8a]])), const<u64>(7)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(6), const<u64>(6))
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(6)>(%[[VALUE_tu16]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(6)>(%[[VALUE_tu16a]])), const<u64>(6)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(6), const<u64>(6))
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(6)>(%[[VALUE_tu32]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(6)>(%[[VALUE_tu32a]])), const<u64>(6)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(6), const<u64>(6))
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(6)>(%[[VALUE_tl]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(6)>(%[[VALUE_tla]])), const<u64>(6)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE11]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(16), const<u64>(16))
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(16)>(%[[VALUE_ts]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(16)>(%[[VALUE_tsa]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(2)>(%[[VALUE_tm16]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(2)>(%[[VALUE_tm16a]])), const<u64>(2)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE13]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), const<u64>(2))
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(2)>(%[[VALUE_tm32]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<const i8>, length=Some(2)>(%[[VALUE_tm32a]])), const<u64>(2)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE14]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(291))), const<i32>(291))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(const<u32>(1110), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1110)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(97), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
