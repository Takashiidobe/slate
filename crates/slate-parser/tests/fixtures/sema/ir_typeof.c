// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
// SLATE-FILECHECK-STD DEFAULT c23

typedef unsigned long word;
typedef word alias;
alias source;
typeof(alias) named;
typeof(source) copied;
const volatile int qualified;
typeof(qualified) kept;
typeof_unqual(qualified) stripped;
typeof_unqual(_Atomic(int)) non_atomic;
int array[3];
typeof(array) other_array;
int function(int x);
typeof(function) other_function;
__typeof(source) alias_one;
__typeof__(source) alias_two;
__typeof_unqual__(qualified) alias_unqual;
typeof(void) nothing(void);
alias returns_alias(void);
typeof(returns_alias()) returned;

int test(int x) {
    typeof(x++) y = 1;
    typeof(function(x++)) z = 2;
    typeof("unused") text;
    typeof((x++, "discarded")) string_pointer;
    typeof((int){x++}) compound = 3;
    typeof(array) local_array;
    typeof(&function) pointer = function;
    typeof_unqual(qualified) local = 4;
    return y + z + compound + local + sizeof(text) + sizeof(local_array);
}

int extents(int n) {
    int original[n];
    typeof(original) copy;
    return sizeof(copy);
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
// DEFAULT-NEXT:     type @type0 word = u64 [c="unsigned long"];
// DEFAULT-NEXT:     type @type1 alias = u64 [c="word"] [c_canon="unsigned long"] [typedef_chain="word"];
// DEFAULT-NEXT:     global %2 source: u64 [storage=static] [linkage=external] [c="alias"] [c_canon="unsigned long"] [typedef_chain="alias -> word"];
// DEFAULT-NEXT:     global %3 named: u64 [storage=static] [linkage=external] [c="typeof(alias)"] [c_canon="unsigned long"] [typedef_chain="alias -> word"];
// DEFAULT-NEXT:     global %4 copied: u64 [storage=static] [linkage=external] [c="typeof(source)"] [c_canon="unsigned long"] [typedef_chain="alias -> word"];
// DEFAULT-NEXT:     global %5 qualified: i32 [storage=static] [linkage=external] [c="const volatile int"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:     global %6 kept: i32 [storage=static] [linkage=external] [c="typeof(qualified)"] [c_canon="const volatile int"] [c_const="true"] [c_volatile="true"];
// DEFAULT-NEXT:     global %7 stripped: i32 [storage=static] [linkage=external] [c="typeof_unqual(qualified)"] [c_canon="int"];
// DEFAULT-NEXT:     global %8 non_atomic: i32 [storage=static] [linkage=external] [c="typeof_unqual(_Atomic(int))"] [c_canon="int"];
// DEFAULT-NEXT:     global %9 array: array<i32, 3> [storage=static] [linkage=external] [c="int[3]"];
// DEFAULT-NEXT:     global %10 other_array: array<i32, 3> [storage=static] [linkage=external] [c="typeof(array)"] [c_canon="int[3]"];
// DEFAULT-NEXT:     global %13 alias_one: u64 [storage=static] [linkage=external] [c="typeof(source)"] [c_canon="unsigned long"] [typedef_chain="alias -> word"];
// DEFAULT-NEXT:     global %14 alias_two: u64 [storage=static] [linkage=external] [c="typeof(source)"] [c_canon="unsigned long"] [typedef_chain="alias -> word"];
// DEFAULT-NEXT:     global %15 alias_unqual: i32 [storage=static] [linkage=external] [c="typeof_unqual(qualified)"] [c_canon="int"];
// DEFAULT-NEXT:     global %18 returned: u64 [storage=static] [linkage=external] [c="typeof(returns_alias())"] [c_canon="unsigned long"] [typedef_chain="alias -> word"];
// DEFAULT-NEXT:     fn %11 @function(%33 x: i32 [c="int"]) -> i32 [linkage=external] [c="int(int)"];
// DEFAULT-NEXT:     fn %12 @other_function(%34 <unnamed>: i32) -> i32 [linkage=external] [c="typeof(function)"] [c_canon="int(int)"];
// DEFAULT-NEXT:     fn %16 @nothing() -> void [linkage=external] [c="typeof(void)(void)"] [c_canon="void(void)"];
// DEFAULT-NEXT:     fn %17 @returns_alias() -> u64 [linkage=external] [c="alias(void)"] [c_canon="unsigned long(void)"] [typedef_chain="alias -> word"];
// DEFAULT-NEXT:     fn %19 @test(%20 x: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// DEFAULT-NEXT:         let %21 y: i32 [storage=automatic] = const<i32>(1) [c="typeof(x++)"] [c_canon="int"];
// DEFAULT-NEXT:         let %22 z: i32 [storage=automatic] = const<i32>(2) [c="typeof(function(x++))"] [c_canon="int"];
// DEFAULT-NEXT:         let %23 text: array<i8, 7> [storage=automatic] [c="typeof(\"unused\")"] [c_canon="char[7]"];
// DEFAULT-NEXT:         let %24 string_pointer: ptr<i8> [storage=automatic] [c="typeof((x++, \"discarded\"))"] [c_canon="char *"];
// DEFAULT-NEXT:         let %25 compound: i32 [storage=automatic] = const<i32>(3) [c="typeof((compound literal))"] [c_canon="int"];
// DEFAULT-NEXT:         let %26 local_array: array<i32, 3> [storage=automatic] [c="typeof(array)"] [c_canon="int[3]"];
// DEFAULT-NEXT:         let %27 pointer: ptr<fn(i32) -> i32> [storage=automatic] = function_decay<ptr<fn(i32) -> i32>>(%11) [c="typeof(&function)"] [c_canon="int (*)(int)"];
// DEFAULT-NEXT:         let %28 local: i32 [storage=automatic] = const<i32>(4) [c="typeof_unqual(qualified)"] [c_canon="int"];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%21), read<i32>(%22)), read<i32>(%25)), read<i32>(%28)))), const<u64>(7) [size_of="array<i8, 7>"]), const<u64>(12) [size_of="array<i32, 3>"])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @extents(%30 n: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// DEFAULT-NEXT:         let %35: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%30))) [c="int[*]"];
// DEFAULT-NEXT:         let %31 original: vla<i32, %35> [storage=automatic] [c="int[*]"];
// DEFAULT-NEXT:         let %32 copy: vla<i32, %35> [storage=automatic] [c="typeof(original)"] [c_canon="int[*]"];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%35), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
