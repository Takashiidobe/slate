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

// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

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
// DEFAULT-NEXT:     type @type0 Obj = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:         field1 extra: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %1 @process(%2 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 v: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:         if not<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%2), null<ptr<@type0>>))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%3), read<i32>(field1(deref(read<ptr<@type0>>(%2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @__builtin_memcpy(%7 <unnamed>: ptr<void>, %8 <unnamed>: ptr<const void>, %9 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @call_memcpy(%5 p: ptr<void>, %6 size: i64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%10, null<ptr<void>>, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%5)), reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
