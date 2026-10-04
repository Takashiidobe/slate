#ifdef __x86_64__
int macro_x86_64 = 1;
#else
int macro_x86_64 = -1;
#endif

#ifdef __i386__
int macro_i386 = 1;
#else
int macro_i386 = -1;
#endif

#ifdef __tune_i386__
int macro_tune_i386 = 1;
#else
int macro_tune_i386 = -1;
#endif

int macro_sizeof_long = __SIZEOF_LONG__;
int macro_sizeof_pointer = __SIZEOF_POINTER__;

int plain(int a, int b) { return a + b; }
int variadic(int a, ...) { return a; }
int __attribute__((stdcall)) standard(int a, int b) { return a + b; }
int __attribute__((fastcall)) fast(int a, int b) { return a + b; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES M64
// SLATE-FILECHECK-PREFIX-ARGS M64 -m64 -mregparm=3
// SLATE-FILECHECK-DEFINES M32
// SLATE-FILECHECK-PREFIX-ARGS M32 -m32
// SLATE-FILECHECK-DEFINES LASTWINS
// SLATE-FILECHECK-PREFIX-ARGS LASTWINS -m32 -m64
// SLATE-FILECHECK-DEFINES M16
// SLATE-FILECHECK-PREFIX-ARGS M16 -m16 -march=i386 -mregparm=3
// SLATE-FILECHECK-DEFINES REGPARM0
// SLATE-FILECHECK-PREFIX-ARGS REGPARM0 -m32 -mregparm=0
// SLATE-FILECHECK-DEFINES SHORT
// SLATE-FILECHECK-PREFIX-ARGS SHORT --target=x86_64-linux-gnu -m32

// SLATE-FILECHECK-BEGIN M64
// M64: module {
// M64-NEXT:     target "x86_64-unknown-linux-gnu" {
// M64-NEXT:         endian = little;
// M64-NEXT:         pointer [size=8, align=8];
// M64-NEXT:         stack_alignment = 16;
// M64-NEXT:         long_double = f80;
// M64-NEXT:         storage bool [size=1, align=1];
// M64-NEXT:         storage i8, u8 [size=1, align=1];
// M64-NEXT:         storage i16, u16 [size=2, align=2];
// M64-NEXT:         storage i32, u32 [size=4, align=4];
// M64-NEXT:         storage i64, u64 [size=8, align=8];
// M64-NEXT:         storage i128, u128 [size=16, align=16];
// M64-NEXT:         storage bf16 [size=2, align=2];
// M64-NEXT:         storage f16 [size=2, align=2];
// M64-NEXT:         storage f32 [size=4, align=4];
// M64-NEXT:         storage f64 [size=8, align=8];
// M64-NEXT:         storage f80 [size=16, align=16];
// M64-NEXT:         storage f128 [size=16, align=16];
// M64-NEXT:         storage d32 [size=4, align=4];
// M64-NEXT:         storage d64 [size=8, align=8];
// M64-NEXT:         storage d128 [size=16, align=16];
// M64-NEXT:     }
// M64-NEXT:     global %[[VALUE_macro_x86_64:[0-9]+]] macro_x86_64: i32 [storage=static] = const<i32>(1) [linkage=external];
// M64-NEXT:     global %[[VALUE_macro_i386:[0-9]+]] macro_i386: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// M64-NEXT:     global %[[VALUE_macro_tune_i386:[0-9]+]] macro_tune_i386: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// M64-NEXT:     global %[[VALUE_macro_sizeof_long:[0-9]+]] macro_sizeof_long: i32 [storage=static] = const<i32>(8) [linkage=external];
// M64-NEXT:     global %[[VALUE_macro_sizeof_pointer:[0-9]+]] macro_sizeof_pointer: i32 [storage=static] = const<i32>(8) [linkage=external];
// M64-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// M64-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// M64-NEXT:     }
// M64-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// M64-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// M64-NEXT:     }
// M64-NEXT:     fn %[[VALUE_standard:[0-9]+]] @standard(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// M64-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_2]]));
// M64-NEXT:     }
// M64-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// M64-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_3]]));
// M64-NEXT:     }
// M64-NEXT: }
// SLATE-FILECHECK-END M64
// SLATE-FILECHECK-BEGIN M32
// M32: module {
// M32-NEXT:     target "i686-unknown-linux-gnu" {
// M32-NEXT:         endian = little;
// M32-NEXT:         pointer [size=4, align=4];
// M32-NEXT:         stack_alignment = 16;
// M32-NEXT:         long_double = f80;
// M32-NEXT:         storage bool [size=1, align=1];
// M32-NEXT:         storage i8, u8 [size=1, align=1];
// M32-NEXT:         storage i16, u16 [size=2, align=2];
// M32-NEXT:         storage i32, u32 [size=4, align=4];
// M32-NEXT:         storage i64, u64 [size=8, align=4];
// M32-NEXT:         storage i128, u128 [size=16, align=16];
// M32-NEXT:         storage bf16 [size=2, align=2];
// M32-NEXT:         storage f16 [size=2, align=2];
// M32-NEXT:         storage f32 [size=4, align=4];
// M32-NEXT:         storage f64 [size=8, align=4];
// M32-NEXT:         storage f80 [size=12, align=4];
// M32-NEXT:         storage f128 [size=16, align=16];
// M32-NEXT:         storage d32 [size=4, align=4];
// M32-NEXT:         storage d64 [size=8, align=8];
// M32-NEXT:         storage d128 [size=16, align=16];
// M32-NEXT:     }
// M32-NEXT:     global %[[VALUE_macro_x86_64:[0-9]+]] macro_x86_64: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// M32-NEXT:     global %[[VALUE_macro_i386:[0-9]+]] macro_i386: i32 [storage=static] = const<i32>(1) [linkage=external];
// M32-NEXT:     global %[[VALUE_macro_tune_i386:[0-9]+]] macro_tune_i386: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// M32-NEXT:     global %[[VALUE_macro_sizeof_long:[0-9]+]] macro_sizeof_long: i32 [storage=static] = const<i32>(4) [linkage=external];
// M32-NEXT:     global %[[VALUE_macro_sizeof_pointer:[0-9]+]] macro_sizeof_pointer: i32 [storage=static] = const<i32>(4) [linkage=external];
// M32-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// M32-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// M32-NEXT:     }
// M32-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// M32-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// M32-NEXT:     }
// M32-NEXT:     fn %[[VALUE_standard:[0-9]+]] @standard(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl stdcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// M32-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_2]]));
// M32-NEXT:     }
// M32-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl fastcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// M32-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_3]]));
// M32-NEXT:     }
// M32-NEXT: }
// SLATE-FILECHECK-END M32
// SLATE-FILECHECK-BEGIN LASTWINS
// LASTWINS: module {
// LASTWINS-NEXT:     target "x86_64-unknown-linux-gnu" {
// LASTWINS-NEXT:         endian = little;
// LASTWINS-NEXT:         pointer [size=8, align=8];
// LASTWINS-NEXT:         stack_alignment = 16;
// LASTWINS-NEXT:         long_double = f80;
// LASTWINS-NEXT:         storage bool [size=1, align=1];
// LASTWINS-NEXT:         storage i8, u8 [size=1, align=1];
// LASTWINS-NEXT:         storage i16, u16 [size=2, align=2];
// LASTWINS-NEXT:         storage i32, u32 [size=4, align=4];
// LASTWINS-NEXT:         storage i64, u64 [size=8, align=8];
// LASTWINS-NEXT:         storage i128, u128 [size=16, align=16];
// LASTWINS-NEXT:         storage bf16 [size=2, align=2];
// LASTWINS-NEXT:         storage f16 [size=2, align=2];
// LASTWINS-NEXT:         storage f32 [size=4, align=4];
// LASTWINS-NEXT:         storage f64 [size=8, align=8];
// LASTWINS-NEXT:         storage f80 [size=16, align=16];
// LASTWINS-NEXT:         storage f128 [size=16, align=16];
// LASTWINS-NEXT:         storage d32 [size=4, align=4];
// LASTWINS-NEXT:         storage d64 [size=8, align=8];
// LASTWINS-NEXT:         storage d128 [size=16, align=16];
// LASTWINS-NEXT:     }
// LASTWINS-NEXT:     global %[[VALUE_macro_x86_64:[0-9]+]] macro_x86_64: i32 [storage=static] = const<i32>(1) [linkage=external];
// LASTWINS-NEXT:     global %[[VALUE_macro_i386:[0-9]+]] macro_i386: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LASTWINS-NEXT:     global %[[VALUE_macro_tune_i386:[0-9]+]] macro_tune_i386: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LASTWINS-NEXT:     global %[[VALUE_macro_sizeof_long:[0-9]+]] macro_sizeof_long: i32 [storage=static] = const<i32>(8) [linkage=external];
// LASTWINS-NEXT:     global %[[VALUE_macro_sizeof_pointer:[0-9]+]] macro_sizeof_pointer: i32 [storage=static] = const<i32>(8) [linkage=external];
// LASTWINS-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// LASTWINS-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// LASTWINS-NEXT:     }
// LASTWINS-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// LASTWINS-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// LASTWINS-NEXT:     }
// LASTWINS-NEXT:     fn %[[VALUE_standard:[0-9]+]] @standard(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// LASTWINS-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_2]]));
// LASTWINS-NEXT:     }
// LASTWINS-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// LASTWINS-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_3]]));
// LASTWINS-NEXT:     }
// LASTWINS-NEXT: }
// SLATE-FILECHECK-END LASTWINS
// SLATE-FILECHECK-BEGIN M16
// M16: module {
// M16-NEXT:     target "i686-unknown-linux-gnu" {
// M16-NEXT:         endian = little;
// M16-NEXT:         pointer [size=4, align=4];
// M16-NEXT:         stack_alignment = 16;
// M16-NEXT:         long_double = f80;
// M16-NEXT:         storage bool [size=1, align=1];
// M16-NEXT:         storage i8, u8 [size=1, align=1];
// M16-NEXT:         storage i16, u16 [size=2, align=2];
// M16-NEXT:         storage i32, u32 [size=4, align=4];
// M16-NEXT:         storage i64, u64 [size=8, align=4];
// M16-NEXT:         storage i128, u128 [size=16, align=16];
// M16-NEXT:         storage bf16 [size=2, align=2];
// M16-NEXT:         storage f16 [size=2, align=2];
// M16-NEXT:         storage f32 [size=4, align=4];
// M16-NEXT:         storage f64 [size=8, align=4];
// M16-NEXT:         storage f80 [size=12, align=4];
// M16-NEXT:         storage f128 [size=16, align=16];
// M16-NEXT:         storage d32 [size=4, align=4];
// M16-NEXT:         storage d64 [size=8, align=8];
// M16-NEXT:         storage d128 [size=16, align=16];
// M16-NEXT:     }
// M16-NEXT:     global %[[VALUE_macro_x86_64:[0-9]+]] macro_x86_64: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// M16-NEXT:     global %[[VALUE_macro_i386:[0-9]+]] macro_i386: i32 [storage=static] = const<i32>(1) [linkage=external];
// M16-NEXT:     global %[[VALUE_macro_tune_i386:[0-9]+]] macro_tune_i386: i32 [storage=static] = const<i32>(1) [linkage=external];
// M16-NEXT:     global %[[VALUE_macro_sizeof_long:[0-9]+]] macro_sizeof_long: i32 [storage=static] = const<i32>(4) [linkage=external];
// M16-NEXT:     global %[[VALUE_macro_sizeof_pointer:[0-9]+]] macro_sizeof_pointer: i32 [storage=static] = const<i32>(4) [linkage=external];
// M16-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl regparm=3(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// M16-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// M16-NEXT:     }
// M16-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// M16-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// M16-NEXT:     }
// M16-NEXT:     fn %[[VALUE_standard:[0-9]+]] @standard(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl stdcall regparm=3(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// M16-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_2]]));
// M16-NEXT:     }
// M16-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl fastcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// M16-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_3]]));
// M16-NEXT:     }
// M16-NEXT: }
// SLATE-FILECHECK-END M16
// SLATE-FILECHECK-BEGIN REGPARM0
// REGPARM0: module {
// REGPARM0-NEXT:     target "i686-unknown-linux-gnu" {
// REGPARM0-NEXT:         endian = little;
// REGPARM0-NEXT:         pointer [size=4, align=4];
// REGPARM0-NEXT:         stack_alignment = 16;
// REGPARM0-NEXT:         long_double = f80;
// REGPARM0-NEXT:         storage bool [size=1, align=1];
// REGPARM0-NEXT:         storage i8, u8 [size=1, align=1];
// REGPARM0-NEXT:         storage i16, u16 [size=2, align=2];
// REGPARM0-NEXT:         storage i32, u32 [size=4, align=4];
// REGPARM0-NEXT:         storage i64, u64 [size=8, align=4];
// REGPARM0-NEXT:         storage i128, u128 [size=16, align=16];
// REGPARM0-NEXT:         storage bf16 [size=2, align=2];
// REGPARM0-NEXT:         storage f16 [size=2, align=2];
// REGPARM0-NEXT:         storage f32 [size=4, align=4];
// REGPARM0-NEXT:         storage f64 [size=8, align=4];
// REGPARM0-NEXT:         storage f80 [size=12, align=4];
// REGPARM0-NEXT:         storage f128 [size=16, align=16];
// REGPARM0-NEXT:         storage d32 [size=4, align=4];
// REGPARM0-NEXT:         storage d64 [size=8, align=8];
// REGPARM0-NEXT:         storage d128 [size=16, align=16];
// REGPARM0-NEXT:     }
// REGPARM0-NEXT:     global %[[VALUE_macro_x86_64:[0-9]+]] macro_x86_64: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// REGPARM0-NEXT:     global %[[VALUE_macro_i386:[0-9]+]] macro_i386: i32 [storage=static] = const<i32>(1) [linkage=external];
// REGPARM0-NEXT:     global %[[VALUE_macro_tune_i386:[0-9]+]] macro_tune_i386: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// REGPARM0-NEXT:     global %[[VALUE_macro_sizeof_long:[0-9]+]] macro_sizeof_long: i32 [storage=static] = const<i32>(4) [linkage=external];
// REGPARM0-NEXT:     global %[[VALUE_macro_sizeof_pointer:[0-9]+]] macro_sizeof_pointer: i32 [storage=static] = const<i32>(4) [linkage=external];
// REGPARM0-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// REGPARM0-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// REGPARM0-NEXT:     }
// REGPARM0-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// REGPARM0-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// REGPARM0-NEXT:     }
// REGPARM0-NEXT:     fn %[[VALUE_standard:[0-9]+]] @standard(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl stdcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// REGPARM0-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_2]]));
// REGPARM0-NEXT:     }
// REGPARM0-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl fastcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// REGPARM0-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_3]]));
// REGPARM0-NEXT:     }
// REGPARM0-NEXT: }
// SLATE-FILECHECK-END REGPARM0
// SLATE-FILECHECK-BEGIN SHORT
// SHORT: module {
// SHORT-NEXT:     target "i686-unknown-linux-gnu" {
// SHORT-NEXT:         endian = little;
// SHORT-NEXT:         pointer [size=4, align=4];
// SHORT-NEXT:         stack_alignment = 16;
// SHORT-NEXT:         long_double = f80;
// SHORT-NEXT:         storage bool [size=1, align=1];
// SHORT-NEXT:         storage i8, u8 [size=1, align=1];
// SHORT-NEXT:         storage i16, u16 [size=2, align=2];
// SHORT-NEXT:         storage i32, u32 [size=4, align=4];
// SHORT-NEXT:         storage i64, u64 [size=8, align=4];
// SHORT-NEXT:         storage i128, u128 [size=16, align=16];
// SHORT-NEXT:         storage bf16 [size=2, align=2];
// SHORT-NEXT:         storage f16 [size=2, align=2];
// SHORT-NEXT:         storage f32 [size=4, align=4];
// SHORT-NEXT:         storage f64 [size=8, align=4];
// SHORT-NEXT:         storage f80 [size=12, align=4];
// SHORT-NEXT:         storage f128 [size=16, align=16];
// SHORT-NEXT:         storage d32 [size=4, align=4];
// SHORT-NEXT:         storage d64 [size=8, align=8];
// SHORT-NEXT:         storage d128 [size=16, align=16];
// SHORT-NEXT:     }
// SHORT-NEXT:     global %[[VALUE_macro_x86_64:[0-9]+]] macro_x86_64: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_macro_i386:[0-9]+]] macro_i386: i32 [storage=static] = const<i32>(1) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_macro_tune_i386:[0-9]+]] macro_tune_i386: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_macro_sizeof_long:[0-9]+]] macro_sizeof_long: i32 [storage=static] = const<i32>(4) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_macro_sizeof_pointer:[0-9]+]] macro_sizeof_pointer: i32 [storage=static] = const<i32>(4) [linkage=external];
// SHORT-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SHORT-NEXT:         return add<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// SHORT-NEXT:     }
// SHORT-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE_a_2:[0-9]+]] a: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SHORT-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// SHORT-NEXT:     }
// SHORT-NEXT:     fn %[[VALUE_standard:[0-9]+]] @standard(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl stdcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// SHORT-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_3]]), read<i32>(%[[VALUE_b_2]]));
// SHORT-NEXT:     }
// SHORT-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_3:[0-9]+]] b: i32) -> i32 [linkage=external] [abi=x86_cdecl fastcall(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// SHORT-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_3]]));
// SHORT-NEXT:     }
// SHORT-NEXT: }
// SLATE-FILECHECK-END SHORT
