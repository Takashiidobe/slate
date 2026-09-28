// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES LONG_LONG_LONG LONG_LONG_LONG
// SLATE-FILECHECK-DEFINES SIGN_CONFLICT SIGN_CONFLICT
// SLATE-FILECHECK-DEFINES LONG_FLOAT LONG_FLOAT
// SLATE-FILECHECK-DEFINES SIGNED_TYPEDEF SIGNED_TYPEDEF
// SLATE-FILECHECK-DEFINES WIDE_BIT_INT WIDE_BIT_INT
// SLATE-FILECHECK-ERROR LONG_LONG_LONG
// SLATE-FILECHECK-ERROR SIGN_CONFLICT
// SLATE-FILECHECK-ERROR LONG_FLOAT
// SLATE-FILECHECK-ERROR SIGNED_TYPEDEF
// SLATE-FILECHECK-ERROR WIDE_BIT_INT
// SLATE-FILECHECK-ARGS --dump-ir

#define SAME(T, U) _Generic((T *)0, U *: 1, default: 0)
typedef int word;

_Static_assert(SAME(int unsigned long, unsigned long), "int unsigned long");
_Static_assert(SAME(long int unsigned, unsigned long), "long int unsigned");
_Static_assert(SAME(char signed, signed char), "char signed");
_Static_assert(SAME(char unsigned, unsigned char), "char unsigned");
_Static_assert(SAME(long const long, const long long), "long const long");
_Static_assert(SAME(int long, long), "int long");
_Static_assert(SAME(long int long unsigned, unsigned long long), "long int long unsigned");
_Static_assert(SAME(short int unsigned, unsigned short), "short int unsigned");
_Static_assert(SAME(int short, short), "int short");
_Static_assert(SAME(double long, long double), "double long");
_Static_assert(SAME(double _Complex long, _Complex long double), "double _Complex long");
_Static_assert(SAME(int _Complex long, _Complex long), "int _Complex long");
_Static_assert(SAME(_BitInt(42) unsigned, unsigned _BitInt(42)), "_BitInt unsigned");
_Static_assert(SAME(__int128 unsigned, unsigned __int128), "__int128 unsigned");
_Static_assert(SAME(word const, const int), "typedef then qualifier");
_Static_assert(SAME(unsigned unsigned, unsigned), "repeated sign");

long volatile unsigned const long counter;
signed static char hidden;
unsigned __attribute__((unused)) long long tagged;
struct fields {
  int unsigned : 4;
  long const long total;
  char signed small;
};

int unsigned long convert(long int unsigned value, char signed small) {
  return (int long unsigned)value + (short int unsigned)small;
}

#ifdef LONG_LONG_LONG
long long long too_long;
#endif
#ifdef SIGN_CONFLICT
int signed unsigned both;
#endif
#ifdef LONG_FLOAT
float long wide_float;
#endif
#ifdef SIGNED_TYPEDEF
word unsigned signed_typedef;
#endif
#ifdef WIDE_BIT_INT
_BitInt(42) long wide_bit_int;
#endif

// SLATE-FILECHECK-BEGIN LONG_LONG_LONG
// LONG_LONG_LONG: Error:   × cannot combine `long` with previous declaration specifiers
// LONG_LONG_LONG: ╰─▶ cannot combine `long` with previous declaration specifiers
// LONG_LONG_LONG: ╭─[tests/fixtures/clang/linux/x86_64/declaration_specifier_order.c:36:11]
// LONG_LONG_LONG: 35 │ #ifdef LONG_LONG_LONG
// LONG_LONG_LONG: 36 │ long long long too_long;
// LONG_LONG_LONG: ·           ────
// LONG_LONG_LONG: 37 │ #endif
// LONG_LONG_LONG: ╰────
// SLATE-FILECHECK-END LONG_LONG_LONG
// SLATE-FILECHECK-BEGIN SIGN_CONFLICT
// SIGN_CONFLICT: Error:   × cannot combine `unsigned` with previous declaration specifiers
// SIGN_CONFLICT: ╰─▶ cannot combine `unsigned` with previous declaration specifiers
// SIGN_CONFLICT: ╭─[tests/fixtures/clang/linux/x86_64/declaration_specifier_order.c:39:12]
// SIGN_CONFLICT: 38 │ #ifdef SIGN_CONFLICT
// SIGN_CONFLICT: 39 │ int signed unsigned both;
// SIGN_CONFLICT: ·            ────────
// SIGN_CONFLICT: 40 │ #endif
// SIGN_CONFLICT: ╰────
// SLATE-FILECHECK-END SIGN_CONFLICT
// SLATE-FILECHECK-BEGIN LONG_FLOAT
// LONG_FLOAT: Error:   × cannot combine `long` with previous declaration specifiers
// LONG_FLOAT: ╰─▶ cannot combine `long` with previous declaration specifiers
// LONG_FLOAT: ╭─[tests/fixtures/clang/linux/x86_64/declaration_specifier_order.c:42:7]
// LONG_FLOAT: 41 │ #ifdef LONG_FLOAT
// LONG_FLOAT: 42 │ float long wide_float;
// LONG_FLOAT: ·       ────
// LONG_FLOAT: 43 │ #endif
// LONG_FLOAT: ╰────
// SLATE-FILECHECK-END LONG_FLOAT
// SLATE-FILECHECK-BEGIN SIGNED_TYPEDEF
// SIGNED_TYPEDEF: Error:   × cannot combine `unsigned` with previous declaration specifiers
// SIGNED_TYPEDEF: ╰─▶ cannot combine `unsigned` with previous declaration specifiers
// SIGNED_TYPEDEF: ╭─[tests/fixtures/clang/linux/x86_64/declaration_specifier_order.c:45:6]
// SIGNED_TYPEDEF: 44 │ #ifdef SIGNED_TYPEDEF
// SIGNED_TYPEDEF: 45 │ word unsigned signed_typedef;
// SIGNED_TYPEDEF: ·      ────────
// SIGNED_TYPEDEF: 46 │ #endif
// SIGNED_TYPEDEF: ╰────
// SLATE-FILECHECK-END SIGNED_TYPEDEF
// SLATE-FILECHECK-BEGIN WIDE_BIT_INT
// WIDE_BIT_INT: Error:   × cannot combine `long` with previous declaration specifiers
// WIDE_BIT_INT: ╰─▶ cannot combine `long` with previous declaration specifiers
// WIDE_BIT_INT: ╭─[tests/fixtures/clang/linux/x86_64/declaration_specifier_order.c:48:13]
// WIDE_BIT_INT: 47 │ #ifdef WIDE_BIT_INT
// WIDE_BIT_INT: 48 │ _BitInt(42) long wide_bit_int;
// WIDE_BIT_INT: ·             ────
// WIDE_BIT_INT: 49 │ #endif
// WIDE_BIT_INT: ╰────
// SLATE-FILECHECK-END WIDE_BIT_INT
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
// DEFAULT-NEXT:     type @type0 word = i32;
// DEFAULT-NEXT:     type @type1 fields = struct {
// DEFAULT-NEXT:         field0 <anonymous>: u32 : 4;
// DEFAULT-NEXT:         field1 total: const i64;
// DEFAULT-NEXT:         field2 small: i8;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16], bit_offsets=[Some(0), None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// DEFAULT-NEXT:     global %1 counter: volatile u64 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %2 hidden: i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %3 tagged: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @convert(%6 value: u64, %7 small: i8) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(read<u64>(%6), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(widen<i16, reason=explicit>(read<i8>(%7))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
