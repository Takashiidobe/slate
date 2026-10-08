/* N3353 - Delimited escape sequences */
/* { dg-do compile } */
/* { dg-require-effective-target wchar } */
/* { dg-options "-std=c2y -Wc23-c2y-compat -Wno-c++-compat" } */

#include <wchar.h>
typedef __CHAR16_TYPE__ char16_t;
typedef __CHAR32_TYPE__ char32_t;

const char32_t *a = U"\u{1234}\u{10fffd}\u{000000000000000000000000000000000000000000000000000000000001234}\u{10FFFD}";		/* { dg-warning "delimited escape sequences are only valid in" } */
const char32_t *b = U"\x{1234}\x{10fffd}\x{000000000000000000000000000000000000000000000000000000000001234}";			/* { dg-warning "delimited escape sequences are only valid in" } */
const char32_t *c = U"\o{1234}\o{4177775}\o{000000000000000000000000000000000000000000000000000000000000000000000000004177775}";/* { dg-warning "delimited escape sequences are only valid in" } */
const char16_t *d = u"\u{1234}\u{bFFd}\u{00000000000000000000000000000001234}";							/* { dg-warning "delimited escape sequences are only valid in" } */
const char16_t *e = u"\x{1234}\x{BffD}\x{000001234}";										/* { dg-warning "delimited escape sequences are only valid in" } */
const char16_t *f = u"\o{1234}\o{137775}\o{000000000000000137775}";								/* { dg-warning "delimited escape sequences are only valid in" } */
const wchar_t *g = L"\u{1234}\u{bFFd}\u{00000000000000000000000000000001234}";							/* { dg-warning "delimited escape sequences are only valid in" } */
const wchar_t *h = L"\x{1234}\x{bFFd}\x{000001234}";										/* { dg-warning "delimited escape sequences are only valid in" } */
const wchar_t *i = L"\o{1234}\o{137775}\o{000000000000000137775}";								/* { dg-warning "delimited escape sequences are only valid in" } */
const char *k = "\x{34}\x{000000000000000003D}";										/* { dg-warning "delimited escape sequences are only valid in" } */
const char *l = "\o{34}\o{000000000000000176}";											/* { dg-warning "delimited escape sequences are only valid in" } */

#if U'\u{1234}' != U'\u1234' || U'\u{10fffd}' != U'\U0010FFFD' \
    || U'\x{00000001234}' != U'\x1234' || U'\x{010fffd}' != U'\x10FFFD' \
    || U'\o{1234}' != U'\x29c' || U'\o{004177775}' != U'\x10FFFD' \
    || u'\u{1234}' != u'\u1234' || u'\u{0bffd}' != u'\uBFFD' \
    || u'\x{00000001234}' != u'\x1234' || u'\x{0Bffd}' != u'\x0bFFD' \
    || u'\o{1234}' != u'\x29c' || u'\o{00137775}' != u'\xBFFD' \
    || L'\u{1234}' != L'\u1234' || L'\u{0bffd}' != L'\uBFFD' \
    || L'\x{00000001234}' != L'\x1234' || L'\x{0bffd}' != L'\x0bFFD' \
    || L'\o{1234}' != L'\x29c' || L'\o{00137775}' != L'\xBFFD' \
    || '\x{34}' != '\x034' || '\x{0003d}' != '\x003D' \
    || '\o{34}' != '\x1C' || '\o{176}' != '\x007E'
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
/* { dg-warning "delimited escape sequences are only valid in" "" { target *-*-* } .-11 } */
#error Bad
#endif

int
main ()
{
  if (a[0] != U'\u1234' || a[0] != U'\u{1234}'					/* { dg-warning "delimited escape sequences are only valid in" } */
      || a[1] != U'\U0010FFFD' || a[1] != U'\u{000010fFfD}'			/* { dg-warning "delimited escape sequences are only valid in" } */
      || a[2] != a[0]
      || a[3] != a[1]
      || b[0] != U'\x1234' || b[0] != U'\x{001234}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || b[1] != U'\x10FFFD' || b[1] != U'\x{0010fFfD}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || b[2] != b[0]
      || c[0] != U'\x29c' || c[0] != U'\o{001234}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || c[1] != U'\x10FFFD' || c[1] != U'\o{4177775}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || c[2] != c[1])
    __builtin_abort ();
  if (d[0] != u'\u1234' || d[0] != u'\u{1234}'					/* { dg-warning "delimited escape sequences are only valid in" } */
      || d[1] != u'\U0000BFFD' || d[1] != u'\u{00000bFfD}'			/* { dg-warning "delimited escape sequences are only valid in" } */
      || d[2] != d[0]
      || e[0] != u'\x1234' || e[0] != u'\x{001234}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || e[1] != u'\xBFFD' || e[1] != u'\x{00bFfD}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || e[2] != e[0]
      || f[0] != u'\x29c' || f[0] != u'\o{001234}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || f[1] != u'\xbFFD' || f[1] != u'\o{137775}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || f[2] != f[1])
    __builtin_abort ();
  if (g[0] != L'\u1234' || g[0] != L'\u{1234}'					/* { dg-warning "delimited escape sequences are only valid in" } */
      || g[1] != L'\U0000BFFD' || g[1] != L'\u{00000bFfD}'			/* { dg-warning "delimited escape sequences are only valid in" } */
      || g[2] != g[0]
      || h[0] != L'\x1234' || h[0] != L'\x{001234}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || h[1] != L'\xBFFD' || h[1] != L'\x{00bFfD}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || h[2] != h[0]
      || i[0] != L'\x29c' || i[0] != L'\o{001234}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || i[1] != L'\xbFFD' || i[1] != L'\o{137775}'				/* { dg-warning "delimited escape sequences are only valid in" } */
      || i[2] != i[1])
    __builtin_abort ();
  if (k[0] != '\x034' || k[0] != '\x{0034}'					/* { dg-warning "delimited escape sequences are only valid in" } */
      || k[1] != '\x3D' || k[1] != '\x{3d}'					/* { dg-warning "delimited escape sequences are only valid in" } */
      || l[0] != '\x1c' || l[0] != '\o{0034}'					/* { dg-warning "delimited escape sequences are only valid in" } */
      || l[1] != '\x07e' || l[1] != '\o{176}' || l[1] != '\176')		/* { dg-warning "delimited escape sequences are only valid in" } */
    __builtin_abort ();
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_char16_t:[0-9]+]] char16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_char32_t:[0-9]+]] char32_t = u32;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<u32, 5> [storage=static] = code_units<array<u32, 5>>([4660, 1114109, 4660, 1114109, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(5)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<u32, 4> [storage=static] = code_units<array<u32, 4>>([4660, 1114109, 4660, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(4)>(%[[VALUE_str_2]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<u32, 4> [storage=static] = code_units<array<u32, 4>>([668, 1114109, 1114109, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(4)>(%[[VALUE_str_3]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<u16, 4> [storage=static] = code_units<array<u16, 4>>([4660, 49149, 4660, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(4)>(%[[VALUE_str_4]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<u16, 4> [storage=static] = code_units<array<u16, 4>>([4660, 49149, 4660, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(4)>(%[[VALUE_str_5]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<u16, 4> [storage=static] = code_units<array<u16, 4>>([668, 49149, 49149, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: ptr<const u16> [storage=static] = pointer_cast<ptr<const u16>, reason=assign>(array_decay<ptr<u16>, length=Some(4)>(%[[VALUE_str_6]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([4660, 49149, 4660, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: ptr<const i32> [storage=static] = pointer_cast<ptr<const i32>, reason=assign>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_str_7]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([4660, 49149, 4660, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: ptr<const i32> [storage=static] = pointer_cast<ptr<const i32>, reason=assign>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_str_8]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([668, 49149, 49149, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: ptr<const i32> [storage=static] = pointer_cast<ptr<const i32>, reason=assign>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_str_9]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([52, 61, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_10]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([28, 126, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_11]])) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(0)))), const<u32>(4660)), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(0)))), const<u32>(4660))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(1)))), const<u32>(1114109))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(1)))), const<u32>(1114109))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(2)))), read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(0)))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(3)))), read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_a]]), const<i32>(1)))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_b]]), const<i32>(0)))), const<u32>(4660))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_b]]), const<i32>(0)))), const<u32>(4660))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_b]]), const<i32>(1)))), const<u32>(1114109))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_b]]), const<i32>(1)))), const<u32>(1114109))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_b]]), const<i32>(2)))), read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_b]]), const<i32>(0)))))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_c]]), const<i32>(0)))), const<u32>(668))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_c]]), const<i32>(0)))), const<u32>(668))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_c]]), const<i32>(1)))), const<u32>(1114109))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_c]]), const<i32>(1)))), const<u32>(1114109))), ne<u32>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_c]]), const<i32>(2)))), read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_c]]), const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_d]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(4660)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_d]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(4660))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_d]]), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(49149))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_d]]), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(49149))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_d]]), const<i32>(2)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_d]]), const<i32>(0)))))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_e]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(4660))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_e]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(4660))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_e]]), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(49149))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_e]]), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(49149))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_e]]), const<i32>(2)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_e]]), const<i32>(0)))))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_f]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(668))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_f]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(668))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_f]]), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(49149))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_f]]), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u16>(49149))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_f]]), const<i32>(2)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(%[[VALUE_f]]), const<i32>(1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_g]]), const<i32>(0)))), const<i32>(4660)), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_g]]), const<i32>(0)))), const<i32>(4660))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_g]]), const<i32>(1)))), const<i32>(49149))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_g]]), const<i32>(1)))), const<i32>(49149))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_g]]), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_g]]), const<i32>(0)))))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_h]]), const<i32>(0)))), const<i32>(4660))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_h]]), const<i32>(0)))), const<i32>(4660))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_h]]), const<i32>(1)))), const<i32>(49149))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_h]]), const<i32>(1)))), const<i32>(49149))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_h]]), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_h]]), const<i32>(0)))))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_i]]), const<i32>(0)))), const<i32>(668))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_i]]), const<i32>(0)))), const<i32>(668))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_i]]), const<i32>(1)))), const<i32>(49149))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_i]]), const<i32>(1)))), const<i32>(49149))), ne<i32>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_i]]), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_i]]), const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_k]]), const<i32>(0))))), const<i32>(52)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_k]]), const<i32>(0))))), const<i32>(52))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_k]]), const<i32>(1))))), const<i32>(61))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_k]]), const<i32>(1))))), const<i32>(61))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_l]]), const<i32>(0))))), const<i32>(28))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_l]]), const<i32>(0))))), const<i32>(28))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_l]]), const<i32>(1))))), const<i32>(126))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_l]]), const<i32>(1))))), const<i32>(126))), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_l]]), const<i32>(1))))), const<i32>(126)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
