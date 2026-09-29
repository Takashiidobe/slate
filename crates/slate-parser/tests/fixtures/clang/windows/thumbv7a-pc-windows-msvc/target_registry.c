#if defined(_MSC_VER)
int msc_ver = _MSC_VER;
#elif defined(__clang__)
int clang_major = __clang_major__;
#elif defined(__GNUC__)
int gnuc = __GNUC__;
#endif
unsigned long sizeof_long = sizeof(long);
unsigned long sizeof_long_double = sizeof(long double);
unsigned long sizeof_va_list = sizeof(__builtin_va_list);
unsigned long alignof_long_long = _Alignof(long long);
struct pair { int a; int b; };
struct pair record(struct pair value) { return value; }



// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CHECK

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "thumbv7a-pc-windows-msvc" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=4, align=4];
// CHECK-NEXT:         stack_alignment = 8;
// CHECK-NEXT:         long_double = f64;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=8];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// CHECK-NEXT:         field0 a: i32;
// CHECK-NEXT:         field1 b: i32;
// CHECK-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// CHECK-NEXT:     global %[[VALUE_msc_ver:[0-9]+]] msc_ver: i32 [storage=static] = const<i32>(1933) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_sizeof_long:[0-9]+]] sizeof_long: u32 [storage=static] = const<u32>(4) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_sizeof_long_double:[0-9]+]] sizeof_long_double: u32 [storage=static] = const<u32>(8) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_sizeof_va_list:[0-9]+]] sizeof_va_list: u32 [storage=static] = const<u32>(4) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_alignof_long_long:[0-9]+]] alignof_long_long: u32 [storage=static] = const<u32>(8) [linkage=external];
// CHECK-NEXT:     fn %[[VALUE_record:[0-9]+]] @record(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_pair]]) -> @type[[TYPE_pair]] [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// CHECK-NEXT:         return copy<@type[[TYPE_pair]], reason=return>(read<@type[[TYPE_pair]]>(%[[VALUE_value]]));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
