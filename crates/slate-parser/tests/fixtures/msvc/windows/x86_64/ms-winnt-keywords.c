struct packed_member { char c; __unaligned int x; };
struct pointer_member { char c; int *__unaligned p; };
typedef __unaligned int UI;
typedef unsigned short WCHAR;
typedef WCHAR __unaligned *LPUWSTR;
typedef int *__unaligned UP;

int * __ptr64 wide;
int * __ptr64 const wide_const = 0;
_Static_assert(_Generic(wide, int *: 1, default: 0), "__ptr64 is a plain pointer");
_Static_assert(sizeof(int * __ptr64) == 8, "__ptr64 size");

__unaligned int unaligned_int;
int *__unaligned unaligned_pointer;
LPUWSTR unaligned_wide_string;
_Static_assert(_Generic(unaligned_wide_string, WCHAR *: 0, __unaligned WCHAR *: 1, default: 0), "__unaligned is a qualifier");
_Static_assert(sizeof(struct packed_member) == 8 && _Alignof(struct packed_member) == 4, "record layout unchanged");
_Static_assert(sizeof(struct pointer_member) == 16, "pointer member layout unchanged");
_Static_assert(_Alignof(int *__unaligned) == 1, "__unaligned pointer");
_Static_assert(_Alignof(UP) == 1, "__unaligned pointer typedef");
_Static_assert(_Alignof(__unaligned int *) == 8, "__unaligned pointee");
_Static_assert(_Alignof(__unaligned int) == 4, "cl: only pointers");
_Static_assert(_Alignof(UI) == 4, "cl: __unaligned typedef");

int *plain;
void convert(void) {
    unaligned_wide_string = (LPUWSTR)plain;
    plain = (int *)unaligned_pointer;
}

__forceinline int forced(void) { return 1; }
static __forceinline int forced_static(void) { return 2; }
int __forceinline forced_after(void) { return 3; }
int use_forced(void) { return forced() + forced_static() + forced_after(); }

// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_packed_member:[0-9]+]] packed_member = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 x: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_pointer_member:[0-9]+]] pointer_member = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 p: ptr<i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_UI:[0-9]+]] UI = i32;
// DEFAULT-NEXT:     type @type[[TYPE_WCHAR:[0-9]+]] WCHAR = u16;
// DEFAULT-NEXT:     type @type[[TYPE_LPUWSTR:[0-9]+]] LPUWSTR = ptr<u16>;
// DEFAULT-NEXT:     type @type[[TYPE_UP:[0-9]+]] UP = ptr<i32>;
// DEFAULT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide_const:[0-9]+]] wide_const: ptr<i32> [storage=static] [const] = null<ptr<i32>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unaligned_int:[0-9]+]] unaligned_int: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unaligned_pointer:[0-9]+]] unaligned_pointer: ptr<i32> [storage=static] [align=1] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unaligned_wide_string:[0-9]+]] unaligned_wide_string: ptr<u16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_convert:[0-9]+]] @convert() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<u16>>(%[[VALUE_unaligned_wide_string]], pointer_cast<ptr<u16>, reason=explicit>(read<ptr<i32>>(%[[VALUE_plain]])));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_plain]], read<ptr<i32>>(%[[VALUE_unaligned_pointer]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_forced:[0-9]+]] @forced() -> i32 [linkage=external] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_forced_static:[0-9]+]] @forced_static() -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_forced_after:[0-9]+]] @forced_after() -> i32 [linkage=external] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_forced:[0-9]+]] @use_forced() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_forced]]), call<i32, signature=fn() -> i32>(%[[VALUE_forced_static]])), call<i32, signature=fn() -> i32>(%[[VALUE_forced_after]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
