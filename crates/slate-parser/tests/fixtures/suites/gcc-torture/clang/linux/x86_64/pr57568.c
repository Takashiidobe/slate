/* PR target/57568 */

extern void abort(void);
int         a[6][9] = {}, b = 1, *c = &a[3][5];

int main() {
  if (b && (*c = *c + *c))
    abort();
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<array<i32, 9>, 6> [storage=static] [align=16] = aggregate<array<array<i32, 9>, 6>, zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<i32> [storage=static] = addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(9)>(deref(ptr_offset<ptr<array<i32, 9>>, subtract=false, element=array<i32, 9>, overflow=ub>(array_decay<ptr<array<i32, 9>>, length=Some(6)>(%[[VALUE_a]]), const<i32>(3)))), const<i32>(5)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_c]]))), read<i32>(deref(read<ptr<i32>>(%[[VALUE_c]]))));
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%[[VALUE_c]])), read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
