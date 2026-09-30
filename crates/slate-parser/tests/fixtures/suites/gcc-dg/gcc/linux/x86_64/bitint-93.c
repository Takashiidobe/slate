/* PR middle-end/114073 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -Wno-psabi" } */
/* { dg-additional-options "-mavx512f" { target i?86-*-* x86_64-*-* } } */

typedef int V __attribute__((vector_size (sizeof (_BitInt(256)))));
typedef int W __attribute__((vector_size (sizeof (_BitInt(512)))));

#if __BITINT_MAXWIDTH__ >= 256 && defined (__SIZEOF_INT128__)
_Complex __int128
f1 (_BitInt(256) x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.x = x;
  return u.y;
}

_Complex __int128
f2 (_BitInt(254) x)
{
  union U { _BitInt(254) x; _Complex __int128 y; } u;
  u.x = x;
  return u.y;
}

_BitInt(256)
f3 (_Complex __int128 x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.y = x;
  return u.x;
}

_BitInt(252)
f4 (_Complex __int128 x)
{
  union U { _BitInt(252) x; _Complex __int128 y; } u;
  u.y = x;
  return u.x;
}

_Complex __int128
f5 (_BitInt(256) x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.x = x + 1;
  return u.y;
}

_Complex __int128
f6 (_BitInt(254) x)
{
  union U { _BitInt(254) x; _Complex __int128 y; } u;
  u.x = x + 1;
  return u.y;
}

_Complex __int128
f7 (_BitInt(256) *x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.x = *x + 1;
  return u.y;
}

_Complex __int128
f8 (_BitInt(254) *x)
{
  union U { _BitInt(254) x; _Complex __int128 y; } u;
  u.x = *x + 1;
  return u.y;
}

_BitInt(256)
f9 (_Complex __int128 x)
{
  union U { _BitInt(256) x; _Complex __int128 y; } u;
  u.y = x;
  return u.x + 1;
}

_BitInt(252)
f10 (_Complex __int128 x)
{
  union U { _BitInt(252) x; _Complex __int128 y; } u;
  u.y = x;
  return u.x + 1;
}
#endif

#if __BITINT_MAXWIDTH__ >= 256
V
f11 (_BitInt(256) x)
{
  union U { _BitInt(256) x; V y; } u;
  u.x = x;
  return u.y;
}

V
f12 (_BitInt(254) x)
{
  union U { _BitInt(254) x; V y; } u;
  u.x = x;
  return u.y;
}

_BitInt(256)
f13 (V x)
{
  union U { _BitInt(256) x; V y; } u;
  u.y = x;
  return u.x;
}

_BitInt(252)
f14 (V x)
{
  union U { _BitInt(252) x; V y; } u;
  u.y = x;
  return u.x;
}

V
f15 (_BitInt(256) x)
{
  union U { _BitInt(256) x; V y; } u;
  u.x = x + 1;
  return u.y;
}

V
f16 (_BitInt(254) x)
{
  union U { _BitInt(254) x; V y; } u;
  u.x = x + 1;
  return u.y;
}

V
f17 (_BitInt(256) *x)
{
  union U { _BitInt(256) x; V y; } u;
  u.x = *x + 1;
  return u.y;
}

V
f18 (_BitInt(254) *x)
{
  union U { _BitInt(254) x; V y; } u;
  u.x = *x + 1;
  return u.y;
}

_BitInt(256)
f19 (V x)
{
  union U { _BitInt(256) x; V y; } u;
  u.y = x;
  return u.x + 1;
}

_BitInt(252)
f20 (V x)
{
  union U { _BitInt(252) x; V y; } u;
  u.y = x;
  return u.x + 1;
}
#endif

#if __BITINT_MAXWIDTH__ >= 512
W
f21 (_BitInt(512) x)
{
  union U { _BitInt(512) x; W y; } u;
  u.x = x;
  return u.y;
}

W
f22 (_BitInt(509) x)
{
  union U { _BitInt(509) x; W y; } u;
  u.x = x;
  return u.y;
}

_BitInt(512)
f23 (W x)
{
  union U { _BitInt(512) x; W y; } u;
  u.y = x;
  return u.x;
}

_BitInt(506)
f24 (W x)
{
  union U { _BitInt(506) x; W y; } u;
  u.y = x;
  return u.x;
}

W
f25 (_BitInt(512) x)
{
  union U { _BitInt(512) x; W y; } u;
  u.x = x + 1;
  return u.y;
}

W
f26 (_BitInt(509) x)
{
  union U { _BitInt(509) x; W y; } u;
  u.x = x + 1;
  return u.y;
}

W
f27 (_BitInt(512) *x)
{
  union U { _BitInt(512) x; W y; } u;
  u.x = *x + 1;
  return u.y;
}

W
f28 (_BitInt(509) *x)
{
  union U { _BitInt(509) x; W y; } u;
  u.x = *x + 1;
  return u.y;
}

_BitInt(512)
f29 (W x)
{
  union U { _BitInt(512) x; W y; } u;
  u.y = x;
  return u.x + 1;
}

_BitInt(506)
f30 (W x)
{
  union U { _BitInt(506) x; W y; } u;
  u.y = x;
  return u.x + 1;
}
#endif

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
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<i32, 8>;
// DEFAULT-NEXT:     type @type[[TYPE_W:[0-9]+]] W = vector<i32, 16>;
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_2:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_3:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_4:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i252b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_5:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_6:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_7:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_8:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_9:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_10:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i252b;
// DEFAULT-NEXT:         field1 y: complex<i128>;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_11:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_12:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_13:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_14:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i252b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_15:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_16:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_17:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_18:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i254b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_19:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i256b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_20:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i252b;
// DEFAULT-NEXT:         field1 y: vector<i32, 8>;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_21:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_22:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i509b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_23:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_24:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i506b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_25:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_26:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i509b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_27:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_28:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i509b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_29:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i512b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_U_30:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 x: i506b;
// DEFAULT-NEXT:         field1 y: vector<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=64, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: i256b) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%[[VALUE_u]]), read<i256b>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%[[VALUE_u]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_2:[0-9]+]] x: i254b) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_2:[0-9]+]] u: @type[[TYPE_U_2]] [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%[[VALUE_u_2]]), read<i254b>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%[[VALUE_u_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_3:[0-9]+]] x: complex<i128>) -> i256b [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_3:[0-9]+]] u: @type[[TYPE_U_3]] [storage=automatic];
// DEFAULT-NEXT:         write<complex<i128>>(field1(%[[VALUE_u_3]]), read<complex<i128>>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         return read<i256b>(field0(%[[VALUE_u_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_x_4:[0-9]+]] x: complex<i128>) -> i252b [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_4:[0-9]+]] u: @type[[TYPE_U_4]] [storage=automatic];
// DEFAULT-NEXT:         write<complex<i128>>(field1(%[[VALUE_u_4]]), read<complex<i128>>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         return read<i252b>(field0(%[[VALUE_u_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_x_5:[0-9]+]] x: i256b) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_5:[0-9]+]] u: @type[[TYPE_U_5]] [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%[[VALUE_u_5]]), add<i256b, overflow=ub>(read<i256b>(%[[VALUE_x_5]]), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%[[VALUE_u_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_x_6:[0-9]+]] x: i254b) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_6:[0-9]+]] u: @type[[TYPE_U_6]] [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%[[VALUE_u_6]]), add<i254b, overflow=ub>(read<i254b>(%[[VALUE_x_6]]), widen<i254b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%[[VALUE_u_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_x_7:[0-9]+]] x: ptr<i256b>) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_7:[0-9]+]] u: @type[[TYPE_U_7]] [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%[[VALUE_u_7]]), add<i256b, overflow=ub>(read<i256b>(deref(read<ptr<i256b>>(%[[VALUE_x_7]]))), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%[[VALUE_u_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_x_8:[0-9]+]] x: ptr<i254b>) -> complex<i128> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_8:[0-9]+]] u: @type[[TYPE_U_8]] [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%[[VALUE_u_8]]), add<i254b, overflow=ub>(read<i254b>(deref(read<ptr<i254b>>(%[[VALUE_x_8]]))), widen<i254b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i128>>(field1(%[[VALUE_u_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_x_9:[0-9]+]] x: complex<i128>) -> i256b [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_9:[0-9]+]] u: @type[[TYPE_U_9]] [storage=automatic];
// DEFAULT-NEXT:         write<complex<i128>>(field1(%[[VALUE_u_9]]), read<complex<i128>>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:         return add<i256b, overflow=ub>(read<i256b>(field0(%[[VALUE_u_9]])), widen<i256b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_x_10:[0-9]+]] x: complex<i128>) -> i252b [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_10:[0-9]+]] u: @type[[TYPE_U_10]] [storage=automatic];
// DEFAULT-NEXT:         write<complex<i128>>(field1(%[[VALUE_u_10]]), read<complex<i128>>(%[[VALUE_x_10]]));
// DEFAULT-NEXT:         return add<i252b, overflow=ub>(read<i252b>(field0(%[[VALUE_u_10]])), widen<i252b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_x_11:[0-9]+]] x: i256b) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_11:[0-9]+]] u: @type[[TYPE_U_11]] [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%[[VALUE_u_11]]), read<i256b>(%[[VALUE_x_11]]));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%[[VALUE_u_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_x_12:[0-9]+]] x: i254b) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_12:[0-9]+]] u: @type[[TYPE_U_12]] [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%[[VALUE_u_12]]), read<i254b>(%[[VALUE_x_12]]));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%[[VALUE_u_12]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f13:[0-9]+]] @f13(%[[VALUE_x_13:[0-9]+]] x: vector<i32, 8>) -> i256b [linkage=external] [abi=sysv64(byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_13:[0-9]+]] u: @type[[TYPE_U_13]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 8>>(field1(%[[VALUE_u_13]]), read<vector<i32, 8>>(%[[VALUE_x_13]]));
// DEFAULT-NEXT:         return read<i256b>(field0(%[[VALUE_u_13]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f14:[0-9]+]] @f14(%[[VALUE_x_14:[0-9]+]] x: vector<i32, 8>) -> i252b [linkage=external] [abi=sysv64(byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_14:[0-9]+]] u: @type[[TYPE_U_14]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 8>>(field1(%[[VALUE_u_14]]), read<vector<i32, 8>>(%[[VALUE_x_14]]));
// DEFAULT-NEXT:         return read<i252b>(field0(%[[VALUE_u_14]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f15:[0-9]+]] @f15(%[[VALUE_x_15:[0-9]+]] x: i256b) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_15:[0-9]+]] u: @type[[TYPE_U_15]] [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%[[VALUE_u_15]]), add<i256b, overflow=ub>(read<i256b>(%[[VALUE_x_15]]), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%[[VALUE_u_15]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f16:[0-9]+]] @f16(%[[VALUE_x_16:[0-9]+]] x: i254b) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_16:[0-9]+]] u: @type[[TYPE_U_16]] [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%[[VALUE_u_16]]), add<i254b, overflow=ub>(read<i254b>(%[[VALUE_x_16]]), widen<i254b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%[[VALUE_u_16]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f17:[0-9]+]] @f17(%[[VALUE_x_17:[0-9]+]] x: ptr<i256b>) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_17:[0-9]+]] u: @type[[TYPE_U_17]] [storage=automatic];
// DEFAULT-NEXT:         write<i256b>(field0(%[[VALUE_u_17]]), add<i256b, overflow=ub>(read<i256b>(deref(read<ptr<i256b>>(%[[VALUE_x_17]]))), widen<i256b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%[[VALUE_u_17]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f18:[0-9]+]] @f18(%[[VALUE_x_18:[0-9]+]] x: ptr<i254b>) -> vector<i32, 8> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_18:[0-9]+]] u: @type[[TYPE_U_18]] [storage=automatic];
// DEFAULT-NEXT:         write<i254b>(field0(%[[VALUE_u_18]]), add<i254b, overflow=ub>(read<i254b>(deref(read<ptr<i254b>>(%[[VALUE_x_18]]))), widen<i254b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 8>>(field1(%[[VALUE_u_18]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f19:[0-9]+]] @f19(%[[VALUE_x_19:[0-9]+]] x: vector<i32, 8>) -> i256b [linkage=external] [abi=sysv64(byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_19:[0-9]+]] u: @type[[TYPE_U_19]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 8>>(field1(%[[VALUE_u_19]]), read<vector<i32, 8>>(%[[VALUE_x_19]]));
// DEFAULT-NEXT:         return add<i256b, overflow=ub>(read<i256b>(field0(%[[VALUE_u_19]])), widen<i256b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f20:[0-9]+]] @f20(%[[VALUE_x_20:[0-9]+]] x: vector<i32, 8>) -> i252b [linkage=external] [abi=sysv64(byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_20:[0-9]+]] u: @type[[TYPE_U_20]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 8>>(field1(%[[VALUE_u_20]]), read<vector<i32, 8>>(%[[VALUE_x_20]]));
// DEFAULT-NEXT:         return add<i252b, overflow=ub>(read<i252b>(field0(%[[VALUE_u_20]])), widen<i252b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f21:[0-9]+]] @f21(%[[VALUE_x_21:[0-9]+]] x: i512b) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_21:[0-9]+]] u: @type[[TYPE_U_21]] [storage=automatic];
// DEFAULT-NEXT:         write<i512b>(field0(%[[VALUE_u_21]]), read<i512b>(%[[VALUE_x_21]]));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%[[VALUE_u_21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f22:[0-9]+]] @f22(%[[VALUE_x_22:[0-9]+]] x: i509b) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_22:[0-9]+]] u: @type[[TYPE_U_22]] [storage=automatic];
// DEFAULT-NEXT:         write<i509b>(field0(%[[VALUE_u_22]]), read<i509b>(%[[VALUE_x_22]]));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%[[VALUE_u_22]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f23:[0-9]+]] @f23(%[[VALUE_x_23:[0-9]+]] x: vector<i32, 16>) -> i512b [linkage=external] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_23:[0-9]+]] u: @type[[TYPE_U_23]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 16>>(field1(%[[VALUE_u_23]]), read<vector<i32, 16>>(%[[VALUE_x_23]]));
// DEFAULT-NEXT:         return read<i512b>(field0(%[[VALUE_u_23]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f24:[0-9]+]] @f24(%[[VALUE_x_24:[0-9]+]] x: vector<i32, 16>) -> i506b [linkage=external] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_24:[0-9]+]] u: @type[[TYPE_U_24]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 16>>(field1(%[[VALUE_u_24]]), read<vector<i32, 16>>(%[[VALUE_x_24]]));
// DEFAULT-NEXT:         return read<i506b>(field0(%[[VALUE_u_24]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f25:[0-9]+]] @f25(%[[VALUE_x_25:[0-9]+]] x: i512b) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_25:[0-9]+]] u: @type[[TYPE_U_25]] [storage=automatic];
// DEFAULT-NEXT:         write<i512b>(field0(%[[VALUE_u_25]]), add<i512b, overflow=ub>(read<i512b>(%[[VALUE_x_25]]), widen<i512b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%[[VALUE_u_25]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f26:[0-9]+]] @f26(%[[VALUE_x_26:[0-9]+]] x: i509b) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_26:[0-9]+]] u: @type[[TYPE_U_26]] [storage=automatic];
// DEFAULT-NEXT:         write<i509b>(field0(%[[VALUE_u_26]]), add<i509b, overflow=ub>(read<i509b>(%[[VALUE_x_26]]), widen<i509b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%[[VALUE_u_26]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f27:[0-9]+]] @f27(%[[VALUE_x_27:[0-9]+]] x: ptr<i512b>) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_27:[0-9]+]] u: @type[[TYPE_U_27]] [storage=automatic];
// DEFAULT-NEXT:         write<i512b>(field0(%[[VALUE_u_27]]), add<i512b, overflow=ub>(read<i512b>(deref(read<ptr<i512b>>(%[[VALUE_x_27]]))), widen<i512b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%[[VALUE_u_27]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f28:[0-9]+]] @f28(%[[VALUE_x_28:[0-9]+]] x: ptr<i509b>) -> vector<i32, 16> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_28:[0-9]+]] u: @type[[TYPE_U_28]] [storage=automatic];
// DEFAULT-NEXT:         write<i509b>(field0(%[[VALUE_u_28]]), add<i509b, overflow=ub>(read<i509b>(deref(read<ptr<i509b>>(%[[VALUE_x_28]]))), widen<i509b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         return read<vector<i32, 16>>(field1(%[[VALUE_u_28]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f29:[0-9]+]] @f29(%[[VALUE_x_29:[0-9]+]] x: vector<i32, 16>) -> i512b [linkage=external] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_29:[0-9]+]] u: @type[[TYPE_U_29]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 16>>(field1(%[[VALUE_u_29]]), read<vector<i32, 16>>(%[[VALUE_x_29]]));
// DEFAULT-NEXT:         return add<i512b, overflow=ub>(read<i512b>(field0(%[[VALUE_u_29]])), widen<i512b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f30:[0-9]+]] @f30(%[[VALUE_x_30:[0-9]+]] x: vector<i32, 16>) -> i506b [linkage=external] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_30:[0-9]+]] u: @type[[TYPE_U_30]] [storage=automatic];
// DEFAULT-NEXT:         write<vector<i32, 16>>(field1(%[[VALUE_u_30]]), read<vector<i32, 16>>(%[[VALUE_x_30]]));
// DEFAULT-NEXT:         return add<i506b, overflow=ub>(read<i506b>(field0(%[[VALUE_u_30]])), widen<i506b, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
