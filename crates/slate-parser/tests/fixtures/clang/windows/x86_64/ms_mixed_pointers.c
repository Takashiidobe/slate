typedef int *P;

int *__ptr32 sign_extended;
int *__ptr32 __sptr explicit_sign;
int *__ptr32 __uptr zero_extended;
int *__ptr64 wide;
int *__uptr plain_uptr;
P __ptr32 through_typedef;
int *__ptr32 const constant_narrow = 0;
int *__ptr32 *pointer_to_narrow;
void (*__ptr32 narrow_function)(void);
int *__ptr32 narrow_array[3];
struct narrow_member { char c; int *__ptr32 p; } narrow_record;
int target_object;
int *__ptr32 narrowed_address = &target_object;

_Static_assert(sizeof(int *__ptr32) == 4 && _Alignof(int *__ptr32) == 4, "ptr32 storage");
_Static_assert(sizeof(int *__ptr64) == 8, "ptr64 is the plain pointer");
_Static_assert(sizeof(through_typedef) == 4, "typedef ptr32");
_Static_assert(sizeof(*pointer_to_narrow) == 4 && sizeof(pointer_to_narrow) == 8, "pointer to ptr32");
_Static_assert(sizeof(narrow_function) == 4, "function ptr32");
_Static_assert(sizeof(narrow_array) == 12, "array of ptr32");
_Static_assert(sizeof(struct narrow_member) == 8, "ptr32 member");
_Static_assert(_Generic(sign_extended, int *: 0, int *__ptr32: 1), "ptr32 is distinct");
_Static_assert(_Generic(zero_extended, int *__ptr32: 0, default: 1), "uptr is distinct");
_Static_assert(_Generic(wide, int *: 1, default: 0), "ptr64 is plain");
_Static_assert(_Generic(plain_uptr, int *: 1, default: 0), "uptr alone is plain");
_Static_assert(_Generic(1 ? sign_extended : zero_extended, int *: 1, default: 0), "composite is plain");

int *widen_signed(void) { return sign_extended; }
int *widen_unsigned(void) { return zero_extended; }
int *__ptr32 narrow(int *p) { return p; }
void take(int *__ptr32 p);
void pass(int *p) { take(p); }
void swap_extension(void) { sign_extended = zero_extended; }
int deref(void) { return *sign_extended; }
int *__ptr32 offset(void) { return sign_extended + 1; }
long long to_integer(void) { return (long long)zero_extended; }
int *__ptr32 from_integer(long long v) { return (int *__ptr32)v; }
int compare(int *p) { return p == sign_extended && sign_extended == zero_extended; }
int truth(void) { return !sign_extended; }

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
// DEFAULT-NEXT:     type @type0 P = ptr<i32>;
// DEFAULT-NEXT:     type @type1 narrow_member = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 p: ptr<i32, ptr32_sptr>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %1 sign_extended: ptr<i32, ptr32_sptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 explicit_sign: ptr<i32, ptr32_sptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 zero_extended: ptr<i32, ptr32_uptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 wide: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 plain_uptr: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 through_typedef: ptr<i32, ptr32_sptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 constant_narrow: ptr<i32, ptr32_sptr> [storage=static] [const] = null<ptr<i32, ptr32_sptr>> [linkage=external];
// DEFAULT-NEXT:     global %8 pointer_to_narrow: ptr<ptr<i32, ptr32_sptr>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 narrow_function: ptr<fn() -> void, ptr32_sptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 narrow_array: array<ptr<i32, ptr32_sptr>, 3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 narrow_record: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 target_object: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 narrowed_address: ptr<i32, ptr32_sptr> [storage=static] = address_space_cast<ptr<i32, ptr32_sptr>, reason=assign>(addr_of<ptr<i32>>(%13)) [linkage=external];
// DEFAULT-NEXT:     fn %15 @widen_signed() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return address_space_cast<ptr<i32>, reason=return>(read<ptr<i32, ptr32_sptr>>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @widen_unsigned() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return address_space_cast<ptr<i32>, reason=return>(read<ptr<i32, ptr32_uptr>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @narrow(%18 p: ptr<i32>) -> ptr<i32, ptr32_sptr> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return address_space_cast<ptr<i32, ptr32_sptr>, reason=return>(read<ptr<i32>>(%18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @take(%32 p: ptr<i32, ptr32_sptr>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %21 @pass(%22 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32, ptr32_sptr>) -> void>(%20, address_space_cast<ptr<i32, ptr32_sptr>, reason=arg>(read<ptr<i32>>(%22)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @swap_extension() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr32_sptr>>(%1, address_space_cast<ptr<i32, ptr32_sptr>, reason=assign>(read<ptr<i32, ptr32_uptr>>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @deref() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32, ptr32_sptr>>(%1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @offset() -> ptr<i32, ptr32_sptr> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ptr_offset<ptr<i32, ptr32_sptr>, subtract=false, element=i32, overflow=ub>(read<ptr<i32, ptr32_sptr>>(%1), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @to_integer() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ptr_to_int<i64, reason=explicit>(read<ptr<i32, ptr32_uptr>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @from_integer(%28 v: i64) -> ptr<i32, ptr32_sptr> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_ptr<ptr<i32, ptr32_sptr>, reason=explicit>(read<i64>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @compare(%30 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<ptr<i32>>(read<ptr<i32>>(%30), address_space_cast<ptr<i32>, reason=usual_arith>(read<ptr<i32, ptr32_sptr>>(%1))), eq<ptr<i32, ptr32_sptr>>(read<ptr<i32, ptr32_sptr>>(%1), address_space_cast<ptr<i32, ptr32_sptr>, reason=usual_arith>(read<ptr<i32, ptr32_uptr>>(%3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @truth() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(not<bool>(ne<ptr<i32, ptr32_sptr>>(read<ptr<i32, ptr32_sptr>>(%1), null<ptr<i32, ptr32_sptr>>)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
