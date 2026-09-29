/* Extracted from gas.  Incorrectly generated non-pic code at -O0 for
   IA-64, which produces linker errors on some operating systems.  */

struct {
  int offset;
  struct slot {
    int field[6];
  } slot[4];
} s;

int x() {
  int toggle = 0;
  int r      = s.slot[0].field[!toggle];
  return r;
}

int main() { return 0; }



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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 offset: i32;
// DEFAULT-NEXT:         field1 slot: array<@type[[TYPE_slot:[0-9]+]], 4>;
// DEFAULT-NEXT:     } [size=100, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_slot]] slot = struct {
// DEFAULT-NEXT:         field0 field: array<i32, 6>;
// DEFAULT-NEXT:     } [size=24, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_x:[0-9]+]] @x() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_toggle:[0-9]+]] toggle: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(field0(deref(ptr_offset<ptr<@type[[TYPE_slot]]>, subtract=false, element=@type[[TYPE_slot]], overflow=ub>(array_decay<ptr<@type[[TYPE_slot]]>, length=Some(4)>(field1(%[[VALUE_s]])), const<i32>(0))))), from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%[[VALUE_toggle]]), const<i32>(0)))))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
