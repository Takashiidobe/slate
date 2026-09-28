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

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
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
// DEFAULT-NEXT:     type @type0 sysfs_dirent = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type1;
// DEFAULT-NEXT:         field1 s_mode: u16;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 s_dir: @type2;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 sysfs_elem_dir = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 Foo = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type4;
// DEFAULT-NEXT:         field1 s_mode: u16;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type4 = union {
// DEFAULT-NEXT:         field0 x: @type5;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type5 empty = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     global %3 sysfs_root: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = aggregate<@type1, zero_fill=false>(), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(16877)))) [linkage=external];
// DEFAULT-NEXT:     global %7 foo: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = aggregate<@type4, zero_fill=false>(), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(16877)))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
