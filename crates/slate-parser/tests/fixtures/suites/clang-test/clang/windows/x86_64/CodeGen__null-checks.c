// Check that null pointer checks are not optimized out if -fms-compatibility is set






struct Obj { int value; int extra; };

int process(struct Obj* p) {
    int v = p->value;
    if (!p)
        return -1;
    return v + p->extra;
}

void* call_memcpy(void* p, long long size) {
  return __builtin_memcpy(0, p, size);
}

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
// DEFAULT-NEXT:     type @type[[TYPE_Obj:[0-9]+]] Obj = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:         field1 extra: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_process:[0-9]+]] @process(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Obj]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_Obj]]>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type[[TYPE_Obj]]>>(read<ptr<@type[[TYPE_Obj]]>>(%[[VALUE_p]]), null<ptr<@type[[TYPE_Obj]]>>))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_v]]), read<i32>(field1(deref(read<ptr<@type[[TYPE_Obj]]>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_call_memcpy:[0-9]+]] @call_memcpy(%[[VALUE_p_2:[0-9]+]] p: ptr<void>, %[[VALUE_size:[0-9]+]] size: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE___builtin_memcpy]], null<ptr<void>>, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_p_2]])), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_size]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
