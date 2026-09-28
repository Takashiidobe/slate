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
// IR-NEXT:     type @type0 Bits = struct {
// IR-NEXT:         field0 a: i32 : 10;
// IR-NEXT:         field1 b: i32 : 10;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 1], bit_offsets=[Some(0), Some(10)], bit_units=[(0, 3)], field_units=[Some(0), Some(0)]];
// IR-NEXT:     fn %0 @f(%16 a: i32, %17 b: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %2 @unsequenced_arguments(%3 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %18: i32 [synthetic, unsequenced] = read<i32>(%3);
// IR-NEXT:         let %19: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%3, read<i32>(%19));
// IR-NEXT:         let %20: i32 [synthetic, unsequenced] = read<i32>(%3);
// IR-NEXT:         let %21: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%3, read<i32>(%21));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%0, read<i32>(%18), read<i32>(%20));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @unsequenced_assignments(%5 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32, unsequenced>(%5, const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%5, const<i32>(2));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%0, const<i32>(1), const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %6 @unsequenced_target(%7 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         read<i32>(%7);
// IR-NEXT:         let %22: i32 [synthetic, unsequenced] = read<i32>(%7);
// IR-NEXT:         let %23: i32 [synthetic, unsequenced] = sub<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%7, read<i32>(%23));
// IR-NEXT:         write<i32, unsequenced>(%7, read<i32>(%23));
// IR-NEXT:         return read<i32>(%7);
// IR-NEXT:     }
// IR-NEXT:     fn %8 @sequenced_argument(%9 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %24: i32 [synthetic] = read<i32>(%9);
// IR-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// IR-NEXT:         write<i32>(%9, read<i32>(%25));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%0, read<i32>(%24), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @sequenced_update(%11 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32>(%11, add<i32, overflow=ub>(read<i32>(%11), const<i32>(1)));
// IR-NEXT:         return read<i32>(%11);
// IR-NEXT:     }
// IR-NEXT:     fn %12 @distinct_members(%13 x: @type0) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         let %26: i32 [synthetic] = read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..10>(%13));
// IR-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// IR-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..3, bits=0..10>(%13), read<i32>(%27));
// IR-NEXT:         let %28: i32 [synthetic] = read<i32>(bitfield1<unit=0, bytes=0..3, bits=10..20>(%13));
// IR-NEXT:         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// IR-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..3, bits=10..20>(%13), read<i32>(%29));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%0, read<i32>(%26), read<i32>(%28));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @distinct_elements(%15 b: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(1))), const<i32>(1));
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(0))), const<i32>(1));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
