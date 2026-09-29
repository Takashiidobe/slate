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
// IR-NEXT:     type @type[[TYPE_triple:[0-9]+]] triple = array<i32, 3> [c="int[3]"];
// IR-NEXT:     type @type[[TYPE_result:[0-9]+]] result = struct {
// IR-NEXT:         field0 value: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %[[VALUE_make:[0-9]+]] @make() -> @type[[TYPE_result]] [linkage=external] [abi=sysv64() -> native_c] [c="struct result(void)"];
// IR-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_pair]]> [c="struct pair *"]) -> void [linkage=external] [c="void(struct pair *)"];
// IR-NEXT:     fn %[[VALUE_conversions:[0-9]+]] @conversions() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// IR-NEXT:         let %[[VALUE_size:[0-9]+]] size: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8) [size_of="i64"])) [c="int"];
// IR-NEXT:         return read<i32>(%[[VALUE_size]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_initialized:[0-9]+]] @initialized() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// IR-NEXT:         let %[[VALUE_values:[0-9]+]] values: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=true>() [c="triple"] [c_canon="int[3]"] [typedef_chain="triple"];
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_values]]), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_hoisted:[0-9]+]] @hoisted(%[[VALUE_p_2:[0-9]+]] p: ptr<atomic i32> [c="_Atomic int *"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(_Atomic int *)"] {
// IR-NEXT:         write<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p_2]])), const<i32>(1)) [c_builtin="__c11_atomic_store"];
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE_p_2]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_add"];
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fallthrough:[0-9]+]] @fallthrough(%[[VALUE_x:[0-9]+]] x: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int)"] {
// IR-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_x]])
// IR-NEXT:             {
// IR-NEXT:                 case %[[VALUE1]] const<i32>(0):
// IR-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// IR-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// IR-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// IR-NEXT:                  [c_attribute="fallthrough"];
// IR-NEXT:                 default %[[VALUE1]]:
// IR-NEXT:                     return read<i32>(%[[VALUE_x]]);
// IR-NEXT:             }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
