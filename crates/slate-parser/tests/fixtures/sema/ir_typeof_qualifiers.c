// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
// SLATE-FILECHECK-STD DEFAULT c23

typedef const volatile int cvint;
typedef cvint row[2];
typedef const int *const pointer;
cvint original;
row table;
pointer ptr;
typeof_unqual(cvint) plain;
typeof_unqual(row) plain_row;
typeof_unqual(ptr) mutable_pointer;
typeof_unqual(_Atomic(const int *)) non_atomic_pointer;
typeof(*ptr) const_element;
typeof(table[0]) qualified_element;
typeof_unqual(table[0]) plain_element;
struct Record { cvint field; pointer ptr; typeof(original) copied; };
const struct Record record;
typeof(record.field) member;
typeof_unqual(record.field) plain_member;
typeof(record.ptr) member_pointer;

int test(cvint value, row array, int function(int)) {
    typeof(value) kept = 1;
    typeof(value + 1) promoted = 2;
    typeof(array) array_pointer = array;
    typeof(function) function_pointer = function;
    typeof(*(array_pointer + 1)) element = 3;
    typeof(_Generic(value, int: original, default: 1)) selected = 4;
    typeof_unqual(typeof(original)) nested = 5;
    return _Generic(nested, typeof_unqual(original): 1, default: 0);
}

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
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 cvint = i32 [c="const volatile int"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:     type @type1 row = array<i32, 2> [c="cvint[2]"] [c_canon="const volatile int[2]"] [typedef_chain="cvint"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:     type @type2 pointer = ptr<const i32> [c="const int *const"] [c_const="true"];
// DEFAULT-NEXT:     type @type3 Record = struct {
// DEFAULT-NEXT:         field0 field: volatile i32;
// DEFAULT-NEXT:         field1 ptr: ptr<const i32>;
// DEFAULT-NEXT:         field2 copied: volatile i32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %3 original: i32 [storage=static] [const] [linkage=external] [c="cvint"] [c_canon="const volatile int"] [typedef_chain="cvint"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:     global %4 table: array<i32, 2> [storage=static] [const] [linkage=external] [c="row"] [c_canon="const volatile int[2]"] [typedef_chain="row -> cvint"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:     global %5 ptr: ptr<const i32> [storage=static] [const] [linkage=external] [c="pointer"] [c_canon="const int *const"] [typedef_chain="pointer"] [c_const="true"];
// DEFAULT-NEXT:     global %6 plain: i32 [storage=static] [linkage=external] [c="typeof_unqual(cvint)"] [c_canon="int"];
// DEFAULT-NEXT:     global %7 plain_row: array<i32, 2> [storage=static] [linkage=external] [c="typeof_unqual(row)"] [c_canon="int[2]"];
// DEFAULT-NEXT:     global %8 mutable_pointer: ptr<const i32> [storage=static] [linkage=external] [c="typeof_unqual(ptr)"] [c_canon="const int *"];
// DEFAULT-NEXT:     global %9 non_atomic_pointer: ptr<const i32> [storage=static] [linkage=external] [c="typeof_unqual(_Atomic(const int *))"] [c_canon="const int *"];
// DEFAULT-NEXT:     global %10 const_element: i32 [storage=static] [const] [linkage=external] [c="typeof(*ptr)"] [c_canon="const int"] [c_const="true"];
// DEFAULT-NEXT:     global %11 qualified_element: i32 [storage=static] [const] [linkage=external] [c="typeof(table[0])"] [c_canon="const volatile int"] [typedef_chain="cvint"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:     global %12 plain_element: i32 [storage=static] [linkage=external] [c="typeof_unqual(table[0])"] [c_canon="int"];
// DEFAULT-NEXT:     global %14 record: @type3 [storage=static] [const] [linkage=external] [c="const struct Record"] [c_const="true"];
// DEFAULT-NEXT:     global %15 member: i32 [storage=static] [const] [linkage=external] [c="typeof(record.field)"] [c_canon="const volatile int"] [typedef_chain="cvint"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:     global %16 plain_member: i32 [storage=static] [linkage=external] [c="typeof_unqual(record.field)"] [c_canon="int"];
// DEFAULT-NEXT:     global %17 member_pointer: ptr<const i32> [storage=static] [const] [linkage=external] [c="typeof(record.ptr)"] [c_canon="const int *const"] [typedef_chain="pointer"] [c_const="true"];
// DEFAULT-NEXT:     fn %18 @test(%19 value: i32 [const] [c="cvint"] [c_canon="const volatile int"] [typedef_chain="cvint"] [c_const="true"] [c_volatile="true"], %20 array: ptr<const volatile i32> [array=2] [c="row"] [c_canon="const volatile int[2]"] [typedef_chain="row -> cvint"] [c_const="true"] [c_volatile="true"], %21 function: ptr<fn(i32) -> i32> [c="int(int)"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(cvint, row, int(int))"] {
// DEFAULT-NEXT:         let %22 kept: i32 [storage=automatic] [const] = const<i32>(1) [c="typeof(value)"] [c_canon="const volatile int"] [typedef_chain="cvint"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:         let %23 promoted: i32 [storage=automatic] = const<i32>(2) [c="typeof(value + 1)"] [c_canon="int"];
// DEFAULT-NEXT:         let %24 array_pointer: ptr<const volatile i32> [storage=automatic] = read<ptr<const volatile i32>>(%20) [c="typeof(array)"] [c_canon="const volatile int *"] [typedef_chain="cvint"];
// DEFAULT-NEXT:         let %25 function_pointer: ptr<fn(i32) -> i32> [storage=automatic] = read<ptr<fn(i32) -> i32>>(%21) [c="typeof(function)"] [c_canon="int (*)(int)"];
// DEFAULT-NEXT:         let %26 element: i32 [storage=automatic] [const] = const<i32>(3) [c="typeof(*(array_pointer + 1))"] [c_canon="const volatile int"] [typedef_chain="cvint"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:         let %27 selected: i32 [storage=automatic] [const] = const<i32>(4) [c="typeof(_Generic(...))"] [c_canon="const volatile int"] [typedef_chain="cvint"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:         let %28 nested: i32 [storage=automatic] = const<i32>(5) [c="typeof_unqual(typeof(original))"] [c_canon="int"];
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
