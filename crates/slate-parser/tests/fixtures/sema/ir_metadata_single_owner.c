// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
typedef int triple[3];

struct result { int value; } make(void);
void take(struct pair { int a, b; } *p);

int conversions(void) {
    int size = sizeof(long);
    return size;
}

int initialized(void) {
    triple values = {};
    return values[0];
}

int hoisted(_Atomic int *p) {
    __c11_atomic_store(p, 1, 5);
    return __c11_atomic_fetch_add(p, 1, 5) + 1;
}

int fallthrough(int x) {
    switch (x) {
    case 0:
        x++;
        [[fallthrough]];
    default:
        return x;
    }
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
// IR-NEXT:     type @type0 triple = array<i32, 3> [c="int[3]"];
// IR-NEXT:     type @type1 result = struct {
// IR-NEXT:         field0 value: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type2 pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %2 @make() -> @type1 [linkage=external] [abi=sysv64() -> coerce<i32>] [c="struct result(void)"];
// IR-NEXT:     fn %4 @take(%13 p: ptr<@type2> [c="struct pair *"]) -> void [linkage=external] [c="void(struct pair *)"];
// IR-NEXT:     fn %5 @conversions() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// IR-NEXT:         let %6 size: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8) [size_of="i64"])) [c="int"];
// IR-NEXT:         return read<i32>(%6);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @initialized() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// IR-NEXT:         let %8 values: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=true>() [c="triple"] [c_canon="int[3]"] [typedef_chain="triple"];
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%8), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @hoisted(%10 p: ptr<atomic i32> [c="_Atomic int *"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(_Atomic int *)"] {
// IR-NEXT:         write<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%10)), const<i32>(1)) [c_builtin="__c11_atomic_store"];
// IR-NEXT:         let %15: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%10)), add<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_add"];
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @fallthrough(%12 x: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         switch %14 read<i32>(%12)
// IR-NEXT:             {
// IR-NEXT:                 case %14 const<i32>(0):
// IR-NEXT:                     let %16: i32 [synthetic] = read<i32>(%12);
// IR-NEXT:                     let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// IR-NEXT:                     write<i32>(%12, read<i32>(%17));
// IR-NEXT:                  [c_attribute="fallthrough"];
// IR-NEXT:                 default %14:
// IR-NEXT:                     return read<i32>(%12);
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
