typedef const volatile unsigned short small;
enum signed_limit : short { LOW = -1, HIGH = 1 };
enum unsigned_limit : unsigned long { ZERO = 0 };
enum ordinary { NEGATIVE = -1, POSITIVE = 1 };
enum forward : unsigned short;

signed char sc_max = _Maxof(signed char);
signed char sc_min = _Minof(signed char);
unsigned char uc_max = _Maxof(unsigned char);
unsigned char uc_min = _Minof(unsigned char);
char char_max = _Maxof(char);
char char_min = _Minof(char);
short short_max = _Maxof(short);
short short_min = _Minof(short);
long long_max = _Maxof(long);
long long_min = _Minof(long);
unsigned long ulong_max = _Maxof(unsigned long);
long long ll_max = _Maxof(long long);
long long ll_min = _Minof(long long);
__int128 extended_min = _Minof(__int128);
unsigned __int128 extended_max = _Maxof(unsigned __int128);
bool bool_max = _Maxof(bool);
bool bool_min = _Minof(bool);
unsigned short qualified = _Maxof(small);
unsigned int atomic = _Maxof(_Atomic(unsigned int));
enum signed_limit enum_max = _Maxof(enum signed_limit);
enum signed_limit enum_min = _Minof(enum signed_limit);
enum unsigned_limit enum_unsigned = _Maxof(enum unsigned_limit);
unsigned short forward_max = _Maxof(enum forward);
unsigned _BitInt(1) bit_max = _Maxof(unsigned _BitInt(1));
_BitInt(5) bit_min = _Minof(_BitInt(5));
_BitInt(575) huge_min = _Minof(_BitInt(575));
unsigned _BitInt(575) huge_max = _Maxof(unsigned _BitInt(575));
int bound[_Maxof(unsigned _BitInt(3))];

_Static_assert(_Maxof(int) == 2147483647, "int maximum");
_Static_assert(_Minof(int) == -2147483647 - 1, "int minimum");
_Static_assert(_Maxof(char) == 127 || _Maxof(char) == 2 * 127 + 1, "char maximum");
_Static_assert(_Maxof(long) == (long)(~0UL >> 1), "long maximum");
_Static_assert(_Minof(long) == -(long)(~0UL >> 1) - 1L, "long minimum");
_Static_assert(_Maxof(enum signed_limit) == 32767, "enum maximum");
_Static_assert(_Minof(enum ordinary) == -2147483647 - 1, "ordinary enum minimum");
_Static_assert(_Maxof(bool) && !_Minof(bool), "bool limits");
_Static_assert(_Maxof(_BitInt(575)) == (((_BitInt(575))1 << 573) - 1) * 2 + 1, "huge maximum");
_Static_assert(_Minof(_BitInt(575)) == -_Maxof(_BitInt(575)) - 1, "huge minimum");
_Static_assert(_Maxof(unsigned _BitInt(575)) == ~(unsigned _BitInt(575))0, "unsigned huge maximum");
_Static_assert(_Generic(_Maxof(unsigned char), unsigned char: 1, default: 0), "no promotion");
_Static_assert(_Generic(typeof(_Maxof(small)), unsigned short: 1, default: 0), "no qualifiers");
_Static_assert(_Generic(typeof(_Minof(_Atomic(int))), int: 1, default: 0), "no atomic qualifier");
_Static_assert(_Generic(_Maxof(_BitInt(575)), _BitInt(575): 1, default: 0), "bit type");

int expression(int *value) {
    return _Maxof(typeof((*value)++)) + _Minof(int);
}
int null_limit(int *value) {
    return value == _Minof(unsigned int);
}

// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES GNU2Y
// SLATE-FILECHECK-STD GNU2Y gnu2y

// SLATE-FILECHECK-BEGIN C2Y
// C2Y: module {
// C2Y-NEXT:     target "x86_64-pc-windows-msvc" {
// C2Y-NEXT:         endian = little;
// C2Y-NEXT:         pointer [size=8, align=8];
// C2Y-NEXT:         stack_alignment = 16;
// C2Y-NEXT:         long_double = f64;
// C2Y-NEXT:         storage bool [size=1, align=1];
// C2Y-NEXT:         storage i8, u8 [size=1, align=1];
// C2Y-NEXT:         storage i16, u16 [size=2, align=2];
// C2Y-NEXT:         storage i32, u32 [size=4, align=4];
// C2Y-NEXT:         storage i64, u64 [size=8, align=8];
// C2Y-NEXT:         storage i128, u128 [size=16, align=16];
// C2Y-NEXT:         storage bf16 [size=2, align=2];
// C2Y-NEXT:         storage f16 [size=2, align=2];
// C2Y-NEXT:         storage f32 [size=4, align=4];
// C2Y-NEXT:         storage f64 [size=8, align=8];
// C2Y-NEXT:         storage f128 [size=16, align=16];
// C2Y-NEXT:         storage d32 [size=4, align=4];
// C2Y-NEXT:         storage d64 [size=8, align=8];
// C2Y-NEXT:         storage d128 [size=16, align=16];
// C2Y-NEXT:     }
// C2Y-NEXT:     type @type[[TYPE_small:[0-9]+]] small = u16;
// C2Y-NEXT:     type @type[[TYPE_signed_limit:[0-9]+]] signed_limit = enum : i16 {
// C2Y-NEXT:         %[[VALUE_LOW:[0-9]+]] LOW = const<@type[[TYPE_signed_limit]]>(-1);
// C2Y-NEXT:         %[[VALUE_HIGH:[0-9]+]] HIGH = const<@type[[TYPE_signed_limit]]>(1);
// C2Y-NEXT:     } [size=2, align=2];
// C2Y-NEXT:     type @type[[TYPE_unsigned_limit:[0-9]+]] unsigned_limit = enum : u32 {
// C2Y-NEXT:         %[[VALUE_LOW]] ZERO = const<@type[[TYPE_unsigned_limit]]>(0);
// C2Y-NEXT:     } [size=4, align=4];
// C2Y-NEXT:     type @type[[TYPE_ordinary:[0-9]+]] ordinary = enum : i32 {
// C2Y-NEXT:         %[[VALUE_LOW]] NEGATIVE = const<i32>(-1);
// C2Y-NEXT:         %[[VALUE_HIGH]] POSITIVE = const<i32>(1);
// C2Y-NEXT:     } [size=4, align=4];
// C2Y-NEXT:     type @type[[TYPE_forward:[0-9]+]] forward = enum : u16 incomplete [size=2, align=2];
// C2Y-NEXT:     global %[[VALUE_sc_max:[0-9]+]] sc_max: i8 [storage=static] = const<i8>(127) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_sc_min:[0-9]+]] sc_min: i8 [storage=static] = const<i8>(-128) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_uc_max:[0-9]+]] uc_max: u8 [storage=static] = const<u8>(255) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_uc_min:[0-9]+]] uc_min: u8 [storage=static] = const<u8>(0) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_char_max:[0-9]+]] char_max: i8 [storage=static] = const<i8>(127) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_char_min:[0-9]+]] char_min: i8 [storage=static] = const<i8>(-128) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_short_max:[0-9]+]] short_max: i16 [storage=static] = const<i16>(32767) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_short_min:[0-9]+]] short_min: i16 [storage=static] = const<i16>(-32768) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_long_max:[0-9]+]] long_max: i32 [storage=static] = const<i32>(2147483647) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_long_min:[0-9]+]] long_min: i32 [storage=static] = const<i32>(-2147483648) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_ulong_max:[0-9]+]] ulong_max: u32 [storage=static] = const<u32>(4294967295) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_ll_max:[0-9]+]] ll_max: i64 [storage=static] = const<i64>(9223372036854775807) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_ll_min:[0-9]+]] ll_min: i64 [storage=static] = const<i64>(-9223372036854775808) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_extended_min:[0-9]+]] extended_min: i128 [storage=static] = const<i128>(-170141183460469231731687303715884105728) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_extended_max:[0-9]+]] extended_max: u128 [storage=static] = const<u128>(340282366920938463463374607431768211455) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_bool_max:[0-9]+]] bool_max: bool [storage=static] = const<bool>(true) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_bool_min:[0-9]+]] bool_min: bool [storage=static] = const<bool>(false) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_qualified:[0-9]+]] qualified: u16 [storage=static] = const<u16>(65535) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_atomic:[0-9]+]] atomic: u32 [storage=static] = const<u32>(4294967295) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_enum_max:[0-9]+]] enum_max: @type[[TYPE_signed_limit]] [storage=static] = const<@type[[TYPE_signed_limit]]>(32767) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_enum_min:[0-9]+]] enum_min: @type[[TYPE_signed_limit]] [storage=static] = const<@type[[TYPE_signed_limit]]>(-32768) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_enum_unsigned:[0-9]+]] enum_unsigned: @type[[TYPE_unsigned_limit]] [storage=static] = const<@type[[TYPE_unsigned_limit]]>(4294967295) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_forward_max:[0-9]+]] forward_max: u16 [storage=static] = enum_to_int<u16, reason=promotion>(const<@type[[TYPE_forward]]>(65535)) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_bit_max:[0-9]+]] bit_max: u1b [storage=static] = const<u1b>(1) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_bit_min:[0-9]+]] bit_min: i5b [storage=static] = const<i5b>(-16) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_huge_min:[0-9]+]] huge_min: i575b [storage=static] = const<i575b>(-61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174784) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_huge_max:[0-9]+]] huge_max: u575b [storage=static] = const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_bound:[0-9]+]] bound: array<i32, 7> [storage=static] [align=16] [linkage=external];
// C2Y-NEXT:     fn %[[VALUE_expression:[0-9]+]] @expression(%[[VALUE_value:[0-9]+]] value: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         return add<i32, overflow=ub>(const<i32>(2147483647), const<i32>(-2147483648));
// C2Y-NEXT:     }
// C2Y-NEXT:     fn %[[VALUE_null_limit:[0-9]+]] @null_limit(%[[VALUE_value_2:[0-9]+]] value: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C2Y-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_value_2]]), null<ptr<i32>>));
// C2Y-NEXT:     }
// C2Y-NEXT: }
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN GNU2Y
// GNU2Y: module {
// GNU2Y-NEXT:     target "x86_64-pc-windows-msvc" {
// GNU2Y-NEXT:         endian = little;
// GNU2Y-NEXT:         pointer [size=8, align=8];
// GNU2Y-NEXT:         stack_alignment = 16;
// GNU2Y-NEXT:         long_double = f64;
// GNU2Y-NEXT:         storage bool [size=1, align=1];
// GNU2Y-NEXT:         storage i8, u8 [size=1, align=1];
// GNU2Y-NEXT:         storage i16, u16 [size=2, align=2];
// GNU2Y-NEXT:         storage i32, u32 [size=4, align=4];
// GNU2Y-NEXT:         storage i64, u64 [size=8, align=8];
// GNU2Y-NEXT:         storage i128, u128 [size=16, align=16];
// GNU2Y-NEXT:         storage bf16 [size=2, align=2];
// GNU2Y-NEXT:         storage f16 [size=2, align=2];
// GNU2Y-NEXT:         storage f32 [size=4, align=4];
// GNU2Y-NEXT:         storage f64 [size=8, align=8];
// GNU2Y-NEXT:         storage f128 [size=16, align=16];
// GNU2Y-NEXT:         storage d32 [size=4, align=4];
// GNU2Y-NEXT:         storage d64 [size=8, align=8];
// GNU2Y-NEXT:         storage d128 [size=16, align=16];
// GNU2Y-NEXT:     }
// GNU2Y-NEXT:     type @type[[TYPE_small:[0-9]+]] small = u16;
// GNU2Y-NEXT:     type @type[[TYPE_signed_limit:[0-9]+]] signed_limit = enum : i16 {
// GNU2Y-NEXT:         %[[VALUE_LOW:[0-9]+]] LOW = const<@type[[TYPE_signed_limit]]>(-1);
// GNU2Y-NEXT:         %[[VALUE_HIGH:[0-9]+]] HIGH = const<@type[[TYPE_signed_limit]]>(1);
// GNU2Y-NEXT:     } [size=2, align=2];
// GNU2Y-NEXT:     type @type[[TYPE_unsigned_limit:[0-9]+]] unsigned_limit = enum : u32 {
// GNU2Y-NEXT:         %[[VALUE_LOW]] ZERO = const<@type[[TYPE_unsigned_limit]]>(0);
// GNU2Y-NEXT:     } [size=4, align=4];
// GNU2Y-NEXT:     type @type[[TYPE_ordinary:[0-9]+]] ordinary = enum : i32 {
// GNU2Y-NEXT:         %[[VALUE_LOW]] NEGATIVE = const<i32>(-1);
// GNU2Y-NEXT:         %[[VALUE_HIGH]] POSITIVE = const<i32>(1);
// GNU2Y-NEXT:     } [size=4, align=4];
// GNU2Y-NEXT:     type @type[[TYPE_forward:[0-9]+]] forward = enum : u16 incomplete [size=2, align=2];
// GNU2Y-NEXT:     global %[[VALUE_sc_max:[0-9]+]] sc_max: i8 [storage=static] = const<i8>(127) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_sc_min:[0-9]+]] sc_min: i8 [storage=static] = const<i8>(-128) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_uc_max:[0-9]+]] uc_max: u8 [storage=static] = const<u8>(255) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_uc_min:[0-9]+]] uc_min: u8 [storage=static] = const<u8>(0) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_char_max:[0-9]+]] char_max: i8 [storage=static] = const<i8>(127) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_char_min:[0-9]+]] char_min: i8 [storage=static] = const<i8>(-128) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_short_max:[0-9]+]] short_max: i16 [storage=static] = const<i16>(32767) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_short_min:[0-9]+]] short_min: i16 [storage=static] = const<i16>(-32768) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_long_max:[0-9]+]] long_max: i32 [storage=static] = const<i32>(2147483647) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_long_min:[0-9]+]] long_min: i32 [storage=static] = const<i32>(-2147483648) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_ulong_max:[0-9]+]] ulong_max: u32 [storage=static] = const<u32>(4294967295) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_ll_max:[0-9]+]] ll_max: i64 [storage=static] = const<i64>(9223372036854775807) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_ll_min:[0-9]+]] ll_min: i64 [storage=static] = const<i64>(-9223372036854775808) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_extended_min:[0-9]+]] extended_min: i128 [storage=static] = const<i128>(-170141183460469231731687303715884105728) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_extended_max:[0-9]+]] extended_max: u128 [storage=static] = const<u128>(340282366920938463463374607431768211455) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_bool_max:[0-9]+]] bool_max: bool [storage=static] = const<bool>(true) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_bool_min:[0-9]+]] bool_min: bool [storage=static] = const<bool>(false) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_qualified:[0-9]+]] qualified: u16 [storage=static] = const<u16>(65535) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_atomic:[0-9]+]] atomic: u32 [storage=static] = const<u32>(4294967295) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_enum_max:[0-9]+]] enum_max: @type[[TYPE_signed_limit]] [storage=static] = const<@type[[TYPE_signed_limit]]>(32767) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_enum_min:[0-9]+]] enum_min: @type[[TYPE_signed_limit]] [storage=static] = const<@type[[TYPE_signed_limit]]>(-32768) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_enum_unsigned:[0-9]+]] enum_unsigned: @type[[TYPE_unsigned_limit]] [storage=static] = const<@type[[TYPE_unsigned_limit]]>(4294967295) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_forward_max:[0-9]+]] forward_max: u16 [storage=static] = enum_to_int<u16, reason=promotion>(const<@type[[TYPE_forward]]>(65535)) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_bit_max:[0-9]+]] bit_max: u1b [storage=static] = const<u1b>(1) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_bit_min:[0-9]+]] bit_min: i5b [storage=static] = const<i5b>(-16) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_huge_min:[0-9]+]] huge_min: i575b [storage=static] = const<i575b>(-61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174784) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_huge_max:[0-9]+]] huge_max: u575b [storage=static] = const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567) [linkage=external];
// GNU2Y-NEXT:     global %[[VALUE_bound:[0-9]+]] bound: array<i32, 7> [storage=static] [align=16] [linkage=external];
// GNU2Y-NEXT:     fn %[[VALUE_expression:[0-9]+]] @expression(%[[VALUE_value:[0-9]+]] value: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// GNU2Y-NEXT:         return add<i32, overflow=ub>(const<i32>(2147483647), const<i32>(-2147483648));
// GNU2Y-NEXT:     }
// GNU2Y-NEXT:     fn %[[VALUE_null_limit:[0-9]+]] @null_limit(%[[VALUE_value_2:[0-9]+]] value: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// GNU2Y-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_value_2]]), null<ptr<i32>>));
// GNU2Y-NEXT:     }
// GNU2Y-NEXT: }
// SLATE-FILECHECK-END GNU2Y
