// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

int f(int a, int b);
struct Bits { int a : 10; int b : 10; };

int unsequenced_arguments(int i) { return f(i++, i++); }
int unsequenced_assignments(int i) { return f(i = 1, i = 2); }
int unsequenced_target(int i) { i = (i, --i); return i; }

int sequenced_argument(int i) { return f(i++, 1); }
int sequenced_update(int i) { i = i + 1; return i; }
int distinct_members(struct Bits x) { return f(x.a++, x.b++); }
int distinct_elements(int *b) { b[0] = b[1] = 1; return b[0]; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_Bits:[0-9]+]] Bits = struct {
// IR-NEXT:         field0 a: i32 : 10;
// IR-NEXT:         field1 b: i32 : 10;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 1], bit_offsets=[Some(0), Some(10)], bit_units=[(0, 3)], field_units=[Some(0), Some(0)]];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_unsequenced_arguments:[0-9]+]] @unsequenced_arguments(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unsequenced_assignments:[0-9]+]] @unsequenced_assignments(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i_2]], const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i_2]], const<i32>(2));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f]], const<i32>(1), const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unsequenced_target:[0-9]+]] @unsequenced_target(%[[VALUE_i_3:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         read<i32>(%[[VALUE_i_3]]);
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i_3]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic, unsequenced] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i_3]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i_3]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         return read<i32>(%[[VALUE_i_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sequenced_argument:[0-9]+]] @sequenced_argument(%[[VALUE_i_4:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE7]]));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE6]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sequenced_update:[0-9]+]] @sequenced_update(%[[VALUE_i_5:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32>(%[[VALUE_i_5]], add<i32, overflow=ub>(read<i32>(%[[VALUE_i_5]]), const<i32>(1)));
// IR-NEXT:         return read<i32>(%[[VALUE_i_5]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_distinct_members:[0-9]+]] @distinct_members(%[[VALUE_x:[0-9]+]] x: @type[[TYPE_Bits]]) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..10>(%[[VALUE_x]]));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// IR-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..3, bits=0..10>(%[[VALUE_x]]), read<i32>(%[[VALUE9]]));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(bitfield1<unit=0, bytes=0..3, bits=10..20>(%[[VALUE_x]]));
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// IR-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..3, bits=10..20>(%[[VALUE_x]]), read<i32>(%[[VALUE11]]));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f]], read<i32>(%[[VALUE8]]), read<i32>(%[[VALUE10]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_distinct_elements:[0-9]+]] @distinct_elements(%[[VALUE_b_2:[0-9]+]] b: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_b_2]]), const<i32>(1))), const<i32>(1));
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_b_2]]), const<i32>(0))), const<i32>(1));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_b_2]]), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
