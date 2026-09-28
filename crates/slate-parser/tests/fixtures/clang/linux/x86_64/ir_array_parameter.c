// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int guaranteed(int a[static 3]) {
    return a[0];
}

int qualified_const(int a[const]) {
    return a[0];
}

int qualified_restrict(int a[restrict 4]) {
    return a[0];
}

int guaranteed_const(int a[static const 5]) {
    return a[0];
}

void unspecified_extent(int n, int a[*]);

int guaranteed_variable(int n, int a[static n]) {
    return a[0];
}

int const_element(const int a[3]) {
    return a[0];
}

int plain(int a[]) {
    return a[0];
}

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
// IR-NEXT:     fn %0 @guaranteed(%1 a: ptr<i32> [array=static 3]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%1), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %2 @qualified_const(%3 a: ptr<i32> [const]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%3), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @qualified_restrict(%5 a: ptr<i32> [restrict] [array=4]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %6 @guaranteed_const(%7 a: ptr<i32> [const] [array=static 5]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @unspecified_extent(%16 n: i32, %17 a: ptr<i32> [array=*]) -> void [linkage=external];
// IR-NEXT:     fn %9 @guaranteed_variable(%10 n: i32, %11 a: ptr<i32> [array=static %18]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %18: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%10)));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%11), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @const_element(%13 a: ptr<const i32> [array=3]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%13), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @plain(%15 a: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%15), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
