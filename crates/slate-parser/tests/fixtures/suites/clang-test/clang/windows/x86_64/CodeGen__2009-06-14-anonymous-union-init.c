// PR4390
struct sysfs_dirent {
 union { struct sysfs_elem_dir { int x; } s_dir; };
 unsigned short s_mode;
};
struct sysfs_dirent sysfs_root = { {}, 16877 };


struct Foo {
 union { struct empty {} x; };
 unsigned short s_mode;
};
struct Foo foo = { {}, 16877 };

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

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
// DEFAULT-NEXT:     type @type[[TYPE_sysfs_dirent:[0-9]+]] sysfs_dirent = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field1 s_mode: u16;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = union {
// DEFAULT-NEXT:         field0 s_dir: @type[[TYPE_sysfs_elem_dir:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_sysfs_elem_dir]] sysfs_elem_dir = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Foo:[0-9]+]] Foo = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:         field1 s_mode: u16;
// DEFAULT-NEXT:     } [size=6, align=2, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = union {
// DEFAULT-NEXT:         field0 x: @type[[TYPE_empty:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_empty]] empty = struct {
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[]];
// DEFAULT-NEXT:     global %[[VALUE_sysfs_root:[0-9]+]] sysfs_root: @type[[TYPE_sysfs_dirent]] [storage=static] = aggregate<@type[[TYPE_sysfs_dirent]], zero_fill=false>(field0 = aggregate<@type[[TYPE0]], zero_fill=false>(), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(16877)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_foo:[0-9]+]] foo: @type[[TYPE_Foo]] [storage=static] = aggregate<@type[[TYPE_Foo]], zero_fill=false>(field0 = aggregate<@type[[TYPE1]], zero_fill=false>(), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(16877)))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
